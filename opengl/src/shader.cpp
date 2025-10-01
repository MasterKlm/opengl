#include "shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "renderer.h"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader()
: m_FilePath("res/shaders/basic.shader"), m_RendererID(0) 
{
    ShaderProgramSource shaderSources = ParseShaders(this->m_FilePath);
    m_RendererID = CreateShader(shaderSources.VertexSource, shaderSources.FragmentSource);  
}

Shader::Shader(const std::string& filePath) : m_FilePath(filePath), m_RendererID(0) {
    // std::cout << "Loading shader from: " << filePath << std::endl;
    
    ShaderProgramSource shaderSources = ParseShaders(filePath);
    
    if (shaderSources.VertexSource.empty()) {
        std::cout << "ERROR: Vertex shader source is empty!" << std::endl;
        return;
    }
    
    if (shaderSources.FragmentSource.empty()) {
        std::cout << "ERROR: Fragment shader source is empty!" << std::endl;
        return;
    }
    
    // std::cout << "Vertex shader preview: " << shaderSources.VertexSource.substr(0, 100) << "..." << std::endl;
    // std::cout << "Fragment shader preview: " << shaderSources.FragmentSource.substr(0, 100) << "..." << std::endl;
    
    m_RendererID = CreateShader(shaderSources.VertexSource, shaderSources.FragmentSource);
    
    if (m_RendererID == 0) {
        std::cout << "ERROR: Failed to create shader from " << filePath << std::endl;
    } else {
        // std::cout << "Successfully loaded shader from " << filePath << " with ID: " << m_RendererID << std::endl;
    }
}

bool Shader::IsValid() const {
    return m_RendererID != 0;
}

Shader::Shader(const std::string& vertexSource, const std::string& fragmentSource) 
: m_FilePath(""), m_RendererID(0) {
    m_RendererID = CreateShader(vertexSource, fragmentSource);
}

// Shader::~Shader()
// {
//     GLCall(glDeleteProgram(m_RendererID));
// }

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    int location = GetUniformLocation(name);
    if (location != -1) {
        GLCall(glUniform4f(location, v0, v1, v2, v3));
    }
}

void Shader::SetUniformMatrix4fv(const std::string& name, glm::mat4 mat)
{
    int location = GetUniformLocation(name);
    if (location != -1) {
        GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat)));
    }
}

void Shader::SetUniform1i(const std::string& name, int i)
{
    int location = GetUniformLocation(name);
    if (location != -1) {
        GLCall(glUniform1i(location, i));
    }
}

void Shader::SetUniform3Float(const std::string& name, glm::vec3 v)
{
    SetUniform3Float(name, v.x, v.y, v.z); 
}

void Shader::SetUniform3Float(const std::string& name, float v1, float v2, float v3)
{
    int location = GetUniformLocation(name);
    if (location != -1) {
        GLCall(glUniform3f(location, v1, v2, v3));
        // std::cout << "Set uniform " << name << std::endl;
    } else {
        // std::cout << "Skipped uniform " << name << " (not found in shader)" << std::endl;
    }
}

void Shader::SetUniformFloat(const std::string& name, float v)
{
    int location = GetUniformLocation(name);
    if (location != -1) {
        GLCall(glUniform1f(location, v));
        // std::cout << "Set uniform " << name << std::endl;
    } else {
        // std::cout << "Skipped uniform " << name << " (not found in shader)" << std::endl;
    }
}

unsigned int Shader::GetUniformLocation(const std::string& name)
{
    if(m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
    {   
        return m_UniformLocationCache[name];
    }
    
    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    m_UniformLocationCache[name] = location;
    
    return location;
}

ShaderProgramSource Shader::ParseShaders(const std::string& filePath){
    std::ifstream stream(filePath);
    if (!stream.is_open()) {
        std::cerr << "Error: Could not open shader file: " << filePath << std::endl;
        return {"", ""};
    }

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };
    std::string line;
    std::stringstream ss[2];

    ShaderType type = ShaderType::NONE;

    while(std::getline(stream, line)){
        if(line.find("#shader") != std::string::npos)
        {
            if(line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }

    return {ss[0].str(), ss[1].str()};
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source){
    // std::cout << "Compiling shader type: " << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << std::endl;
    // std::cout << "Source length: " << source.length() << std::endl;
    // std::cout << "First 100 chars: " << source.substr(0, 100) << std::endl;
    
    unsigned int id = glCreateShader(type);
    if (id == 0) {
        std::cout << "ERROR: Failed to create shader object" << std::endl;
        return 0;
    }
    
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    //error handling:
    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if(result == GL_FALSE){
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*) alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER? "Vertex" : "Fragment") << " shader" << std::endl;
        std::cout << "Error: " << message << std::endl;
        GLCall(glDeleteShader(id));
        return 0;
    } 

    // std::cout << "Successfully compiled shader with ID: " << id << std::endl;
    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader){
    std::cout << "Creating shader program..." << std::endl;
    
    GLCall(unsigned int program = glCreateProgram());
    if (program == 0) {
        std::cout << "ERROR: Failed to create shader program" << std::endl;
        return 0;
    }
    
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    if (vs == 0 || fs == 0) {
        std::cout << "ERROR: Shader compilation failed" << std::endl;
        if (vs != 0) glDeleteShader(vs);
        if (fs != 0) glDeleteShader(fs);
        glDeleteProgram(program);
        return 0;
    }

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    
    // Check linking
    int linkResult;
    GLCall(glGetProgramiv(program, GL_LINK_STATUS, &linkResult));
    if (linkResult == GL_FALSE) {
        int length;
        GLCall(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*) alloca(length * sizeof(char));
        GLCall(glGetProgramInfoLog(program, length, &length, message));
        std::cout << "ERROR: Shader program linking failed: " << message << std::endl;
        
        GLCall(glDeleteShader(vs));
        GLCall(glDeleteShader(fs));
        GLCall(glDeleteProgram(program));
        return 0;
    }
    
    GLCall(glValidateProgram(program));
    
    // Check validation
    int validateResult;
    GLCall(glGetProgramiv(program, GL_VALIDATE_STATUS, &validateResult));
    if (validateResult == GL_FALSE) {
        int length;
        GLCall(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*) alloca(length * sizeof(char));
        GLCall(glGetProgramInfoLog(program, length, &length, message));
        std::cout << "WARNING: Shader program validation failed: " << message << std::endl;
        // Don't return 0 here, validation can fail for reasons that don't prevent usage
    }

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    // std::cout << "Successfully created shader program with ID: " << program << std::endl;
    return program;
}