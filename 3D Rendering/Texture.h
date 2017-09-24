#pragma once
#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <iostream>

using namespace std;

class Texture
{
public:
	Texture();
	~Texture();

	bool loadFromFile(string filename);
	glm::vec3 getColour(glm::vec2 uv) const;

private:
	int width, height;
	SDL_Surface* pixelData;
};