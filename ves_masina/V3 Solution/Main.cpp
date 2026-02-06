#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>
#include <thread>

#include "App.h"

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    auto* app = (App*)glfwGetWindowUserPointer(window);
    if (app) app->onKey(key, scancode, action, mods);
}

static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
    auto* app = (App*)glfwGetWindowUserPointer(window);
    if (app) app->onCursorPos(xpos, ypos);
}

static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    auto* app = (App*)glfwGetWindowUserPointer(window);
    if (app) app->onMouseButton(button, action, mods);
}

int main()
{
    if (!glfwInit()) {
        std::cout << "GLFW nije uspeo da se inicijalizuje.\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // fullscreen
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Projekat", monitor, NULL);
    if (!window) {
        std::cout << "Prozor nije uspeo da se kreira.\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cout << "GLEW nije uspeo da se inicijalizuje.\n";
        glfwTerminate();
        return -1;
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    App app;
    glfwSetWindowUserPointer(window, &app);

    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);

    if (!app.init(window)) {
        glfwTerminate();
        return -1;
    }

    // 75 FPS limiter
    const double targetFrame = 1.0 / 75.0;
    double last = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        double frameStart = glfwGetTime();
        double now = frameStart;
        double dt = now - last;
        last = now;

        app.update(now, dt);
        app.render();

        glfwSwapBuffers(window);
        glfwPollEvents();

        double frameEnd = glfwGetTime();
        double frameTime = frameEnd - frameStart;
        if (frameTime < targetFrame) {
            std::this_thread::sleep_for(std::chrono::duration<double>(targetFrame - frameTime));
        }
    }

    app.cleanup();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
