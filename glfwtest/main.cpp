#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "learnopengl/shader.h"
#include "learnopengl/camera.h"

#include <iostream>
#include "ShaderManager.h"

#include "Time.h"
#include "MeshRenderer.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void switch_mode_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 1200;

// camera
Camera camera(glm::vec3(0.0f, 20.0f, 20.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool isCursorLocked = true;

// timing
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(100.2f, 130.0f, 200.0f);
glm::vec3 groundPos(0.0f, 0.0f, -2.0f);


float noiseScale = 0.05f;
float heightScale = 1.0f;

GLFWwindow* window;

int init() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Voxel terrain renderer", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetKeyCallback(window, switch_mode_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glEnable(GL_DEPTH_TEST);

    // load shaders
    ShaderManager& shaderManager = ShaderManager::getInstance();
    shaderManager.loadShader("lightingShader", "lighting.vert", "lighting.frag");
    shaderManager.loadShader("lightCubeShader", "lightCube.vert", "lightCube.frag");
    shaderManager.loadShader("waterShader", "water.vert", "water.frag");
}

int main()
{
    init();
    float vertices[] = {
        // Front face
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  // Bottom-left
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  // Bottom-right
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  // Top-right
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  // Top-left

        // Back face
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  // Bottom-left
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  // Top-left
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  // Top-right
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  // Bottom-right

        // Left face
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  // Bottom-back
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  // Bottom-front
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  // Top-front
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  // Top-back

        // Right face
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  // Bottom-back
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  // Top-back
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  // Top-front
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  // Bottom-front

        // Top face
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  // Back-left
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  // Front-left
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  // Front-right
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  // Back-right

        // Bottom face
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  // Back-left
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  // Back-right
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  // Front-right
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f   // Front-left
    };

    std::vector<unsigned int> indices = {
        // Front face
        0, 1, 2, 2, 3, 0,
        // Back face
        4, 5, 6, 6, 7, 4,
        // Left face
        8, 9, 10, 10, 11, 8,
        // Right face
        12, 13, 14, 14, 15, 12,
        // Top face
        16, 17, 18, 18, 19, 16,
        // Bottom face
        20, 21, 22, 22, 23, 20
    };

    float vertices_quad[] = {
         // Bottom face
         -10000.0f, 0, -10000.0f,  0.0f, 1.0f,  0.0f,  // Back-left
         10000.0f, 0, -10000.0f,  0.0f, 1.0f,  0.0f,  // Back-right
         10000.0f, 0,  10000.0f,  0.0f, 1.0f,  0.0f,  // Front-right
         -10000.0f, 0,  10000.0f,  0.0f, 1.0f,  0.0f   // Front-left
    };
    std::vector<unsigned int> indices_quad = {
        // Bottom face
        0, 1, 2, 2, 3, 0
    };

    const int size = 1000;

    const int instanceCount = size * size;
    glm::vec3* translations = new glm::vec3[instanceCount]; // Allocate on heap
    int index = 0;

    for (float x = -size/2; x < size / 2; x += 1)
    {
        for (float z = -size / 2; z < size / 2; z += 1)
        {
            glm::vec3 translation;
            translation.x = x;
            translation.y = 0;
            translation.z = z;
            translations[index++] = translation;
        }
    }

    unsigned int instanceVBO;
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * instanceCount, &translations[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    unsigned int voxelVAO, voxelVBO, voxelEBO;
    glGenVertexArrays(1, &voxelVAO);
    glGenBuffers(1, &voxelVBO);
    glBindVertexArray(voxelVAO);
    glBindBuffer(GL_ARRAY_BUFFER, voxelVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(2, 1);

    glGenBuffers(1, &voxelEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, voxelEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);


    unsigned int quadVAO, quadVBO, quadEBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_quad), vertices_quad, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    glGenBuffers(1, &quadEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_quad.size() * sizeof(unsigned int), indices_quad.data(), GL_STATIC_DRAW);

    ShaderManager& shaderManager = ShaderManager::getInstance();
    auto lightingShader = shaderManager.getShader("lightingShader");
    auto waterShader = shaderManager.getShader("waterShader");

    lightingShader->use();
  

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");



    // render loop
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        Time::delta_time = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        glClearColor(0.816f, 0.933f, 0.98f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        lightingShader->use();
        lightingShader->setVec3("light.position", lightPos);
        lightingShader->setVec3("viewPos", camera.Position);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 1.0f, 1000.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader->setMat4("projection", projection);
        lightingShader->setMat4("view", view);

        glm::vec3 lightColor(1.0, 1.0, 1.0);
        glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);
        lightingShader->setVec3("light.ambient", ambientColor);
        lightingShader->setVec3("light.diffuse", diffuseColor);
        lightingShader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);

        lightingShader->setVec3("material.diffuse", 0.5f, 0.5f, 0.5f);
        lightingShader->setVec3("material.specular", 0.1f, 0.1f, 0.1f);
        lightingShader->setFloat("material.shininess", 32.0f);
        lightingShader->setFloat("heightScale", heightScale);
        lightingShader->setFloat("noiseScale", noiseScale);

        glm::mat4 model = glm::mat4(1.0f);
        lightingShader->setMat4("model", model);

        lightingShader->setVec3("material.ambient", 0.439, 0.318, 0.161);

        glBindVertexArray(voxelVAO);
        glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, instanceCount);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        waterShader->use();
        waterShader->setVec3("light.position", lightPos);
        waterShader->setVec3("viewPos", camera.Position);
        waterShader->setMat4("projection", projection);
        waterShader->setMat4("view", view);
        waterShader->setVec3("light.ambient", ambientColor);
        waterShader->setVec3("light.diffuse", diffuseColor);
        waterShader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        waterShader->setVec3("material.diffuse", 0.5f, 0.5f, 0.5f);
        waterShader->setVec3("material.specular", 0.1f, 0.1f, 0.1f);
        waterShader->setFloat("material.shininess", 32.0f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0,14.4,0));
        waterShader->setMat4("model", model);

        waterShader->setVec3("material.ambient", 0.49, 0.824, 0.949);
        glBindVertexArray(quadVAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        ImGui::Begin("new window");
        ImGui::Text("hello");
        ImGui::SliderFloat("noiseScale", &noiseScale, 0, 0.2f);
        ImGui::SliderFloat("heightScale", &heightScale, 0, 10.0f);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    delete[] translations;
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    isCursorLocked = glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED;
    if (isCursorLocked) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.ProcessKeyboard(FORWARD, Time::delta_time);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.ProcessKeyboard(BACKWARD, Time::delta_time);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.ProcessKeyboard(LEFT, Time::delta_time);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.ProcessKeyboard(RIGHT, Time::delta_time);
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            camera.ProcessKeyboard(UP, Time::delta_time);
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
            camera.ProcessKeyboard(DOWN, Time::delta_time);
    }
} 

void switch_mode_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_F && action == GLFW_PRESS)
    {
        isCursorLocked = glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED;
        if (isCursorLocked)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        else
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        isCursorLocked = glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    if (!isCursorLocked) return;
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}