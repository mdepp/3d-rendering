#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <iostream>

using namespace std;

class Camera
{
public:
	Camera() : viewMatrixUpdated(false) {}

	void setPosition(glm::vec3 pos)
	{
		position = pos;
		viewMatrixUpdated = false;
	}
	void setDirection(glm::vec3 dir)
	{
		rotation = dir;
		viewMatrixUpdated = false;
	}
	void rotateY(float angle) // Normally this would be called before rotateX()
	{
		static glm::vec3 axis = glm::vec3(0, 1, 0);
		rotation = glm::rotate(rotation, angle, axis);
		viewMatrixUpdated = false;
	}
	void rotateX(float angle)
	{
		static glm::vec3 axis = glm::vec3(1, 0, 0);
		rotation = glm::rotate(rotation, angle, axis);
		viewMatrixUpdated = false;
	}
	void translate(glm::vec3 displacement)
	{
		position += displacement;
		viewMatrixUpdated = false;
	}
	glm::mat4 getViewMatrix()
	{
		static glm::vec3 up = glm::vec3(0, 1, 0);
		if (!viewMatrixUpdated)
		{
			viewMatrix = glm::lookAt(position, position + rotation, up);
			viewMatrixUpdated = true;
		}
		return viewMatrix;
	}
	glm::vec3 getRotation() { return rotation; }
private:
	glm::vec3 position, rotation;
	glm::mat4 viewMatrix;
	bool viewMatrixUpdated;
};

class Player
{
public:
	Player();
	~Player();

	void moveByDirection(glm::vec2 moveDir, float amount)
	{
		if (moveDir.x == 0 && moveDir.y == 0) return;
		static glm::vec3 up(0, 1, 0);
		glm::vec3 displacement = glm::vec3(0, 0, 0);
		displacement += moveDir.y * cam.getRotation();
		displacement += glm::cross(moveDir.x * cam.getRotation(), up);
		displacement *= amount;
		cam.translate(displacement);
	}
	Camera* getCamera() { return &cam; }

private:
	Camera cam;
};
