#include <iostream>
#include <fstream>
#include <cmath>
#include <complex>
#include <array>

#define MAX_ITER 50
#define BOUND 4
#define WIDTH 1000
#define HEIGHT 1000
#define RANGE_Y 2.0
#define RANGE_X 2.5
#define X_START -1.0
#define Y_START 0.0


struct Pixel {
  int r;
  int g;
  int b;
};


void save_ppm(const std::array<int, WIDTH*HEIGHT>& pixels, const char* filepath) {
  std::ofstream file_out(filepath);
  file_out << "P3" << std::endl;
  file_out << WIDTH << " " << HEIGHT << std::endl;
  file_out << 255 << std::endl;
  for (const int& pixel: pixels) {
    file_out << pixel*2.5 << " " << pixel * 2.5 << " " << pixel << std::endl;
  }
}


int intercount(const std::complex<double>& c) {
  std::complex<double> z{};
  int n;
  for ( n = 0; n < MAX_ITER && abs(z) < BOUND; n++) {
    z += z*z + c;
  }
  return n;
}



int main(int argc, char ** argv) {
  std::array<int, WIDTH*HEIGHT> pixels;
  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {

      std::complex<double> c{
        X_START + ((double)x/WIDTH) * RANGE_X - RANGE_X/2,
        Y_START + ((double)y/HEIGHT) * RANGE_Y - RANGE_Y/2
      };
      pixels[y*WIDTH+x] = 10 * intercount(c);

    }
  }

  save_ppm(pixels, "mandlebrot.ppm");
  return 0;
}
