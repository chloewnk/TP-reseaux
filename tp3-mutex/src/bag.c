#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>

#include "bag.h"

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
    if (bag->is_closed){
        pthread_mutex_unlock(&bag->mutex);
        return;
    }

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
    pthread_mutex_lock(&bag->mutex);
    while( bag->size == 0 && !bag->is_closed) {
        pthread_cond_wait(&bag->element_dispo, &bag->mutex);
    } // CAUTION: this synchronization is bogus
    if (bag->size==0 && bag->is_closed){
        pthread_mutex_unlock(&bag->mutex);
        return NULL;
    }
    assert( bag->size > 0); // sanity check
    usleep(rand() % 10);// artificial delay to increase the occurence of race conditions

    bag->size -= 1;
    void *r = bag->elem[bag->size];

    pthread_cond_signal(&bag->espace_dispo);
    pthread_mutex_unlock(&bag->mutex);

    
    return r;
}

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
