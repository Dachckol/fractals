#include <iostream>
#include <fstream>
#include <array>

#include "Mandlebrot.h"

#define WIDTH  15000
#define HEIGHT 15000

#define RANGE_Y 0.15
#define RANGE_X 0.15

#define X_START -1.2
#define Y_START -0.3


Color get_color(double ratio) {
  return Color{
    (int)(255 * ratio),
    (int)(255 * (1.0/ratio)),
    (int)(255 * ratio)
  };
}



int main(int argc, char ** argv) {
  if (argc < 3) {
    std::cout << "Usage: bound max_iter" << std::endl;
    return 1;
  }

  const int bound = std::atoi(argv[1]);
  const int max_iter = std::atoi(argv[2]);

  Mandlebrot mandlebrot(
      &get_color,
      max_iter,
      bound
      );

  double x_step = RANGE_X / WIDTH;
  double y_step = RANGE_Y / HEIGHT;


  std::ofstream file_out("mandlebrot.ppm");
  file_out << "P3" << std::endl;
  file_out << WIDTH << " " << HEIGHT << std::endl;
  file_out << 255 << std::endl;

  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {

      Color color = mandlebrot.get_color(
          x_step * x + X_START,
          y_step * y + Y_START
      );
      file_out << color.r << " "
               << color.g << " "
               << color.b << std::endl;

    }
  }

  return 0;
}
