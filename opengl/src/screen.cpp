#include "screen.h"

unsigned int Screen::SCR_WIDTH = 800;
unsigned int Screen::SCR_HEIGHT = 600;

void Screen::FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);  // Remove GLCall macro here
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
}

Screen::Screen() : window(nullptr)
{
}

Screen::~Screen()
{
    if (window)
    {
        glfwDestroyWindow(window);
        window = nullptr;
    }
}

bool Screen::init()
{
    // Set OpenGL version hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable VSync
    
    return true;
}

void Screen::setParameters()
{
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    
    // Set the framebuffer callback
    glfwSetFramebufferSizeCallback(window, Screen::FrameBufferSizeCallback);
    
    // Disable cursor (for camera controls)
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

void Screen::update()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // Changed alpha to 1.0f
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Screen::newFrame()
{
    glfwSwapBuffers(window);
    glfwPollEvents();
}

bool Screen::shouldClose()
{
    return glfwWindowShouldClose(window);
}

void Screen::setShouldClose(bool shouldClose)
{
    glfwSetWindowShouldClose(window, shouldClose);
}

GLFWwindow* Screen::getWindow()
{
    return window;
}