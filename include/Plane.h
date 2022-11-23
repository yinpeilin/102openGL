#pragma once
#include<GL_Header.h>
#include<vector>
#include<string>
#include <iostream>
#include <fstream>
#include <sys/stat.h> // struct stat
using namespace std;

enum class vectorType{
    POINT,
    TEXCOORD,
    INDICE
};

class Plane
{
public:
    glm::vec3 cvectices[4];
    glm::vec3 cposition;
    glm::vec3 cdirection;
    GLfloat cmin_len;
    int cwidth_num;
    int clength_num;
    vector<glm::vec3> cpoints;
    vector<glm::vec2> ctex_coord;
    vector<GLuint> cindices;
    string filePath [3];
    Plane(glm::vec3 position1,glm::vec3 position2, glm::vec3 positinCenter,GLfloat min_len)
    {
        cmin_len = min_len;
        cvectices[0] = position1;
        cvectices[1] = position2;
        cvectices[2] = 2.0f*positinCenter - position1;
        cvectices[3] = 2.0f*positinCenter - position2;
        cdirection = glm::normalize(glm::cross(cvectices[1] - cvectices[0], cvectices[2] - cvectices[1]));
        clength_num = (int)(glm::length(cvectices[1] - cvectices[0])/cmin_len);
        cwidth_num = (int)(glm::length(cvectices[3] - cvectices[0])/cmin_len);
        glm::vec3 min_length_vector = glm::normalize(cvectices[1] - cvectices[0])*cmin_len;
        glm::vec3 min_width_vector = glm::normalize(cvectices[3] - cvectices[0])*cmin_len;

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

    void setFilePath(string fileDir = "..\\resource\\", string point_name = "points.bin", string texcoord_name = "texcoords.bin", string indice_name="indices.bin")
    {
        filePath[0] = fileDir + point_name;
        filePath[1] = fileDir + texcoord_name;
        filePath[2] = fileDir + indice_name;

    }
    // 点列,shader索引,indice
    void write_all()
    {
        //创建流对象
        ofstream file;
        //打开文件 
        file.open(this->filePath[0], ios::out | ios::binary);
        //先储存四个顶点
        file.write((const char *)(&this->cvectices[0]), 4*3*sizeof(GLfloat));
        //再存储所有顶点
        file.write((const char *)(&this->cpoints[0]), this->cpoints.size()*3*sizeof(GLfloat));
        file.close();

        file.open(this->filePath[1], ios::out | ios::binary);
        file.write((const char *)(&this->ctex_coord[0]), this->ctex_coord.size()*2*sizeof(GLfloat));
        file.close();

        file.open(this->filePath[2], ios::out | ios::binary);
        file.write((const char *)(&this->cindices[0]), this->cindices.size()*sizeof(GLuint));
        file.close();
    } 
    // 读取 点列,shader索引,indice
    void read(vectorType type)
    {
        ifstream file;
        switch (type)
        {
        case vectorType::POINT:
            {
                file.open(this->filePath[0], ios::in|ios::binary);
            // GLfloat temp ;
                glm::vec3 temp[4];
                file.read((char *)temp,4*3*4);
                cout<<glm::to_string(this->cvectices[0])<<glm::to_string(this->cvectices[1])<<endl;
                cout<<glm::to_string(temp[0])<<glm::to_string(temp[1])<<endl;
            }
            break;
        case vectorType::TEXCOORD:
            cout<<2<<endl;
            break;
        case vectorType::INDICE:
            cout<<3<<endl;
            break;
        default:
            cout<<"get a error of type"<<endl;
            break;
        }
    }

};

class terrain:public Plane
{
    GLfloat cmax_height;
    GLfloat cmin_height;
    terrain(glm::vec3 position1,glm::vec3 position2, glm::vec3 positinCenter,GLfloat min_len,GLfloat max_height, GLfloat min_height)
    :Plane(position1,position2,positinCenter,min_len) 
    {
        cmax_height = max_height;
        cmin_height = min_height;

    }

    void getHeightRandom(){
        
    }
    void getHeightPicture(){

    }




};