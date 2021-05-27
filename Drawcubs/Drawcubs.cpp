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
#include<vector>

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
glm::mat4 pMat, vMat, mMat, mvMat,tMat, rMat;

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
	cameraX = 0.0f, cameraY = 0.0f, cameraZ = 24.0f;
	cubeLocX = 0.0f, cubeLocY = -2.0f, cubeLocZ = 0.0f; /*shiftdown Y to reveal prospective*/
	setupVertices();
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
	for (int i = 0; i < 24; i++)
	{
		tf = currTime + i;

		/*use current time to compute differetn translations in x, y and z*/
		tMat = glm::translate(glm::mat4(1.0f),
			glm::vec3(sin(0.35f * tf) * 8.0f, cos(0.52f * tf) * 8.0f, sin(0.7f * tf) * 8.0f));
		rMat = glm::rotate(glm::mat4(1.0f), 1.75f * (float)tf, glm::vec3(0.0f, 1.0f, 0.0f));
		rMat = glm::rotate(rMat, 1.75f * (float)tf, glm::vec3(1.0f, 0.0f, 0.0f));
		rMat = glm::rotate(rMat, 1.75f * (float)tf, glm::vec3(0.0f, 0.0f, 1.0f));
		mMat = tMat * rMat;

		/*build view matrix, model matrix, and model-view matrix*/
		vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
		/*mMat = glm::translate(glm::mat4(1.0f), glm::vec3(cubeLocX, cubeLocY, cubeLocZ)); This one is obselete now*/
		mvMat = vMat * mMat;



		/*copy perspective and MV matrices' data to uniform variables*/
		glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

		/*associate VBO with corresponding vertex attribute in the vertex shader*/
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		/*adjust OpenGL settings and draw model*/
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

}

int main()
{
	vector<int>ant;
	
	for (int i = 0; i < 10;  i++)
	{
		ant.push_back(i + 1);
	}
	cout << sizeof(ant)<<" "<<ant.size() << endl;

	float bvertexPositions[108] =
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
	cout << sizeof(bvertexPositions) << endl;

	glm::mat4 trans = glm::mat4(0.8, 0.0, 0.6, 0.0,
		0.0, 1.0, 0.0, 0.0,
		-0.6, 0.0, 0.8, 0.0,
		0.0, 0.0, 0.0, 1.0);
	cout << trans[0][0];

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

