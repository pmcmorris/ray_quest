void write_ppm(void* rgb, int width, int height, const char* filename);
int do_conversion();
void grayScale(const unsigned char* rgbaImage, float* output, const int width, const int height, const int channels);

