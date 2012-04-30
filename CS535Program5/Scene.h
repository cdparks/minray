#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <vector>
#include <fstream>

using namespace std;

#include "Globals.h"
#include "Ray.h"
#include "Sphere.h"
#include "Triangle.h"
#include "AreaLight.h"
#include "PointLight.h"
#include "glm/glm.hpp"

#define AT(row, col) ((row) * width + (col))

class Scene {
public:
	Scene(int height, int width);
	~Scene();
	Result load(string filename);
	void status();
	void add(Sphere *sphere);
	void add(Triangle *triangle);
	void add(PointLight *light);
	void add(AreaLight *light);
	void setAmbient(glm::vec3 &light);
	void raytrace();
	void draw();
private:
	void destroy();
	glm::vec3 trace(Ray &ray, int level);
	glm::vec3 ambient;
	vector<Sphere*> spheres;
	vector<Triangle*> triangles;
	vector<PointLight*> pointLights;
	vector<AreaLight*> areaLights;
	int height;
	int width;
	float *pixels;
};

#endif