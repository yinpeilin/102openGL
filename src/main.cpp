
#include "GL_Header.h"
#include "Utils.h"
GLuint renderingProgram;
GLuint vao[numVAOs];

GLuint createShaderProgram()
{
    string vertShaderStr = readShaderSource("..\\shader\\vertShader.glsl");
    string fragShaderStr = readShaderSource("..\\shader\\fragShader.glsl");
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

    // cout<<vfProgram<<endl;
    return vfProgram;

}

void init(GLFWwindow* window)
{
    renderingProgram = createShaderProgram();
    glGenVertexArrays(numVAOs, vao);
    glBindVertexArray(vao[0]);
    
}
void display(GLFWwindow* window, GLdouble currentTime)
{
    // glClearColor(1.0, 1.0, 0.0, 1.0);
    // glClear(GL_COLOR_BUFFER_BIT);
    static float x = 0.0f;
    static float inc = 0.01f;
    // glPointSize(30.0f);

    glClear(GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(renderingProgram);
    // glPointSize(50.0f);
    x += inc;
    if(x > 1.0f){ inc = -0.01f;}
    if(x < -1.0f){ inc = 0.01f;}
    GLuint offsetLoc = glGetUniformLocation(renderingProgram, "offset");
    glProgramUniform1f(renderingProgram, offsetLoc, x);
    // glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); GL_FILL
    glDrawArrays(GL_TRIANGLES, 0, 3);

}
int main(void)
{

    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(600, 600, "Hello World", NULL, NULL);
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
    init(window);
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {

        
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
