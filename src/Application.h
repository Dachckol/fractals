#ifndef APPLICATION_H
#define APPLICATION_H


#include "Mandelbrot.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <complex>


struct GLFWWindowDeleter {
  void operator()(GLFWwindow* window) {
    glfwDestroyWindow(window);
  }
};


class Application {
public:
  Application();
  Application(const Application&) = delete;
  Application(const Application&&) = delete;
  Application& operator=(const Application&) = delete;
  Application& operator=(Application&&) = delete;

  GLFWwindow* start(const char* title);
  void loop();

  ~Application();

private:
  std::unique_ptr<GLFWwindow, GLFWWindowDeleter> window;


  std::complex<double> centre;
  double range;
  int current_width;
  int current_height;

  static int width;
  static int height;

  unsigned int shader_program;
  unsigned int VBO;
  unsigned int VAO;

  void fill_buffer();

  static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

};


#endif
