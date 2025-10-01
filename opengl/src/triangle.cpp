#include "triangle.h"
#include <iostream>
#include <glad/glad.h>  // MUST come before GLFW
#include <GLFW/glfw3.h>



Triangle::Triangle(float positions[6]){
    this->pPositions = positions;
}

void Triangle::compute(){
    float positions[6];
    
    // Copy positions from member pointer
    for (int i = 0; i < 6; i++){
        positions[i] = *(this->pPositions+i);
    }
    
    // Generate and bind VAO and VBO
    glGenVertexArrays(1, &triangleVAO);
    
    glBindVertexArray(triangleVAO);

    // Upload vertex data
    glBufferData(GL_ARRAY_BUFFER, 6*sizeof(float), positions, GL_STATIC_DRAW);
    
    // Set up vertex attribute (2D coordinates: x, y)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  // Enable the attribute here, not in draw()
    
    // Unbind to prevent accidental modification
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Triangle::draw(){
    glBindVertexArray(triangleVAO);  // Bind the VAO before drawing
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);  // Unbind after drawing (optional but good practice)
}

Triangle::~Triangle(){
    // Clean up OpenGL resources
    glDeleteVertexArrays(1, &triangleVAO);
    glDeleteBuffers(1, &triangleVBO);
}