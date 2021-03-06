#ifndef SHADER_H
#define SHADER_H
#include <glad/glad.h>
#include <cglm/cglm.h>
#define SHADER_ARRAY_SIZE 32
// todo: shader serialization
// more property setters (vectors, etc)
// geometry shader support
typedef struct Shader
{
	unsigned int ID;
	char* name;
} Shader;

Shader shaders[SHADER_ARRAY_SIZE]; // can't be bothered to make a working map structure, so fuck it
extern int shaderCount;

int createShader(char* name, char* vertexPath, char* fragmentPath);
int useShader(char* shader);
unsigned int getShader(char* name);
int checkShaderErrors(unsigned int object, char* type);
char* getCodeFromPath(char* path);

void setInt(char* shader, char* name, int value);
void setFloat(char* shader, char* name, float value);
void setMat4(char* shader, char* name, mat4 value);
#endif