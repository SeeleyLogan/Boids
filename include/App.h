#ifndef APP_H
#define APP_H

#ifndef __gl_h_
#include <windows.h>
#include <glad/glad.h>
#endif
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <stb/stb_ds.h>

#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include <Shader.h>
#include <SSBO.h>
#include <Macros.h>

typedef struct App
{
    GLFWwindow* window;
    HWND hwnd;
    const GLFWvidmode* vidmode;
    Shader* shaders;
    SSBO_a* SSBO_arrays;
    void** mappedBuffers;
} App;

#include <Update.h>
#include <Render.h>

App initApp();
void setWindowHints();
void initWindow(GLFWwindow** window, const GLFWvidmode** vidmode);
void run(App* app);

#endif