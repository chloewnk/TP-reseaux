#ifndef _BAG_H
#define _BAG_H
#include <pthread.h>

// bounded bag with blocking add and take operations

typedef struct {
    int     capacity;  // max number of elements (== length of the 'elem' array)
    void ** elem;      // array of pointers to elements (each pointer has type 'void*' )
    int     size;      // number of elements currently in the bag
    pthread_mutex_t mutex;
    pthread_cond_t espace_dispo;
    pthread_cond_t element_dispo;
    int is_closed; // boolean
} bag_t ;

// Create a new bag with room for 'capacity' elements
bag_t * bb_create(int capacity);

// Insert an element in the bag.
// - if bag is full, block until there is room
void bb_add(bag_t *b, void * element);

// Retrieve an element from the bag
// - if bag is non-empty: remove one element and return it
// - if bag is empty and open, block until something is added
// - if bag is empty and closed, return NULL
void* bb_take(bag_t *b);

// Close the bag. When a bag is closed:
// - closing it again is an error
// - calling bb_add is an error
// - bb_take doesn't block anymore on an empty bag, but returns NULL instead
// - all threads previously blocked in bb_take are unblocked (and receive NULL)
void bb_close(bag_t *bag);


#endif
