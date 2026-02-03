#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <pthread.h>

#include "bag.h"


// shared variables

int sum;
bag_t *bag;

pthread_mutex_t sum_mutex;

// each consumer thread runs this function
void *consumer(void *arg)
{
	(void)arg; // suppress compiler warning

	int cnum=*(int*)arg;
	printf("consumer %d: start\n",cnum);

	while(1){
		int *box = bb_take(bag);
		//assert( box != NULL ); // sanity check
		if (box==NULL){ // sinon segfault
			break;
		}
		pthread_mutex_lock(&sum_mutex);
		sum = sum + *box;
		pthread_mutex_unlock(&sum_mutex);

		free(box);
	}

	printf("consumer %d: end\n",cnum);
	return NULL; // dummy return to comply with required signature
}

// each producer thread runs this function
void *producer(void *arg)
{
	(void)arg; // suppress compiler warning

	int pnum=*(int*)arg;
	printf("producer %d:start \n",pnum);

	int k;
	for( k = 0 ; k < pnum+1 ; k++){
		int *box = malloc(sizeof(int));
		assert( box != NULL ); 
		*box = 1;

		bb_add(bag, box);
	}

	printf("producer %d:end\n",pnum);
	return NULL; // dummy return to comply with required signature
}

int main(int argc, char ** argv)
{
	assert(argc == 3);

	int N = strtol(argv[1], NULL, 0);
	assert( N > 0);

	pthread_t* prod = calloc (N, sizeof(pthread_t));
	pthread_t* cons = calloc (N, sizeof(pthread_t));

	int S = (int)strtol(argv[2], NULL, 0);
	assert( S > 0 );

	// initialize shared variables

	bag=bb_create(S); 
	assert(bag != NULL);
	sum=0;

    if (bag == NULL) {
        printf("Erreur : sac non créé\n");
        return 1;
    }

    printf("Sac créé avec capacité = %d\n", bag->capacity);
    printf("Taille actuelle = %d\n", bag->size);
    printf("is_closed = %d\n", bag->is_closed);

	pthread_mutex_init(&sum_mutex, NULL);





	int r;

	for(int pnum = 0 ; pnum < N ; pnum++){
		int *thread_arg=malloc(sizeof(int));
		*thread_arg = pnum;
		r=pthread_create(&prod[pnum], NULL, producer, thread_arg);

		if (r){
			printf("error: could not spawn producer %d\n", pnum);
			exit(1);
		}
	} 

	for(int cnum = 0 ; cnum < N ; cnum++){
		int *thread_arg=malloc(sizeof(int));
		*thread_arg = cnum;
		r=pthread_create(&cons[cnum], NULL, consumer, thread_arg);

		if (r){
			printf("error: could not spawn consumer %d\n", cnum);
			exit(1);
		}
	}
    for (int pnum = 0; pnum < N; pnum++) {
        pthread_join(prod[pnum], NULL);
    }
	bb_close(bag);
    //bag->is_closed = 1;

	for (int cnum=0; cnum<N; cnum++){
		pthread_join(cons[cnum], NULL);
	}

	sleep(5); // wait
	printf("theroretical result=%d\n", N*(N+1)/2); // replace 0
	printf("actual computed sum=%d\n", sum);

	return 0;
}
