#ifndef MANDLEBROT_H
#define MANDLEBROT_H

#include <complex>


struct Color {
  float r;
  float g;
  float b;
};


using ColorFunction=Color (*)(double);


class Mandelbrot {
  public:
    Mandelbrot(ColorFunction color_f, int max_iter, float bound);

    Color get_color(double x, double y);

  private:
    int iterate(const std::complex<double>& number);

    int max_iter;
    float bound;
    ColorFunction color_f;
};


#endif
