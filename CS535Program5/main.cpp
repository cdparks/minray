// Ray Tracer for CS535
// Author: Christopher D. Parks
// Email: chris.parks@uky.edu
// Date: 2 May 2012

#include <iostream>

using namespace std;

#include "Globals.h"
#include "Scene.h"

Scene *scene = NULL;

void usage(char *name) {
	cerr << "Usage: " << name << " scenefile [--debug]" << endl;
	exit(0);
}

// Display callback
void on_display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
	// Draw raytraced scene
	scene->draw();

	glFlush();
	glFinish();
	glutSwapBuffers();
}

// Keyboard callback
void on_keyboard(unsigned char key, int x, int y) {
	switch(key) {
	// Exit
	case 'q':
	case 'Q':
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

int main(int argc, char **argv){
	if(argc < 2) {
		usage(argv[0]);
	}

	char *filename = NULL;
	int height = HEIGHT;
	int width = WIDTH;
	int level = 10;
	bool antialiasing = true;

	for(int i = 1; i < argc; ++i) {
		if(strcmp(argv[i], "--debug") == 0) { // if --debug, make running faster
			height = 400;
			width = 400;
			level = 5;
			antialiasing = false;
		} else {
			filename = argv[i];
		}
	}

	if(!filename) {
		cerr << "Must provide scenefile as argument." << endl;
		usage(argv[0]);
	}

	// Initialization functions
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);  
	glutInitWindowSize(width, height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("CS535 Program 5");

	// Call-back functions
	glutDisplayFunc(on_display);
	glutKeyboardFunc(on_keyboard);

	// Set up scene
	scene = new Scene(width, height, level, antialiasing);

	// Set up viewport
	glDisable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, width, height);
	gluOrtho2D(0, width, 0, height);

	// Load, raytrace, and draw
	if(scene->load(filename) == FAILURE) {
		exit(0);
	}
	scene->status();
	scene->raytrace();
	glutMainLoop();

	return 0;
}
