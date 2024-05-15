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

constexpr float to_radian(float angle) { return angle * 3.14 / 180; }

#define GLCall(x)   \
    GLClearError(); \
    x;              \
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

bool GLLogCall(const char* function, const char* file, int line);
void GLClearError();

inline constexpr double calcPow(float x, int y) {
    float temp = 0;
    if (y == 0)
        return 1;
    temp = calcPow(x, y / 2);
    if ((y % 2) == 0) {
        return temp * temp;
    } else {
        if (y > 0)
            return x * temp * temp;
        else
            return (temp * temp) / x;
    }
}