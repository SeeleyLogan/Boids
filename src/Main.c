#include <App.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

int main()
{
    App app = initApp();
    run(&app);

    return 0;
}
