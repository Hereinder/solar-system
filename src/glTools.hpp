#pragma once

#ifdef __linux__
// linux code goes here
#include <signal.h>
#define ASSERT(x) \
    if (!(x))     \
        __debugbreak();
#elif _WIN32
// windows code goes here
#define ASSERT(x) \
    if (!(x))     \
        __debugbreak();
#else

#endif

#define GLCall(x)   \
    GLClearError(); \
    x;              \
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

bool GLLogCall(const char* function, const char* file, int line);
void GLClearError();