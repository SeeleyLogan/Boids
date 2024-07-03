#include <Shader.h>

const char* loadShaderFile(const char* shaderFileName)
{
    char * buffer = 0;
    int64_t length = 0;
    FILE * f = fopen(shaderFileName, "rb");

    if (f)
    {
        fseek (f, 0, SEEK_END);
        length = ftell(f);
        fseek (f, 0, SEEK_SET);
        
        buffer = malloc(length);
        mallocCheck(buffer);

        fread(buffer, 1, length, f);
        buffer[length] = 0;

        fclose (f);
    } else {
        println("error: Failed to open %s", shaderFileName);
        exit(5);
    }

    return buffer;
}

uint32_t compileShader(const char* shaderSource, int shaderType)
{
    glCheck( uint32_t shader = glCreateShader(shaderType) );

    glCheck( glShaderSource(shader, 1, &shaderSource, NULL) );
    glCheck( glCompileShader(shader) );

    int  success;
    char infoLog[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        println("error: Shader compilation failed: %1024s", infoLog);
    }

    return shader;
}

Shader newShader(const char** shaderFiles, int32_t* shaderTypes, uint8_t shaderCount)
{
    Shader shader = {
        0
    };

    uint32_t* shaders = (uint32_t*) malloc(shaderCount*4);  // Array of compiled shader sources
    mallocCheck(shaders);

    glCheck( uint32_t shaderProgram = glCreateProgram() );

    // Load, compile and attach shaders
    for (uint8_t i=0; i<shaderCount; i++)
    {
        const char* shaderSource = loadShaderFile(shaderFiles[i]);
        shaders[i] = compileShader(shaderSource, shaderTypes[i]);
        glCheck( glAttachShader(shaderProgram, shaders[i]) );
        glCheck( glDeleteShader(shaders[i]) );
    }

    glCheck( glLinkProgram(shaderProgram) );

    int  success;
    char infoLog[1024];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 1024, NULL, infoLog);
        println("error: Shader program linking failed: %1024s", infoLog);
    }

    shader.ID = shaderProgram;

    return shader;
}
