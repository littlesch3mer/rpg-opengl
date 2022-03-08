#include "mesh.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>

void setupMesh(mesh* m)
{
    glGenVertexArrays(1, &m->vao);
    glGenBuffers(1, &m->vbo);
    glGenBuffers(1, &m->ebo);

    glBindVertexArray(m->vao);
    glBindBuffer(GL_ARRAY_BUFFER, m->vbo);
    glBufferData(GL_ARRAY_BUFFER, m->vertSize * sizeof(struct meshVertex), &m->vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->indSize * sizeof(unsigned int), &m->indices[0], GL_STATIC_DRAW);
    for (int i = 0; i < m->indSize; i++)
    {
        printf("%d. %d\n", i, m->indices[i]);
    }

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct meshVertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(struct meshVertex), (void*)offsetof(struct meshVertex, normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(struct meshVertex), (void*)offsetof(struct meshVertex, uv));

    glBindVertexArray(0);
}

void drawMesh(mesh* m, char* shader)
{
    mat4 model;
    glm_mat4_identity(model);

    useShader(shader);
    setMat4(shader, "model", model);

    // draw mesh
    glBindVertexArray(m->vao);
    glDrawElements(GL_TRIANGLES, m->indSize, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}
void processNode(mesh* m, struct aiNode* node, const struct aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        if (m->indSize != 0)
        {
            struct aiMesh* ma = scene->mMeshes[node->mMeshes[i]];
            processMesh(m, ma, scene);
        }
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(m, node->mChildren[i], scene);
    }
}
void processMesh(mesh* m, struct aiMesh* am, const struct aiScene* scene)
{
    m->vertSize = am->mNumVertices;
    m->vertices = malloc(sizeof(struct meshVertex) * m->vertSize);
    for (unsigned int i = 0; i < m->vertSize; i++)
    {
        m->vertices[i].pos[0] = am->mVertices[i].x;
        m->vertices[i].pos[1] = am->mVertices[i].y;
        m->vertices[i].pos[2] = am->mVertices[i].z;
        m->vertices[i].normal[0] = 0;
        m->vertices[i].normal[1] = 0;
        m->vertices[i].normal[2] = 0;
        m->vertices[i].uv[0] = 0;
        m->vertices[i].uv[1] = 011;
    }

    int indSize = 0;
    for (unsigned int i = 0; i < am->mNumFaces; i++)
    {
        struct aiFace face = am->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {         
            indSize++;
        }
    }
    m->indSize = indSize;
    m->indices = malloc(sizeof(unsigned int) * indSize);

    int curr = 0;
    for (unsigned int i= 0; i < am->mNumFaces; i++)
    {
        struct aiFace face = am->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            m->indices[curr] = face.mIndices[j];
            curr++;
        }
    }


}

void loadMesh(mesh* m, char* path)
{
    const struct aiScene* scene = aiImportFile("models/untitled.obj", aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);

    processNode(m,scene->mRootNode, scene);
}
