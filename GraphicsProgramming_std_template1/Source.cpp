#include <GL\glew.h> 
#include <GLFW\glfw3.h> 
#include <string> 
#include <iostream> 
#include <fstream> 
#include <cmath> 
#include <glm\glm.hpp> 
#include <glm\gtc\type_ptr.hpp> 
#include <glm\gtc\matrix_transform.hpp> 
#include"Utils.h"

#define numVAOs 1
#define numVBOs 2

float tf = 0;
float cameraX, cameraY, cameraZ;
float cubeLocX, cubeLocY, cubeLocZ;
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

/*allocate variables used in the display() function, so that they won't needed be allocated during rendering*/
GLuint mvLoc, projLoc,vLoc,tfLoc;
int width, height;
float aspect,timeFactor;
glm::mat4 pMat, vMat, mMat, mvMat, tMat, rMat;

void setupVertices(void)  /*12 triangles, 36 vertices, makes a 2:2:2 cube centered at the origin */
{
	float vertexPositions[108] =
	{
		-1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f
	};
	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(numVBOs, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); /*activate the buffer indicated by vbo[0]*/
	/*copy the array containing the vertices into the active buffer(s)*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
}

void init(GLFWwindow* window)
{
	renderingProgram = createShaderProgram("vertShader.glsl", "fragShader.glsl");
	cameraX = 0.0f, cameraY = 0.0f, cameraZ = 420.0f;
	cubeLocX = 0.0f, cubeLocY = -2.0f, cubeLocZ = 0.0f; /*shiftdown Y to reveal prospective*/
	setupVertices();
}

void display(GLFWwindow* window, double currTime)
{
	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(renderingProgram);

	/*get locations of uniform variables in the shader program (from the MV and projection matrices)*/
	vLoc = glGetUniformLocation(renderingProgram, "v_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

	/*build perspective matrix*/
	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f); /*1.0742 rad=60 degrees*/
	
	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));


	/*originally in loop*/
	



	/*copy perspective and MV matrices' data to uniform variables*/
	glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(vMat)); /*shader needs the V matrix*/ 
	timeFactor = ((float)currTime); /*uniform for the time factor*/ 
	tfLoc = glGetUniformLocation(renderingProgram, "tf"); /* (the shader needs that too) */
	glUniform1f(tfLoc, (float)timeFactor);	
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	

	/*associate VBO with corresponding vertex attribute in the vertex shader*/
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	/*adjust OpenGL settings and draw model*/
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 36,10000);

}

int main(void)
{
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(600, 600, "Tumbling", NULL, NULL);
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

