#include <cstdio>
#include <cstdlib>
#include <GLFW/glfw3.h>
#include "Sprite.h"

static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

GLFWwindow* get_window(int width, int height) {
#ifdef GLFW_OPENGL_ES_API_DEFINED
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
#else
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
#endif
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(width, height, "runner application", nullptr, nullptr);
    if (!window) {
        fprintf(stderr, "Failed to create GLFW window.\n");
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    glViewport(0, 0, width, height);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glLoadIdentity();
    return window;
}

int main() {
    int height = 600;
    int width = 1024;
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        fprintf(stderr, "Failed to init GLFW.\n");
        return 1;
    }
    GLFWwindow* window = get_window(width, height);
    Sprite bg_trees("../assets/img/background/trees-grass.png");
    Sprite bg_building("../assets/img/background/building.png");
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();
        bg_building.draw();
        bg_trees.draw();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
