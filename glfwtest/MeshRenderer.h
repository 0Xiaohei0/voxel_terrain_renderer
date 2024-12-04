#pragma once
#include <vector>
#include <glad/glad.h>

class MeshRenderer
{
public:
	// format: // positions          // normals  
	//		    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f
    MeshRenderer() : VAO(0), VBO(0), EBO(0), vertexCount(0), instanceCount(0){}

    ~MeshRenderer() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    void Render() {
        glBindVertexArray(VAO);
        if (EBO) {
            if (instanceCount > 0) {
                glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, instanceCount);
            }
            else {
                glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
            }
        }
        else {
            if (instanceCount > 0) {
                glDrawArraysInstanced(GL_TRIANGLES, 0, vertexCount, instanceCount);
            }
            else {
                glDrawArrays(GL_TRIANGLES, 0, vertexCount);
            }
            
        }
    }
	unsigned int VAO, VBO, EBO;
    int instanceCount;
	int vertexCount;
    std::vector<unsigned int> indices;

    void InitializeBuffers(const std::vector<float>& vertices) {
        vertexCount = vertices.size() / 6; 

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    }

    void InitializeBuffers(const std::vector<float>& vertices, std::vector<unsigned int> indices) {
        vertexCount = vertices.size() / 6;

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    }

    void setInstanceCount(const int newInstanceCount) {
        this->instanceCount = newInstanceCount;
    }
};

