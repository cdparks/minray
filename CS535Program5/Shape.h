#ifndef SHAPE_H
#define SHAPE_H

#include "Globals.h"
#include "Ray.h"

struct Phong {
	glm::vec3 normal;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shiny;
};

struct Shape {

	Shape():
		material(PHONG)
	{}

	// Subclass must implement intersect and normal
	virtual bool intersect(Ray &ray, float &t) = 0;
	virtual void shading(glm::vec3 &intersection, Phong &p) = 0;

	Material material;
};

#endif