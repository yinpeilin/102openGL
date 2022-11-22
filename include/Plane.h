#pragma once
#include<GL_Header.h>
#include<random>
class Plane
{
public:
    glm::vec3 cvectices[4];
    glm::vec3 cposition;
    glm::vec3 cdirection;
    int cwidth_num;
    int clength_num;
    vector<float> cpoints;
    vector<int> cindices;
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
                cpoints.push_back(cvectices[0][0]+i*min_width_vector[0]+j*min_length_vector[0]);
                cpoints.push_back(cvectices[0][1]+i*min_width_vector[1]+j*min_length_vector[1]);
                cpoints.push_back(cvectices[0][2]+i*min_width_vector[2]+j*min_length_vector[2]);
                cpoints.push_back(i/cwidth_num);
                cpoints.push_back(j/clength_num);
                if(i<cwidth_num-1&&j<clength_num)
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