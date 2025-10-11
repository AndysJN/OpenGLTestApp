#include "Texture.h"

Texture::Texture() : Texture("")
{
}

Texture::Texture(const char* FileLoc) : TextureID(0), Width(0), Height(0), BitDepth(0), FileLocation (FileLoc)
{
}

void Texture::LoadTexture()
{
	unsigned char* TextData = stbi_load(FileLocation, &Width, &Height, &BitDepth, 0);
	if (!TextData)
	{
		printf("Failed to find %s\n", FileLocation);
	}

	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_2D, TextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, TextData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(TextData);
}

void Texture::UseTexture()
{
	glActiveTexture(GL_TEXTURE0); //Texture Unit.
	glBindTexture(GL_TEXTURE_2D, TextureID);
}

void Texture::ClearTexture()
{
	glDeleteTextures(1, &TextureID);
	TextureID = 0;
	Width = 0;
	Height = 0;
	BitDepth = 0;
	FileLocation = "";
}

Texture::~Texture()
{
	ClearTexture();
}


