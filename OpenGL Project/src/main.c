#define _CRT_SECURE_NO_WARNINGS
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include "shader.h"
#include "sprite_renderer.h"
#include <cglm/cglm.h>
#include "renderer.h"
#include "texture.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

void setupShaderMatrices();
void updateCamera();

// settings
const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;
float delta;
vec3 cameraPos = { 0,0,0 };
float cameraSize = 8;

int main(int argc, char** argv)
{
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Engine Prototype v0", NULL, NULL);
	if (window == NULL)
	{
		printf("Failed to create GLFW window\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);

	// stbi setup
	stbi_set_flip_vertically_on_load(1);
	
	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to initialize GLAD\n");
		return -1;
	}

	// texture
	createTexture("default", "textures/default.png", 1);
	createTexture("player", "textures/player.png", 1);
	createTexture("enemy", "textures/enemy.png", 1);
	createTexture("tile", "textures/tile.png", 1);

	// shaders
	createShader("sprite", "Shaders/sprite.vert", "Shaders/sprite.frag");
	//createShader("basic", "Shaders/basic.vert", "Shaders/basic.frag");
	useShader("sprite");
    setInt("sprite", "texture1", 0);
	setupShaderMatrices();



	// render loop
	delta = 0;
	float last = glfwGetTime();
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window))
	{
		updateCamera();
		delta = glfwGetTime() - last;
		last = glfwGetTime();
		// input
		processInput(window);
		float speed = 5;
		float rotSpeed = 60 * delta;
		//if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) c.position[1] += speed * delta;
		//if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) c.position[2] -= speed * delta;
		//if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) c.position[0] += speed * delta;
		//if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) c.position[0] -= speed * delta;
		//if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) c.position[2] += speed * delta;
		//if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) c.position[2] -= speed * delta;

        glActiveTexture(GL_TEXTURE0);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//renderCube("test");
		renderSprite((vec3) { 0, 0, 0 }, (vec3) { 1, 1, 1 },"sprite","player");
		renderSprite((vec3) { 2, 0, 0 }, (vec3) { 1, 1, 1 }, "sprite", "enemy");
		

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
	return 0;
}
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, 1);

}
// setup projection, and basic model
void setupShaderMatrices()
{
	// iterate through matrices later
	unsigned int shader = getShader("sprite");
	unsigned int modelLoc = glGetUniformLocation(shader, "model");
	unsigned int projLoc = glGetUniformLocation(shader, "projection");

	mat4 proj, model;
	glm_mat4_identity(model);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float*)model);

	glm_mat4_identity(proj);
	float ratio = SCR_WIDTH / (float)SCR_HEIGHT;
	float sizeH = cameraSize / 2;
	float sizeW = sizeH * ratio;
	glm_ortho(-sizeW,sizeW,-sizeH,sizeH, -100, 100, proj);
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, (float*)proj);
}
void updateCamera()
{
	unsigned int shader = getShader("sprite");
	unsigned int viewLoc = glGetUniformLocation(shader, "view");
	mat4 view;
	glm_mat4_identity(view);
	vec3 cPos = { 0,0,-1 };
	vec3 cFront = { 0,0,1 };
	vec3 cUp = { 0,1,0 };
	vec3 cRight = { 1,0,0 };
	glm_lookat(cFront, cPos, cUp, view);
	setMat4("sprite", "view", view);
	//glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (float*)view);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

