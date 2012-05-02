#ifndef SHAPE_H
#define SHAPE_H

#include "Globals.h"
#include "Ray.h"

struct Material {
	glm::vec3 normal;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shiny;
	float reflection;
};

struct Shape {

	Shape() {}

	// Subclass must implement intersect and normal
	virtual bool intersect(Ray &ray, float &t) = 0;
	virtual void shading(glm::vec3 &intersection, Material &m) = 0;
};

#endif