#ifndef DRAW_FUNCTIONS_H
#define DRAW_FUNCTIONS_H

#include <sstream>
#include <ctime>
#include <algorithm>
#include <iostream>

using namespace std;

#include "DrawContext.h"

// Initialize single instance once. Allow static access
DrawContext& DrawContext::get_instance() {
	static DrawContext dc;
	return dc;
}

// Initialize window and logical state
DrawContext::DrawContext():
	scene(HEIGHT, WIDTH),
	width(WIDTH),
	height(HEIGHT)
{
	glDisable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, width, height);
	gluOrtho2D(0, width, 0, height);
}

void DrawContext::on_load(string filename) {
	if(scene.load(filename) == FAILURE) {
		exit(0);
	} else {
		scene.status();
		scene.raytrace(1);
	}
}

// Private display callback
void DrawContext::on_display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
	// Draw raytraced scene
	scene.draw();

	glFlush(); //forces previously issued OpenGL commands to begin execution
	glFinish(); //forces all previously issued OpenGl commands to be complete
	glutSwapBuffers();
}

// Private keyboard callback
void DrawContext::on_keyboard(unsigned char key, int x, int y) {
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

// Public callbacks must get singleton and call corresponding
// private callback
void DrawContext::Display(void) {
	get_instance().on_display();
}

void DrawContext::Keyboard(unsigned char key, int x, int y) {
	get_instance().on_keyboard(key, x, y); 
}

void DrawContext::Load(string filename) {
	get_instance().on_load(filename);
}

#endif
