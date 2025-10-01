#include <iostream>
#include <glad/glad.h>  // MUST come before GLFW
#include <GLFW/glfw3.h>
#include "renderer.h"
#include "vertexbuffer.h"
#include "vertexbufferlayout.h"
#include "indexbuffer.h"
#include "vertexarray.h"
#include "shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "camera.h"
#include "screen.h"
#include "texture.h"
#include "../models/lamp.hpp"
#include "../models/sphere.hpp"
#include "../models/cube.hpp"

using std::cout;



// Add global variable to store scroll delta
double g_scrollDY = 0.0;

// GLFW scroll callback to update scroll delta
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    g_scrollDY = yoffset;
}

void processInput(GLFWwindow* window, double dt, Camera &camera, double &mX, double &mY)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE))
    {
        glfwSetWindowShouldClose(window, true);
    }
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.updateCameraPos(CameraDirection::FORWARD, dt);
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.updateCameraPos(CameraDirection::BACKWARD, dt);
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.updateCameraPos(CameraDirection::RIGHT, dt);
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.updateCameraPos(CameraDirection::LEFT, dt);
    }
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        camera.updateCameraPos(CameraDirection::UP, dt);
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        camera.updateCameraPos(CameraDirection::DOWN, dt);
    }

    if(mX != 0.0 || mY != 0.0)
    {
        camera.updateCameraDirection(mX, mY);
    }
    glfwSetScrollCallback(window, scroll_callback);
    // Use global scroll delta for zoom
    if (g_scrollDY != 0.0)
    {
        camera.updateCameraZoom(g_scrollDY);
        g_scrollDY = 0.0; // Reset after use
    }
}







int main(void)
{
    
    /* Initialize the library */
    
    Screen screen;
    
    if (!glfwInit())
    {
        return -1;

    }
    if (screen.init()==false)
    {
        glfwTerminate();
        return -1;
    }
    // Set OpenGL version hints

    /* Create a windowed mode window and its OpenGL context */

    /* Make the window's context current */
    
    
    // Initialize GLAD - THIS IS CRUCIAL!
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << "\n";
        glfwTerminate();
        return -1;
    }
    
    screen.setParameters();
    

    
    
    {
        Camera camera(glm::vec3(0.0f,0.0f,3.0f));
        // cout << "Camera: " << camera.getZoom() <<"\n"; 
        float deltaTime = 0.0f;
        float lastFrame = 0.0f;
        
        // float vertices[] = {       //positions           //colors             //texture coords
        //     -0.5f, -0.5f, 0.0f,   1.0f, 0.1f, 0.5f,     0.0f, 0.0f,  // bottom-left
        //      0.5f, -0.5f, 0.0f,   0.5f, 1.0f, 0.75f,    1.0f, 0.0f,  // bottom-right  
        //      0.5f,  0.5f, 0.0f,   0.6f, 1.0f, 0.2f,     1.0f, 1.0f,  // top-right
        //     -0.5f,  0.5f, 0.0f,   0.6f, 1.0f, 0.2f,     0.0f, 1.0f   // top-left
        // };
     

        // Initialize rotation axis variables
        // float trans_x = 0.0f;
        // float trans_y = 0.0f;
        // float trans_z = 1.0f; // Default to Z-axis rotation
        
        

//         std::string vertexSource = R"(
// #version 330 core

// layout(location = 0) in vec3 position;
// layout(location = 1) in vec2 textureCord;

// uniform mat4 model;
// uniform mat4 view;
// uniform mat4 projection;
// out vec2 v_textureCord;

// void main()
// {
//    gl_Position = projection * view * model * vec4(position, 1.0);
//    v_textureCord = textureCord;
// }
//         )";

//         std::string fragmentSource = R"(
// #version 330 core
 
//         )";

        Shader shader;
        Shader lamp_shader("res/shaders/lamp.shader");
        
        if (!shader.IsValid()) {
            std::cout << "ERROR: Basic shader failed to load!" << std::endl;
            return -1;
        }

        if (!lamp_shader.IsValid()) {
            std::cout << "ERROR: Lamp shader failed to load!" << std::endl;
            return -1;
        }

        // Cube cube(Material::emerald,glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.75f));
        // cube.init();
        Sphere sphere(Material::chrome,glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.75f));
        sphere.init();
        // Cube cube2(Material::chrome,glm::vec3(1.0f, 0.0f,1.0f), glm::vec3(0.75f));
        // cube2.init();

        
        Lamp lamp(glm::vec3(1.0f), glm::vec3(0.2f), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(-1.0f, -0.5f, -0.5f), glm::vec3(0.25f) );
        lamp.init();

        glm::vec3 lightPos;
        
        /* Loop until the user closes the window */
        while (!screen.shouldClose())
        {
            double currentTime = glfwGetTime();
            deltaTime = currentTime - lastFrame;
            lastFrame = currentTime;
            // Process input - THIS WAS MISSING!
            static double lastX = screen.SCR_WIDTH / 2.0;
            static double lastY = screen.SCR_HEIGHT / 2.0;
            static bool firstMouse = true;

            double mX, mY;
            glfwGetCursorPos(screen.getWindow(), &mX, &mY);
            
            if (firstMouse) {
                lastX = mX;
                lastY = mY;
                firstMouse = false;
            }

            float xoffset = (float)(mX - lastX);
            float yoffset = (float)(lastY - mY); // reversed since y-coordinates go top to bottom
            lastX = mX;
            lastY = mY;

            // Scale down sensitivity
            float sensitivity = 0.5f;
            double dx, dy;
            dx = (double)xoffset * (double)sensitivity;
            dy = (double)yoffset * (double)sensitivity;

            processInput(screen.getWindow(), deltaTime, camera, dx, dy);
            
            /* Render here */
            // renderer.Clear();
            lightPos.x = 2.0f * cos(glfwGetTime());
            lightPos.z = 2.0f * sin(glfwGetTime());
            lightPos.y = 0.0f;
            lamp.pos = lightPos;
            screen.update();
            shader.Bind();
            
            shader.SetUniform3Float("light.position", lamp.pos);
            shader.SetUniform3Float("viewPos", camera.cameraPos);

            //light strength value to cube/other object shader(s)
            shader.SetUniform3Float("light.ambient", lamp.ambient);
            shader.SetUniform3Float("light.diffuse", lamp.diffuse);
            shader.SetUniform3Float("light.specular", lamp.specular);

            //create transformations for screen

            glm::mat4 view = glm::mat4(1.0f);
            glm::mat4 projection = glm::mat4(1.0f);

            // model = glm::rotate(model, (float)glfwGetTime() * glm::radians(-55.0f), glm::vec3(0.5f));

            // view = glm::translate(view,  glm::vec3(-x,-y,-z)); //negative the x,y,z its a opengl problem just how it is
            view = camera.getViewMatrix();
            projection = glm::perspective(glm::radians(camera.getZoom()), (float)screen.SCR_WIDTH/(float)screen.SCR_HEIGHT, 0.1f, 100.0f);
            
            
            // shader.Bind();
            
            // Create a fresh transformation matrix each frame
            // glm::mat4 trans = glm::mat4(1.0f);
            // trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(trans_x, trans_y, trans_z));
            
            // shader.SetUniformMatrix4fv("transform", trans);
            
            shader.SetUniformMatrix4fv("view", view);
            shader.SetUniformMatrix4fv("projection", projection);
            
            
            
            // cube.render(shader);
            sphere.render(shader);
            // cube2.render(shader);
            // shader.Unbind();

            lamp_shader.Bind();
            lamp_shader.SetUniformMatrix4fv("view", view);
            lamp_shader.SetUniformMatrix4fv("projection", projection);
            lamp.render(lamp_shader);
            // lamp_shader.Unbind();

            // shader.Unbind();

            /* Swap front and back buffers */
            screen.newFrame();

            /* Poll for and process events */
        }
        // cube.cleanup();
        sphere.cleanup();
        // cube2.cleanup();
        lamp.cleanup();
    }
    glfwTerminate();
    return 0;
}