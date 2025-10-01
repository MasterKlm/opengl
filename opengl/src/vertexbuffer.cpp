#include "vertexbuffer.h"
#include "renderer.h"
#include <glad/glad.h>  // MUST come before GLFW
#include <GLFW/glfw3.h>

 
VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    GLCall(glGenBuffers(1, &m_RendererId));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererId));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}
VertexBuffer::~VertexBuffer() 
{
    GLCall(glDeleteBuffers(1, &m_RendererId));
}
void VertexBuffer::Bind() const
{
 GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererId));

 }
void VertexBuffer::Unbind() const
{
     GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}


