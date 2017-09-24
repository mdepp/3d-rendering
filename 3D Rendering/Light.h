#pragma once
#include <glm/glm.hpp>

class Light
{
public:
	enum Type { TYPE_AMBIENT, TYPE_DIFFUSE }; 
	
	Light(glm::vec4 p, glm::vec3 c, float i, Light::Type tp) : position(p), colour(c), intensity(i), type(tp) {}
	~Light();

	glm::vec4 position; // w=0 - directional, w=1 - positional
	glm::vec3 colour;
	float intensity;

	Type type; // How it should apply the light. Diffuse is multiplied, then ambient is added.
};