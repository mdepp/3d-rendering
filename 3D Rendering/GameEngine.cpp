#include "GameEngine.h"

GameEngine::GameEngine()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		cerr << "Error in GameEngine::GameEngine() - " << SDL_GetError() << endl;
	}
	else
	{
		window = NULL;
		window = SDL_CreateWindow("3D Rendering", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			cerr << "Error in GameEngine::GameEngine() - " << SDL_GetError() << endl;
		}
		else
		{
			screenSurface = SDL_GetWindowSurface(window);
		}
	}
	renderer = new RenderingEngine(this);
	player.getCamera()->setPosition(glm::vec3(0, 0, 500));
	player.getCamera()->setDirection(glm::vec3(0, 0, -1));

	models.push_back(Model());
	models.back().loadFromFile("TIEF.3DS", "tief3DS/");
	models.back().setScale(glm::vec3(100, 100, 100));
	models.back().setTranslation(glm::vec3(0, 0 , 0));

	lights.push_back(Light(glm::normalize(glm::vec4(1, 0, 1, 0)), glm::vec3(255, 255, 255), .3f, Light::TYPE_DIFFUSE));
	lights.push_back(Light(glm::vec4(0, 0, 1, 0), glm::vec3(255, 255, 255), 0.5, Light::TYPE_DIFFUSE));
	lights.push_back(Light(glm::vec4(0, 0, -1, 0), glm::vec3(255, 255, 255), 0.03f, Light::TYPE_AMBIENT));
}
GameEngine::~GameEngine()
{
	delete renderer;

	SDL_DestroyWindow(window);
	SDL_Quit();
}
static void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
	int bpp = surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to set */
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch (bpp) {
	case 1:
		*p = pixel;
		break;

	case 2:
		*(Uint16 *)p = pixel;
		break;

	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
			p[0] = (pixel >> 16) & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = pixel & 0xff;
		}
		else {
			p[0] = pixel & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = (pixel >> 16) & 0xff;
		}
		break;

	case 4:
		*(Uint32 *)p = pixel;
		break;
	}
}
void GameEngine::mainLoop()
{
	SDL_Event event;
	bool quit = false;

	SDL_ShowCursor(false);
	bool wPressed = false, sPressed = false, aPressed = false, dPressed = false;
	glm::vec2 moveDir;

	while (!quit)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT) quit = true;
			else if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_ESCAPE) quit = true;
				if (event.key.keysym.sym == SDLK_w) wPressed = true;
				if (event.key.keysym.sym == SDLK_a) aPressed = true;
				if (event.key.keysym.sym == SDLK_s) sPressed = true;
				if (event.key.keysym.sym == SDLK_d) dPressed = true;
			}
			else if (event.type == SDL_KEYUP)
			{
				if (event.key.keysym.sym == SDLK_w) wPressed = false;
				if (event.key.keysym.sym == SDLK_a) aPressed = false;
				if (event.key.keysym.sym == SDLK_s) sPressed = false;
				if (event.key.keysym.sym == SDLK_d) dPressed = false;
			}
			else if (event.type == SDL_MOUSEMOTION)
			{
				if (event.motion.x != HALF_SCREEN_WIDTH || event.motion.y != HALF_SCREEN_HEIGHT)
				{
					player.getCamera()->rotateY((HALF_SCREEN_WIDTH  - event.motion.x) / 100.f);
					player.getCamera()->rotateX(-(HALF_SCREEN_HEIGHT - event.motion.y) / 100.f);
					SDL_WarpMouseInWindow(window, HALF_SCREEN_WIDTH, HALF_SCREEN_HEIGHT);
				}
			}
		}
		// Process
		if (wPressed) moveDir.y = 1;
		else if (sPressed) moveDir.y = -1;
		else moveDir.y = 0;
		if (dPressed) moveDir.x = 1;
		else if (aPressed) moveDir.x = -1;
		else moveDir.x = 0;

		{
			static int lastTime = SDL_GetTicks(), deltaTime;
			deltaTime = SDL_GetTicks() - lastTime;
			player.moveByDirection(moveDir, 0.05f*deltaTime);
			lastTime = SDL_GetTicks();
		}
		{
			static int lastTime = SDL_GetTicks(), deltaTime;
			deltaTime = SDL_GetTicks() - lastTime;
			models[0].rotate(0.00005f*deltaTime, glm::vec3(0, 0, 1));
			lastTime = SDL_GetTicks();
		}

		// Rendering code
		renderer->setViewMatrix(player.getCamera()->getViewMatrix());
		vector<vector<DepthPixel>>* buffer = renderer->renderModels(models, lights);
		{
			static SDL_Rect r = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
			SDL_FillRect(screenSurface, &r, 0x000000);
			for (int i = 0; i < SCREEN_WIDTH; i++)
			{
				for (int j = 0; j < SCREEN_HEIGHT; j++)
				{
					if ((*buffer)[i][j].depth < 0) continue;

					putpixel(screenSurface, i, j, SDL_MapRGBA(screenSurface->format, (*buffer)[i][j].r, (*buffer)[i][j].g, (*buffer)[i][j].b, 1));
				}
			}
			SDL_UpdateWindowSurface(window);
		}
	}
}