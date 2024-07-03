#include <Render.h>

void renderSetup(App* app)
{
    glfwSetKeyCallback(app->window, quit);
    glfwSwapInterval(0);

    glCheck( glClearColorI(0, 0, 0, 0) );
    glCheck( glViewport(0, 0, app->vidmode->width, app->vidmode->height) );

    glCheck( glGenVertexArrays(1, &VAO) ); 
    glCheck( glBindVertexArray(VAO) );

    createShaders(app);
}

void render(App* app)
{
    SetWindowPos(app->hwnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

    bindSSBO_a(&app->SSBO_arrays[0]);

    glCheck( glUseProgram(app->shaders[0].ID) );

    float aspectRatio = (float) app->vidmode->width/(float) app->vidmode->height;
    mat4 orangutanProj = GLM_MAT4_IDENTITY_INIT;
    glm_ortho(-100.0f*aspectRatio, 100.0f*aspectRatio, -100.0f, 100.0f, -1.0f, 1.0f, orangutanProj);
    glCheck( glUniformMatrix4fv(app->shaders[0].uniforms[0], 1, GL_FALSE, orangutanProj[0]) );

    glCheck( glUniform1f(app->shaders[0].uniforms[1], BOID_SIZE) );

    glCheck( glEnable(GL_DEPTH_TEST) );
    glCheck( glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) );

    glCheck( glDrawArrays(GL_POINTS, 0, app->SSBO_arrays[0].vert_count) );

    glfwSwapBuffers(app->window);
    glfwPollEvents();
}

void createShaders(App* app)
{
    const char* shaderFiles[] = {
        "assets\\shaders\\BoidVert.glsl",
        "assets\\shaders\\BoidGeom.glsl",
        "assets\\shaders\\BoidFrag.glsl"
    };

    int32_t shaderTypes[] = {
        GL_VERTEX_SHADER,
        GL_GEOMETRY_SHADER,
        GL_FRAGMENT_SHADER
    };

    Shader basicShader = newShader(shaderFiles, shaderTypes, len(shaderTypes));

    glCheck( int32_t orangutanProjUniLoc = glGetUniformLocation(basicShader.ID, "orangutanProj") );
    glCheck( int32_t boidSizeUniLoc = glGetUniformLocation(basicShader.ID, "boidSize") );

    arrput(basicShader.uniforms, orangutanProjUniLoc);
    arrput(basicShader.uniforms, boidSizeUniLoc);

    arrput(app->shaders, basicShader);
}
