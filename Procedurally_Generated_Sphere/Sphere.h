#pragma once
#include<cmath>
#include<vector>
#include<glm/glm.hpp>
#include<math.h>
#define _USE_MATH_DEFINES
using namespace std;

class Sphere
{
public:
	Sphere();
	Sphere(int prec);
	int getNumVertices();
	int getNumIndices();
	vector<int>getIndices();
	vector<glm::vec3>getVertices();
	vector<glm::vec2>getTexCoords();
	vector<glm::vec3>getNormals();

	

private:
	int numVertices;
	int numIndices;
	vector<int> indices;
	vector<glm::vec3>vertices;
	vector<glm::vec2>texCoords;
	vector<glm::vec3>normals;
	void init(int);
	float toRadians(float degrees) { return degrees * ((float)atan(1))/ 45; }
};



