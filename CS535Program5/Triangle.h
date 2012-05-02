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

	// Extremely helpful:
	// http://www.cs.washington.edu/education/courses/csep557/10au/lectures/triangle_intersection.pdf
	virtual bool intersect(Ray &ray, float &t) {

		// d is dot(normal, any point in plane)
		float d = glm::dot(normal, vertices[0]);

		// Compare direction of ray direction and normal
		float denom = glm::dot(normal, ray.direction);

		// Ray direction and normal are parallel
		if(fabs(denom) < EPS) {
			return false;
		}

		// Solve for t
		t = (d - glm::dot(normal, ray.origin)) / denom;
		if(t <= EPS) {
			return false;
		}

		// Get hit point
		glm::vec3 hit = ray.origin + ray.direction * t;

		// Check hit is inside triangle
		areas[0] = glm::dot(normal, glm::cross(vertices[2] - vertices[1], hit - vertices[1]));		
		if(areas[0] < 0) {
			return false;
		}

		areas[1] = glm::dot(normal, glm::cross(vertices[0] - vertices[2], hit - vertices[2]));
		if(areas[1] < 0) {
			return false;
		}

		areas[2] = glm::dot(normal, glm::cross(vertices[1] - vertices[0], hit - vertices[0]));
		if(areas[2] < 0) {
			return false;
		}
		return true;
	}

	glm::vec3 transform(glm::vec3 &barycentric, glm::vec3 values[3]) {
		return barycentric.x * values[0] + barycentric.y * values[1] + barycentric.z * values[2];
	}

	float transform(glm::vec3 &barycentric, float values[3]) {
		return barycentric.x * values[0] + barycentric.y * values[1] + barycentric.z * values[2];
	}

	float transform(glm::vec3 &barycentric, float x, float y, float z) {
		return barycentric.x * x + barycentric.y * y + barycentric.z * z;
	}

	virtual void shading(glm::vec3 &intersection, Material &m) {
		glm::vec3 barycentric(areas[0] / area, areas[1] / area, areas[2] / area);

		m.normal = glm::normalize(transform(barycentric, normals));
		glm::vec3 tex(1);
		if(texture) {
			float u = transform(barycentric, texcoords[0].s, texcoords[1].s, texcoords[2].s);
			float v = transform(barycentric, texcoords[0].t, texcoords[1].t, texcoords[2].t);
			tex = texture->atUV(u, v);
		}
		m.diffuse = tex * transform(barycentric, diffuse);
		m.specular = transform(barycentric, specular);
		m.shiny = transform(barycentric, shiny);
		m.reflection = transform(barycentric, reflection);
	}

	glm::vec3 vertices[3];
	glm::vec3 normal;
	glm::vec3 normals[3];
	glm::vec2 texcoords[3];
	glm::vec3 diffuse[3];
	glm::vec3 specular[3];
	float shiny[3];
	float reflection[3];
	float area;
	float areas[3];
	Bitmap *texture;
};

#endif