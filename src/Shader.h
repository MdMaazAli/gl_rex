#include <iostream>
#include <bits/stdc++.h>
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <glm/glm.hpp>
using namespace std;

class Shader{
    public:
        unsigned int ID;
        Shader(const char* vertexShaderPath,const char* fragShaderPath){
            string vertexCode;
            string fragCode;
            ifstream vertexShaderFile;
            ifstream fragShaderFile;
            
            vertexShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
            fragShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
            try{
                stringstream vertexShaderStream;
                stringstream fragShaderStream;

                vertexShaderFile.open(vertexShaderPath);
                fragShaderFile.open(fragShaderPath);
                vertexShaderStream<<vertexShaderFile.rdbuf();
                fragShaderStream<<fragShaderFile.rdbuf();

                vertexCode = vertexShaderStream.str();
                fragCode = fragShaderStream.str();
            }
            catch(ifstream::failure& e){
                cout<<"ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ"<<e.what()<<endl;
            }

            const char* vertexShaderSource = vertexCode.c_str();
            const char* fragShaderSource = fragCode.c_str();

            unsigned int vertexShader;
            vertexShader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertexShader,1,&vertexShaderSource,0);
            glCompileShader(vertexShader);
            
            unsigned int fragShader;
            fragShader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragShader,1,&fragShaderSource,0);
            glCompileShader(fragShader);

            ID = glCreateProgram();
            glAttachShader(ID,vertexShader);
            glAttachShader(ID,fragShader);
            glLinkProgram(ID);
        }

        void use(){
            glUseProgram(ID);
        }

        void setBool(const string &name,bool value){
            glUniform1i(glGetUniformLocation(ID,name.c_str()),(int)value);
        }

        void setInt(const string &name,int value){
            glUniform1i(glGetUniformLocation(ID,name.c_str()),value);
        }

        void setFloat(const string &name,float value){
            glUniform1f(glGetUniformLocation(ID,name.c_str()),value);
        }

        void setVec2(const std::string &name, const glm::vec2 &value) const
        { 
            glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
        }
        void setVec2(const std::string &name, float x, float y) const
        { 
            glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y); 
        }
        // ------------------------------------------------------------------------
        void setVec3(const std::string &name, const glm::vec3 &value) const
        { 
            glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
        }
        void setVec3(const std::string &name, float x, float y, float z) const
        { 
            glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z); 
        }
        // ------------------------------------------------------------------------
        void setVec4(const std::string &name, const glm::vec4 &value) const
        { 
            glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
        }
        void setVec4(const std::string &name, float x, float y, float z, float w) const
        { 
            glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w); 
        }
        // ------------------------------------------------------------------------
        void setMat2(const std::string &name, const glm::mat2 &mat) const
        {
            glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
        // ------------------------------------------------------------------------
        void setMat3(const std::string &name, const glm::mat3 &mat) const
        {
            glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
        // ------------------------------------------------------------------------
        void setMat4(const std::string &name, const glm::mat4 &mat) const
        {
            glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
};