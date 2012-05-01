#ifndef LIGHT_H
#define LIGHT_H

#include "Shape.h"
#include "glm/glm.hpp"

struct Light {
	virtual glm::vec3 illuminate(Shape &shape, 
};

#endif