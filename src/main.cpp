#include <iostream>
#include <bits/stdc++.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
using namespace std;
using namespace glm;

void framebuffer_size_callback(GLFWwindow* window,int width,int height){
    glViewport(0,0,width,height);
}

void processInput(GLFWwindow* window){
    if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window,true);
    }
}

static bool isJumping = false;
static float velocity = 0.0f;
float gravity = -0.01f;
vec3 playerInput(GLFWwindow* window,vec3 position){
    if(glfwGetKey(window,GLFW_KEY_SPACE) == GLFW_PRESS){
        isJumping = true;
        velocity = 0.015f/10;
    }
    if(isJumping){
        position.y += velocity;
        velocity += gravity;
        if(position.y <= -1.75f){
            position.y = -1.75f;
            isJumping = false;
        }
    }
    return position;
}

int main(){
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800,600,"gl_rex",NULL,NULL);
    if(window==NULL){
        cout<<"failed to initialize a window"<<endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        cout<<"failed to initialize GLAD"<<endl;
        return -1;
    }

    Shader rectShader("src/rectVertShader.glsl","src/rectFragShader.glsl");
    Shader playerShader("src/playerVertShader.glsl","src/playerFragShader.glsl");

    float vertices[] = {
        // vertices
        -1.0f,1.0f,0.0f,
        1.0f,1.0f,0.0f,
        1.0f,-1.0f,0.0f,
        -1.0f,-1.0f,0.0f
    };

    unsigned int indices[] = {
        0,1,3,
        1,2,3
    };

    unsigned int VAO;
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    
    unsigned int EBO;
    glGenBuffers(1,&EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    float worldWidth = 10.f;
    float worldHeight = 8.0f;

    vec3 groundPos = vec3(0.0f, -4.0f, 0.0f);
    vec3 playerPos = vec3(-4.5f, -1.75f, 0.0f);

    glViewport(0,0,800,600);
    glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);

    while(!glfwWindowShouldClose(window)){
        processInput(window);
        
        glClearColor(0.1f,0.1f,0.1f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        mat4 projection = scale(mat4(1.0f),vec3(1.0f/(worldWidth/2.0f),1.0f/(worldHeight/2.0f),1.0f));

        // ground;
        rectShader.use();
        mat4 groundModel = mat4(1.0f);
        groundModel = translate(groundModel,groundPos);
        groundModel = scale(groundModel,vec3(10.0f, 2.0f, 1.0f));
        rectShader.setMat4("Transform",projection*groundModel);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);

        // player
        playerShader.use();
        playerPos = playerInput(window,playerPos);
        mat4 playerModel = mat4(1.0f);
        playerModel = translate(playerModel,playerPos);
        playerModel = scale(playerModel,vec3(0.25f, 0.25f, 1.0f));
        playerShader.setMat4("Transform",projection*playerModel);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}