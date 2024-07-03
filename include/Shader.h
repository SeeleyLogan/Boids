#ifndef SHADER_H
#define SHADER_H

#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <Macros.h>

typedef struct Shader
{
    uint32_t ID;
    int32_t* uniforms;
} Shader;

const char* loadShaderFile(const char* shaderFileName);
uint32_t compileShader(const char* shaderSource, int32_t shaderType);
Shader newShader(const char** shaderFiles, int32_t* shaderTypes, uint8_t shaderCount);
void useShader(Shader* shader);

#endif