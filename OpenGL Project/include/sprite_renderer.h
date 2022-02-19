#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// todo: implement vector2, vector3 and quaternion class
// implement batch rendering

void renderSpriteViewSpace(float centerX, float centerY, float halfX, float halfY);

#endif