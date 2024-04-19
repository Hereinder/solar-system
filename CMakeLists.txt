cmake_minimum_required(VERSION 3.10)
project(STLViewer)

# Set preference for GLVND
set(OpenGL_GL_PREFERENCE "GLVND")
set(CMAKE_BUILD_TYPE Debug)

# Find OpenGL package
find_package(OpenGL REQUIRED)

# Manually specify GLUT include directory and library
set(GLUT_INCLUDE_DIR /usr/include/GL)
set(GLUT_LIBRARY -lglut)

# Add executable
add_executable(STLViewer main.cpp)

# Include OpenGL and GLUT directories
target_include_directories(STLViewer PRIVATE ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIR})

# Link OpenGL and GLUT libraries
target_link_libraries(STLViewer PRIVATE ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})