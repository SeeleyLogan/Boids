#include <App.h>

App initApp()
{
    srand((uint32_t) time(NULL));
    
    App app = {
        NULL,
        NULL
    };

    glfwInit();

    setWindowHints();
    initWindow(&app.window, &app.vidmode);

    app.hwnd = glfwGetWin32Window(app.window);

    if (app.window == NULL)
    {
        println("error: Failed to create GLFW window");
        exit(2);
    }

    glfwMakeContextCurrent(app.window);
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        println("error: GLAD failed to initialize");
        exit(3);
    }

    return app;
}

void setWindowHints()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_DECORATED, GL_TRUE);
    glfwWindowHint(GLFW_SCALE_TO_MONITOR , GL_TRUE);
}

void initWindow(GLFWwindow** window, const GLFWvidmode** vidmode)
{
    *vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    *window = glfwCreateWindow((*vidmode)->width, (*vidmode)->height, "Boids Simulation", NULL, NULL);
    glfwMakeContextCurrent(*window);

    glfwSetWindowPos(*window, 0, 0);
}

void run(App* app)
{
    updateSetup(app);
    renderSetup(app);

    #ifdef WIREFRAME_DEBUG
    glCheck( glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) );
    #endif

    while (!glfwWindowShouldClose(app->window))
    {
        update(app);
        render(app);
    }

    glfwDestroyWindow(app->window);
    glfwTerminate();
}

double GetTime()
{
    uint64_t fr,t1;
    QueryPerformanceFrequency((void*)&fr);
    QueryPerformanceCounter((void*)&t1);
    return (double)t1/(double)fr;
}