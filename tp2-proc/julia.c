#include "julia.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	int height,width ;
	unsigned char* img = NULL;

	// YOUR CODE HERE

    // ============ ex 1.1

    // void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);

    height = atoi(argv[1]);
    width = height*2;
    size_t size = width * height * 3;

    img= mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    // ============= ex 1.2

    //compute_julia_pixel(int x, int y, int width, int height, float tint_bias, unsigned char *rgb) 

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

    
    // ============= ex 1.3

	FILE *out=fopen("outfile.bmp","w");
	assert(out != NULL);	
    write_bmp_header(out, width, height);
	fwrite(img, width*height*3, 1, out);
	fclose(out);


    // ========= EXERCICE 2 

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

    // ============== EXERCICE 3 

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
    for(int y=0; y<height; y++){
        write(t[1], &y, sizeof(int));
    }
    close(t[1]);

    for(int p=0; p<nb_proc; p++) wait(NULL);

    FILE *out3 = fopen("3outfile.bmp","w");
    assert(out3 != NULL);
    write_bmp_header(out3, width, height);
    fwrite(img, size, 1, out3);
    fclose(out3);


	return 0;
}

