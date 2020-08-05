cmake_minimum_required(VERSION 3.14.3)

project(fractal_generation LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB PROJ_SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/**/*.cpp)


add_subdirectory(glfw glfw)
add_subdirectory(glad glad)



add_executable(mandelbrot)
target_link_libraries(mandelbrot PRIVATE
  glfw
  glad
  )
target_include_directories(mandelbrot PRIVATE
  ${PROJECT_SOURCE_DIR}/src
  )
target_sources(mandelbrot PRIVATE ${PROJ_SOURCES})
set_target_properties(mandelbrot PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build")
