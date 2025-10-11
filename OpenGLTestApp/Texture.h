#pragma once

#include <GL\glew.h>

#include "stb_image.h"

class Texture
{
public:

	Texture();
	Texture(const char* FileLoc);

	void LoadTexture();
	void UseTexture();
	void ClearTexture();

	~Texture();

private:

	GLuint TextureID; //ID de la textura en la placa de video.
	int Width, Height, BitDepth;

	const char* FileLocation;


};

