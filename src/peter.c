#include <stdio.h>
#include <stdlib.h>
#include "peter.h"
#include "stb_image.h"
#include "stb_image_write.h"
// http://paulcuth.me.uk/netpbm-viewer/
//
void write_ppm(void* rgb, int width, int height, const char* filename) {
    size_t image_size = 3 * width * height;

    FILE* f = fopen(filename, "wb");
    // TODO: check for errors

    // write the file header
    fprintf(f, "P6\n%d %d\n255\n", width, height);

    // write the pixel data
    fwrite(rgb, 1, image_size, f);

    // do stuff
    fflush(f);
    fclose(f);
}

int do_conversion() {
    printf("Pistol's Quest! Day 1\n");
	int width,height,channels;
	
    unsigned char *data = stbi_load("white.png", &width, &height, &channels, 0);
	float *x_grad;
	float *y_grad;
	float *grayImage;
	x_grad = (float *) malloc(width*height*4);
	y_grad = (float *) malloc(width*height*4);
	grayImage = (float *) malloc(width*height*4);
	grayScale(data, grayImage, width, height, channels);
    //pixel * image = malloc(width*height*channels);
    stbi_write_png("grayImage.png", width, height, channels, data, width*channels);
    // generate some image data
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width-1; ++x) {
            int index = (y*width + x);
            x_grad[index] = (grayImage[(index+1)] - grayImage[index])/2;
			x_grad[index+1] = 0;
        }
    }
	for (int y = 0; y < height-1; ++y) {
        for (int x = 0; x < width; ++x) {
            int index = (y*width + x);
            y_grad[index] = (grayImage[(index+width)] - grayImage[index])/2;
			y_grad[index+width] = 0;
        }
    }
	for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int index = (y*width + x);
            data[index*channels] = (char)(x_grad[index] + 128);
			data[index*channels+1] = data[index*channels];
			data[index*channels+2] = data[index*channels];
			data[index*channels+3] = 255;
        }
    }
	stbi_write_png("x_grad.png", width, height, channels, data, width*channels);
	for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int index = (y*width + x);
            data[index*channels] = (char)(y_grad[index] + 128);
			data[index*channels+1] = data[index*channels];
			data[index*channels+2] = data[index*channels];
			data[index*channels+3] = 255;
        }
    }
	stbi_write_png("y_grad.png", width, height, channels, data, width*channels);
	
	for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int index = (y*width + x);
            data[index*channels] = (char)(grayImage[index]);
			data[index*channels+1] = data[index*channels];
			data[index*channels+2] = data[index*channels];
			data[index*channels+3] = 255;
        }
    }
	stbi_write_png("grayImage.png", width, height, channels, data, width*channels);
    // write_ppm(image, width, height, "hello.ppm");
	stbi_image_free(data);
    return 0;
}
void grayScale(const unsigned char* rgbaImage, float* output, const int width, const int height, const int channels){

	for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
			int index = (y*width + x);
			output[index] = 0.299f*rgbaImage[index*channels];
			output[index] += 0.587f*rgbaImage[index*channels+1];
			output[index] += 0.114f*rgbaImage[index*channels+2];

        }
    }
}
