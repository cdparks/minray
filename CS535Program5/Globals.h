#ifndef GLOBALS_H
#define GLOBALS_H

#include <Windows.h>
#include <gl\GL.h>
#include <glut.h>

// Global data
const GLint WIDTH = 800;
const GLint HEIGHT = 800;
const float EPS = std::numeric_limits<float>::epsilon();
const int AREASAMPLE = 5; // Sample area lights 5 across and 5 down (for 25 samples)

// I can never decide whether 0 or 1 should be failure
enum Result { SUCCESS, FAILURE };

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

// Access red, green, and blue components of pixel array
#define AT_R(row, col) ((row) * width * 3 + ((col) * 3))
#define AT_G(row, col) (AT_R(row, col) + 1)
#define AT_B(row, col) (AT_R(row, col) + 2)

// Initialize vec3
#define RGBV3(pixels, row, col) (pixels)[AT_R(row, col)], (pixels)[AT_G(row, col)], (pixels)[AT_B(row, col)]

#endif
