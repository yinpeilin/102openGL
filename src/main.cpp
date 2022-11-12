#include "GL_Header.h"

GLuint renderingProgram;
GLuint vao[numVAOs];

GLuint createShaderProgram()
{
    const char* vshaderSource = 
    "#version 430 \n"
    "void main(void) \n"
    "{ gl_Position = vec4(0.0, 0.0, 0.0, 1.0); }";
    const char* fshaderSource = 
    "#version 430 \n"
    "out vec4 color;\n" 
    "void main(void)\n"  
    "{ if(gl_FragCoord.x<200) {color = vec4(1.0, 0.0, 0.0, 1.0);}\n"
    "  else {color = vec4(0.0, 0.0, 1.0, 1.0);}}";
    cout<<fshaderSource<<endl;
    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vShader, 1, &vshaderSource, NULL);
    glShaderSource(fShader, 1, &fshaderSource, NULL);
    glCompileShader(vShader);
    glCompileShader(fShader);

    GLuint vfProgram = glCreateProgram();
    glAttachShader(vfProgram, vShader);
    glAttachShader(vfProgram, fShader);
    glLinkProgram(vfProgram);
    cout<<vfProgram<<endl;
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

    // glPointSize(30.0f);
    glUseProgram(renderingProgram);
    glPointSize(50.0f);

    // glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); GL_FILL
    glDrawArrays(GL_POINTS, 0, 1);

}
int main(void)
{

    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(400, 400, "Hello World", NULL, NULL);
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
