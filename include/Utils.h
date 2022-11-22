#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h> 
#include<string>
#include<fstream> 
#include<iostream>
#define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"
using namespace std;

void printShaderLog(GLuint shader);
void printProgramLog(int prog);
bool checkOpenGLError();
string readShaderSource(const char *filePath);
GLuint loadTexture(const char *textImagePath);
GLuint createShaderProgram(const char *vertShaderPath, const char *fragShaderPath);