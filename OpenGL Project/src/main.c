#define _CRT_SECURE_NO_WARNINGS
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "texture.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "shader.h"
#include "sprite_renderer.h"
#include <cglm/cglm.h>
#include "renderer.h"
#include "mesh.h"

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
	srand(time(NULL));
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
	createTexture("shot", "textures/shot.png", 1);

	// shaders
	createShader("sprite", "Shaders/sprite.vert", "Shaders/sprite.frag");
	createShader("basic", "Shaders/basic.vert", "Shaders/basic.frag");
	createShader("line", "Shaders/line.vert", "Shaders/line.frag");

	useShader("sprite");
    setInt("sprite", "texture1", 0);
	setupShaderMatrices();

	// render loop
	delta = 0;
	float last = glfwGetTime();
	glEnable(GL_DEPTH_TEST);

	// model?
	mesh m;
	loadMesh(&m, "fc");
	setupMesh(&m);
	float timeleft = 1;
	int c = 0;
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
		
		// clear color
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//drawMesh(&m, "basic");
		useShader("basic");
		renderCube("basic");
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//renderCube("basic");

		/*
		if (timeleft < 0)
		{
			printf("COUNT: %d\n", c);
			printf("<%f,%f,%f>\n", m.vertices[c].pos[0], m.vertices[c].pos[1], m.vertices[c].pos[2]);
			printf("<%f,%f,%f>\n", m.vertices[c+1].pos[0], m.vertices[c+1].pos[1], m.vertices[c+1].pos[2]);
			printf("<%f,%f,%f>\n\n\n", m.vertices[c+2].pos[0], m.vertices[c+2].pos[1], m.vertices[c+2].pos[2]);

			timeleft = 0.2f;

			c += 3;
			if (c >= m.indSize - 3)
				c = 0;
		}
		renderSprite(m.vertices[c].pos, (vec3) { 0.1f, 0.1, 0.1 }, "sprite", "enemy");
		renderSprite(m.vertices[c + 1].pos, (vec3) { 0.1f, 0.1, 0.1 }, "sprite", "enemy");
		renderSprite(m.vertices[c + 2].pos, (vec3) { 0.1f, 0.1, 0.1 }, "sprite", "enemy");
		for (int i = 0; i < m.vertSize; i++)
		{
			renderSprite(m.vertices[i].pos, (vec3) { 0.05f, 0.05f, 0.05f }, "sprite", "player");
		}
		timeleft -= delta;
		*/
		renderSprite((vec3) { 0, 0, 0 }, (vec3) { 1, 1, 1 }, "sprite", "player");

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
	mat4 proj, model;
	glm_mat4_identity(model);
	setMat4("sprite", "model", model);
	setMat4("basic", "model", model);
	setMat4("line", "model", model);

	glm_mat4_identity(proj);
	float ratio = SCR_WIDTH / (float)SCR_HEIGHT;
	float sizeH = cameraSize / 2;
	float sizeW = sizeH * ratio;
	//glm_ortho(-sizeW,sizeW,-sizeH,sizeH, -100, 100, proj);
	glm_perspective(glm_rad(60), SCR_WIDTH / (float)SCR_HEIGHT, 0.1, 1000, proj);
	setMat4("sprite", "projection", proj);
	setMat4("basic", "projection", proj);
	setMat4("line", "projection", proj);

}
void updateCamera()
{
	mat4 view;
	glm_mat4_identity(view);
	vec3 cPos = { 0,0,2 };
	//cPos[2] = sin(glfwGetTime()) * 2 + 2;
	//cPos[1] = sin(glfwGetTime()) * 6;

	float ang = sin(glfwGetTime() / 5) * 3.14f;
	vec3 cFront = { 0,0,1 };
	glm_vec3_add(cFront, cPos, cFront);
	vec3 cUp = { 0,1,0 };
	vec3 cRight = { 1,0,0 };
	glm_lookat(cFront, cPos, cUp, view);
	glm_rotate(view, ang, cUp);
	setMat4("sprite", "view", view);
	setMat4("basic", "view", view);
	setMat4("line", "view", view);
	//glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (float*)view);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


