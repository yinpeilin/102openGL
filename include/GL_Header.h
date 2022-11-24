#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>  
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <cmath>
#include <string>
#include "Utils.h"
#include "Camera.h"
#include "Plane.h"
using namespace std;


#define numVAOs 2
#define numVBOs 28
#define MIN_LENGTH 0.1f
#define MAX_TERRAIN 25
// settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 1200;
