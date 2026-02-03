# TP 2 A RENDRE - JULIA

## EXERCICE 1 

### 1.1 

> alloue suffisamment de mémoire pour stocker l'image (vousutiliserez mmap). La résolution verticale nn sera passée sur la ligne de commande, la résolution horizontale sera 2n2n. Il faut donc un tableau de taille n×(2n)×3

``` c

 height = atoi(argv[1]);
    width = height*2;
    size_t size = width * height * 3;

    img= mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

```

--> Definition de la longueur / largeur 

--> creation d'un espace memoire avec mmap
Pour les arguments de mmap : http://manpagesfr.free.fr/man/man2/mmap.2.html

La fonction de base est : 

void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);

PROT_READ | PROT_WRITE ==> lire et écrire sur la szone de mémoire

MAP_PRIVATE | MAP_ANONYMOUS ==> unique processus, un seul bloc de memoire vierge 


### 1.2

> calcul l'image

``` c

    float tint_bias = 1.0;
    unsigned char rgb[3];

    for (int y = 0; y<height; y++){
        for (int x = 0 ; x<width ; x++ ){
            compute_julia_pixel(x, y, width, height, tint_bias, rgb); 
            int pixel = (y * width + x) * 3;
            img[pixel] = rgb[0];
            img[pixel+1] = rgb[1];
            img[pixel+2] = rgb[2];
        }
    }


```
Le pos calcule la position

Dans une boucle qui parcours en largeur et longueur, je réfinis la couleur des pixel avec 

    - 0 qui vaut rouge

    - 1 qui est le vert

    - 2 qui est le bleu



### 1.3

> sauvegarde l'image

``` c
	FILE *out=fopen("outfile.bmp","w");
	assert(out != NULL);	
    write_bmp_header(out, width, height);
	fwrite(img, width*height*3, 1, out);
	fclose(out);
```


## EXERCICE 2

> Écrire une version parallèle du programme. Vous prendrez sur la ligne de commande le nombre de processus. Chaque processus fera le calcul d'une bande de l'image. Ajustez les paramètres de mmap de manière à pouvoir partager le tableau de pixels entre les processus.

``` c
    pid_t p;
    int nb_proc = atoi(argv[2]);

    for (int i = 0 ; i< nb_proc ; i++){
        p = fork();

        if (p==0){
            int y1 = i*height / nb_proc;
            int y2 = (i+1)*height/nb_proc; 


            for (int y3 = y1; y3 < y2; y3++) {
                for (int x3 = 0; x3 < width; x3++) {
                    compute_julia_pixel(x3, y3, width, height, tint_bias, rgb);
                    int pixel = (y3 * width + x3) * 3;
                    img[pixel] = rgb[0];
                    img[pixel+1] = rgb[1];
                    img[pixel+2] = rgb[2];
                }
           // printf("%d = de %d à %d\n", i, y1, y3);

            }
            exit(0);
        }
    }

    for(int i=0; i<nb_proc; i++) wait(NULL); // on attend les fils


    FILE *out2=fopen("2outfile.bmp","w");
	assert(out2 != NULL);	
    write_bmp_header(out2, width, height);
	fwrite(img, width*height*3, 1, out2);
	fclose(out2);
```

On définis d'abord :

    - le pid 

    - le nb de fils

Si c'est un fils, on calcul une bande distincte de l'image sans chevauchement (+1)

y1 --> debut de la bande & y2 --> fin de la bande


``` bash
chloe@LAPTOP-9SM2CR8V:~/BUT2/SCR3.1/r305_dm/r305_dm/tp2-a-rendre$ make
gcc -c julia.c
gcc -o julia julia.o write_bmp_header.o compute_julia_pixel.o -lm
chloe@LAPTOP-9SM2CR8V:~/BUT2/SCR3.1/r305_dm/r305_dm/tp2-a-rendre$ ./julia 15 3
0 = de 0 à 0
0 = de 0 à 1
0 = de 0 à 2
0 = de 0 à 3
0 = de 0 à 4
1 = de 5 à 5
1 = de 5 à 6
1 = de 5 à 7
1 = de 5 à 8
1 = de 5 à 9
2 = de 10 à 10
2 = de 10 à 11
2 = de 10 à 12
2 = de 10 à 13
2 = de 10 à 14
```

Ainsi on constate bien au terminal 3 processus en train de travailler et qui se sont répartis les taches.


## EXERCICE 3 

> Certaines bandes de l'image ne nécessitent pas toujours le même temps de calcul. Pour équilibrer les traitement entre les différents processus, on partionnera l'image, et chaque partie représentera une tâche à effectuer. Tant qu'il reste des tâches, un processus en prend une, fait le calcul, et recommence. C'est à vous à faire le choix d'une solution correcte.

``` c
int t[2];
    assert(pipe(t)==0);

    for (int i = 0 ; i< nb_proc ; i++){
        pid_t p = fork();
        if(p<0) break;

        if (p==0){
            close(t[1]);
            dup2(t[0], STDIN_FILENO);
            close(t[0]);

            int y;
            while(read(STDIN_FILENO, &y, sizeof(int))>0){
                for(int x=0; x<width; x++){
                    compute_julia_pixel(x, y, width, height, tint_bias, rgb);
                    int pixel = (y*width + x)*3;
                    img[pixel] = rgb[0];
                    img[pixel+1] = rgb[1];
                    img[pixel+2] = rgb[2];
                }
            }
            exit(0);
        }
    }

    close(t[0]); 
    for(int y=0; y<height; y++){ // toutes les lignes de calcul sont envoyés au fils avec le tube
        write(t[1], &y, sizeof(int));
    }
    close(t[1]);

    for(int p=0; p<nb_proc; p++) wait(NULL);

    FILE *out3 = fopen("3outfile.bmp","w");
    assert(out3 != NULL);
    write_bmp_header(out3, width, height);
    fwrite(img, size, 1, out3);
    fclose(out3);
```

Pour cette question, j'ai fais un tube pour optimiser le temps de calcul par le fils. Les fils lisent les valeurs depuis y depuis le tube puis calcule les pixel. Ensuite le pere lit les lignes y et attend la fin des fils. Puis la sauvegarde.