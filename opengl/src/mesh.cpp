#include "mesh.h"
#include "renderer.h"


std::vector<Vertex> Vertex::genList(float* vertices, int noVertices)
{
    std::vector<Vertex> ret(noVertices);
    int stride = sizeof(Vertex) / sizeof(float);


    for(int i =0; i<noVertices; i++)
    {
        ret[i].pos = glm::vec3(
            vertices[i*stride + 0],
            vertices[i*stride + 1],
            vertices[i*stride + 2]
        );

        ret[i].normal = glm::vec3(
            vertices[i*stride+3],
            vertices[i*stride+4],
            vertices[i*stride+5]
        );


        ret[i].texCoord = glm::vec2(
            vertices[i*stride+6],
            vertices[i*stride+7]

        );
    }
    return ret;
}

Mesh::Mesh(){}


Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
:vertices(vertices), indices(indices), textures(textures)
{
    setup();
}

void Mesh::render(Shader shader)
{
    //texturees


    for(unsigned int i = 0; i <textures.size(); i++)
    {
        textures[i].activate();
        textures[i].bind();
    }

    GLCall(glBindVertexArray(VAO));
    GLCall(glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0));
    GLCall(glBindVertexArray(0));
    GLCall(glActiveTexture(GL_TEXTURE0));
}



void Mesh::cleanup()
{
    GLCall(glDeleteVertexArrays(1,&VAO));
    GLCall(glDeleteBuffers(1, &VBO));
    GLCall(glDeleteBuffers(1, &EBO));
}

void Mesh::setup()
{
    GLCall(glGenVertexArrays(1, &VAO));
    GLCall(glGenBuffers(1, &VBO));
    GLCall(glGenBuffers(1, &EBO));

    GLCall(glBindVertexArray(VAO));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    GLCall(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW));
    
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW));

    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, pos) ));
    GLCall(glEnableVertexAttribArray(1));
    GLCall(glVertexAttribPointer(1,3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, normal)));
    GLCall(glEnableVertexAttribArray(2));
    GLCall(glVertexAttribPointer(2, 2,GL_FLOAT,GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, texCoord)));

    GLCall(glBindVertexArray(0));
}