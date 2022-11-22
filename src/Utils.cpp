#include"Utils.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
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
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // glGenerateMipmap(GL_TEXTURE_2D);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 如果还启用各向异性过滤
    if (!glewIsSupported("GL_EXT_texture_filter_anisotropic")) { 
        cout<<1<<endl;
        GLfloat anisoSetting = 0.0f;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisoSetting); 
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT,
        anisoSetting); 
    }

    int width, height, nrChannels;
    unsigned char *data = stbi_load(textImagePath, &width, &height, &nrChannels, 0);
    cout<<data<<endl;
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        cout<<2<<endl;
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    return textureID;
}

GLuint createShaderProgram(const char *vertShaderPath, const char *fragShaderPath)
{
    string vertShaderStr = readShaderSource(vertShaderPath);
    string fragShaderStr = readShaderSource(fragShaderPath);
    const char* vshaderSource = vertShaderStr.c_str();
    const char* fshaderSource = fragShaderStr.c_str();
    
    // cout << vshaderSource<<endl;
    // cout<<fshaderSource<<endl;

    GLint vertCompiled;
    GLint fragCompiled;
    GLint linked;

    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
    // cout<<"vShader: " <<vShader<<endl;
    // cout<<"fShader: "<< fShader<<endl;
    glShaderSource(vShader, 1, &vshaderSource, NULL);
    glCompileShader(vShader);
    
    //捕获编译着色器错误
    checkOpenGLError();
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
    if(vertCompiled !=1)
    {
        cout << "vertex compilation failed" << endl;
        printShaderLog(vShader);
    }

    
    glShaderSource(fShader, 1, &fshaderSource, NULL);
    glCompileShader(fShader);
    checkOpenGLError();
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
    if(fragCompiled !=1)
    {
        cout << "fragment compilation failed" << endl;
        printShaderLog(fShader);
    }


    GLuint vfProgram = glCreateProgram();
    glAttachShader(vfProgram, vShader);
    glAttachShader(vfProgram, fShader);
    glLinkProgram(vfProgram);
    checkOpenGLError();
    glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
    if(linked !=1)
    {
        cout<<"linking failed"<<endl;
        printProgramLog(vfProgram);
    }
    return vfProgram;
}