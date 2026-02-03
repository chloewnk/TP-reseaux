#include <stdio.h>

/* Constants defining the view to render */
// "Zoom in" to a pleasing view of the Julia set
#define X_MIN -1.6
#define X_MAX  1.6
#define Y_MIN -0.9
#define Y_MAX  0.9

// Point that defines the Julia set (c in the pdf)
#define julia_real -.79
#define julia_img   .15

/* Prototypes of the provided functions -- see each file for the functions' documentation */
int compute_julia_pixel(int x, int y, int width, int height, float tint_bias, unsigned char *rgb);
int write_bmp_header(FILE *f, int width, int height);
