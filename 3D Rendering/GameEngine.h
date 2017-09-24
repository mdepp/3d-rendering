#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include "RenderingEngine.h"
#include <vector>
#include <fstream>
#include <sstream>
#include "Model.h"
#include "Light.h"
#include "Player.h"

using namespace std;

class RenderingEngine;

// Uses SDL2
class GameEngine
{
public:
	GameEngine();
	~GameEngine();

	void mainLoop();

	const int SCREEN_WIDTH = 1024;
	const int SCREEN_HEIGHT = 768;
	const int HALF_SCREEN_WIDTH = SCREEN_WIDTH / 2;
	const int HALF_SCREEN_HEIGHT = SCREEN_HEIGHT / 2;

private:

	void loadFromFile(string filename);

	SDL_Window* window;
	SDL_Surface* screenSurface;

	RenderingEngine* renderer;
	Player player;

	vector<Model> models;
	vector<Light> lights;
};