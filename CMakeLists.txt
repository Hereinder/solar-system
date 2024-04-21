
cmake_minimum_required(VERSION 3.10)
project(STLViewer)
set(CMAKE_BUILD_TYPE Debug)
# Find OpenGL and GLUT
find_package(OpenGL REQUIRED)
set(GLUT_INCLUDE_DIR "C:/Program Files (x86)/msys2/mingw64")
set(GLUT_LIBRARY -lglfw3)
set(GLEW_LIBRARY -lglew32)



# Add executable
add_executable(STLViewer main.cpp)

# Include OpenGL and GLUT directories
target_include_directories(STLViewer PRIVATE ${GLUT_INCLUDE_DIR})

message("Value of VAR_NAME: ${OPENGL_INCLUDE_DIRS}")
# Link OpenGL and GLUT libraries
target_link_libraries(STLViewer PRIVATE ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARY})