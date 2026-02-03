all : julia

julia.o:julia.c
	gcc -c julia.c

compute_julia_pixel.o:compute_julia_pixel.c
	gcc -c compute_julia_pixel.c

write_bmp_header.o:write_bmp_header.c
	gcc -c write_bmp_header.c

julia:julia.o write_bmp_header.o compute_julia_pixel.o
	gcc -o julia julia.o write_bmp_header.o compute_julia_pixel.o -lm
