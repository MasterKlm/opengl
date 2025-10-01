#pragma once

#include "vertexarray.h"
#include "indexbuffer.h"
#include "shader.h"

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLCLEARERROR();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))
    
void GLCLEARERROR();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
    private:

    public:
    void Clear() const;
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
    void Draw(const VertexArray& va, int count, const Shader& shader) const;
};
