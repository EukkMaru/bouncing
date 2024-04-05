// #include <GLFW/glfw3.h>
// Commented out because it would throw an error: "fatal error: GLFW/glfw3.h: No such file or directory"

// #define GLFW_DLL
// Dynamic Linking
#include <GLFW/glfw3.h>
#include <iostream>

int main(void) {
    GLFWwindow *window;

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        std::cerr << "Failed to create GLFW window\n";
        return -1;
    }

    glfwMakeContextCurrent(window);

    while (!glfwWindowShouldClose(window)) {

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
