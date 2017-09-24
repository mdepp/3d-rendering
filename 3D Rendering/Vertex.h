#pragma once

#include <glm/glm.hpp>
#include <iostream>

using namespace std;

class Vertex
{
public:
	Vertex() {}
	Vertex(glm::vec4 pos, glm::vec2 _uv) : position(pos), uv(_uv) {}
	Vertex(float x, float y, float z, float u, float v, float nx, float ny, float nz) : position(x, y, z, 1), uv(u, v), normal(nx, ny, nz, 0) {}

	glm::vec4 position, transformedPosition;
	glm::vec2 uv;
	glm::vec4 normal; // TODO - Chcek if this works as a vec3
	glm::vec3 transformedNormal;

	glm::vec3 ambientColour, diffuseColour;
};