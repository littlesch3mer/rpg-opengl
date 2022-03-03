#include "texture.h"
#include <glad/glad.h>
int textureCount = 0;

int createTexture(char* name, char* path, int alpha)
{
	if (textureCount == TEXTURE_ARRAY_SIZE) return 1;
	int width, height, nrChannels;
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);

	unsigned int format = alpha == 1 ? GL_RGBA : GL_RGB;	

	unsigned int tex = 0;
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_image_free(data);

	return 0;
}

unsigned int getTexture(char* name)
{
	for (int i = 0; i < TEXTURE_ARRAY_SIZE; i++)
	{
		if (name == textures[i].name)
			return textures[i].ID;
	}
	return 0;
}

int bindTexture(char* name)
{
	glBindTexture(GL_TEXTURE_2D, getTexture(name));
	return 0;
}
