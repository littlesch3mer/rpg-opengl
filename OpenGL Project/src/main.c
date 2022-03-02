#define _CRT_SECURE_NO_WARNINGS
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include "shader.h"
#include "sprite_renderer.h"
#include "stb_image.h"
#include <cglm/cglm.h>
#include "renderer.h"
#include "camera.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float delta;
Camera c;
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

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to initialize GLAD\n");
		return -1;
	}
    // load and create a texture 
// -------------------------
    unsigned int texture;
    // texture 1
    // ---------
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(1); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char* data = stbi_load("textures/default.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        printf("Failed to load texture1\n");
    }
    stbi_image_free(data);
    
	// shaders
	createShader("test", "Shaders/shader.vert", "Shaders/shader.frag");
	useShader("test");
    setInt("test", "texture1", 0);

    unsigned int shader = getShader("test");
    unsigned int modelLoc = glGetUniformLocation(shader, "model");
    unsigned int viewLoc = glGetUniformLocation(shader, "view");
    unsigned int projLoc = glGetUniformLocation(shader, "projection");

	Camera c = { .position = {0,2,0} };
	glm_quat_identity(c.rotation);
	mat4 proj, view, model;
	glm_mat4_identity(model);

	glm_scale(model, (vec3) { 25, 0.1f, 25 });
	glm_translate(model, (vec3) {0, 0,0 });

	glm_mat4_identity(view);
	cameraViewMatrix(c, view);

	glm_mat4_identity(proj);
	glm_perspective(glm_rad(60), SCR_WIDTH / (float)SCR_HEIGHT, 0.1, 1000, proj);


    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float*)model);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (float*)view);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, (float*)proj);
    
	// render loop
	delta = 0;
	float last = glfwGetTime();
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window))
	{
		delta = glfwGetTime() - last;
		last = glfwGetTime();
		// input
		processInput(window);
		float speed = 5;
		float rotSpeed = 60 * delta;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) c.position[2] -= speed * delta;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) c.position[2] += speed * delta;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) c.position[0] += speed * delta;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) c.position[0] -= speed * delta;
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) c.position[1] += speed * delta;
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) c.position[1] -= speed * delta;
		versor rot;
		glm_quat(rot, glm_rad(rotSpeed), -1, 0, 0);
		//if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) glm_quat_mul(c.rotation,rot,c.rotation);
		glm_quat(rot, glm_rad(rotSpeed), 1, 0, 0);
		//if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) glm_quat_mul(c.rotation, rot, c.rotation);
		glm_quat(rot, glm_rad(rotSpeed), 0, 1, 0);
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) glm_quat_mul(c.rotation, rot, c.rotation);
		glm_quat(rot, glm_rad(rotSpeed), 0, -1, 0);
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) glm_quat_mul(c.rotation, rot, c.rotation);

		// vbo thing
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// model matrix		
		//glm_rotate(model, glm_rad(0.01), (vec3) { 0,1, 0 });
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float*)model);
		
		cameraViewMatrix(c, view);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (float*)view);

		//renderSpriteViewSpace(0, 0, 1.0f, 1.0f);
		renderCube("test");
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
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

