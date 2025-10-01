#include <iostream>
#include <glad/glad.h>  // MUST come before GLFW
#include <GLFW/glfw3.h>
// #include "triangle.h"
// #include "square.h"
#include "renderer.h"
#include "vertexbuffer.h"
#include "vertexbufferlayout.h"
#include "indexbuffer.h"
#include "vertexarray.h"
#include "shader.h"
#include "<glm/glm.hpp>"
#include "<glm/gtc/matrix_transform.hpp>"

// #include <random>

using std::cout;



int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    // Set OpenGL version hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "OpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    // Initialize GLAD - THIS IS CRUCIAL!
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << "\n";
        glfwTerminate();
        return -1;
    }

    cout << "OpenGL Version: " << glGetString(GL_VERSION) << "\n";
    cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
    {

    float vertices[] = {       //colors
        -0.25f, -0.5f, 0.0f,   1.0f, 0.1f, 0.5f,
        0.15f, 0.0f, 0.0f,     0.5f, 1.0f, 0.75f,
        0.0f, 0.5f, 0.0f,      0.6f, 1.0f, 0.2f,
        0.5f, -0.4f, 0.0f,     1.0f, 0.2f, 1.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        3, 1, 2
    };



    VertexArray va;
    VertexBuffer vb(vertices, 4*6 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(3);
    layout.Push<float>(3);
    va.AddBuffer(vb, layout);
    
    

    IndexBuffer ib(indices, 6);
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::rotate(trans, glm::radians(45.0f), glm::vec3(0.0f,0.0f,1.0f))


    Shader shader("res/shaders/basic.shader");
    shader.SetUniformMatrix4fv("transform", trans)
    shader.Bind();

    Renderer renderer;


    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    shader.Unbind();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        renderer.Clear();

        shader.Bind();


        renderer.Draw(va, ib, shader);

      
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }


}
    glfwTerminate();
    return 0;
}