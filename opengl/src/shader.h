#pragma once
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <optional>

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};


class Shader
{
    private:
        unsigned int m_RendererID;
        std::string m_FilePath;
        std::unordered_map<std::string,  int> m_UniformLocationCache;
        // caching for uniforms
    public:
        Shader(const std::string& filePath);  // File-based
        Shader();
        Shader(const std::string& vertexSource, const std::string& fragmentSource);
        // ~Shader();

        void Bind() const;
        void Unbind() const;
        bool IsValid() const;
        //set uniforms

        void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
        void SetUniformMatrix4fv(const std::string& name,glm::mat4 mat);
        void SetUniform1i(const std::string& name,int i);
        void SetUniformFloat(const std::string& name,float v);
        void SetUniform3Float(const std::string& name, glm::vec3 v);
        void SetUniform3Float(const std::string& name, float v1, float v2, float v3);
    private:

        ShaderProgramSource ParseShaders(const std::string& filePath);
        unsigned int CompileShader(unsigned int type, const std::string& source);
        unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
        unsigned int GetUniformLocation(const std::string& name);
};