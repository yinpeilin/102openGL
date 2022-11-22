#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h> 
#include<string>
#include<fstream> 
#include<iostream>

using namespace std;

void printShaderLog(GLuint shader);
void printProgramLog(int prog);
bool checkOpenGLError();
string readShaderSource(const char *filePath);
GLuint loadTexture(const char *textImagePath);
GLuint createShaderProgram(const char *vertShaderPath, const char *fragShaderPath);