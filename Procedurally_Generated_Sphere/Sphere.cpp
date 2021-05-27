#pragma once
#include<cmath>
#include<vector>
#include<iostream>
#include<glm/glm.hpp>
#include"Sphere.h"
using namespace std;

void Sphere::init(int prec)
{
	numVertices = (prec + 1) * (prec + 1);
	numIndices = 6 * prec*prec;
	for (int i = 0; i < numVertices; i++) { vertices.push_back(glm::vec3(0.0)); } /*push function*/
	for (int i = 0; i < numVertices; i++) { texCoords.push_back(glm::vec2(0.0)); }/*inserts new element*/
	for (int i = 0; i < numVertices; i++) { normals.push_back(glm::vec3(0.0)); }/*at the end of a vector*/
	for (int i = 0; i <numIndices; i++) { indices.push_back(0); }

	/*calculate triangle vertices*/
	for (int i = 0; i <=prec; i++)
	{
		for (int j = 0; j <= prec; j++)
		{
			float y = (float)cos(toRadians(180.0f - i * 180.0f / prec));
			float x = -(float)cos(toRadians(j * 360.0f / prec)) * (float)abs(cos(asin(y)));
			float z = (float)sin(toRadians(j * 360.0f / prec)) * (float)abs(cos(asin(y)));
			vertices[i * (prec + 1) + j] = glm::vec3(x, y, z);
			texCoords[i * (prec + 1) + j] = glm::vec2(((float)j / prec), ((float)i / prec));			
			normals[i * (prec + 1) + j] = glm::vec3(x, y, z);
		}
	}

	/*calculate triangle vertices*/
	for (int i = 0; i < prec; i++)
	{
		for (int j = 0; j < prec; j++)
		{
			indices[6 * (i * prec + j) + 0] = i * (prec + 1) + j; 
			indices[6 * (i * prec + j) + 1] = i * (prec + 1) + j + 1; 
			indices[6 * (i * prec + j) + 2] = (i + 1) * (prec + 1) + j; 
			indices[6 * (i * prec + j) + 3] = i * (prec + 1) + j + 1; 
			indices[6 * (i * prec + j) + 4] = (i + 1) * (prec + 1) + j + 1; 
			indices[6 * (i * prec + j) + 5] = (i + 1) * (prec + 1) + j;			
		}
	}
}

/*accessors*/ 
int Sphere::getNumVertices() { return numVertices; } 
int Sphere::getNumIndices() { return numIndices; } 
std::vector<int> Sphere::getIndices() { return indices; } 
std::vector<glm::vec3> Sphere::getVertices() { return vertices; } 
::vector<glm::vec2> Sphere::getTexCoords() { return texCoords; } 
std::vector<glm::vec3> Sphere::getNormals() { return normals; }


Sphere::Sphere() { init(48); }
Sphere::Sphere(int prec) { init(prec); }

