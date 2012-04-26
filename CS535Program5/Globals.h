#ifndef GLOBALS_H
#define GLOBALS_H

#include "LoadGL.h"

// Global data
const GLint WIDTH = 700;
const GLint HEIGHT = 700;
const GLfloat PI = 3.14159265;
const GLfloat PI2 = 2 * PI;
const GLfloat PI_OVER_180 = PI / 180.0;
const GLfloat PI_OVER_2 = PI / 2.0;

// Menu header
static const char *TOGGLE[] = {
	"+--------------------+---+",
	"| Toggle Help        | H |",
	"+--------------------+---+"
};
const GLint TOGGLE_SIZE = sizeof(TOGGLE) / sizeof(char*);

// Dropdown menu
static const char *MENU[] = {
	"",
	"+------------+---------------------+",
	"| Rotate     |   Left Click + Drag |",
	"+------------+---------------------+",
	"",
	"+--------------------+---+",
	"| Draw Mode          | D |",
	"| Next Texture       | N |",
	"| Reset View         | R |",
	"| Quit               | Q |",
	"+--------------------+---+"
};
const GLint MENU_SIZE = sizeof(MENU) / sizeof(char*);

// Useful colors
const GLfloat BLUE[] = {0.0f, 0.4f, 1.0f};
const GLfloat GREEN[] = {0.0f, 1.0f, 0.4};
const GLfloat RED[] = {1.0f, 0.0f, 0.0f};
const GLfloat WHITE[] = {1.0f, 1.0f, 1.0f};
const GLfloat ORANGE[] = {8.0f, 0.5f, 0.0f}; 

// Represents a point in 2D
struct Point2D{
	GLint x;
	GLint y;
	
	// Constructors
	Point2D(): x(0), y(0) {}
	Point2D(GLint xc, GLint yc): x(xc), y(yc) {}
};

// Menu position
const Point2D MENU_POS(12, 24);

// I can never decide whether 0 or 1 should be failure
enum Result { SUCCESS, FAILURE };

// Sphere and triangle materials
enum Material { PHONG, GLASS, MIRROR };

// Keep value in range
template<typename T>
T clamp(T x, T lo, T hi){
	if(x < lo){
		x = lo;
	}else if(x > hi){
		x = hi;
	}
	return x;
}

#endif
