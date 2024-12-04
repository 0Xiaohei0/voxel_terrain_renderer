#pragma once
#include "learnopengl/shader.h"
#include <unordered_map>

class ShaderManager
{
public:
    static ShaderManager& getInstance();

    void loadShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);

    std::shared_ptr<Shader> getShader(const std::string& name);

private:
    ShaderManager() = default; // Private constructor for Singleton pattern

    std::unordered_map<std::string, std::shared_ptr<Shader>> shaders;

};

ShaderManager& ShaderManager::getInstance() {
    static ShaderManager instance;
    return instance;
}

void ShaderManager::loadShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath) {
    if (shaders.find(name) != shaders.end()) {
        throw std::runtime_error("Shader with name '" + name + "' already exists!");
    }

    auto shader = std::make_shared<Shader>(vertexPath.c_str(), fragmentPath.c_str());

    shaders[name] = shader;
}

std::shared_ptr<Shader> ShaderManager::getShader(const std::string& name) {
    auto it = shaders.find(name);
    if (it == shaders.end()) {
        throw std::runtime_error("Shader with name '" + name + "' not found!");
    }

    return it->second;
}