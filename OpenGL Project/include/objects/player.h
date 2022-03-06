#ifndef PLAYER_H
#define PLAYER_H
#include <cglm/cglm.h>
typedef struct Player {
	vec3 position;
	float speed;
	vec3 minBounds, maxBounds;
} Player;

void movePlayer(Player* player, vec2 input, float delta)
{
	vec2 move;
	glm_vec2_normalize_to(input, move);
	player->position[0] += move[0] * player->speed * delta;
	player->position[1] += move[1] * player->speed * delta;
	if (player->position[0] > player->maxBounds[0]) player->position[0] = player->maxBounds[0];
	if (player->position[0] < player->minBounds[0]) player->position[0] = player->minBounds[0];
	if (player->position[1] > player->maxBounds[1]) player->position[1] = player->maxBounds[1];
	if (player->position[1] < player->minBounds[1]) player->position[1] = player->minBounds[1];

}
#endif