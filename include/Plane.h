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
    int call_num;
    float *cpoints;
    int cpoints_len = 0;
    GLuint *cindices;
    int cindices_len = 0;
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
        call_num = clength_num*cwidth_num;
        cpoints_len = call_num * 5;
        cindices_len = (clength_num - 1)*(cwidth_num-1)*6;
        cpoints = new float [cpoints_len];
        cindices = new GLuint [cindices_len];
        int temp = 0;
        int temp2 = 0;
        for(GLfloat i=0.0f;i<cwidth_num;i++)
        {
            for(GLfloat j=0.0f;j<clength_num;j++)
            {
                cpoints[temp] = (cvectices[0][0]+i*min_width_vector[0]+j*min_length_vector[0]);
                temp++;
                cpoints[temp] = (cvectices[0][1]+i*min_width_vector[1]+j*min_length_vector[1]);
                temp++;
                cpoints[temp] = (cvectices[0][2]+i*min_width_vector[2]+j*min_length_vector[2]+j);
                temp++;
                cpoints[temp] = (i/cwidth_num);
                temp++;
                cpoints[temp] = (j/clength_num);
                temp++;
                if(i<cwidth_num-1&&j<clength_num-1)
                {
                    cindices[temp2] = ((GLint)(i*clength_num+j));
                    temp2++;
                    cindices[temp2] = ((GLint)(i*clength_num+j+1));
                    temp2++;
                    cindices[temp2] = ((GLint)((i+1)*clength_num+j));
                    temp2++;
                    cindices[temp2] = ((GLint)(i*clength_num+j+1));
                    temp2++;
                    cindices[temp2] = ((GLint)((i+1)*clength_num+j));
                    temp2++;
                    cindices[temp2] = ((GLint)((i+1)*clength_num+j+1));
                    temp2++;
                }
                
            }
        }
        
    } 
    void show_all()
    {
        cout<<this->call_num<<endl;
        cout<<this->cpoints_len<<endl;
        cout<<this->cindices_len<<endl;
        for(int i=0; i<this->cpoints_len; i++)
        {
            cout<<this->cpoints[i]<<endl;
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