#include "shader.h"
#include <stdio.h>
#include <stdlib.h>"
int shaderCount = 10;

int createShader(char* name, char* vertexPath, char* fragmentPath)
{	
	if (shaderCount == SHADER_ARRAY_SIZE) return 1;
	char* vertexSource = getCodeFromPath(vertexPath);
	char* fragmentSource = getCodeFromPath(fragmentPath);

	// vertex
	unsigned int sVertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(sVertex, 1, &vertexSource, NULL);
	glCompileShader(sVertex);
	if (!checkShaderErrors(sVertex, "VERTEX"))
	{
		printf("Vertex Shader Compile Error\n");
	}

	// fragment
	unsigned int sFragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(sFragment, 1, &fragmentSource, NULL);
	glCompileShader(sFragment);
	printf("%s\n", fragmentSource);
	if (!checkShaderErrors(sFragment, "FRAGMENT"))
	{
		printf("Fragment Shader Compile Error\n");
	}

	// shader program
	unsigned int ID = glCreateProgram();
	glAttachShader(ID, sVertex);
	glAttachShader(ID, sFragment);
	glLinkProgram(ID);
	if (!checkShaderErrors(ID, "PROGRAM"))
	{
		printf("Shader Program Compile Error\n");
	}
	Shader s = { .ID = ID, .name = name };
	shaders[shaderCount] = s;
	shaderCount++;

	// delete shaders
	glDeleteShader(sVertex);
	glDeleteShader(sFragment);

	return 0;
}
unsigned int getShader(char* shader)
{
	for (int i = 0; i < SHADER_ARRAY_SIZE; i++)
	{
		if (shader == shaders[i].name)
			return shaders[i].ID;
	}
	return 0;
}

void setInt(char* shader, char* name, int value)
{
	glUniform1i(glGetUniformLocation(getShader(shader), name), value);
}

void setFloat(char* shader, char* name, float value)
{
	glUniform1f(glGetUniformLocation(getShader(shader), name), value);
}

int useShader(char* shader)
{
	// testing only ofc
	glUseProgram(getShader(shader));
	return 0;
}

int checkShaderErrors(unsigned int object, char* type)
{
	int success;
	char infoLog[1024];

	if (type != "PROGRAM")
	{
		glGetShaderiv(object, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(object, 1024, NULL, infoLog);
			printf("%s",infoLog);
		}
	}
	else
	{
		glGetProgramiv(object, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(object, 1024, NULL, infoLog);
			printf("%s",infoLog);
		}
	}
	return success;
}

char* getCodeFromPath(char* path)
{
	char* fcontent = NULL;
	int fsize = 0;
	FILE* fp;

	fopen_s(&fp,path, "r");
	if (fp) {
		fseek(fp, 0, SEEK_END);
		fsize = ftell(fp);
		rewind(fp);

		fcontent = (char*)malloc(sizeof(char) * fsize);
		int n = fread(fcontent, 1, fsize, fp);
		fcontent[n] = '\0';

		fclose(fp);
	}
	return fcontent;
	/*
	char* buffer = NULL;
	size_t size = 0;
	FILE* fp;
	errno_t err = fopen_s(&fp, path, "r");
	if (err != 0)
	{
		printf("Shit!\n");
		return NULL; //idk lmo
	}
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);

	rewind(fp);

	buffer = malloc((size + 1) * sizeof(*buffer));
	int n = fread(buffer, size, 1, fp);
	buffer[n] = '\0';

	return buffer;*/
}
