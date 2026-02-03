# Chloé WIENCEK



# 1. Vérifiez que vous pouvez allouer successivement six blocs de petite taille, par exemple 42 octets.

        ```
        chloe@LAPTOP-9SM2CR8V:~/SCR3.1/r305_dm/r305_dm/tp1-a-rendre$ make
            gcc -g -Wall -Wextra -Werror -std=gnu99 -o main mem.c main.c

        chloe@LAPTOP-9SM2CR8V:~/SCR3.1/r305_dm/r305_dm/tp1-a-rendre$ ./main
            Bloc 1 = allocated 42 bytes at 0x7ad97514b008
            Bloc 2 = allocated 42 bytes at 0x7ad97514b058
            Bloc 3 = allocated 42 bytes at 0x7ad97514b0a8
            Bloc 4 = allocated 42 bytes at 0x7ad97514b0f8
            Bloc 5 = allocated 42 bytes at 0x7ad97514b148
            Bloc 6 = allocated 42 bytes at 0x7ad97514b198
            heap_base = 0x7ad97514b000
            block at 0x7ad97514b000: header=0x00000051 size=  80 flags=1 (taken)
            block at 0x7ad97514b050: header=0x00000051 size=  80 flags=1 (taken)
            block at 0x7ad97514b0a0: header=0x00000051 size=  80 flags=1 (taken)
            block at 0x7ad97514b0f0: header=0x00000051 size=  80 flags=1 (taken)
            block at 0x7ad97514b140: header=0x00000051 size=  80 flags=1 (taken)
            block at 0x7ad97514b190: header=0x00000191 size= 400 flags=1 (taken)
            heap_end = 0x7ad97514b320
        ```


        Les allocations n'ont pas échoué, les mem_alloc ne sont pas NULL.
        Les adresses semblent cohérentes

        L'assertion :

                assert(block_ptr == heap_end)

            est validé donc le heap a été parcouru et toutes les allocations n'ont créé aucun conflit



    
    Vérifiez que vous ne pouvez pas allouer un septième bloc (pourquoi ?).
    
        ```
        chloe@LAPTOP-9SM2CR8V:~/SCR3.1/r305_dm/r305_dm/tp1-a-rendre$ ./main
            [...]
            Bloc 7 = allocated 42 bytes at (nil)
            heap_base = 0x7be4ebdd3000
            block at 0x7be4ebdd3000: header=0x00000051 size=  80 flags=1 (taken)
            block at 0x7be4ebdd3050: header=0x00000051 size=  80 flags=1 (taken)
            block at 0x7be4ebdd30a0: header=0x00000051 size=  80 flags=1 (taken)
            block at 0x7be4ebdd30f0: header=0x00000051 size=  80 flags=1 (taken)
            block at 0x7be4ebdd3140: header=0x00000051 size=  80 flags=1 (taken)
            block at 0x7be4ebdd3190: header=0x00000191 size= 400 flags=1 (taken)
            heap_end = 0x7be4ebdd3320
        ```

        On ne peut pas créer un 7è bloc car comme écrit sur la sortie standard on a que 6 blocs ( 5 de 80 et 1 de 400), plus précisement cette partie de code mentionne la repartition : 

        ```
            for(int i=0; i<5;i++) {        
                block_ptr = heap_base + 80*i;        
                *( (int64_t*)block_ptr ) = 80;    
            }
             block_ptr = heap_base + 400;    
             *( (int64_t*)block_ptr ) = 400;
        ```
        
        Le programme dit que 5 bloc de 80 octets on été alloué puis 1 bloc de 400 octets.
        Donc il n'est jamais pris en compte qu'un 7è bloc peut etre créé d'où l'erreur sur la sortie standard.



    
    Vérifiez que les tailles sont traitées correctement : une allocation de 200 doit aller dans le bloc libre de 400.

    ```
        chloe@LAPTOP-9SM2CR8V:~/SCR3.1/r305_dm/r305_dm/tp1-a-rendre$ ./main
            [...]
            Bloc 6 = allocated 42 bytes at 0x7ede4cba9198
            heap_base = 0x7ede4cba9000
            block at 0x7ede4cba9000: header=0x00000051 size=  80 flags=1 (taken)
            block at 0x7ede4cba9050: header=0x00000051 size=  80 flags=1 (taken)
            block at 0x7ede4cba90a0: header=0x00000051 size=  80 flags=1 (taken)
            block at 0x7ede4cba90f0: header=0x00000051 size=  80 flags=1 (taken)
            block at 0x7ede4cba9140: header=0x00000051 size=  80 flags=1 (taken)
            block at 0x7ede4cba9190: header=0x00000191 size= 400 flags=1 (taken)
            heap_end = 0x7ede4cba9320.
    ```

        Apres avoir changé le mem_alloc de 42 à 200, l'allocation se fait toujours aussi bien.




    Autres tests

    --> Par exemple, allouer de trop grande quantité par rapport aux allocations définies (en enlevant le assert =! NULL pour étudier le comportement)

    ```
    chloe@LAPTOP-9SM2CR8V:~/SCR3.1/r305_dm/r305_dm/tp1-a-rendre$ ./main
        Bloc 1 = allocated 42 bytes at 0x79792db3b008
        Bloc 2 = allocated 42 bytes at 0x79792db3b198
        Bloc 3 = allocated 42 bytes at 0x79792db3b058
        Bloc 4 = allocated 42 bytes at 0x79792db3b0a8
        Bloc 5 = allocated 42 bytes at 0x79792db3b0f8
        Bloc 6 = allocated 42 bytes at (nil)
        heap_base = 0x79792db3b000
        block at 0x79792db3b000: header=0x00000051 size=  80 flags=1 (taken)
        block at 0x79792db3b050: header=0x00000051 size=  80 flags=1 (taken)
        block at 0x79792db3b0a0: header=0x00000051 size=  80 flags=1 (taken)
        block at 0x79792db3b0f0: header=0x00000051 size=  80 flags=1 (taken)
        block at 0x79792db3b140: header=0x00000050 size=  80 flags=0 (free)
        block at 0x79792db3b190: header=0x00000191 size= 400 flags=1 (taken)
        heap_end = 0x7ede4cba9320.
    ```

    Dans ce cas j'ai essayé d'allouer 81 octets (limite fixé à 80) : le bloc est trop petit donc l'allocation n'a pas réussi. Le programme cherchera un autre bloc libre, dans notre cas, il n'y en a pas donc le bloc passe juste à un statut free.

    Mais grace au assert p5 =! NULL , le assert bloque l'allocation trop conséquente et evite ce genre d'erreur





# 2. Fragmentation interne
Modifiez la fonction mem_show_heap pour calculer et afficher la fragmentation interne de votre allocateur. Expliquez votre calcul.


    **Fragmentation interne** : innéficacite qui survient lorsque la mémoire est allouée en blocs de taille fixe, ce qui fait qu'une partie de la mémoire allouée n'est pas utilisée car la taille demandée ne conrrespond pas exactmenet à la taille du bloc (definition phoenixmap.fr)

    Pour calculer la fragmentation interne on prend chaque bloc alloué, on prend la taille entrée en argument et on compare avec la taille qui a réellement été alloué.
    donc : fragmentation = taillé alloué - taille réelle


    Donc j'observe dans la sortie standard comment trouver ces valeurs : 
    Par exemple pour le bloc 1 : 

    Bloc 1 = allocated 42 bytes at 0x7ddb03fc5008
    block at 0x7ddb03fc5050: header=0x00000051 size=  80 flags=1 (taken)

    Ici, on a voulu allouer 42 octets alors que le bloc total est de 80 octets, la fragmentation interne est donc de 38 octets ( donc 38 octets d'espace "perdu").

    Donc pour calculer la fragmentation on crée une variable globale qui va récupérer la taille allouée par l'utilisateur qui sera osustrait à la taille globale d'octet.
    donc le mem_alloc :             taille_utilisateur += save; // ce que voulait l'utilisateur
    et dans le mem_show_heap :      fragmentation = 800 - taille_utilisateur;
                                    printf("Fragmentation interne = %ld octets\n", fragmentation);




# 3. Recyclage des blocs désalloués.
Implémentez la fonction mem_release en marquant les blocs désalloués comme libre. Rajoutez dans votre main des tests. Vérifiez que vous pouvez allouer 200, désallouer, puis redemander 300.

    ```
    void mem_release(void *ptr)
    {
        assert( mem_initialized == 1);
        assert( ((int64_t)ptr % 8) == 0 ); // sanity check

        // TODO: your deallocation algorithm goes here

        int64_t *header = (int64_t*)(ptr-8);
        int64_t size = *header & ~0b111;
        *header = size;

        return;
    }
    ```

    Dans le main :

    ```
        char *p6 = mem_alloc(200); // bloc de 400 octets
        mem_release(p6);
        char *p7 = mem_alloc(300);
        mem_release(p7);
    ``` 

    ```
    chloe@LAPTOP-9SM2CR8V:~/BUT2/SCR3.1/r305_dm/r305_dm/tp1-a-rendre$ ./main
        Bloc 1 = allocated 42 bytes at 0x745f8f52f008
        Bloc 2 = allocated 42 bytes at 0x745f8f52f058
        Bloc 3 = allocated 42 bytes at 0x745f8f52f0a8
        Bloc 4 = allocated 42 bytes at 0x745f8f52f0f8
        Bloc 5 = allocated 42 bytes at 0x745f8f52f148
        Bloc 6 = allocated 200 bytes at 0x745f8f52f198
        Bloc 7 = APRES AVOIR DESALLOUE : allocated 300 bytes at 0x745f8f52f198
        heap_base = 0x745f8f52f000
        block at 0x745f8f52f000: header=0x00000051 size=  80 flags=1 (taken)
        block at 0x745f8f52f050: header=0x00000051 size=  80 flags=1 (taken)
        block at 0x745f8f52f0a0: header=0x00000051 size=  80 flags=1 (taken)
        block at 0x745f8f52f0f0: header=0x00000051 size=  80 flags=1 (taken)
        block at 0x745f8f52f140: header=0x00000051 size=  80 flags=1 (taken)
        block at 0x745f8f52f190: header=0x00000191 size= 400 flags=1 (taken)
        heap_end = 0x745f8f52f320
    ```

    On constate sur la sortie standard que le bloc a bien été désalloué l'adresse du 6 et la meme que le bloc 7 donc le contenu du bloc 6 est bien supprimé et l'espace mémoire a bien été réatribué.

    Explication du code :
    ptr - 8 car on va au début de l'en-tête et on caste ce résultat en int64_t
    On lit ensuite la valeur de l'en tete et on efface les 3 derniers but du *header comme dans le mem_show_heap puis on le met a jour dans size


# 4. Découpage des blocs trop grands

    **Dans la fonction mem_alloc, ajoutez le nécessaire pour découper le bloc choisi s'il s'avére plus grand que la taille demandée par l'utilisateur. Ajoutez des cas de tests pour mettre en évidence les améliorations :**

    ```
     if (size >= length + 8) { 
                int64_t size_restante = size - length;
                void *next_bloc = block_ptr + length;
                *((int64_t*)next_bloc) = size_restante | 0; 
                *((int64_t*)block_ptr) = length | 1; 
            }
    ```

    On regarde d'abord si le bloc n'est pas rempli.
    Si il y une fragmentation interne, on la calcule pour savoir l'espace restant et on calcule l'adresse du nouveau bloc libre 

        **Vérifiez que vous pouvez faire 7 allocations de taille 42.**

    
    Le code : 
    - si le bloc est assez grand, on le découpe en deux et on marque la partie utilisée comme allouée et le reste comme libre.

    Dans le main, j'ai du énelever les mem_release sinon j'avais que des core dumped.


    ```
        Bloc 1 = allocated 42 bytes at 0x766e71f0c008
        Bloc 2 = allocated 42 bytes at 0x766e71f0c058
        Bloc 3 = allocated 42 bytes at 0x766e71f0c0a8
        Bloc 4 = allocated 42 bytes at 0x766e71f0c0f8
        Bloc 5 = allocated 42 bytes at 0x766e71f0c148
        Bloc 6 = allocated 200 bytes at 0x766e71f0c198
        Bloc 7 = DECCOUPAGE : allocated 4 bytes at 0x766e71f0c040
        verif finale pas de core dumped
        heap_base = 0x766e71f0c000
        block at 0x766e71f0c000: header=0x00000039 size=  56 flags=1 (taken)
        block at 0x766e71f0c038: header=0x00000011 size=  16 flags=1 (taken)
        block at 0x766e71f0c048: header=0x00000008 size=   8 flags=0 (free)
        block at 0x766e71f0c050: header=0x00000039 size=  56 flags=1 (taken)
        block at 0x766e71f0c088: header=0x00000018 size=  24 flags=0 (free)
        block at 0x766e71f0c0a0: header=0x00000039 size=  56 flags=1 (taken)
        block at 0x766e71f0c0d8: header=0x00000018 size=  24 flags=0 (free)
        block at 0x766e71f0c0f0: header=0x00000039 size=  56 flags=1 (taken)
        block at 0x766e71f0c128: header=0x00000018 size=  24 flags=0 (free)
        block at 0x766e71f0c140: header=0x00000039 size=  56 flags=1 (taken)
        block at 0x766e71f0c178: header=0x00000018 size=  24 flags=0 (free)
        block at 0x766e71f0c190: header=0x000000d1 size= 208 flags=1 (taken)
        block at 0x766e71f0c260: header=0x000000c0 size= 192 flags=0 (free)
        heap_end = 0x766e71f0c320
    ```


    **Combien d'allocations successives de 42 octets peut-on faire en partant de l'état initial du tas ?**

    On arrondi à un multiple de 8 --> 48 + header (8) = 56
    800 / 56 = 14
    donc 14 blocs possibles pour de allocations de 42 octets.

    le mem_release échoue --> core dumped

# 5. BOUNDARY TAGS 

--> on duplique à la fin du bloc le header (footer) 

1. **supprimez les blocs libres créés dans mem_init, pour un seul bloc de 800 octets (avec header et footer):** 

    ```
    int64_t *header = (int64_t*)heap_base;
    int64_t *footer = (int64_t*)(heap_end - 8); // le heap end s'arrete mtn 8 octets avant pour le footer

    *header = 800; // écrire la taille totale dans le header
    *footer = 800; // same pour footer


2. **dans mem_shiow_heap, affichez également le footer.**
    
        int64_t *footer_ptr = (int64_t *)(block_ptr + size - 8);
        int64_t footer = *footer_ptr;

        [...]

        printf("footer at %p: 0x%08lx\n", footer_ptr, footer);
    
3. **dans mem_alloc et mem_release, chaque fois que vous changez le header d'un bloc, vous devez changer également le footer.**

            /mem_alloc

            if (size >= length + 16) { // +16 pour prendre en compte header et footer
                
                int64_t size_restante = size - length;
                
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
    

            /mem_release :

            void mem_release(void *ptr)
            {
            assert( mem_initialized == 1);
            assert( ((int64_t)ptr % 8) == 0 ); // sanity check

            // TODO: your deallocation algorithm goes here

            int64_t *header = (int64_t*)(ptr-8);
            int64_t size = *header & ~0b111;

            *header = size | 0;

            int64_t *footer = (int64_t*)(header + size - 8);
            *footer = size | 0;

            return;
            }

    --> Sortie Standard : 


```
    chloe@LAPTOP-9SM2CR8V:~/BUT2/SCR3.1/r305_dm/r305_dm/tp1-a-rendre$ ./main
        Bloc 1 = allocated 42 bytes at 0x7d4f8edac008
        Bloc 2 = allocated 42 bytes at 0x7d4f8edac040
        Bloc 3 = allocated 42 bytes at 0x7d4f8edac078
        Bloc 4 = allocated 42 bytes at 0x7d4f8edac0b0
        Bloc 5 = allocated 42 bytes at 0x7d4f8edac0e8
        Bloc 6 = allocated 200 bytes at 0x7d4f8edac120
        Bloc 7 = DECCOUPAGE : allocated 42 bytes at 0x7d4f8edac1f0
        verif finale pas de core dumped youpi
        heap_base = 0x7d4f8edac000
        block at 0x7d4f8edac000: header=0x00000039 size=  56 flags=1 (taken)
        footer at 0x7d4f8edac030: 0x00000039
        block at 0x7d4f8edac038: header=0x00000039 size=  56 flags=1 (taken)
        footer at 0x7d4f8edac068: 0x00000039
        block at 0x7d4f8edac070: header=0x00000039 size=  56 flags=1 (taken)
        footer at 0x7d4f8edac0a0: 0x00000039
        block at 0x7d4f8edac0a8: header=0x00000039 size=  56 flags=1 (taken)
        footer at 0x7d4f8edac0d8: 0x00000039
        block at 0x7d4f8edac0e0: header=0x00000039 size=  56 flags=1 (taken)
        footer at 0x7d4f8edac110: 0x00000039
        block at 0x7d4f8edac118: header=0x000000d1 size= 208 flags=1 (taken)
        footer at 0x7d4f8edac1e0: 0x000000d1
        block at 0x7d4f8edac1e8: header=0x00000039 size=  56 flags=1 (taken)
        footer at 0x7d4f8edac218: 0x00000039
        block at 0x7d4f8edac220: header=0x00000100 size= 256 flags=0 (free)
        footer at 0x7d4f8edac318: 0x00000100
        heap_end = 0x7d4f8edac320
```
    --> Donc on constate bien que chaque bloc est découpé selon sa taille et qu'il reste un dernier bloc de 256 octets.


# 6. Défragmentation du tas par fusion des blocs désalloués


    **Si les deux blocs voisins sont occupés, alors il suffit de** marquer simplement B comme libre mais pas de réelles fusions possibles


    **Si le bloc suivant S est libre mais pas le précédent P, alors il faut** fusionner B et S (maj de la taille)

    **Si le bloc précédent P est libre, mais pas le bloc suivnat S, alors il faut** fusionner P et S (+maj de la taille)


    **si les deux blocs voisins sont libres, alors il faut** fusionner les trois blocs 

        block at 0x7b3d951d4220: header=0x00000021 size=  32 flags=1 (taken)
        footer at 0x7b3d951d4238: 0x00000021
        block at 0x7b3d951d4240: header=0x00000020 size=  32 flags=0 (free)
        footer at 0x7b3d951d4258: 0x00000020
        block at 0x7b3d951d4260: header=0x00000021 size=  32 flags=1 (taken)
        footer at 0x7b3d951d4278: 0x00000021
        block at 0x7b3d951d4280: header=0x000000a0 size= 160 flags=0 (free)
        footer at 0x7b3d951d4318: 0x000000a0
        heap_end = 0x7b3d951d4320


    --> donc ici on constate que : le deuxieme bloc 32 bytes est à free et n'a pas fusionné par ses deux voisins sont taken et il ne possède aucun voisins disponibles.
    Le dernier bloc a subi plusiuers allocations et plusiuers mem_release (test 2, 3 et 4) et se retrouve toujours en un seul bloc donc la fusion a correctement fonctionné.