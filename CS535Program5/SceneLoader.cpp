#include <sstream>

using namespace std;

#include "Scene.h"
#include "SceneLoader.h"

Result SceneLoader::load(string filename, Scene &scene) {
	// Open file
	fin = ifstream(filename);
	if(fin.fail()) {
		return loadError("Could not load scene from file '" + filename + ".'");
	}

	lineno = 0;
	stringstream ss = nextLine();
	int numObjects;
	ss >> numObjects;
	if(ss.fail()) {
		return loadError("Could not read number of objects in scene from file '" + filename + ".'");
	}

	// Get ambient light
	if(readTag("amb:", ss) == FAILURE) {
		return FAILURE;
	}
	glm::vec3 ambient;
	ss >> ambient.r >> ambient.g >> ambient.b;
	scene.setAmbient(ambient);

	// Read in objects
	for(int i = 0; i < numObjects; ++i) {
		ss = nextLine();
		string type;
		ss >> type;
		Result readObject;
		if(type == "sphere") {
			readObject = readSphere(scene);
		} else if(type == "triangle") {
			readObject = readTriangle(scene);
		} else if(type == "point-light") {
			readObject = readPointLight(scene);
		} else if(type == "area-light") {
			readObject = readAreaLight(scene);
		} else {
			return loadError("Unrecognized object type '" + type + "' found in file '" + filename + ".'");
		}
		if(readObject == FAILURE) {
			return FAILURE;
		}
	}
	return SUCCESS;
}

stringstream SceneLoader::nextLine() {
	stringstream ss;
	string line;
	for(getline(fin, line), lineno += 1; line.size() == 0 || line[0] == '#'; getline(fin, line), lineno += 1)
		; // Skip empty lines and comments;
	ss << line;
	return ss;
}

Result SceneLoader::loadError(string message) {
	fin.close();
	cerr << lineno << ": " << message << endl;
	return FAILURE;
}

Result SceneLoader::readTag(string expected, stringstream &ss) {
	string prefix;
	ss = nextLine();
	ss >> prefix;
	if(prefix != expected) {
		return loadError("Expected attribute tag '" + expected + ".' Found '" + prefix + "' instead.");
	}
	return SUCCESS;
}

Result SceneLoader::readMaterial(Shape *shape) {
	stringstream ss;
	if(readTag("mt:", ss) == FAILURE) {
		return FAILURE;
	}
	string material;
	ss >> material;
	if(material == "Phong") {
		shape->material = PHONG;
	} else if(material == "glass") {
		shape->material = GLASS;
	} else if(material == "mirror") {
		shape->material = MIRROR;
	} else {
		return loadError("Unrecognized material type '" + material + ".'");
	}
	return SUCCESS;
}

Result SceneLoader::readSphere(Scene &scene) {
	Sphere *sphere = new Sphere;
	stringstream ss;

	// Read material
	if(readMaterial(sphere) == FAILURE) {
		goto error;
	}
	
	// Read position
	if(readTag("pos:", ss) == FAILURE) {
		goto error;
	}
	glm::vec3 &position = sphere->position;
	ss >> position.x >> position.y >> position.z;
	
	// Read Phong (specular, diffuse, shininess)
	if(readTag("Phong:", ss) == FAILURE) {
		goto error;
	}
	glm::vec3 &diff = sphere->diffuse;
	glm::vec3 &spec = sphere->specular;
	ss >> diff.r >> diff.g >> diff.b;
	ss >> spec.r >> spec.g >> spec.b;
	ss >> sphere->shiny;

	// Read refraction index
	if(readTag("ref:", ss) == FAILURE) {
		goto error;
	}
	ss >> sphere->refraction;

	// Read radius
	if(readTag("rad:", ss) == FAILURE) {
		goto error;
	}
	ss >> sphere->radius;

	// Add to shape vector
	scene.add(sphere);
	return SUCCESS;
error:
	delete sphere;
	return FAILURE;
}

Result SceneLoader::readTriangle(Scene &scene) {
	Triangle *triangle = new Triangle;
	stringstream ss;
	string texturefile;

	// Read material
	Result r = readMaterial(triangle);
	if(r == FAILURE) {
		goto error;
	}

	// Read texture
	if(readTag("tex:", ss) == FAILURE) {
		goto error;
	}
	ss >> texturefile;
	if(texturefile != "null") {
		triangle->texture = new Bitmap;
		if(triangle->texture->load(texturefile.c_str()) == FAILURE) {
			goto error;
		}
	}
	
	// For each vertex
	for(int i = 0; i < 3; ++i) {
		// Read position
		if(readTag("pos:", ss) == FAILURE) {
			goto error;
		}
		glm::vec3 &vertex = triangle->vertices[i];
		ss >> vertex.x >> vertex.y >> vertex.z;

		// Read normal
		if(readTag("nor:", ss) == FAILURE) {
			goto error;
		}
		glm::vec3 &normal = triangle->normals[i];
		ss >> normal.x >> normal.y >> normal.z;
		normal = glm::normalize(normal);

		// Read Phong (specular, diffuse, shininess)
		if(readTag("Phong:", ss) == FAILURE) {
			goto error;
		}
		glm::vec3 &diff = triangle->diffuse[i];
		glm::vec3 &spec = triangle->specular[i];
		ss >> diff.r >> diff.g >> diff.b;
		ss >> spec.r >> spec.g >> spec.b;
		ss >> triangle->shiny[i];

		// Read refraction index
		if(readTag("ref:", ss) == FAILURE) {
			goto error;
		}
		ss >> triangle->refraction[i];

		// Read uv texture coordinates
		if(readTag("uv:", ss) == FAILURE) {
			goto error;
		}
		glm::vec2 &texcoords = triangle->texcoords[i];
		ss >> texcoords.s >> texcoords.t;
	}

	// Add to shape vector
	scene.add(triangle);
	return SUCCESS;
error:
	delete triangle;
	return FAILURE;
}

Result SceneLoader::readPointLight(Scene &scene) {
	PointLight *light = new PointLight;
	stringstream ss;

	// Read position
	if(readTag("pos:", ss) == FAILURE) {
		goto error;
	}
	glm::vec3 &position = light->position;
	ss >> position.x >> position.y >> position.z;

	// Read color
	if(readTag("col:", ss) == FAILURE) {
		return FAILURE;
	}
	glm::vec3 &color = light->color;
	ss >> color.r >> color.g >> color.b;

	// Add to point light vector
	scene.add(light);
	return SUCCESS;
error:
	delete light;
	return FAILURE;
}

Result SceneLoader::readAreaLight(Scene &scene) {
	AreaLight *light = new AreaLight;
	stringstream ss;

	// For each vertex
	for(int i = 0; i < 4; ++i) {
		// Read position
		if(readTag("pos:", ss) == FAILURE) {
			goto error;
		}
		glm::vec3 &position = light->position[i];
		ss >> position.x >> position.y >> position.z;
	}

	// Read color
	if(readTag("col:", ss) == FAILURE) {
		goto error;
	}
	glm::vec3 &color = light->color;
	ss >> color.x >> color.y >> color.z;

	// Add to area light vector
	scene.add(light);
	return SUCCESS;
error:
	delete light;
	return FAILURE;
}
