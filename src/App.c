#include <App.h>

App initApp()
{
    timeBeginPeriod(1);
    srand((uint32_t) time(NULL));
    
    App app = { NULL };

    glfwInit();

    setWindowHints();
    initWindow(&app.window, &app.vidmode);

    if (!app.window)
    {
        println("error: Failed to create GLFW window");
        exit(2);
    }

    // Pass input through transparent window
    app.wnd_hwnd =  glfwGetWin32Window(app.window);
    SetWindowLongPtr(app.wnd_hwnd, GWL_EXSTYLE, WS_EX_TOOLWINDOW);
    LONG_PTR style = GetWindowLongPtr(app.wnd_hwnd, GWL_EXSTYLE);
    style |= WS_EX_LAYERED | WS_EX_TRANSPARENT;
    SetWindowLongPtr(app.wnd_hwnd, GWL_EXSTYLE, style);

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
    glfwWindowHint(GLFW_DECORATED, GL_FALSE);
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

        SetWindowPos(app->wnd_hwnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

        Sleep(1);
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