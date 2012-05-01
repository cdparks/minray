#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"
#include "glm/ext.hpp"

struct Sphere : Shape {
	Sphere():
		Shape(),
		radius(0),
		position(0.0),
		diffuse(0.0),
		specular(0.0),
		shiny(0.0),
		refraction(0.0)
	{}

	virtual bool intersect(Ray &ray, float &t) {
		float b = (glm::dot(ray.direction, ray.origin) - glm::dot(ray.direction, position)) * 2;
		float c = glm::dot(ray.origin, ray.origin) + glm::dot(position, position) - glm::dot(ray.origin, position) * 2 - radius * radius;
		float d = b * b - 4 * c;
		if(d < 0) {
			return false;
		}
		float sqrtD = sqrt(d);
		float t0 = (-b - sqrtD) / 2;
		if(t0 <= 0) {
			float t1 = (-b + sqrtD) / 2;
			if(t1 <= 0) {
				return false;
			} else {
				t = t1;
			}
		} else {
			t = t0;
		}
		return true;
	}

	virtual void shading(glm::vec3 &intersection, Phong &p) {
		p.normal = glm::normalize((intersection - position) / radius);
		p.diffuse = diffuse;
		p.specular = specular;
		p.shiny = shiny;
	}

	float radius;
	glm::vec3 position;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shiny;
	float refraction;
};

#endif