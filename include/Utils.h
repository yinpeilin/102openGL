#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h> 
#include<string>
#include<fstream> 
#include<iostream>
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