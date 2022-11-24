#include "GL_Header.h"

float cubeLocX, cubeLocY, cubeLocZ;

void window_reshape_callback(GLFWwindow* window, int newWidth, int newHeight);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);


// camera
Camera camera(glm::vec3(0.0f,50.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f));
int camera_Loc_terrain[2] = {2, 2};
glm::vec3 camera_center_terrain = glm::vec3(0.0f,0.0f,0.0f);
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
 
// plane
// Plane testPlane("..\\resource\\");
Terrain **TerrainMatrix;
// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
GLuint myTexture;


GLuint renderingProgram;
GLuint terrainRenderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

// 分配在 display() 函数中使用的变量空间，这样它们就不必在渲染过程中分配 
GLuint mLoc,vLoc,projLoc;

int width = SCR_WIDTH;
int height = SCR_HEIGHT; 
float aspect;
glm::mat4 pMat, vMat, mMat, mvMat;

void setupVertices(void) {    
    // 36个顶点，12个三角形，组成了放置在原点 处的2×2×2立方体
    //四面体绘制   
    float vertexPositions[54] = {
      -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // 前面
1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // 右面
1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // 后面
-1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // 左面
-1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, // 底面 — 左前一半
1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f // 底面 — 右后一半
};
    float pyrTexCoords[36] =
    {   0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,   0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 1.0f,   // 前侧面、右侧面
        0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,   0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 1.0f,   // 后侧面、左侧面
        0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,   1.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 0.0f }; // 底面的两个三角形
    glGenVertexArrays(numVAOs, vao); 
    glGenBuffers(numVBOs, vbo);


    glBindVertexArray(vao[0]); 
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyrTexCoords), pyrTexCoords, GL_STATIC_DRAW);

    glBindVertexArray(vao[1]); 
    
    // glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
    // glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(pyrTexCoords), pyrTexCoords, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,TerrainMatrix[0][0].cindices.size()*4, &(TerrainMatrix[0][0].cindices[0]), GL_STATIC_DRAW);
    
    // glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
    // glBufferData(GL_ARRAY_BUFFER, TerrainMatrix[0][0].cpoints.size()*3*4, &(TerrainMatrix[0][0].cpoints[0]), GL_STATIC_DRAW);
    
    int temp_num = (GLuint)sqrt(MAX_TERRAIN);
    for(int i=0;i<temp_num;i++)
    {
        for(int j=0;j<temp_num;j++)
        {
            glBindBuffer(GL_ARRAY_BUFFER, vbo[3+i*temp_num+j]);
            glBufferData(GL_ARRAY_BUFFER, TerrainMatrix[i][j].cpoints.size()*3*4, &(TerrainMatrix[i][j].cpoints[0]), GL_STATIC_DRAW);
        }
    }
    
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, testPlane.cindices_len, testPlane.cindices, GL_STATIC_DRAW);
    

    

}


void init(GLFWwindow* window)
{
    Terrain testTerrain(glm::vec3(50.0f,0.0f,-50.0f),glm::vec3(50.0f,0.0f,50.0f),glm::vec3(0.0f,0.0f,0.0f),1.0f);
    testTerrain.setMax_height(10.0f);
    testTerrain.setFilePath("..\\resource\\","terrainPoints1.bin","texcoords.bin","indices.bin");
    testTerrain.getHeightChangeFromPicture("..\\resource\\","terrainHeight.png");
    testTerrain.write_all();

    int temp_num = (GLuint)sqrt(MAX_TERRAIN);
    TerrainMatrix = new Terrain* [temp_num];
    for(int i=0;i<temp_num;i++)
    {
        TerrainMatrix[i] = new Terrain [temp_num];
    }
    for(int i=0;i<temp_num;i++)
    {
        for(int j=0;j<temp_num;j++)
        {
            TerrainMatrix[i][j].setFilePath("..\\resource\\","terrainPoints1.bin","texcoords.bin","indices.bin");
            TerrainMatrix[i][j].read(vectorType::POINT);
            TerrainMatrix[i][j].read(vectorType::INDICE);
            TerrainMatrix[i][j].read(vectorType::TEXCOORD);
            TerrainMatrix[i][j].setPosition(glm::vec3((i-2)*100.0f,0,(j-2)*100.0f));
        }
    }
    // testPlane.setFilePath();
    // testPlane.write_all();
    // testPlane.read(vectorType::POINT);
    // TerrainMatrix[0][0].setMax_height(50.0f);
    // TerrainMatrix[0][0].getHeightChangeFromPicture("..\\resource\\","terrainHeight.png");
    // TerrainMatrix[0][0].setFilePath("..\\resource\\","terrainPoints1.bin","texcoords.bin","indices.bin");
    // TerrainMatrix[0][0].write_all();

    renderingProgram = createShaderProgram("..\\shader\\vertShader.glsl","..\\shader\\fragShader.glsl");
    terrainRenderingProgram = createShaderProgram("..\\shader\\terrainVertShader.glsl",
                                                "..\\shader\\terrainFragShader.glsl");
    // glGenVertexArrays(numVAOs, vao);
    // glBindVertexArray(vao[0]);
    cubeLocX = 0.0f; cubeLocY = -2.0f; cubeLocZ = 0.0f;
    // 沿Y轴下移 以展示透视
    setupVertices();

    glfwGetFramebufferSize(window, &width, &height); 
    aspect = (float)width / (float)height;
    pMat = glm::perspective(glm::radians(camera.Fov), aspect, 0.1f, 2000.0f);  
    // 1.0472 radians = 60 degrees

    myTexture = loadTexture("../resource/image.jpg");
    
}
void display(GLFWwindow* window, GLdouble currentTime)
{
    // glClearColor(1.0, 1.0, 0.0, 1.0);
    // glClear(GL_COLOR_BUFFER_BIT);
    // static float x = 0.0f;
    // static float inc = 0.01f;
    // glPointSize(30.0f);

    glClear(GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray(vao[0]); 
    glUseProgram(renderingProgram);

    // // 获取MV矩阵和投影矩阵的统一变量
    mLoc = glGetUniformLocation(renderingProgram, "m_matrix"); 
    vLoc = glGetUniformLocation(renderingProgram, "v_matrix"); 
    projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");
    // // 构建透视矩阵
    // // glfwGetFramebufferSize(window, &width, &height); 
    // // aspect = (float)width / (float)height;

    // // pMat = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 1000.0f); // 1.0472 radians = 60 degrees


    // //transform矩阵和旋转矩阵
    // // glm::mat4 tMat = glm::translate(glm::mat4(1.0f), glm::vec3(sin(0.35f*currentTime)*2.0f,cos(0.52f*currentTime)*2.0f, sin(0.7f*currentTime)*2.0f)); 
    // // glm::mat4 rMat = glm::rotate(glm::mat4(1.0f), 1.75f*(float)currentTime, glm::vec3(0.0f, 1.0f, 0.0f));
    // // rMat = glm::rotate(rMat, 1.75f*(float)currentTime, glm::vec3(1.0f,0.0f, 0.0f));
    // // rMat = glm::rotate(rMat, 1.75f*(float)currentTime, glm::vec3(0.0f,0.0f, 1.0f));

    // //构建视图矩阵、模型矩阵和视图-模型矩阵
    vMat = camera.GetViewMatrix();
    // // vMat = glm::translate(glm::mat4(1.0f), -camera.Position);
    mMat = glm::translate(glm::mat4(1.0f), glm::vec3(cubeLocX, cubeLocY, cubeLocZ));
    // // mMat = tMat * rMat;
    // // mvMat = vMat * mMat;

    // // 将透视矩阵和MV矩阵复制给相应的统一变量
    glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(vMat));
    glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(mMat));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
    // // 将VBO关联给顶点着色器中相应的顶点属性 
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); 
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0); 
    glEnableVertexAttribArray(1);

    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, myTexture);

    // // 调整OpenGL设置，绘制模型 
    glEnable(GL_DEPTH_TEST); 
    glDepthFunc(GL_LEQUAL);
    // // glDrawArrays(GL_TRIANGLES, 0, 36);
    // //背面剔除
    // // glFrontFace(GL_CW);             // 顶点的缠绕顺序为顺时针方向
    // // glFrontFace(GL_CCW);            // 顶点缠绕顺序为逆时针方向
    // //多实例化绘图
    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, 10000);
    

    glBindVertexArray(vao[1]); 
    glUseProgram(terrainRenderingProgram);

    // 获取MV矩阵和投影矩阵的统一变量
    mLoc = glGetUniformLocation(terrainRenderingProgram, "m_matrix"); 
    vLoc = glGetUniformLocation(terrainRenderingProgram, "v_matrix"); 
    projLoc = glGetUniformLocation(terrainRenderingProgram, "proj_matrix");

    //构建视图矩阵、模型矩阵和视图-模型矩阵
    vMat = camera.GetViewMatrix();
    // camera.position
    // camera.Loc
    //camera.现在的矩阵中心
    int temp = 5;
    if(camera.Position[0]>=camera_center_terrain[0]+50.0f)
    {
        camera_Loc_terrain[0]++;
        camera_Loc_terrain[0]%=temp;
        camera_center_terrain = TerrainMatrix[camera_Loc_terrain[0]][camera_Loc_terrain[1]].cposition;
    }
    else if(camera.Position[0]<=camera_center_terrain[0]+50.0f)
    {
        camera_Loc_terrain[0]--;
        camera_Loc_terrain[0]+=temp;
        camera_Loc_terrain[0]%=temp;
        camera_center_terrain = TerrainMatrix[camera_Loc_terrain[0]][camera_Loc_terrain[1]].cposition;
    }

    if(camera.Position[1]>=camera_center_terrain[1]+50.0f)
    {
        camera_Loc_terrain[1]++;
        camera_Loc_terrain[1]%=temp;
        camera_center_terrain = TerrainMatrix[camera_Loc_terrain[0]][camera_Loc_terrain[1]].cposition;
    }
    else if(camera.Position[1]<=camera_center_terrain[1]+50.0f)
    {
        camera_Loc_terrain[1]--;
        camera_Loc_terrain[1]+=temp;
        camera_Loc_terrain[1]%=temp;
        camera_center_terrain = TerrainMatrix[camera_Loc_terrain[0]][camera_Loc_terrain[1]].cposition;
    }

    for(int i=camera_Loc_terrain[0]-1;i<=camera_Loc_terrain[0]+1;i++)
    {
        for(int j=camera_Loc_terrain[1]-1;j<=camera_Loc_terrain[1]+1;j++)
        {
            
            TerrainMatrix[i%temp][j%temp].setPosition(camera_center_terrain+(i-camera_Loc_terrain[0])*glm::vec3(100.0f,0.0f,0.0f)+(j-camera_Loc_terrain[1])*glm::vec3(0.0f,0.0f,100.0f));
            cout<<glm::to_string(TerrainMatrix[i%temp][j%temp].cposition)<<endl;
            mMat = glm::translate(glm::mat4(1.0f), TerrainMatrix[i%temp][j%temp].cposition);
            // 将透视矩阵和MV矩阵复制给相应的统一变量
            glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(vMat));
            glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(mMat));
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
            // 将VBO关联给顶点着色器中相应的顶点属性 
            glBindBuffer(GL_ARRAY_BUFFER, vbo[3+i*temp+j]);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0); 
            glEnableVertexAttribArray(0);
            glEnable(GL_DEPTH_TEST); 
            glEnable(GL_MULTISAMPLE);
            glDepthFunc(GL_LEQUAL);
            glDrawElementsInstanced(GL_TRIANGLES, TerrainMatrix[0][0].cindices.size(), GL_UNSIGNED_INT, nullptr, 1);
        }
    }
    
    // vMat = glm::translate(glm::mat4(1.0f), -camera.Position);
    
    // mMat = tMat * rMat;
    // mvMat = vMat * mMat;

    

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);
    // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0); 
    // glEnableVertexAttribArray(1);


    // 调整OpenGL设置，绘制模型 
    
    // glDrawArrays(GL_TRIANGLES, 0, 36);
    //背面剔除
    // glFrontFace(GL_CW);             // 顶点的缠绕顺序为顺时针方向
    // glFrontFace(GL_CCW);            // 顶点缠绕顺序为逆时针方向
    //多实例化绘图
    // glDrawElementsInstanced(GL_LINE_STRIP, TerrainMatrix[0][0].cindices.size(), GL_UNSIGNED_INT, nullptr, 1);
     
    // glDrawArraysInstanced(GL_TRIANGLES, 0, testPlane.cpoints_len,1);
    // glPointSize(50.0f);
    // x += inc;
    // if(x > 1.0f){ inc = -0.01f;}
    // if(x < -1.0f){ inc = 0.01f;}
    // GLuint offsetLoc = glGetUniformLocation(renderingProgram, "offset");
    // glProgramUniform1f(renderingProgram, offsetLoc, x);
    // glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); GL_FILL
    // glDrawArrays(GL_TRIANGLES, 0, 3);

}
int main(void)
{
    

    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    if (glewInit() == GLEW_OK)
    {
        cout << "GLEW IS OK\n";
    }
    glfwSwapInterval(1);

    //get your I/O input
    glfwSetWindowSizeCallback(window, window_reshape_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);



    
    init(window);

    float currentFrame;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        cout<<glm::to_string(camera.Position)<<endl;
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
 
        // input
        // -----
        processInput(window);

        /* Render here */
        display(window, glfwGetTime());

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
    return 0;
}


void window_reshape_callback(GLFWwindow* window, int newWidth, int newHeight) 
{
    aspect = (float)newWidth / (float)newHeight;    // 回调提供的新的 宽度、高度
    glViewport(0, 0, newWidth, newHeight);          // 设置和帧缓冲区 相关的屏幕区域
    pMat = glm::perspective(glm::radians(camera.Fov), aspect, 0.1f, 10000.0f); 
}
void processInput(GLFWwindow *window)
{
    // std::cout<<glm::to_string(camera.Position)<<std::endl;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
        // cout<<deltaTime<<endl;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;
    camera.ProcessMouseMovement(xoffset, yoffset);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
    pMat = glm::perspective(glm::radians(camera.Fov), aspect, 0.1f, 10000.0f); 
}