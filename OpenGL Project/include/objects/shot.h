#ifndef SHOT_H
#define SHOT_H
#include <cglm/cglm.h>

typedef struct Shot
{
	vec3 position;
	vec2 velocity;
	float timeleft;
	int active;
} Shot;

void updateShot(Shot* shot, float delta)
{
	if (shot->active == 0) return;
	shot->timeleft -= delta;
	shot->position[0] += shot->velocity[0] * delta;
	shot->position[1] += shot->velocity[1] * delta;
	if (shot->timeleft <= 0)
		shot->active = 0;
}
#endif