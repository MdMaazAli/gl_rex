#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <bits/stdc++.h>
#include "Shader.h"
#include "Camera.h"
#include "stb_image.h"
#include "SimpleMesh.h"
using namespace std;
using namespace glm;

Camera camera(glm::vec3(0.0f,0.0f,3.0f));

bool mouseFirst = true;
float lastX = 800.0f/2.0f;
float lastY = 600.0f/2.0f;
glm::vec3 lightPos(1.0f, 0.0f, 1.0f);

void mouse_callback(GLFWwindow* window,double xPosIn,double yPosIn){
    float xPos = (float)xPosIn;
    float yPos = (float)yPosIn;

    if(mouseFirst){
        xPos = lastX;
        yPos = lastY;
        mouseFirst = false;
    }

    float xOffset = xPos-lastX;
    float yOffset = lastY-yPos;
    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset,yOffset);
}

void scroll_callback(GLFWwindow* window,double xOffset,double yOffset){
    camera.ProcessMouseScroll((float)yOffset);
}

float deltaTime = 0.0f;
float lastFrame = 0.0f;

void framebuffer_size_callback(GLFWwindow* window,int width,int height){
    glViewport(0,0,width,height);
}

void processInput(GLFWwindow* window){
    if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window,true);
    }
    if(glfwGetKey(window,GLFW_KEY_W) == GLFW_PRESS){
        camera.ProcessKeyboard(FORWARD,deltaTime);
    }
    if(glfwGetKey(window,GLFW_KEY_S) == GLFW_PRESS){
        camera.ProcessKeyboard(BACKWARD,deltaTime);
    }
    if(glfwGetKey(window,GLFW_KEY_A) == GLFW_PRESS){
        camera.ProcessKeyboard(LEFT,deltaTime);
    }
    if(glfwGetKey(window,GLFW_KEY_D) == GLFW_PRESS){
        camera.ProcessKeyboard(RIGHT,deltaTime);
    }
}

int main(){
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800,600,"rex",NULL,NULL);
    if(window == NULL){
        cout<<"failed to initialize window";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        cout<<"failed to initialize GLAD"<<endl;
        return -1;
    }

    glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);
    glfwSetCursorPosCallback(window,mouse_callback);
    glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
    glfwSetScrollCallback(window,scroll_callback);

    glViewport(0,0,800,600);
    glEnable(GL_DEPTH_TEST);

    Shader testShader("src/vertexShader.glsl","src/fragShader.glsl");
    SimpleMesh cubeMesh("cube");
    SimpleMesh pyramidMesh("pyramid");
    SimpleMesh sphereMesh("sphere");

    while(!glfwWindowShouldClose(window)){
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);

        glClearColor(0.1f,0.1f,0.1f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(radians(camera.Zoom),800.0f/600.0f,0.1f,1000.0f);
        
        glm::mat4 view = glm::mat4(1.0f);
        view = camera.GetViewMatrix();

        testShader.use();
        
        glm::mat4 cube = glm::mat4(1.0f);
        cube = glm::scale(cube,glm::vec3(0.5f,0.5f,0.5f));
        cube = glm::translate(cube,glm::vec3(0.0f,0.0f,0.0f));
        testShader.setMat4("model",cube);
        testShader.setMat4("view",view);
        testShader.setMat4("projection",projection);
        cubeMesh.draw();
        
        glm::mat4 pyramid = glm::mat4(1.0f);
        pyramid = glm::scale(pyramid,glm::vec3(0.5f,0.5f,0.5f));
        pyramid = glm::translate(pyramid,glm::vec3(1.5f,0.0f,0.0f));
        testShader.setMat4("model",pyramid);
        testShader.setMat4("view",view);
        testShader.setMat4("projection",projection);
        pyramidMesh.draw();
        
        glm::mat4 sphere = glm::mat4(1.0f);
        sphere = glm::scale(sphere,glm::vec3(0.5f,0.5f,0.5f));
        sphere = glm::translate(sphere,glm::vec3(-1.5f,0.0f,0.0f));
        testShader.setMat4("model",sphere);
        testShader.setMat4("view",view);
        testShader.setMat4("projection",projection);
        sphereMesh.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}