#include <GL/glew.h>
#include <GLFW/glfw3.h> 
#include <iostream> 
#include<fstream>
#include<string>
#include"Utils.h"
using namespace std;

#define numVAOS 1

GLuint renderingProgram;
GLuint vao[numVAOS];

float x = 0.0f; /*location of triangle on the x axix*/
float inc = 0.01f; /*offset for moving the triangle*/

GLuint createShaderProgram()
{
    const char* vshaderSource =
        "#version 430 \n"
        "void main(void) \n"
        "{ if (gl_VertexID == 0) gl_Position = vec4( 0.25, -0.25, 0.0, 1.0);\n"
        "else if (gl_VertexID == 1) gl_Position = vec4(-0.25, -0.25, 0.0, 1.0);\n"
        "else gl_Position = vec4(0.25, 0.25, 0.0, 1.0);}";


    const char* fshaderSource =
        "#version 430 \n"
        "out vec4 color; \n"
        "void main(void) \n"
        "{ color = vec4(0.0, 0.0, 1.0, 1.0); }";

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

    return vfProgram;

}



void init(GLFWwindow* window)
{
    renderingProgram = createShaderProgram1("VertShader.glsl", "FragShader.glsl");

    glGenVertexArrays(numVAOS, vao);
    glBindVertexArray(vao[0]);


}

void display(GLFWwindow* window, double currentTime)
{
    glClear(GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT); /*Clear the background color to black*/

    glUseProgram(renderingProgram);

    x += inc;
    if (x > 1.0f) inc = -0.01f;
    if (x < -1.0f) inc = 0.01f;
    GLuint offsetLoc = glGetUniformLocation(renderingProgram, "offset"); /*get ptr to "offset"*/
    glProgramUniform1f(renderingProgram, offsetLoc, x); /*send value in x to "offset"*/

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

int main(void)
{
    if (!glfwInit()) { exit(EXIT_FAILURE); }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    GLFWwindow* window = glfwCreateWindow(600, 600, "Animated Triangle", NULL, NULL);
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
    glfwSwapInterval(1);
    init(window);
    while (!glfwWindowShouldClose(window))
    {
        display(window, glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}