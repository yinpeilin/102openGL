#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h> 
#include<string>
#include<fstream> 
#include<iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


using namespace std;
void printShaderLog(GLuint shader)
{
    int len = 0;
    int chWritten = 0;
    char *log;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    if(len > 0)
    {
        log = new char[len];
        glGetShaderInfoLog(shader, len, &chWritten, log);
        cout<< "Shader Info Log:"<<log<<endl;
        delete [] log;
    }
}

void printProgramLog(int prog)
{
    int len = 0;
    int chWritten = 0;
    char *log;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
    if(len > 0)
    {
        log = new char[len];
        glGetProgramInfoLog(prog, len, &chWritten, log);
        cout<< "Program Info Log:"<<log<<endl;
        delete [] log;
    }

}

bool checkOpenGLError()
{
    bool foundError = false;
    int glErr = glGetError();
    while(glErr != GL_NO_ERROR)
    {
        cout<<"glError: "<<glErr<<endl;
        foundError = true;
        glErr = glGetError();
    }
    return foundError;
}

string readShaderSource(const char *filePath)
{
    string content;
    ifstream fileStream(filePath,ios::in);
    string line = "";
    while(!fileStream.eof())
    {
        getline(fileStream, line);
        content.append(line + "\n");
    }
    fileStream.close();
    // cout<<content<<endl;
    return content;
}

GLuint loadTexture(const char *textImagePath) 
{ 
    GLuint textureID;
    

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(textImagePath, &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    return textureID;
}