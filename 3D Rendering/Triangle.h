#pragma once

#include "Vertex.h"

class Triangle
{
public:
	Triangle() {};
	Triangle(Vertex* v1, Vertex* v2, Vertex* v3);
	Triangle(Vertex* v[3]);

	/*void calcNormal()
	{
		glm::vec3 n = glm::cross(glm::vec3(vertices[0].position - vertices[1].position), glm::vec3(vertices[0].position - vertices[2].position));
		
		normal = glm::normalize(glm::vec4(n.x, n.y, n.z, 0));
	}*/

	Vertex* vertices[3];
};