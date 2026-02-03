Lien GIT : https://grond.iut-fbleau.fr/wiencek/r305_dm/src/branch/main/tp3-a-rendre



# 0

```c
bag_t * bb_create(int capacity)
{
    assert(capacity > 0);
    bag_t *bag = malloc (sizeof(bag_t));

    bag -> capacity = capacity;
    bag -> size = 0;
    bag -> is_closed = 0;
    bag -> elem = malloc (sizeof(void*) * capacity);


	 return bag;
}
```

Résultat : 

``` bash
chloe@pc-chloe:~/cours/scr/r305_dm/tp3-a-rendre/src$ ./main 3 2
Sac créé avec capacité = 2
Taille actuelle = 0
is_closed = 0
```

Dans un premier temps j'ai alloué la taille d'un sac avec un malloc. Ensuite chaque champ du struct a été initialisé. La capacité est celle passée en argument de la fonction bb_create. La taille est pour le moment à 0 car rien n'a été ajouté. Par défaut le sac est ouvert. Et chaque case du tableau elem multiplié par la capcité a été alloué.


# 1

``` c
	printf("theroretical result=%d\n", N*(N+1)/2); // replace 0
```
La valeur théorique est N*(N+1)/2 car chaque producteur produit une valeur correspondant à son numeor de thread. La somme de tous ces nombres est la somme des N premiers entiers.


# 2

Variable globale : 
``` c
pthread_mutex_t sum_mutex;
```

Dans consumer : 

``` c
	while(1){
		int *box = bb_take(bag);
		//assert( box != NULL ); // sanity check
		pthread_mutex_lock(&sum_mutex);
		sum = sum + *box;
		pthread_mutex_unlock(&sum_mutex);

		free(box);
	}
```

Dans le main : 

``` c
pthread_mutex_init(&sum_mutex, NULL);
```

Dans un premier temps j'ai ajouté une variable globale pour le mutex (sum_mutex) où il a été initialisé dans le main avec pthread_mutex_init. Puis ensuite j'ai ajouté un lock et unlock pour que sum ne soit jamais modifié simultanément par plusiuers threads.



# 3

``` bash
chloe@pc-chloe:~/cours/scr/r305_dm/tp3-a-rendre/src$ ./main 2 1 producer 1:start 
consumer 1: start 
producer 0:start 
consumer 0: start 
producer 0:end 
producer 1:end 
Segmentation fault (core dumped) 
chloe@pc-chloe:~/cours/scr/r305_dm/tp3-a-rendre/src$
```
En effet, la synchronisation dans bag.c est naive car les fonctions bb_add et bb_take utilisent des boucles sans mutex donc plusiuers threads modifient activement les variables en meme temps.

Donc : il y a un accès concurrent à bag->size (car pas de mutex), les éléments sont donc perdus et il y a seg fault a cause d'un conflit de mémoire.


# 4 

``` c
bag_t * bb_create(int capacity)
{
    assert(capacity > 0);
    bag_t *bag = malloc (sizeof(bag_t));
    if (bag==NULL){
        return NULL;
    }

    bag -> capacity = capacity;
    bag -> size = 0;
    bag -> is_closed = 0;
    bag -> elem = malloc (sizeof(void*) * capacity);
    if (bag-> elem==NULL){
        free(bag);
        return NULL;
    }

    pthread_mutex_init(&bag->mutex, NULL);
    pthread_cond_init(&bag->element_dispo, NULL);
    pthread_cond_init(&bag->espace_dispo, NULL);

	 return bag;
}

void bb_add(bag_t * bag, void * element)
{
    assert(bag != NULL);              // sanity check

    pthread_mutex_lock(&bag->mutex);

    while( bag->size >= bag->capacity ) { 
        pthread_cond_wait(&bag->espace_dispo, &bag->mutex);
    } // CAUTION: this synchronization is bogus

    assert( bag-> is_closed == 0 );   // adding to a closed bag is an error
    assert( bag->size < bag->capacity ); // sanity check

    bag->elem[bag->size]  = element;
    bag->size += 1;
    pthread_cond_signal(&bag->element_dispo);

    pthread_mutex_unlock(&bag->mutex);
}

void * bb_take(bag_t *bag)
{
    assert(bag != NULL);         // sanity check

    while( bag->size <= 0 ) { 
        pthread_cond_wait(&bag->element_dispo, &bag->mutex);
    } // CAUTION: this synchronization is bogus
    
    assert( bag->size > 0); // sanity check
    usleep(rand() % 10);// artificial delay to increase the occurence of race conditions

    bag->size -= 1;
    void *r = bag->elem[bag->size];

    pthread_cond_signal(&bag->espace_dispo);
    pthread_mutex_unlock(&bag->mutex);

    
    return r;
}
```

# 5
``` c
    for (int pnum = 0; pnum < N; pnum++) {
        pthread_join(prod[pnum], NULL);
    }
```

Dans le main, on ajoute une boucle qui appele pthread_joiin et qui agit sur chacun des threads producteurs. Ainsi, le programme principal attend bien la fin de tous les producteurs avant de pousuivvre l'exec. (un peu comme le wait des processus)


# 6 
Les consommateurs tournent en boucle infini et ne peuvent par terminer tout seul. Si on rajoute un pthread_join on bloquerait le programme principal pour toujours.


# 7

Pour bb_close : 

```c
void bb_close(bag_t *bag)
{
    //assert("not implemented" == 0);
    assert(bag!=NULL);

    // TODO (at the end): remove the line below, which is here only
    // to suppress the "unused parameter" warning/error
    pthread_mutex_lock(&bag->mutex);
    assert(bag->is_closed == 0);
    bag->is_closed=1;
    pthread_cond_broadcast(&bag->element_dispo);
    pthread_mutex_unlock(&bag->mutex);
}
```

Adaptation dans bb_add : 
- on empeche les ajouts si le sac est fermé et ca retourne NULL
```c
    if (bag->is_closed){
        pthread_mutex_unlock(&bag->mutex);
        return NULL;
    }
```

Dans le main : 
- on ferme le sac et on attend les consommateurs jusqu'à ce que bb_take renvoie NULL;
```c
	bb_close(bag);
    //bag->is_closed = 1;
	for (int cnum=0; cnum<N; cnum++){
		pthread_join(cons[cnum], NULL);
	}
```


J'avais un problème persistant de segfault : j'ai donc du adapter la méthode consummer() : 

```c
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
```

si ca condition de la box est null on break sinon on a un segmentation fault.



 --


# RESULTAT FINAL 

```bash
chloe@pc-chloe:~/cours/scr/r305_dm/tp3-a-rendre/src$ ./main 5 2
Sac créé avec capacité = 2
Taille actuelle = 0
is_closed = 0
producer 1:start
producer 0:start
producer 2:start
producer 3:start
producer 4:start
consumer 1: start
consumer 0: start
consumer 2: start
consumer 3: start
consumer 4: start
producer 0:end
producer 2:end
producer 1:end
producer 4:end
producer 3:end
consumer 1: end
consumer 4: end
consumer 0: end
consumer 2: end
consumer 3: end
theroretical result=15
actual computed sum=15
chloe@pc-chloe:~/cours/scr/r305_dm/tp3-a-rendre/src$
```