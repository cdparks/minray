// Cube Map Utah Teapot

// Author: Christopher D. Parks
// Email: chris.parks@uky.edu
// Class: CS535
// Date: 4 April 2012

#include <iostream>

using namespace std;

#include "Globals.h"
#include "DrawContext.h"
#include "LoadGL.h"

int main(int argc, char **argv){
	// Initialization functions
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);  
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("CS535 Program 5");

    // Call-back functions
    glutDisplayFunc(DrawContext::Display);
    glutKeyboardFunc(DrawContext::Keyboard);

    // Load extensions
    InitShaderAPI();

	// Set up scene
	DrawContext::Load("scenes/simple.txt");

    // Infinite Loop
    glutMainLoop();           
    return 0;
}
// End of the program
