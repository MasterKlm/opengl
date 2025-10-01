#include "texture.h"
#include "renderer.h" // For GLCall macro
#include <iostream>

// Only include stb_image.h in the source file, not the header
#include <stb_image.h>

int Texture::currentId = 0;

Texture::Texture()
    : id(-1), tex(0), path(nullptr), name(nullptr), width(0), height(0), nChannels(0)
{
}

Texture::Texture(const char* path, const char* name, bool defaultParams)
    : path(path), name(name), id(currentId++), tex(0), width(0), height(0), nChannels(0)
{
    generate();
    
    if(defaultParams)
    {
        setFilters(GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
        setWrap(GL_REPEAT);
    }
}

// Texture::~Texture()
// {
//     if(tex != 0)
//     {
//         GLCall(glDeleteTextures(1, &tex));
//     }
// }

void Texture::generate()
{
    GLCall(glGenTextures(1, &tex));
    GLCall(glBindTexture(GL_TEXTURE_2D, tex));
}

void Texture::load(bool flip)
{
    stbi_set_flip_vertically_on_load(flip);
    
    unsigned char* data = stbi_load(path, &width, &height, &nChannels, 0);
    
    GLenum colorMode = GL_RGB;
    
    switch(nChannels)
    {
        case 1:
            colorMode = GL_RED;
            break;
        case 3:
            colorMode = GL_RGB;
            break;
        case 4:
            colorMode = GL_RGBA;
            break;
        default:
            std::cout << "Unsupported number of channels: " << nChannels << " for texture: " << path << std::endl;
            stbi_image_free(data);
            return;
    }
    
    if(data)
    {
        GLCall(glBindTexture(GL_TEXTURE_2D, tex));  // Bind the actual texture, not the ID
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, colorMode, width, height, 0, colorMode, GL_UNSIGNED_BYTE, data));
        GLCall(glGenerateMipmap(GL_TEXTURE_2D));
        
        // std::cout << "Texture loaded successfully: " << path << " (" << width << "x" << height << ", " << nChannels << " channels)" << std::endl;
    }
    else
    {
        std::cout << "Failed to load texture at: " << path << std::endl;
        std::cout << "STB Error: " << stbi_failure_reason() << std::endl;
    }
    
    stbi_image_free(data);
}

void Texture::setFilters(GLenum all)
{
    setFilters(all, all);
}

void Texture::setFilters(GLenum mag, GLenum min)
{
    GLCall(glBindTexture(GL_TEXTURE_2D, tex));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min));
}

void Texture::setWrap(GLenum all)
{
    setWrap(all, all);
}

void Texture::setWrap(GLenum s, GLenum t)
{
    GLCall(glBindTexture(GL_TEXTURE_2D, tex));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t));
}

void Texture::bind()
{
    GLCall(glBindTexture(GL_TEXTURE_2D, tex));  // Bind the actual texture, not the ID
}

void Texture::activate()
{
    GLCall(glActiveTexture(GL_TEXTURE0 + id));
}