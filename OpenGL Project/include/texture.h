#ifndef TEXTURE_H
#define TEXTURE_H
#include "stb_image.h"
#define TEXTURE_ARRAY_SIZE 32
typedef struct Texture
{
	unsigned int ID;
	char* name;	
} Texture;

Texture textures[TEXTURE_ARRAY_SIZE];
extern int textureCount;
int createTexture(char* name, char* path, int alpha);
unsigned int getTexture(char* name);
int bindTexture(char* name);
#endif