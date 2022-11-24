#pragma once
#include<GL_Header.h>
#include<vector>
#include<string>
#include <iostream>
#include <fstream>
#include <sys/stat.h> // struct stat
#include <stb_image.h>
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
    Plane()
    {

    }
    void setPosition(glm::vec3 position)
    {
       this->cposition = position;
    }
    Plane(string fileDir, string point_name = "points.bin", string texcoord_name = "texcoords.bin", string indice_name="indices.bin")
    {
        filePath[0] = fileDir + point_name;
        filePath[1] = fileDir + texcoord_name;
        filePath[2] = fileDir + indice_name;
        
        ifstream file;

        file.open(this->filePath[0], ios::in|ios::binary);
        file.read((char *)(&this->cmin_len), sizeof(GLfloat));
        file.read((char *)(&this->cdirection), 1*3*sizeof(GLfloat));
        file.read((char *)(&this->cvectices[0]), 4*3*sizeof(GLfloat));
        // 这是一个存储文件(夹)信息的结构体，其中有文件大小和创建时间、访问时间、修改时间等
	    struct stat statbuf;
	    // 提供文件名字符串，获得文件属性结构体
	    stat(this->filePath[0].c_str(), &statbuf);
	    // 获取文件大小
	    size_t filesize = statbuf.st_size;
        cout<<"the type is "<<0<<" and the filesize is "<<filesize<<endl;
        cpoints.resize((filesize - 16*sizeof(GLfloat))/(3*sizeof(GLfloat)));
        file.read((char *)(&this->cpoints[0]), this->cpoints.size()*3*sizeof(GLfloat));
        file.close();   

        stat(this->filePath[1].c_str(), &statbuf);
	    // 获取文件大小
	    filesize = statbuf.st_size;
        cout<<" the type is "<<1<<" and the filesize is "<<filesize<<endl;
        ctex_coord.resize(filesize/(2*sizeof(GLfloat)));
        file.open(this->filePath[1], ios::in|ios::binary);
        file.read((char *)(&this->ctex_coord[0]), this->ctex_coord.size()*2*sizeof(GLfloat));
        file.close();  

        stat(this->filePath[2].c_str(), &statbuf);
	    // 获取文件大小
	    filesize = statbuf.st_size;
        cout<<"the type is "<<2<<" and the filesize is"<<filesize<<endl;
        cindices.resize(filesize/(sizeof(GLuint)));
        file.open(this->filePath[2], ios::in|ios::binary);
        file.read((char *)(&this->cindices[0]), this->cindices.size()*sizeof(GLuint));
        file.close();  

        clength_num = (int)(glm::length(cvectices[1] - cvectices[0])/cmin_len);
        cwidth_num = (int)(glm::length(cvectices[3] - cvectices[0])/cmin_len);                                                                                    

    }
    Plane(glm::vec3 position1,glm::vec3 position2, glm::vec3 positinCenter,GLfloat min_len)
    {
        cmin_len = min_len;
        cvectices[0] = position1;
        cvectices[1] = position2;
        cvectices[2] = 2.0f*positinCenter - position1;
        cvectices[3] = 2.0f*positinCenter - position2;
        cdirection = glm::normalize(glm::cross(cvectices[1] - cvectices[0], cvectices[2] - cvectices[1]));
        if(cdirection[1]<0.0f){ cdirection = -cdirection;}
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
        //先储存最小距离，方向和四个顶点
        file.write((const char *)(&this->cmin_len), sizeof(GLfloat));
        file.write((const char *)(&this->cdirection), 1*3*sizeof(GLfloat));
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
                file.read((char *)(&this->cmin_len), sizeof(GLfloat));
                file.read((char *)(&this->cdirection), 1*3*sizeof(GLfloat));
                file.read((char *)(&this->cvectices[0]), 4*3*sizeof(GLfloat));
                // 这是一个存储文件(夹)信息的结构体，其中有文件大小和创建时间、访问时间、修改时间等
	            struct stat statbuf;
	            // 提供文件名字符串，获得文件属性结构体
	            stat(this->filePath[0].c_str(), &statbuf);
	            // 获取文件大小
	            size_t filesize = statbuf.st_size;
                cout<<"the type is "<<(GLuint)type<<" and the filesize is"<<filesize<<endl;
                cpoints.resize((filesize - 16*sizeof(GLfloat))/(3*sizeof(GLfloat)));
                file.read((char *)(&this->cpoints[0]), this->cpoints.size()*3*sizeof(GLfloat));
                file.close();
                this->clength_num = (int)(glm::length(cvectices[1] - cvectices[0])/cmin_len);
                this->cwidth_num = (int)(glm::length(cvectices[3] - cvectices[0])/cmin_len);          
            }
            break;
        case vectorType::TEXCOORD:
            {
                // 这是一个存储文件(夹)信息的结构体，其中有文件大小和创建时间、访问时间、修改时间等
	            struct stat statbuf;
	            // 提供文件名字符串，获得文件属性结构体
	            stat(this->filePath[1].c_str(), &statbuf);
	            // 获取文件大小
	            size_t filesize = statbuf.st_size;
                cout<<"the type is "<<(GLuint)type<<" and the filesize is"<<filesize<<endl;
                ctex_coord.resize(filesize/(2*sizeof(GLfloat)));
                file.open(this->filePath[1], ios::in|ios::binary);
                file.read((char *)(&this->ctex_coord[0]), this->ctex_coord.size()*2*sizeof(GLfloat));
                file.close();
            }
            break;
        case vectorType::INDICE:
            {
                // 这是一个存储文件(夹)信息的结构体，其中有文件大小和创建时间、访问时间、修改时间等
	            struct stat statbuf;
	            // 提供文件名字符串，获得文件属性结构体
	            stat(this->filePath[2].c_str(), &statbuf);
	            // 获取文件大小
	            size_t filesize = statbuf.st_size;
                cout<<"the type is "<<(GLuint)type<<" and the filesize is"<<filesize<<endl;
                cindices.resize(filesize/(sizeof(GLuint)));
                file.open(this->filePath[2], ios::in|ios::binary);
                file.read((char *)(&this->cindices[0]), this->cindices.size()*sizeof(GLuint));
                file.close();
            }
            break;
        default:
            cout<<"get a error of type"<<endl;
            break;
        }
    }

};

class Terrain:public Plane
{
    public:
    GLfloat cmax_height;
    Terrain()
    {

    }
    Terrain(glm::vec3 position1,glm::vec3 position2, glm::vec3 positinCenter,GLfloat min_len)
    :Plane(position1,position2,positinCenter,min_len) 
    {

    }
    Terrain(string fileDir, string point_name = "points.bin", string texcoord_name = "texcoords.bin", string indice_name="indices.bin"):Plane(fileDir, point_name , texcoord_name, indice_name)
    {

    }
    void setMax_height(GLfloat max_height)
    {
        cmax_height = max_height;
    }
    void getHeightChangeFromPicture(string fileDir = "..\\resource\\",string img_name = "image.jpg")
    {
        
        int width, height, nrChannels;
        string img_path = fileDir+img_name;
        const char * new_img_path = img_path.c_str();
        unsigned char *data = stbi_load(new_img_path, &width, &height, &nrChannels, 0);
        unsigned bytePerPixel = nrChannels;
        int temp = 0;
        // cout<<glm::to_string(cdirection)<<endl;
        // cout<<cmax_height<<endl;
        for(int i=0;i<cwidth_num;i++)
        {
	        for(int j=0;j<clength_num;j++)
	        {
                // cout<<this->ctex_coord[i*clength_num+j][0] *height +(int)this->ctex_coord[i*clength_num+j][1]<<endl;
                // unsigned char* pixelOffset = data + ((int)this->ctex_coord[i*clength_num+j][0] *height +(int)this->ctex_coord[i*clength_num+j][1] *width) * bytePerPixel;
                unsigned char* pixelOffset = data + (int)(this->ctex_coord[i*clength_num+j][0] *width*height +this->ctex_coord[i*clength_num+j][1]*height) * bytePerPixel;
                // cout<<(GLuint64)pixelOffset<<endl;
                GLint r = (GLuint)pixelOffset[0];
                // cpoints[i*clength_num+j] = ((cmax_height)*(r)*cdirection)/200.0f;
                
                cpoints[i*clength_num+j] += ((GLfloat)((GLfloat)(cmax_height)*(r-55)/200.0f)*cdirection);
                // cpoints[i*clength_num+j][0] += ((cmax_height)*(r-55)*cdirection[0])/200.0f;
                // cpoints[i*clength_num+j][1] += ((cmax_height)*(r-55)*cdirection[1])/200.0f;
                // cpoints[i*clength_num+j][2] += ((cmax_height)*(r-55)*cdirection[2])/200.0f;
                // cout<<glm::to_string(cpoints[i*clength_num+j]);
                // unsigned char g = pixelOffset[1];
                // unsigned char b = pixelOffset[2];
                // unsigned char a = nrChannels >= 4 ? pixelOffset[3] : 0xff;
                
            }
        }
        stbi_image_free(data);
    }

};

