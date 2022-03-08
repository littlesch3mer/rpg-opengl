#ifndef ENEMY_H
#define ENEMY_H
#include <cglm/cglm.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
typedef struct Enemy
{
	vec3 position;
	vec3 destination;
	float speed;
	float shotCooldown;
	bool active;
	vec3 minBounds, maxBounds;
} Enemy;
float* getRandomPos(Enemy* enemy, vec3 minPos, vec3 maxPos)
{
	float x = (float)rand() / RAND_MAX;
	float y = (float)rand() / RAND_MAX;
	vec3 res = { 0,0,0 };
	res[0] = (minPos[0] * (1.0 - x)) + (x * maxPos[0]);
	res[1] = (minPos[1] * (1.0 - y)) + (y * maxPos[1]);
	printf("Random: [(%f,%f),(%f,%f)]\n", minPos[0], minPos[1], maxPos[0], maxPos[1]);
	printf("rand(%f,%f): %f,%f\n\n",x,y,enemy->position[0], enemy->position[1]);
	return res;
}
void updateEnemy(Enemy* enemy, float delta)
{
	if (enemy->active == 0) return;
	vec3 velocity;
	glm_vec3_sub(enemy->destination, enemy->position, velocity);
	glm_vec3_normalize(velocity);
	enemy->position[0] += velocity[0] * enemy->speed * delta;
	enemy->position[1] += velocity[1] * enemy->speed * delta;
	//if (enemy->position[0] > enemy->maxBounds[0]) enemy->position[0] = enemy->maxBounds[0];
	//if (enemy->position[0] < enemy->minBounds[0]) enemy->position[0] = enemy->minBounds[0];
	//if (enemy->position[1] > enemy->maxBounds[1]) enemy->position[1] = enemy->maxBounds[1];
	//if (enemy->position[1] < enemy->minBounds[1]) enemy->position[1] = enemy->minBounds[1];

	float dist=	glm_vec3_distance(enemy->position, enemy->destination);
	if (dist < 0.1f)
	{
		float* d = getRandomPos(enemy, enemy->minBounds, enemy->maxBounds);
		glm_vec3_copy(d, enemy->destination);
	}
}
void initEnemy(Enemy* enemy)
{
	float* pos = getRandomPos(enemy, enemy->minBounds, enemy->maxBounds);
	glm_vec3_copy(pos, enemy->position);
	enemy->active = 1;
	enemy->shotCooldown = 2;
	float* d = getRandomPos(enemy, enemy->minBounds, enemy->maxBounds);
	glm_vec3_copy(d, enemy->destination);
}

#endif