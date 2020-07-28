#include "Mandlebrot.h"

#include <cmath>


Mandlebrot::Mandlebrot(ColorFunction color_f, int max_iter, float bound) :
  max_iter(max_iter),
  bound(bound),
  color_f(color_f)
{
}


Color Mandlebrot::get_color(double x, double y) {
  std::complex<double> c {x, y};
  return color_f(
      (double)iterate(c)/max_iter
      );
}


int Mandlebrot::iterate(const std::complex<double>& c) {
  std::complex<double> z{};
  int n;
  for ( n = 0; n < max_iter && abs(z) < bound; n++) {
    z += z*z + c;
  }
  return n;
}
