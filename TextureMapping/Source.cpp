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
GLuint mvLoc, projLoc;
int width, height;
float aspect;
glm::mat4 pMat, vMat, mMat, mvMat, tMat, rMat;
GLint brickTexture;

void setupVertices(void)  
{
	float pyramidPositions[54] =
	{
		-1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, /*front face*/
		1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, /*right face*/
		1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, /*back face*/
		-1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, /*left face*/
		-1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, /*base ¨C left front*/
		1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f /*base ¨C right back*/
	};

	float pyrTexCoords[36] =
	{
		0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,   0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,   0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,   1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f
	};

	/*generate the VAO as before, and at least two VBOs, then load the two buffers as follows:*/	
	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(numVBOs, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); /*activate the buffer indicated by vbo[0]*/
	/*copy the array containing the vertices into the active buffer(s)*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidPositions), pyramidPositions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyrTexCoords), pyrTexCoords, GL_STATIC_DRAW);
}

void init(GLFWwindow* window)
{
	renderingProgram = createShaderProgram("vertShader.glsl", "fragShader.glsl");
	cameraX = 1.0f, cameraY = 0.0f, cameraZ = 5.0f;
	cubeLocX = 0.0f, cubeLocY = 0.0f, cubeLocZ = 0.0f; /*shiftdown Y to reveal prospective*/
	setupVertices();
	brickTexture = loadTexture("iceTex.jpg");	
}

void display(GLFWwindow* window, double currTime)
{
	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(renderingProgram);

	/*get locations of uniform variables in the shader program (from the MV and projection matrices)*/
	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

	/*build perspective matrix*/
	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f); /*1.0742 rad=60 degrees*/
	



	// build view matrix, model matrix, and model-view matrix 
	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ)); 
	mMat = glm::translate(glm::mat4(1.0f), glm::vec3(cubeLocX, cubeLocY, cubeLocZ)); 
	mvMat = vMat * mMat;




	/*copy perspective and MV matrices' data to uniform variables*/
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

	/*associate VBO with corresponding vertex attribute in the vertex shader*/
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]); 
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(1);

	glActiveTexture(GL_TEXTURE0); 
	glBindTexture(GL_TEXTURE_2D, brickTexture);


	/*adjust OpenGL settings and draw model*/
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 18);

}

int main()
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

