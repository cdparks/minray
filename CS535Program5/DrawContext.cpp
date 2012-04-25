#ifndef DRAW_FUNCTIONS_H
#define DRAW_FUNCTIONS_H

#include <sstream>
#include <ctime>
#include <algorithm>
#include <iostream>

using namespace std;

#include "DrawContext.h"

// Initialize single instance once. Allow static access
DrawContext& DrawContext::get_instance(){
	static DrawContext dc;
	return dc;
}

// Initial reset-able state values
const float DrawContext::RESET_YAW = 0;
const float DrawContext::RESET_PITCH = 0;

// For drawing menus
#define BEGIN2D\
	glMatrixMode(GL_PROJECTION);\
	glPushMatrix();\
	glLoadIdentity();\
	gluOrtho2D(0.0, WIDTH, 0.0, HEIGHT);\
	glMatrixMode(GL_MODELVIEW);\
	glPushMatrix();\
	glLoadIdentity()
#define END2D\
	glMatrixMode(GL_PROJECTION);\
	glPopMatrix();\
	glMatrixMode(GL_MODELVIEW);\
	glPopMatrix();\

// Initialize window and logical state
DrawContext::DrawContext():
	program(),
	draw_menu(false),
	left_down(false),
	right_down(false),
	mouse(0, 0),
	pitch(RESET_PITCH),
	yaw(RESET_YAW),
	width(WIDTH),
	height(HEIGHT)
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, 1, 0.1f, 100.0f);

	// Set up the view matrix
	glMatrixMode(GL_MODELVIEW);
	gluLookAt(0.0, 0.0, 1.0,   // eye location
	          0.0, 0.0, 0.0,   // center is at (0,0,0)
	          0.0, 1.0, 0.0);  // up is in positive Y direction

	// Compile and link shader
	if(program.load("vertex_shader.vert", "fragment_shader.frag") == FAILURE) {
		exit(-1);
	}
}

// Write contents of a stream to screen at (x, y)
void DrawContext::write(ostringstream &os, GLfloat x, GLfloat y, float *color){	
	string output = os.str();
	glColor3fv(color);
	glRasterPos2f(x, y);
	for(GLint i = 0; i < (GLint)output.length(); ++i){
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, output[i]);
	}
}

// Draw the menu, state, and mouse position
void DrawContext::draw_interface(void){	
	BEGIN2D;
	Point2D cursor(MENU_POS.x, height - MENU_POS.y);
	// Help toggle
	for(size_t i = 0; i < TOGGLE_SIZE; ++i, cursor.y -= 13){
		ostringstream toggle_stream;
		toggle_stream << TOGGLE[i];
		write(toggle_stream, cursor.x, cursor.y, (float*)GREEN);
	}
	// Help dropdown menu
	if(draw_menu){
		for(size_t i = 0; i < MENU_SIZE; ++i, cursor.y -= 13){
			ostringstream stream;
			stream << MENU[i];
			write(stream, cursor.x, cursor.y, (float*)GREEN);
		}
	}
	cursor.y = MENU_POS.y;
	ostringstream stream;
	stream << "(" << mouse.x << ", " << mouse.y << ") ";
	write(stream, cursor.x, cursor.y, (float*)GREEN);
	END2D;
}

// Private display callback
void DrawContext::on_display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glTranslatef( 0.0f, 0.0f, -8.0f );
	glRotatef(-pitch, 1.0f, 0.0f, 0.0f);
	glRotatef(-yaw, 0.0f, 1.0f, 0.0f);

	glEnable(GL_LIGHTING);

	// Use shader
	glUseProgram(program.ID());
	
	glFrontFace(GL_CCW);
	glutSolidSphere(2.0, 100, 100);

	// Unbind shader
	glUseProgram(NULL);
	glDisable(GL_LIGHTING);
	
	// Draw menu
	draw_interface();

	glFlush(); //forces previously issued OpenGL commands to begin execution
	glFinish(); //forces all previously issued OpenGl commands to be complete
	glutSwapBuffers();
}

// Private keyboard callback
void DrawContext::on_keyboard(unsigned char key, int x, int y){
	switch(key){
	// Toggle help menu
	case 'h':
	case 'H':
		draw_menu = !draw_menu;
		break;

	// Reset
	case 'r':
	case 'R':
		pitch = DrawContext::RESET_PITCH;
		yaw = DrawContext::RESET_YAW;
		break;

	// Exit
	case 'q':
	case 'Q':
	case 27:
		exit(0);
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

// Private mouse-button-press callback
void DrawContext::on_mouse(int button, int state, int x, int y){
	if(button == GLUT_LEFT_BUTTON){
		left_down = state == GLUT_DOWN;
		mouse.x = x;
		mouse.y = y;
	}
	if(button == GLUT_RIGHT_BUTTON){
		right_down = state == GLUT_DOWN;
	}
	glutPostRedisplay();
}

// Private motion callback
void DrawContext::on_motion(int x, int y){
	if(left_down){		
		yaw -= (GLfloat)(x - mouse.x);
		pitch -= (GLfloat)(y - mouse.y);
	}
	mouse.x = x;
	mouse.y = y;
	glutPostRedisplay();
}

// Public callbacks must get singleton and call corresponding
// private callback
void DrawContext::Display(void){
	get_instance().on_display();
}

void DrawContext::Keyboard(unsigned char key, int x, int y){
	get_instance().on_keyboard(key, x, y); 
}

void DrawContext::Mouse(int button, int state, int x, int y){
	get_instance().on_mouse(button, state, x, y);
}

void DrawContext::Motion(int x, int y){
	get_instance().on_motion(x, y);
}

#endif
