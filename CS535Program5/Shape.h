#ifndef SHAPE_H
#define SHAPE_H

#include "Globals.h"
#include "Ray.h"

struct Shape {

	Shape():
		material(PHONG)
	{}

	// Subclass must implement intersect - returns true on intersection and 
	// sets distSquared .
	virtual bool intersect(Ray &ray, glm::vec3 &intersection, glm::vec3 &normal) = 0;

	Material material;
};

#endif