#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glew.h>
#include <string>
#include <iostream>

using namespace std;

class Texture2D
{
public:
	//Texture2D() { };
	//~Texture2D() { };

	static GLuint LoadTexture(string TextureLocation)
	{ 
		GLuint textureID;
		int width, height, numcomponents;
		GLenum format;

		unsigned char* ImageData = stbi_load(TextureLocation.c_str(), &width, &height, &numcomponents, STBI_rgb_alpha);

		if (ImageData == NULL)
		{
			cerr << "Texture loading failed for texture: " << TextureLocation << endl;
		}

		if (numcomponents == 1)
			format = GL_RED;
		if (numcomponents == 3)
			format = GL_RGB;
		if (numcomponents == 4)
			format = GL_RGBA;

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ImageData);

		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(ImageData);

		return textureID;
	};

	//GLuint getTextureID() { return textureID; };
	//int getWidth() { return width; };
	//int getHeight() { return height; };

	//dont think i will need to get numcomponents and format out side of the class, but can add a function later if i find out i need to

private:
    //GLuint textureID;
	//int width, height, numcomponents;
	//GLenum format;
};