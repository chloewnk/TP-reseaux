#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "mem.h"

int main()
{
    // initialize the allocator
    mem_init();

    // EX 1 ------------------------------------------------- TEST ALLOC
    
    char *p1 = mem_alloc(42);
    char *p2 = mem_alloc(42);
    char *p3 = mem_alloc(42);
    char *p4 = mem_alloc(42);
    char *p5 = mem_alloc(42);
    char *p6 = mem_alloc(204); // test pour le bloc de 400


    printf("Bloc 1 = allocated 42 bytes at %p\n", p1);
    assert( p1 != NULL );
    printf("Bloc 2 = allocated 42 bytes at %p\n", p2);
    assert( p2 != NULL );
    printf("Bloc 3 = allocated 42 bytes at %p\n", p3);
    assert( p3 != NULL );
    printf("Bloc 4 = allocated 42 bytes at %p\n", p4);
    assert( p4 != NULL );
    printf("Bloc 5 = allocated 42 bytes at %p\n", p5);
    assert( p5 != NULL );
    printf("Bloc 6 = allocated 200 bytes at %p\n", p6);
    assert( p6 != NULL );

    // EX 3 ------------------------------------------------ TEST MEM_RELEASE
    
    /* 
    mem_release(p6);
    char *p7 = mem_alloc(300);
    mem_release(p7);
    char *p8 = mem_alloc(4);
    mem_release(p8);  
    

    printf("Bloc 7 = DESALLOUER: allocated 300 bytes at %p\n", p7);
    assert( p7 != NULL );
    printf("Bloc 8 = DESALOUER : allocated 4 bytes at %p\n", p8);
    assert( p8 != NULL ); */

    // EX 4 ------------------------------------------------ TEST MEM_RELEASE

    char *p7 = mem_alloc(42);
    assert( p7 != NULL );
    printf("Bloc 7 = DECCOUPAGE : allocated 42 bytes at %p\n", p7);

    printf("verif finale pas de core dumped youpi\n");


    
    // char *p = mem_alloc( 42 ); 
    // assert( p != NULL ); // check whether the allocation was successful
    // printf("allocated 42 bytes at %p\n", p);
    // mem_show_heap();

    // EX 5 ----------------
    // pas de changments dans le main, tout est dans le mem_c


    // EX 6 ----------------------------------------------------- fusion


    // TEST 1 ----------------
    char *a = mem_alloc(20);
    assert(a != NULL);
    printf("allocated 20 bytes at %p\n", a);

    char *b = mem_alloc(20);
    assert(b != NULL);
    printf("allocated 20 bytes at %p\n", b);

    char *c = mem_alloc(20);
    assert(c != NULL);
    printf("allocated 20 bytes at %p\n", c);

    mem_release(b);
    printf("released block at %p\n", b);

    // pas de fusion cra seul b est libre  


    // TEST 2 ----------------
    char *d = mem_alloc(20);
    char *e = mem_alloc(20);
    mem_release(e);
    mem_release(d);
    printf("released at %p and %p\n", d, e); // fusion de d et e


    // TEST 3 ----------------
    char *f = mem_alloc(20);
    char *g = mem_alloc(20);
    mem_release(f);
    mem_release(g);
    printf("released at %p and %p\n", f, g); // fusion de f, g


    // TEST 4 ----------------
    char *h = mem_alloc(10);
    char *i = mem_alloc(10);
    char *j = mem_alloc(10);
    mem_release(h);
    mem_release(j);
    mem_release(i);
    printf("released at %p, %p and %p\n", h, i, j); // fusion de h, i, j 

    mem_show_heap();

    /*
            char *p8 = mem_alloc(42);
    printf("Bloc 7 = DECCOUPAGE : allocated 4 bytes at %p\n", p8);
    assert( p8 != NULL );
        char *p9 = mem_alloc(42);
    printf("Bloc 7 = DECCOUPAGE : allocated 4 bytes at %p\n", p9);
    assert( p9 != NULL );
            char *p10 = mem_alloc(42);

        char *p11 = mem_alloc(42);
        char *p12 = mem_alloc(42);
        char *p13 = mem_alloc(42);
        char *p14 = mem_alloc(42);
        char *p15 = mem_alloc(42);
        char *p16 = mem_alloc(42);*/
}
