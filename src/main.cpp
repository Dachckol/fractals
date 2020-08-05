#include <iostream>

#include "Application.h"


int main() {
  {
    Application app{};
    app.start("Mandelbrot Set");
    app.loop();

  }

  return 0;
}
