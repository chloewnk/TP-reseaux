#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <unistd.h>
#include <sys/mman.h>

#include "mem.h"

void * heap_base = NULL;// first address of the heap
void * heap_end  = NULL;// first address beyond the heap

int mem_initialized = 0;

int64_t taille_utilisateur= 0;

// initialize the memory manager
void mem_init(void)
{
    // request memory from the kernel
    heap_base = mmap(NULL, 800, PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
    assert(heap_base != MAP_FAILED);

    heap_end = heap_base + 800 ;

    // create some free blocks: five of 80 bytes and one of 400 bytes

    /* void *block_ptr;
    for(int i=0; i<5;i++)
    {
        block_ptr = heap_base + 80*i;
        *( (int64_t*)block_ptr ) = 80;
    }
    block_ptr = heap_base + 400;
    *( (int64_t*)block_ptr ) = 400; */



    // ------ ex 5

    int64_t *header = (int64_t*)heap_base;
    int64_t *footer = (int64_t*)(heap_end - 8); // le heap end s'arrete mtn 8 octets avant pour le footer

    *header = 800; // écrire la taille totale dans le header
    *footer = 800; // same pour footer
    
    mem_initialized = 1;
}

void * mem_alloc(int64_t length)
{
    assert(mem_initialized == 1);

    int64_t save = length;
    
    // compute actual size of block
    length = (length + 7)&~7 ; // round up to nearest multiple of 8
    length += 8;              // add space for the header

    // heap traversal
    void *  block_ptr ;
    int64_t header ;
    int64_t size;   
    char    flags;
    
    block_ptr = heap_base;
    while(block_ptr < heap_end)
    {
        header = *( (int64_t*)block_ptr );
        flags  = header & 7;  // keep only three least significant bits
        size = header & ~7;   // discard the three least significant bits

        if( (flags == 0 ) &&      // if current block is free, and
            (size >= length)) {    // is also large enough, then we have a winner


            if (size >= length + 16) { // +16 pour prendre en compte header et footer
                
                int64_t size_restante = size - length;



                /* ----- ex 4
                
                void *next_bloc = block_ptr + length;
                *((int64_t*)next_bloc) = size_restante | 0; // creation du nv bloc et on met flag à 0 (c libre)
                *((int64_t*)block_ptr) = length | 1; // maj du bloc comme alloué 
                
                
                */

                
                void *next_bloc = block_ptr + length; // bloc suivant 
                *((int64_t*)next_bloc) = size_restante | 0; 
                int64_t *footer_rest = (int64_t*)(next_bloc + size_restante - 8);
                *footer_rest = size_restante | 0;

                // header + footer du bloc alloué
                *((int64_t*)block_ptr) = length | 1; 
                int64_t *footer_alloc = (int64_t*)(block_ptr + length - 8);
                *footer_alloc = length | 1;


            }

            else {
                // utiliser tout le bloc
                *((int64_t*)block_ptr) = size | 1;
                int64_t *footer_alloc = (int64_t*)(block_ptr + size - 8);
                *footer_alloc = size | 1;
                length = size; // taille réelle utilisée
            }
            
            taille_utilisateur += save; // ce que voulait l'utilisateur

            return block_ptr + 8; // return l'adrr apres entete
        }



        block_ptr += size; // passe au bloc suivant
    }

    return NULL; // si aucun bloc libre on return null
}


void mem_release(void *ptr)
{
    assert( mem_initialized == 1);
    assert( ((int64_t)ptr % 8) == 0 ); // sanity check

    // TODO: your deallocation algorithm goes here

    int64_t *header = (int64_t*)(ptr-8);
    int64_t size = *header & ~0b111;
    //*header = size;


    // header libre
    *header = size | 0;

    int64_t *next_header = (int64_t *)((char *)header + size);
    int64_t next_size = *next_header & ~0b111;

    // fusionne avec le bloc suivant
    if ((*next_header & 0b111) == 0 && next_size > 0) {
        size += next_size;
        *header = size | 0;
    }

    // bloc precedent ou pas 
    int64_t *last_footer = (int64_t *)((char *)header - 8);
    int64_t last_size = *last_footer & ~0b111;
    int64_t *last_header = (int64_t *)((char *)header - last_size);

    if (((char *)last_header >= (char *)heap_base) && (last_size == 0)) {
        // fusion avec bloc precedent 
        size += last_size;
        *last_header = size | 0;
        header = last_header;
    }

    // Mettre à jour le footer du bloc fusionné
    int64_t *footer = (int64_t *)((char *)header + size - 8);
    *footer = size | 0;

    return;
}




void mem_show_heap(void)
{
    assert( mem_initialized == 1);
    
    void * block_ptr = heap_base;
    int64_t fragmentation=0;

    printf("heap_base = %p\n",heap_base);
    while(block_ptr < heap_end)
    {
        int64_t header = *( (int64_t*)block_ptr );
        char    flags  = header & 0b111;  //   keep only three least significant bits
        int64_t size   = header & ~0b111; // discard the three least significant bits

        if( (size < 8) ||
            (size%8 != 0) )
        {
            printf("error: block at %p has incorrect size %ld\n",block_ptr,size);
            exit(1);
        }

        int64_t *footer_ptr = (int64_t *)(block_ptr + size - 8);
        int64_t footer = *footer_ptr;

        printf("  block at %p: header=0x%08lx size=%4ld flags=%d (%s)\n",
               block_ptr,header,size,flags,
               (flags ? "taken" : "free")
               );

        printf("footer at %p: 0x%08lx\n",
       footer_ptr, footer);

       // ex 2 (fragmentatio)

        fragmentation = 800 - taille_utilisateur;

        block_ptr += size; // move on to next block
    }

    printf("Fragmentation interne = %ld octets\n", fragmentation);
    printf("heap_end = %p\n",heap_end);

    //sanity check: a full heap traversal should reach *exactly* the end
    assert( block_ptr == heap_end); 
}
