#ifndef MESH_H
#define MESH_H
#include <cglm/cglm.h>
#include "shader.h"
#include <assimp/cimport.h>        // Plain-C interface
#include <assimp/scene.h>          // Output data structure
#include <assimp/postprocess.h>    // Post processing flags
struct meshVertex
{
	vec3 pos;
	vec3 normal;
	vec2 uv;
};
struct meshTexture
{
	unsigned int id;
	char* type;
};
typedef struct mesh
{
	struct meshVertex* vertices;
	struct meshTexture* textures;
	unsigned int* indices;
	int vertSize, indSize, texSize;
	unsigned int vao, vbo, ebo;
} mesh;

void setupMesh(mesh* m);
void drawMesh(mesh* m, char* shader);

void processNode(mesh* m, struct aiNode* node, const struct aiScene* scene);
void processMesh(mesh* m, struct aiMesh* am, const struct aiScene* scene);
void loadMesh(mesh* m, char* path);

#endif