//	Screen dimension
#define SCREEN_WIDTH	800
#define SCREEN_HEIGHT	600

//	Camera movement settings
#define ROTATE_SPEED	5.0
#define ZOOM_SPEED		0.1

//	Data source
#include "Data.hpp"

//	Standard includes
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>

//	GLFW/OpenGL
#include <GLFW/glfw3.h>
#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE 0x809D
#endif
#ifndef GL_SAMPLE_ALPHA_TO_COVERAGE
#define GL_SAMPLE_ALPHA_TO_COVERAGE 0x809E
#endif

//	GLFW/Windows
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

//	Functions
void windowResizeCallback(GLFWwindow* window, int width, int height);
void keyboardCharacterCallback(GLFWwindow* window, unsigned int code);
void mouseScrollCallback(GLFWwindow* window, double offsetX, double offsetY);
void rainbowColor(float hue, float alpha);

//	Solid cube vertices (GL_QUADS)
static GLfloat solid_cube[] =
{
	-1, -1, -1,   -1, -1,  1,   -1,  1,  1,   -1,  1, -1,
	 1, -1, -1,    1, -1,  1,    1,  1,  1,    1,  1, -1,
	-1, -1, -1,   -1, -1,  1,    1, -1,  1,    1, -1, -1,
	-1,  1, -1,   -1,  1,  1,    1,  1,  1,    1,  1, -1,
	-1, -1, -1,   -1,  1, -1,    1,  1, -1,    1, -1, -1,
	-1, -1,  1,   -1,  1,  1,    1,  1,  1,    1, -1,  1
};

//	Wireframe cube vertices (GL_LINES)
static GLfloat wireframe_cube[] =
{
	-1, -1, -1,    1, -1, -1,    1, -1, -1,    1,  1, -1,
	 1,  1, -1,   -1,  1, -1,   -1,  1, -1,   -1, -1, -1,
	-1, -1,  1,    1, -1,  1,    1, -1,  1,    1,  1,  1,
	 1,  1,  1,   -1,  1,  1,   -1,  1,  1,   -1, -1,  1,
	-1, -1, -1,   -1, -1,  1,    1, -1, -1,    1, -1,  1,
	 1,  1, -1,    1,  1,  1,   -1,  1, -1,   -1,  1,  1
};
