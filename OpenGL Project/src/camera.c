#include "camera.h"
#include <math.h>

void cameraViewMatrix(Camera cam, vec3* dest)
{
	vec3 up = { 0,1,0 }, fwd = { 0,0,1 }, right = { 1,0,0 };
    glm_vec3_add(fwd, cam.position, fwd);
	glm_lookat(fwd, cam.position, up, dest);
	glm_quat_rotate(dest, cam.rotation, dest);
	glm_quat_rotate_at(dest, cam.rotation, cam.position);
}
