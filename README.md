# CS535 Program 5 - A simple ray tracer
---------------------------------------
![Spheres](/CS535Program5/images/final/blueSpheres.png)

## Notes
Usage: CS535Program5 scenefile [--debug]

This program ray traces the provided scene and draws it using OpenGL.
During tracing, the following interface is shown:

	Scene content:
	 Shapes             | 4
	 Point Lights       | 1
	 Area Lights        | 1
	Scene settings:
	 Height             | 800
	 Width              | 800
	 Max Recursion      | 10
	 Antialiasing       | ON
	Ray tracing progress
	====================

If the --debug flag is used, the following settings are used instead:

	Scene content:
	 Shapes             | 4
	 Point Lights       | 1
	 Area Lights        | 1
	Scene settings:
	 Height             | 400
	 Width              | 400
	 Max Recursion      | 5
	 Antialiasing       | OFF
	Ray tracing progress
	====================

The scenefiles read by this program are slightly different than the one
shown in the assignment. The following is a short, valid scenefile:

	# Last Run
	#
	# Scene content:
	#  Shapes             | 4
	#  Point Lights       | 1
	#  Area Lights        | 1
	# Scene settings:
	#  Height             | 800
	#  Width              | 800
	#  Max Recursion      | 10
	#  Antialiasing       | ON
	# Ray tracing progress
	# ====================
	# ====================
	# Elapsed time [mm:ss] - 03:52
	#

	# Ambient light must be first instruction
	amb: .3 .3 .3

	# Spheres have position, radius, and material
	sphere
	pos: -1 0.5 -0.5
	rad: 0.75
	mat: .3 .3 .3 .5 .5 .5 20 0.9

	# material is Diffuse, Specular, Phong exponent, and Reflection index

	# Triangles have a texture (filename or null) and three vertices.
	# Each vertex has position, normal, material (as above), and uv
	# coordinates
	triangle
	tex: scenes/checker.bmp
	pos: -10 -2 10
	nor: 0 1 0
	mat: 0.60648 0.22648 0 0.628281 0.555802 0.366065 50 0.6
	uv: 0 0
	pos: -10 -2 -10
	nor: 0 1 0
	mat: 0.60648 0.22648 0 0.628281 0.555802 0.366065 50 0.6
	uv: 0 1
	pos: 10 -2 -10
	nor: 0 1 0
	mat: 0.60648 0.22648 0 0.628281 0.555802 0.366065 50 0.6
	uv: 1 1

	# Point lights and area lights are the same as described in the
	# assignment
	point-light
	pos: 1 1 3 
	col: 0.8 0.8 0.8

	area-light
	pos: 0.125 2.0 0
	pos: 0.125 2.0 -0.125
	pos: -0.125 2.0 -0.125
	pos: -0.125 2.0 0
	col: 1.0 1.0 1.0
	
Some example scene files are in the scenes/ directory. 

## Completed requirements
1. Triangle intersection
2. Sphere intersection
3. Triangle texture mapping
4. Sphere & triangle Phong shading
5. Antialiasing (4X supersampling)
6. Recursive reflection (10 recursion limit)
7. Test scene (images/final/redSpheres.png or blueSpheres.png)

## Completed extra credit
1. Shadow rays - I showed this at the presentation.
2. Soft shadow for area light source - This is new. The updated final
	images in images/final/ have area lights, so this should be
	easy to verify.

## Files included in archive:

	// This file
	README.txt

	// Solution file
	CS535Program5.sln

	// Source directory
	CS535Program5/
		
		// main routine
		main.cpp

		// Raytracing and drawing routines
		Scene.cpp
		Scene.h
		
		// Read scene file and load objects
		SceneLoader.cpp
		SceneLoader.h

		// Scene objects
		Ray.h
		Shape.h
		Sphere.h
		Triangle.h
		
		// Lights
		AreaLight.h
		PointLight.h
		
		// Loads Windows format 24bit BGR bitmaps
		Bitmap.cpp
		Bitmap.h
		
		// Useful constants, enums, and functions
		Globals.h
		
		// Scene description files and textures
		scenes/
			blueSpheres.txt
			checker.bmp
			planet.bmp
			redSpheres.txt
			test.txt

		// Screenshots
		images/

			// Test scene and two scenes I made
			final/
				blueSpheres.png
				redSpheres.png
				test.png

			// Images showing progression from just having
			// Phong shading to reflections to shadow rays
			// to soft shadows.
			progress/
				progress1.png
				progress2.png
				progress3.png
				progress4.png
				progress5.png
				progress6.png
				progress7.png
				progress8.png
				progress9.png

		
		// Visual Studio files
		CS535Program5.vcxproj
		CS535Program5.vcxproj.filters
		CS535Program5.vcxproj.user
		
		// Vector math library
		glm/
			(There's a lot of files here)

## Acknowledgements:
* [GLM OpenGL Math Library](http://glm.g-truc.net/)
* Information about [Triangle intersection and Barycentric Coordinates](http://www.cs.washington.edu/education/courses/csep557/10au/lectures/triangle_intersection.pdf)

## Contact:
Christopher D. Parks, chris.parks@uky.edu

