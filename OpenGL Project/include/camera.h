#ifndef CAMERA_H
#define CAMERA_H
#include <cglm/cglm.h>

// simple camera struct built for ease of control with yaw and pitch values
typedef struct Camera {
	vec3 position;
	vec4 rotation;
} Camera;

void cameraViewMatrix(Camera cam, vec3* dest);


#endif