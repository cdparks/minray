#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

#include "Scene.h"
#include "SceneLoader.h"

Scene::Scene(int height, int width):
	height(height),
	width(width),
	pixels(NULL)
{	
	pixels = new float[height * width * 3];
}

Result Scene::load(string filename) {
	destroy();
	return SceneLoader().load(filename, *this);
}

void Scene::status() {
	cout << "Spheres:      " << spheres.size() << endl;
	cout << "Triangles:    " << triangles.size() << endl;
	cout << "Point Lights: " << pointLights.size() << endl;
	cout << "Area Lights:  " << areaLights.size() << endl;
}

Scene::~Scene() {
	delete pixels;
	pixels = NULL;
	destroy();
}

void Scene::destroy() {
	for(size_t i = 0; i < spheres.size(); ++i) {
		delete spheres[i];
	}
	spheres.clear();

	for(size_t i = 0; i < triangles.size(); ++i) {
		delete triangles[i];
	}
	triangles.clear();

	for(size_t i = 0; i < pointLights.size(); ++i) {
		delete pointLights[i];
	}
	pointLights.clear();

	for(size_t i = 0; i < areaLights.size(); ++i) {
		delete areaLights[i];
	}
	areaLights.clear();
}

void Scene::add(Sphere *sphere) {
	spheres.push_back(sphere);
}

void Scene::add(Triangle *triangle) {
	triangles.push_back(triangle);
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

// Access red, green, and blue components of pixel array
#define R(row, col) ((row) * width + ((col) * 3))
#define G(row, col) (R(row, col) + 1)
#define B(row, col) (R(row, col) + 2)

void Scene::raytrace() {
	glm::vec3 eye(0.0, 0.0, 1.0);
	for(int i = 0; i < height; ++i) {
		float y = i * (1.0 / height) * 2 - 1;
		for(int j = 0; j < width; ++j) {
			float x = j * (1.0 / width) * 2 - 1;
			Ray ray(eye, glm::normalize(glm::vec3(x, y, 0.0) - eye)); 
			glm::vec3 color = trace(ray, 0);
			pixels[R(i, j)] = color.r;
			pixels[G(i, j)] = color.g;
			pixels[B(i, j)] = color.b;
		}
	}
}

void Scene::draw() {

}

glm::vec3 Scene::trace(Ray &ray, int level) {
	return glm::vec3(0);
}