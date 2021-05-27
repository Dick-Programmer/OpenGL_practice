#pragma once
#include<cmath>
#include<vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h> 
#include <cmath>
#include <iostream> 
#include<fstream>
#include<string>
#include <glm\glm.hpp> 
#include <glm\gtc\type_ptr.hpp> 
#include <glm\gtc\matrix_transform.hpp> 
#include"Sphere.h"
#include"Utils.h"

#define numVAOs 1
#define numVBOs 3
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
Sphere mySphere(50);

void setupVertices()
{
	vector<int> ind = mySphere.getIndices();
	vector<glm::vec3>vert = mySphere.getVertices();
	vector<glm::vec2>tex = mySphere.getTexCoords();
	vector<glm::vec3>norm = mySphere.getNormals();

	vector<float>pvalues; /*vertex positions*/
	vector<float>tvalues; /*texture coordinates*/
	vector<float>nvalues; /*normal vectors*/

	int numIndices = mySphere.getNumIndices();

	for (int i = 0; i < numIndices; i++)
	{
		pvalues.push_back(vert[ind[i]].x);
		pvalues.push_back(vert[ind[i]].y);
	    pvalues.push_back(vert[ind[i]].z);
		

		tvalues.push_back((tex[ind[i]]).s); 
		tvalues.push_back((tex[ind[i]]).t);

		nvalues.push_back((norm[ind[i]]).x); 
		nvalues.push_back((norm[ind[i]]).y); 
		nvalues.push_back((norm[ind[i]]).z);
	}
	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(numVBOs, vbo);

	// put the vertices into buffer #0 
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); 
	glBufferData(GL_ARRAY_BUFFER, pvalues.size()*4, &pvalues[0], GL_STATIC_DRAW);

	// put the texture coordinates into buffer #1 
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]); 
	glBufferData(GL_ARRAY_BUFFER, tvalues.size()*4, &tvalues[0], GL_STATIC_DRAW);

	// put the normals into buffer #2 
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]); 
	glBufferData(GL_ARRAY_BUFFER, nvalues.size()*4, &nvalues[0], GL_STATIC_DRAW);
}

void init(GLFWwindow* window)
{
	renderingProgram = createShaderProgram("vertShader.glsl", "fragShader.glsl");
	cameraX = 0.0f, cameraY = 0.0f, cameraZ = 3.0f;
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


	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, brickTexture);


	/*adjust OpenGL settings and draw model*/
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, mySphere.getNumIndices());

}


int main()
{

	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(600, 600, "Sphere", NULL, NULL);
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
