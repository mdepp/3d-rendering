#include "Texture.h"


Texture::Texture() : pixelData(NULL), width(0), height(0)
{

}

bool Texture::loadFromFile(string filename)
{
	width = height = 0;
	if (pixelData != NULL)
	{
		SDL_FreeSurface(pixelData);
		pixelData = NULL;
	}
	pixelData = IMG_Load(filename.c_str());
	if (pixelData == NULL)
	{
		cerr << "Error in Texture::loadFromFile() - Cannot load " << filename << endl;
		return false;
	}
	else
	{
		width = pixelData->w;
		height = pixelData->h;
		return true;
	}
}

Texture::~Texture()
{
	if (pixelData != NULL)
		SDL_FreeSurface(pixelData);
}

static Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
	int bpp = surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to retrieve */
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch (bpp) {
	case 1:
		return *p;
		break;

	case 2:
		return *(Uint16 *)p;
		break;

	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			return p[0] << 16 | p[1] << 8 | p[2];
		else
			return p[0] | p[1] << 8 | p[2] << 16;
		break;

	case 4:
		return *(Uint32 *)p;
		break;

	default:
		return 0;       /* shouldn't happen, but avoids warnings */
	}
}

static glm::vec3 translate_color(Uint32 int_color)
{	//Change from an "int color" to an SDL_Color

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	glm::vec3 colour( (int_color & 0x00ff0000) / 0x10000, (int_color &
		0x0000ff00) / 0x100, (int_color & 0x000000ff));
#else
	glm::vec3 color( (int_color & 0x000000ff), (int_color &
		0x0000ff00) / 0x100, (int_color & 0x00ff0000) / 0x10000);
#endif
	return color;
}

glm::vec3 Texture::getColour(glm::vec2 uv) const
{
	return translate_color(getpixel(pixelData, uv.x*(float)width, uv.y*(float)height));
}