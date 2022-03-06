#define _CRT_SECURE_NO_WARNINGS
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "shader.h"
#include "sprite_renderer.h"
#include <cglm/cglm.h>
#include "renderer.h"
#include "texture.h"
#include "objects/player.h"
#include "objects/shot.h"
#include "objects/enemy.h"

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
	//createShader("basic", "Shaders/basic.vert", "Shaders/basic.frag");
	useShader("sprite");
    setInt("sprite", "texture1", 0);
	setupShaderMatrices();


	vec3 minBounds = { -5,-3.7f, 0 };
	vec3 maxBounds = {  5, 3.7f, 0 };
	Player player = { .position = {0,0,0},.speed = 4 };
	glm_vec3_copy(minBounds, player.minBounds);
	glm_vec3_copy(maxBounds, player.maxBounds);
	Shot shots[16];
	Enemy enemies[4];
	float nextEnemy = 2;
	for (int i = 0; i < 16; i++)
	{
		shots[i].position[0] = shots[i].position[1] = shots[i].position[2] = 0;
		shots[i].velocity[0] = shots[i].velocity[1] = 0;
		shots[i].timeleft = 0;
		shots[i].active = 0;
	}
	for (int i = 0; i < 4; i++)
	{
		enemies[i].active = 0;
		enemies[i].position[0] = enemies->position[2] = 0;
		enemies[i].position[1] = 1;
		enemies[i].destination[0] = enemies[i].destination[1] = enemies[i].destination[2] = 0;
		enemies[i].shotCooldown = 0;
		enemies[i].speed = 2;
		glm_vec3_copy(minBounds, enemies[i].minBounds);
		glm_vec3_copy(maxBounds, enemies[i].maxBounds);
		enemies[i].minBounds[1] = 0;
	}
	initEnemy(&enemies[0]);
	float shotCooldown = 0;
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
		
		// clear color
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// gameloop
		vec2 moveInput = { 0,0 };
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) moveInput[1] = 1;
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) moveInput[1] = -1;
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) moveInput[0] = -1;
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) moveInput[0] = 1;

		//shoot
		if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		{
			for (int i = 0; i < 16; i++)
			{
				if (shots[i].active == 0 && shotCooldown <= 0)
				{
					shots[i].position[0] = player.position[0];
					shots[i].position[1] = player.position[1];
					shots[i].active = 1;
					shots[i].velocity[1] = 7;
					shots[i].timeleft = 2;
					printf("Shooting %d\n", i);
					shotCooldown = 0.15f;
				}
			}
		}
		shotCooldown -= delta;
		nextEnemy -= delta;

		// move and render shots
		for (int i = 0; i < 16; i++)
		{
			updateShot(&shots[i], delta);
			if (shots[i].active == 1)
			{
				//renderSprite(shots[i].position, (vec3) { 0.5f, 0.5f, 1 }, "sprite", "shot");
				renderSprite(shots[i].position, (vec3) { 0.25f,0.25f, 1 }, "sprite", "shot");
			}
		}
		for (int i = 0; i < 4; i++)
		{
			updateEnemy(&enemies[i], delta);
			if (enemies[i].active == 1)
			{
				//printf("%f,%f\n", enemies[i].position[0], enemies[i].position[1]);
				renderSprite(enemies[i].position, (vec3) { 1, 1, 1 }, "sprite", "enemy");

				// hack for projectiles!
				for (int i = 0; i < 16; i++)
				{
					if (shots[i].active == 0) continue;
					float dist = glm_vec3_distance(shots[i].position, enemies[i].position);
					if (dist < 0.5f)
					{
						shots[i].active = 0;
						enemies[i].active = 0;
					}
				}
			}
			else if (nextEnemy <= 0)
			{
				nextEnemy = 2;
				initEnemy(&enemies[i]);
			}
		}

		// move player
		movePlayer(&player, moveInput, delta);
		renderSprite(player.position, (vec3) { 1, 1, 1 }, "sprite", "player");

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

