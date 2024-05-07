#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Camera.hpp"
#include "Renderer.hpp"
#include "StaticLight.hpp"
#include "StoneObject.hpp"
#include "TreeObject.hpp"
#include "glTools.hpp"
#include "timer.hpp"

const unsigned int SCREEN_WIDTH = 1280;
const unsigned int SCREEN_HEIGHT = 960;

class Engine {
public:
    Engine();
    GLFWwindow* Init();
    void Run();

private:
    void CalculateFPS();
    bool CheckDrawTime(const int fps);
    void Prepare();

private:
    GLFWwindow* window;
   
    Renderer m_Renderer;
    Timer m_FrameTimer;
    Timer m_GlobalTimer;

    int m_Width, m_Height;

    long long int m_FPSValue = 0;
    int m_FrameCnt = 0;
};