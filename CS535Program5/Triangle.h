#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Shape.h"
#include "Bitmap.h"
#include "glm/ext.hpp"

struct Triangle : Shape {
	Triangle():
		Shape(),
		texture(NULL)
	{}

	virtual bool intersect(Ray &ray, glm::vec3 &intersection, glm::vec3 &normal) {
		return glm::intersectRayTriangle(ray.origin, ray.direction, vertices[0], vertices[1], vertices[2], intersection);
	}

	glm::vec3 vertices[3];
	glm::vec3 normals[3];
	glm::vec2 texcoords[3];
	glm::vec3 diffuse[3];
	glm::vec3 specular[3];
	float shiny[3];
	float refraction[3];
	Bitmap *texture;
};

#endif