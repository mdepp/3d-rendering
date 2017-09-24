#pragma once

#include "Vertex.h"
#include <glm/glm.hpp>

class Transform
{
public:
	virtual Vertex transformVertex(Vertex& v) = 0;
};

class ViewTransform : public Transform
{
public:
	Vertex transformVertex(Vertex& v);
	void lookAt(glm::vec3 cameraPosition, glm::vec3 cameraTarget);
private:
	glm::mat4 vewMatrix;
};

class ProjectionTransform : public Transform
{
public:
	ProjectionTransform(float _scaleFactor) : scaleFactor(_scaleFactor) {}
	Vertex transformVertex(Vertex& v);

private:
	float scaleFactor;
};