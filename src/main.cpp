#include <iostream>
#include "Window.h"
#include "Application.h"

int main()
{
    auto app = vis::Application::create_instance();

    app->initialize();
    app->run();

    delete app;

    return 0;
}
