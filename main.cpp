#include <GL/glut.h>

#include <fstream>
#include <iostream>
#include <vector>

struct Vertex {
    float x, y, z;
};

struct Triangle {
    Vertex v1, v2, v3;
};

std::vector<Triangle> readSTL(const std::string& filename) {
    std::vector<Triangle> triangles;
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return triangles;
    }

    file.seekg(80);  // Skip the header
    uint32_t numTriangles;
    file.read(reinterpret_cast<char*>(&numTriangles), sizeof(numTriangles));

    for (uint32_t i = 0; i < numTriangles; ++i) {
        Triangle triangle;
        file.read(reinterpret_cast<char*>(&triangle), sizeof(triangle));
        triangles.push_back(triangle);
        file.seekg(2, std::ios::cur);  // Skip attribute byte count
    }

    file.close();
    return triangles;
}

std::vector<Triangle> triangles;

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Set up your camera position and orientation here
    gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);  // Example camera position

    // Render the triangles
    glBegin(GL_TRIANGLES);
    for (const auto& triangle : triangles) {
        // Vertex 1
        glVertex3f(triangle.v1.x, triangle.v1.y, triangle.v1.z);
        // Vertex 2
        glVertex3f(triangle.v2.x, triangle.v2.y, triangle.v2.z);
        // Vertex 3
        glVertex3f(triangle.v3.x, triangle.v3.y, triangle.v3.z);
    }
    glEnd();

    glutSwapBuffers();
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Load STL file
    triangles = readSTL("/home/kdudka/Projects/OpenGl/simple-aerodynamic/files/test.stl");
}

int main(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("STL Viewer");

    // Initialize OpenGL
    init();

    // Set up callbacks
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
