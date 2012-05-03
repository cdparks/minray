#include <fstream>
#include <iostream>
#include <algorithm>
#include <iomanip>

using namespace std;

#include "Scene.h"
#include "SceneLoader.h"

// Constructor
Scene::Scene():
	width(0),
	height(0),
	maxlevel(0),
	antialiasing(false),
	pixels(NULL)
{}

// Constructor - builds pixel array
Scene::Scene(int width, int height, int maxlevel, bool antialiasing):
	width(width),
	height(height),
	maxlevel(maxlevel),
	antialiasing(antialiasing),
	pixels(NULL)
{	
	pixels = new float[height * width * 3];
}

// Use SceneLoader to fill in scene
Result Scene::load(string filename) {
	destroy();
	return SceneLoader().load(filename, *this);
}

// Print useful things
void Scene::status() {
	int FIELD = 20;
	string edge = "| ";
	cout << left;
	cout << setw(FIELD) << "Scene content:" << endl;
	cout << setw(FIELD) << " Shapes" << edge << shapes.size() << endl;
	cout << setw(FIELD) << " Point Lights" << edge << pointLights.size() << endl;
	cout << setw(FIELD) << " Area Lights" << edge << areaLights.size() << endl;
	cout << setw(FIELD) << "Scene settings:" << endl;
	cout << setw(FIELD) << " Height" << edge << height << endl;
	cout << setw(FIELD) << " Width" << edge << width << endl;
	cout << setw(FIELD) << " Max Recursion" << edge << maxlevel << endl;
	cout << setw(FIELD) << " Antialiasing" << edge << (antialiasing ? "ON" : "OFF") << endl;
}

// Clear pixels and objects
Scene::~Scene() {
	delete pixels;
	pixels = NULL;
	destroy();
}

// Clear objects
void Scene::destroy() {
	for(size_t i = 0; i < shapes.size(); ++i) {
		delete shapes[i];
	}
	shapes.clear();

	for(size_t i = 0; i < pointLights.size(); ++i) {
		delete pointLights[i];
	}
	pointLights.clear();

	for(size_t i = 0; i < areaLights.size(); ++i) {
		delete areaLights[i];
	}
	areaLights.clear();
}

// Add objects to scene (used by SceneLoader)
void Scene::add(Shape *shape) {
	shapes.push_back(shape);
}

void Scene::add(PointLight *light) {
	pointLights.push_back(light);
}

void Scene::add(AreaLight *light) {
	areaLights.push_back(light);
}

void Scene::setAmbient(glm::vec3 &ambient) {
	this->ambient = ambient;
}

// Public interface to ray tracing
void Scene::raytrace() {
	glm::vec3 eye(0.0, 0.0, 1.5);
	int step = startProgress();

	// Set antialiasing coefficients
	float inc = 1.0f, adj = 1.0f;
	if(antialiasing) {
		inc = 0.5f;
		adj = inc * inc;
	}

	// For pixel in screen ...
	for(int i = 0; i < height; ++i) {
		for(int j = 0; j < width; ++j) {
			glm::vec3 color(0);
			for(float isample = 0.0f; isample < 1.0f; isample += inc) {
				float y = (i + isample) * (1.0 / height) * 2 - 1;
				for(float jsample = 0.0f; jsample < 1.0f; jsample += inc) {
					float x = (j + jsample) * (1.0 / width) * 2 - 1;

					// ... generate ray in normalized device coordinates ...
					Ray ray(eye, glm::normalize(glm::vec3(x, y, 0.0) - eye)); 
					
					// .. and trace it.
					color += trace(ray, 0);
				}
			}
			color *= adj;
			pixels[AT_R(i, j)] = color.r;
			pixels[AT_G(i, j)] = color.g;
			pixels[AT_B(i, j)] = color.b;
		}
		updateProgress(i, step);
	}
	endProgress();
}

// Write pixel buffer to screen
void Scene::draw() {
	glRasterPos2i(0, 0);
	glDrawPixels(width, height, GL_RGB, GL_FLOAT, pixels);
}

// Internal recursive ray tracing function
glm::vec3 Scene::trace(Ray &ray, int level) {
	if(level >= maxlevel) {
		return glm::vec3(0);
	}

	// Find closest intersected shape
	Shape *shape = NULL;
	float t, closest = 10000000.0f;
	for(size_t i = 0; i < shapes.size(); ++i) {
		if(shapes[i]->intersect(ray, t)) {
			if(t < closest) {
				closest = t;
				shape = shapes[i];
			}
		}
	}

	if(shape == NULL) {
		// No intersection, return black
		return glm::vec3(0);
	} else {
		// Get shading information at intersection
		Material m;
		glm::vec3 intersection = ray.origin + ray.direction * closest;
		shape->shading(intersection, m);

		// Shade using Phong illumination model, shadow rays, and soft shadows
		glm::vec3 color = ambient * m.diffuse + illuminate(shape, m, -ray.direction, intersection);

		// Reflect if possible
		if(level < maxlevel) {
			if(m.reflection > 0) {
				color += m.reflection * trace(reflect(shape, intersection, ray.direction, m.normal), level + 1);
			}
		}
		return glm::clamp(color, 0.0f, 1.0f);
	}
	return glm::vec3(0);
}

// Apply a single light to a shape at intersection if the
// shape is not occluded.
glm::vec3 Scene::shade(Shape *shape, Material &m, glm::vec3 &viewer, glm::vec3 &intersection, glm::vec3 &light, glm::vec3 &intensity) {
	glm::vec3 color(0);
	glm::vec3 toLight = light - intersection;
	float distance = glm::length(toLight);
	glm::vec3 lightDir = toLight / distance;
	Ray shadow(intersection, lightDir);
	bool occluded = false;
	for(size_t j = 0; j < shapes.size(); ++j) {
		float t;
		if(shapes[j] != shape && shapes[j]->intersect(shadow, t) && t < distance) {
			occluded = true;
			break;
		} 
	}
	if(!occluded) {
		glm::vec3 diffuse = m.diffuse * max(0.0f, glm::dot(m.normal, lightDir));
		glm::vec3 specular = m.specular * pow(max(0.0f, glm::dot(viewer, glm::reflect(-lightDir, m.normal))), m.shiny);
		color = intensity * (diffuse + specular);
	}
	return color;
}

// Illuminate shape with all lights in scene
glm::vec3 Scene::illuminate(Shape *shape, Material &m,  glm::vec3 &viewer, glm::vec3 &intersection) {
	glm::vec3 color(0);
	// Point lights
	for(size_t i = 0; i < pointLights.size(); ++i) {
		color += shade(shape, m, viewer, intersection, pointLights[i]->position, pointLights[i]->color);
	}
	float factor = 1.0f / (AREASAMPLE * AREASAMPLE);
	// Area lights
	for(size_t i = 0; i < areaLights.size(); ++i) {
		for(int j = 0; j < AREASAMPLE; ++j) {
			for(int k = 0; k < AREASAMPLE; ++k) {
				glm::vec3 light = areaLights[i]->sample(j, k);
				color += factor * shade(shape, m, viewer, intersection, light, areaLights[i]->color);
			}
		}
	}
	return color;
}

// Generate new reflection ray.
Ray Scene::reflect(Shape *shape, glm::vec3 &intersection, glm::vec3 &direction, glm::vec3 &normal) {
	Ray newRay(intersection, glm::normalize(glm::reflect(direction, normal)));
	nudge(shape, newRay);
	return newRay;
}

// Fix self-intersection
void Scene::nudge(Shape *shape, Ray &ray) {
	float t;
	if(shape->intersect(ray, t)) {
		ray.origin += ray.direction * (t + 0.001f);
	}
}

