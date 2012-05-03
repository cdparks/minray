#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H
// Simulate a point light illuminating in all directions

#include "glm/glm.hpp"

struct PointLight {
	glm::vec3 position;
	glm::vec3 color;
};

#endif