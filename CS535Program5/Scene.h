#ifndef SCENE_H
#define SCENE_H
// Build scene, raytrace to pixels, and draw to screen

#include <string>
#include <vector>
#include <fstream>
#include <iomanip>

using namespace std;

#include "Globals.h"
#include "Ray.h"
#include "Sphere.h"
#include "Triangle.h"
#include "AreaLight.h"
#include "PointLight.h"
#include "glm/glm.hpp"

class Scene {
public:
	// Constructors
	Scene();
	Scene(int width, int height, int maxlevel, bool antialiasing);

	// Destructors
	~Scene();

	// Load scene from file
	Result load(string filename);

	// Print some useful data
	void status();

	// Add objects to scene
	void add(Shape *shape);
	void add(PointLight *light);
	void add(AreaLight *light);

	// Set ambient light
	void setAmbient(glm::vec3 &light);

	// Raytrace scene to pixel array
	void raytrace();

	// Draw pixel array in OpenGL context
	void draw();

private:
	// Destroy objects
	void destroy();

	// Internal recursive ray tracing function. level == number of recursions.
	glm::vec3 trace(Ray &ray, int level);

	// Apply all point and area lights to shape at intersection (shadow rays and soft shadows here)
	glm::vec3 illuminate(Shape *shape, Material &m, glm::vec3 &viewer, glm::vec3 &intersection);

	// Apply a single light to shape at intersection
	glm::vec3 shade(Shape *shape, Material &m, glm::vec3 &viewer, glm::vec3 &intersection, glm::vec3 &light, glm::vec3 &intensity);

	// Fix self-intersection
	void nudge(Shape *shape, Ray &ray);

	// Generate reflected ray
	Ray Scene::reflect(Shape *shape, glm::vec3 &intersection, glm::vec3 &direction, glm::vec3 &normal);

	// Start progress bar
	inline int Scene::startProgress() {
		start = clock();
		cout << "Ray tracing progress" << endl;
		for(int i = 0; i < 20; ++i) {
			cout << "=";
		}
		cout << endl;
		return height / 20;
	}

	// Update progress bar
	inline void Scene::updateProgress(int i, int step) {
		if(i && i % step == 0) {
			cout << "=";
		}
	}

	// Finish progress bar and print time
	inline void Scene::endProgress() {
		cout << "=" << endl;
		clock_t seconds = (clock() - start) / CLOCKS_PER_SEC;
		cout << "Elapsed time [mm:ss] - " << right << setw(2) << setfill('0') << seconds / 60;
		cout << ":" << setw(2) << setfill('0') << seconds % 60 << endl;
	}

	glm::vec3 ambient;					// ambient light
	vector<Shape*> shapes;				// object vectors
	vector<PointLight*> pointLights;
	vector<AreaLight*> areaLights;
	int height;
	int width;
	int maxlevel;						// maximum recursion level
	bool antialiasing;					// is antialising on?
	clock_t start;						// for timing
	float *pixels;						// pixel array
};

#endif