#ifndef AREA_LIGHT_H
#define AREA_LIGHT_H

#include "glm/glm.hpp"

struct AreaLight {
	glm::vec3 position[4];
	glm::vec3 color;
};

#endif