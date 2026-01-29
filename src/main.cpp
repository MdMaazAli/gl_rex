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

Camera camera(glm::vec3(0.0f,0.0f,8.0f));

float xPos = 0.0f;
float zPos = 0.0f;
glm::vec3 lightPos = glm::vec3(1.0f,1.0f,2.0f);
glm::vec3 lightColor = glm::vec3(0.8f,0.8f,0.2f);

bool mouseFirst = true;
float lastX = 800.0f/2.0f;
float lastY = 600.0f/2.0f;

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

// rexxy attributes
glm::vec3 playerPos = glm::vec3(0.0f);
float flightTime = 0.0f;
float xSpeed = 3.0f;
float ySpeed = 1.0f;
bool isFlight = false;

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
    if(glfwGetKey(window,GLFW_KEY_SPACE) == GLFW_PRESS){
        isFlight = true;
        ySpeed = 5.0f;
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

    Shader lightSrcShader("src/vertexShader.glsl","src/fragLightSrc.glsl");
    SimpleMesh lightCube("cube");

    Shader testShader("src/vertexShader.glsl","src/fragShader.glsl");
    SimpleMesh sphereMesh("sphere");

    Shader obsShader("src/vertexShader.glsl","src/fragShaderObs.glsl");
    SimpleMesh pyramidMesh("pyramid");

    Shader grndShader("src/vertexShader.glsl","src/fragShaderGrnd.glsl");
    SimpleMesh cubeMesh("cube");

    vector<float> grndCoords(50);
    int startX = -25.0f;
    for(int i=0; i<50; i++){
        grndCoords[i] = startX + i; 
    }

    vector<float> obsCoords(10);
    int obsStartX = 3.0f;
    int gap=0.0f;
    for(int i=0; i<10; i++){
        gap+=5.0f;
        obsCoords[i] = obsStartX + gap;
    }

    float dt = 1.0f/60.0f;
    float accTime = 0.0f;
    while(!glfwWindowShouldClose(window)){
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);
        accTime += deltaTime;

        xPos = sin(glfwGetTime());
        zPos = cos(glfwGetTime());

        glClearColor(0.1f,0.1f,0.1f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // physics loop
        while(accTime>=dt){
            if(isFlight){
                float gravity = 9.8f;
                ySpeed -= gravity*dt;
                playerPos.y += ySpeed*dt;
                if(playerPos.y <= 0.0f){
                    ySpeed = 0.0f;
                    playerPos.y = 0.0f;
                    isFlight = false;
                }
            }
            playerPos.x += xSpeed*accTime;
            lightPos.x += xSpeed*accTime;
            accTime -= dt;
        }
        
        camera.Position.x = playerPos.x;

        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(radians(camera.Zoom),800.0f/600.0f,0.1f,1000.0f);
        
        glm::mat4 view = glm::mat4(1.0f);
        view = camera.GetViewMatrix();

        lightSrcShader.use();

        glm::mat4 lightSrc = glm::mat4(1.0f);
        // lightPos.x = xPos;
        // lightPos.z = zPos;
        lightSrc = glm::translate(lightSrc,lightPos);
        lightSrc = glm::scale(lightSrc,vec3(0.25f,0.25f,0.25f));
        lightSrcShader.setMat4("model",lightSrc);
        lightSrcShader.setMat4("view",view);
        lightSrcShader.setMat4("projection",projection);
        lightSrcShader.setVec3("lightColor",lightColor);
        lightCube.draw();
        
        glm::vec3 lightPosView = vec3(view*vec4(lightPos,1.0f));
        testShader.use();
        
        // rexxy
        glm::mat4 sphere = glm::mat4(1.0f);
        sphere = glm::translate(sphere,glm::vec3(playerPos.x,playerPos.y,0.0f));
        sphere = glm::scale(sphere,glm::vec3(0.5f,0.5f,0.5f));
        testShader.setMat4("model",sphere);
        testShader.setMat4("view",view);
        testShader.setMat4("projection",projection);
        testShader.setVec3("lightColor",lightColor);
        testShader.setVec3("lightPosView",lightPosView);
        sphereMesh.draw();
        
        // obstacles
        obsShader.use();
        obsShader.setMat4("view",view);
        obsShader.setMat4("projection",projection);
        obsShader.setVec3("lightColor",lightColor);
        obsShader.setVec3("lightPosView",lightPosView);

        for(int i=0; i<10; i++){
            int obsX = obsCoords[i];
            while(obsX < camera.Position.x-5.0f) obsX += 10.0f;
            while(obsX > camera.Position.x+5.0f) obsX -= 10.0f;
            glm::mat4 pyramid = glm::mat4(1.0f);
            pyramid = glm::translate(pyramid,glm::vec3(obsX,0.0f,0.0f));
            pyramid = glm::scale(pyramid,glm::vec3(0.5f,0.5f,0.5f));
            pyramid = glm::rotate(pyramid,(float)glfwGetTime(),glm::vec3(0.0f,1.0f,0.0f));
            obsShader.setMat4("model",pyramid);
            pyramidMesh.draw();
        }
        
        // ground
        grndShader.use();
        grndShader.setMat4("view",view);
        grndShader.setMat4("projection",projection);
        grndShader.setVec3("lightColor",lightColor);
        grndShader.setVec3("lightPosView",lightPosView);

        for(int i=0; i<50; i++){
            int grndX = grndCoords[i];
            while(grndX < camera.Position.x-25.0f) grndX += 50.0f;
            while(grndX > camera.Position.x+25.0f) grndX -= 50.0f;
            glm::mat4 cube = glm::mat4(1.0f);
            cube = glm::translate(cube,glm::vec3(grndX,-1.0f,0.0f));
            cube = glm::scale(cube,glm::vec3(1.0f,1.0f,1.0f));
            grndShader.setMat4("model",cube);
            cubeMesh.draw();
        }
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}