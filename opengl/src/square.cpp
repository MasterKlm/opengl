#include "square.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

Square::Square(float positions[8]){
    this->pPositions = positions;
}

void Square::compute(){
    float positions[8];
    
    // Copy positions from member pointer
    for (int i = 0; i < 8; i++){
        positions[i] = *(this->pPositions+i);
    }

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    // Generate VAO and buffers (only once each!)
    glGenVertexArrays(1, &squareVAO);
    glGenBuffers(1, &squareVBO);
    glGenBuffers(1, &squareIBO);

    // Bind VAO first
    glBindVertexArray(squareVAO);

    // Setup vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, squareVBO);
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), positions, GL_STATIC_DRAW);  // Fixed size

    // Setup index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, squareIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    // Set up vertex attribute (2D coordinates: x, y)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind to prevent accidental modification
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Square::draw(){
    glBindVertexArray(squareVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

Square::~Square(){
    glDeleteVertexArrays(1, &squareVAO);
    glDeleteBuffers(1, &squareVBO);  // Delete VBO too
    glDeleteBuffers(1, &squareIBO);
}