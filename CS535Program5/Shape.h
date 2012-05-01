#ifndef SHAPE_H
#define SHAPE_H

#include "Globals.h"
#include "Ray.h"

struct Shape {

	Shape():
		material(PHONG)
	{}

	// Subclass must implement intersect and normal
	virtual bool intersect(Ray &ray, float &t) = 0;
	virtual glm::vec3 normal(glm::vec3 &intersection) = 0;
	virtual glm::vec3 color(glm::vec3 &intersection) = 0;
	virtual glm::vec3 phong(glm::vec3 &intersection, glm::vec3 &viewer, glm::vec3 &normal, glm::vec3 &light) = 0;

	virtual glm::vec3 intersection(Ray &ray, float t) = 0;

	Material material;
};

#endif