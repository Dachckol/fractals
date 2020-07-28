#ifndef MANDLEBROT_H
#define MANDLEBROT_H

#include <complex>


struct Color {
  int r;
  int g;
  int b;
};


using ColorFunction=Color (*)(double);


class Mandlebrot {
  public:
    Mandlebrot(ColorFunction color_f, int max_iter, float bound);

    Color get_color(double x, double y);

  private:
    int iterate(const std::complex<double>& number);

    int max_iter;
    float bound;
    ColorFunction color_f;
};


#endif
