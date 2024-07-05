#ifndef RENDER_H
#define RENDER_H

#include <stdint.h>

#ifndef __gl_h_
#include <glad/glad.h>
#endif
#include <GLFW/glfw3.h>

#include <stb/stb_ds.h>

#include <App.h>
#include <Callbacks.h>
#include <Shader.h>
#include <SSBO.h>
#include <Macros.h>
#include <Debug.h>
#include <Globals.h>

uint32_t VAO;

void renderSetup(App* app);
void render(App* app);
void createShaders(App* app);
double GetTime();

#endif