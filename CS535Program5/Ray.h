#ifndef RAY_H
#define RAY_H
// Ray used for intersection tests

#include "glm/glm.hpp"

struct Ray {
	Ray(glm::vec3 &origin, glm::vec3 &direction):
		origin(origin),
		direction(direction)
	{}

	glm::vec3 origin;
	glm::vec3 direction;
};

#endif