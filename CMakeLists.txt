cmake_minimum_required(VERSION 3.3)
project(bouncing_ball)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
add_compile_options(-std=c++11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

include_directories("/usr/include")
find_library(OIIO "OpenImageIO" "/usr/lib")
find_library(GLUT "glut")
find_library(GL "GL")
find_library(GLU "GLU")

set(SOURCE_FILES BouncingBall.cpp Utility.cpp Camera.cpp Matrix.cpp Vector.cpp Ball.cpp Box.cpp Solver.cpp)
add_executable(bouncing_ball ${SOURCE_FILES})
target_link_libraries(bouncing_ball ${GLUT} ${GL} ${GLU} ${OIIO})
