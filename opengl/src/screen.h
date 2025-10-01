// screen.h
#ifndef SCREEN_H
#define SCREEN_H
#include <glad/glad.h>  // MUST come before GLFW
#include <GLFW/glfw3.h>
#include <iostream>

class Screen
{
public:
    static unsigned int SCR_WIDTH;
    static unsigned int SCR_HEIGHT;

    static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);

    Screen();
    ~Screen();  // Add destructor

    bool init();
    void setParameters();
    void update();
    void newFrame();
    bool shouldClose();
    void setShouldClose(bool shouldClose);
    GLFWwindow* getWindow();

private:
    GLFWwindow* window;
};

#endif
