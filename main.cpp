#define GLFW_INCLUDE_GLU

#define GLFW_EXPOSE_NATIVE_WGL
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <Windows.h>
#include <iostream>
struct Vec2 {
    float x, y;

    Vec2(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}

    Vec2 operator+(const Vec2 &other) const {
        return Vec2(x + other.x, y + other.y);
    }

    Vec2 &operator+=(const Vec2 &other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vec2 operator*(float scalar) const {
        return Vec2(x * scalar, y * scalar);
    }

    void applyAccel(const Vec2 &acc, float dTime) {
        x += acc.x * dTime;
        y += acc.y * dTime;
    }
};

#define Tab GLFWwindow

void getScreenSize();
void setTPos(Tab *tab, int posX, int posY);
void initializeScene();
void updatePhysics(double deltaTime);
void checkTCollisions(Tab *tab, int &posX, int &posY);
void renderScene(Tab *tab);
void processInput(Tab *tab);
double calculateDTime();

Vec2 velocity = {5, -10};
Vec2 acceleration = {0, 20};
float elasticity = 0.75;
int posX = 100, posY = 100;
int screenWidth, screenHeight, tabWidth, tabHeight;
double lastTime = 0.0;

int main() {
    if (!glfwInit()) {
        return -1;
    }
    getScreenSize();

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    Tab *tab = glfwCreateWindow(640, 480, "Physics Simulation", NULL, NULL);
    if (!tab) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(tab);
    glfwSwapInterval(1);
    int width, height;
    glfwGetFramebufferSize(tab, &width, &height);
    glViewport(0, 0, width, height);
    HWND hwnd = glfwGetWin32Window(tab);

    glfwGetWindowSize(tab, &tabWidth, &tabHeight);

    setTPos(tab, posX, posY);

    while (!glfwWindowShouldClose(tab)) {
        double deltaTime = calculateDTime();

        processInput(tab);
        updatePhysics(deltaTime);
        checkTCollisions(tab, posX, posY);
        renderScene(tab);

        glfwSwapBuffers(tab);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void getScreenSize() {
    GLFWmonitor *primaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(primaryMonitor);

    if (mode != NULL) {
        screenWidth = mode->width;
        screenHeight = mode->height;
    }
}

void updatePhysics(double deltaTime) {
    velocity.applyAccel(acceleration, deltaTime);

    posX += static_cast<int>(velocity.x * deltaTime);
    posY += static_cast<int>(velocity.y * deltaTime);

    std::cout << "Velocity: (" << velocity.x << ", " << velocity.y << "), Pos: (" << posX << ", " << posY << ")\n";
    std::cout << "Delta Time: " << deltaTime << ", PosX: " << posX << ", PosY: " << posY << std::endl;
}

void checkTCollisions(Tab *tab, int &posX, int &posY) {
    bool collided = false;

    if (posX < 0) {
        posX = 0;
        velocity.x = -velocity.x * elasticity;
        collided = true;
    } else if (posX > screenWidth - tabWidth) {
        posX = screenWidth - tabWidth;
        velocity.x = -velocity.x * elasticity;
        collided = true;
    }

    if (posY < 0) {
        posY = 0;
        velocity.y = -velocity.y * elasticity;
        collided = true;
    } else if (posY > screenHeight - tabHeight) {
        posY = screenHeight - tabHeight;
        velocity.y = -velocity.y * elasticity;
        collided = true;
    }

    if (collided) {

        setTPos(tab, posX, posY);
    }
}

void renderScene(Tab *tab) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void processInput(Tab *tab) {
    if (glfwGetKey(tab, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(tab, true);
}

double calculateDTime() {
    double currentTime = glfwGetTime();
    double deltaTime = currentTime - lastTime;
    lastTime = currentTime;
    return deltaTime;
}

void setTPos(Tab *tab, int posX, int posY) {
    HWND hwnd = glfwGetWin32Window(tab);
    SetWindowPos(hwnd, NULL, posX, posY, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}
