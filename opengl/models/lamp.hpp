#ifndef LAMP_HPP
#define LAMP_HPP
#include "cube.hpp"
#include <iostream>

class Lamp : public Cube 
{
    public:
        glm::vec3 lightColor;

        //light strength values
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular; 

        Lamp(glm::vec3 lightColor, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,glm::vec3 pos, glm::vec3 size)
        : lightColor(lightColor), ambient(ambient), diffuse(diffuse), specular(specular),
        Cube(Material::white_plastic, pos, size)
        {

        }
        
        void render(Shader shader){
            // Set the light color uniform for the lamp shader
            shader.SetUniform3Float("lightColor", lightColor);
            
            // Create model matrix for the lamp WITHOUT rotation
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, pos);     // Move to position
            model = glm::scale(model, size);        // Scale (no rotation for light source)
            
            shader.SetUniformMatrix4fv("model", model);
            
            // Call Model::render directly to skip Cube's material uniform setting
            Model::render(shader);
        }
};

#endif