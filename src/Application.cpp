#include "Application.h"

#include <iostream>
#include <vector>
#include <cassert>


int Application::width = 100;
int Application::height = 100;


const char* shader_source = R"(
#version 330 core

layout (location = 0) in vec3 aPos;

void main() {
  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}

)";

const char* fragment_source = R"(
#version 330 core
precision highp float;

out vec4 FragColor;

uniform vec3   color_scales;
uniform int    max_iter;
uniform float  bound;
uniform vec2   origin;
uniform vec2   step;

void main()
{
  vec2 c = vec2(
    gl_FragCoord.x*step.x + origin.x,
    gl_FragCoord.y*step.y + origin.y
  );
  vec2 z = c;

  int iter = 0;
  for(; sqrt(z.x*z.x + z.y*z.y) < bound && iter < max_iter; iter++) {
    z = vec2((z.x*z.x) - (z.y*z.y) + c.x, (2*z.x*z.y) + c.y);
  }

  vec3 color = color_scales * float(iter)/float(max_iter);
  float trans = 1.0 - float(iter)/float(max_iter);

  FragColor = vec4(color.xyz, 0.0);
}

)";

Application::Application() :
  window(nullptr),
  centre{0,0},
  range(4),
  current_width(Application::width),
  current_height(Application::height)
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}


GLFWwindow* Application::start(const char* title) {
  window.reset(glfwCreateWindow(width, height, title, NULL, NULL));
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return nullptr;
  }
  glfwMakeContextCurrent(window.get());
  glfwSetFramebufferSizeCallback(window.get(), framebuffer_size_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return nullptr;
  }

//vertext shader
  char infoLog[512];
  unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &shader_source, NULL);
  glCompileShader(vertex_shader);
  int  success;
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
  if(!success)
  {
    glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

// fragment shader
  unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragment_source, NULL);
  glCompileShader(fragment_shader);
  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
  if(!success)
  {
    glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

// link shaders
  shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glLinkProgram(shader_program);

  glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
  }

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  fill_buffer();
//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  return window.get();
}



void Application::loop() {
  while (!glfwWindowShouldClose(window.get())) {
    if (current_width != width || current_height != height) {
      fill_buffer();
      current_width = width;
      current_height = height;
    }
    glClearColor(0, 1, 1, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    int color_scales_location = glGetUniformLocation(shader_program, "color_scales");
    int max_iter_location = glGetUniformLocation(shader_program, "max_iter");
    int bound_location = glGetUniformLocation(shader_program, "bound");
    int origin_location = glGetUniformLocation(shader_program, "origin");
    int step_location = glGetUniformLocation(shader_program, "step");

    assert(color_scales_location != -1);
    assert(max_iter_location != -1);
    assert(bound_location != -1);
    assert(origin_location != -1);
    assert(step_location != -1);

    glUseProgram(shader_program);
    glUniform3f(color_scales_location, 1, 1, 0);
    glUniform1i(max_iter_location, 50);
    glUniform1f(bound_location, 1000);
    glUniform2f(origin_location, centre.real() - range/2, centre.imag() - range/2);
    glUniform2f(step_location, range/current_width, range/current_height);

    glBindVertexArray(VAO);

    glDrawArrays(GL_POINTS, 0, current_width*current_height);
    glBindVertexArray(0);
    glfwSwapBuffers(window.get());
    glfwPollEvents();
  }
}


void Application::fill_buffer() {
  std::vector<GLfloat> vertices(width*height * 3);

  int count = 0;
  for (int y = 0; y < height; y++ ) {
    for (int x = 0; x < width; x++ ) {
      int index = 3*(x + width*y);
      vertices[index+0] = (float)x*2.0/width  - 1;
      vertices[index+1] = (float)y*2.0/height - 1;
      vertices[index+2] = 0.0f;
    }
  }

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}


Application::~Application() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shader_program);
  glfwTerminate();
}


void Application::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  std::cout << width << "," << height << std::endl;
  glViewport(0, 0, width, height);
  Application::width = width;
  Application::height = height;
}
