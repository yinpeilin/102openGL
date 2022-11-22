#pragma once
#include<GL_Header.h>
#include<vector>
#include<random>
using namespace std;
class Plane
{
public:
    glm::vec3 cvectices[4];
    glm::vec3 cposition;
    glm::vec3 cdirection;
    int cwidth_num;
    int clength_num;
    vector<glm::vec3> cpoints;
    vector<glm::vec2> ctex_coord;
    vector<GLuint> cindices;
    Plane(glm::vec3 position1,glm::vec3 position2, glm::vec3 positinCenter)
    {
        cvectices[0] = position1;
        cvectices[1] = position2;
        cvectices[2] = 2.0f*positinCenter - position1;
        cvectices[3] = 2.0f*positinCenter - position2;
        cdirection = glm::cross(cvectices[1] - cvectices[0], cvectices[2] - cvectices[1]);
        clength_num = (int)glm::length(cvectices[1] - cvectices[0]);
        cwidth_num = (int)glm::length(cvectices[3] - cvectices[0]);
        glm::vec3 min_length_vector = glm::normalize(cvectices[1] - cvectices[0]);
        glm::vec3 min_width_vector = glm::normalize(cvectices[3] - cvectices[0]);

        for(GLfloat i=0.0f;i<cwidth_num;i++)
        {
            for(GLfloat j=0.0f;j<clength_num;j++)
            {
                cpoints.push_back(cvectices[0]+i*min_width_vector+j*min_length_vector);
                ctex_coord.push_back(glm::vec2(i/cwidth_num,j/clength_num));

                if(i<cwidth_num-1&&j<clength_num-1)
                {
                    cindices.push_back((GLint)(i*clength_num+j));
                    cindices.push_back((GLint)(i*clength_num+j+1));
                    cindices.push_back((GLint)((i+1)*clength_num+j));
                    cindices.push_back((GLint)(i*clength_num+j+1));
                    cindices.push_back((GLint)((i+1)*clength_num+j));
                    cindices.push_back((GLint)((i+1)*clength_num+j+1));
                
                }
                
            }
        }
        
    } 
    void show_all()
    {

    }


};

class terrain:public Plane
{
    GLfloat cmax_height;
    GLfloat cmin_height;
    terrain(glm::vec3 position1,glm::vec3 position2, glm::vec3 positinCenter,GLfloat max_height, GLfloat min_height)
    :Plane(position1,position2,positinCenter) 
    {
        cmax_height = max_height;
        cmin_height = min_height;

    }

    void getHeightRandom(){
        
    }
    void getHeightPicture(){

    }


};