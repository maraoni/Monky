#include "Cube.h"

static float vertices[] =
{//    Positions             Texture cords
	   0.5f,  0.5f,  0.5f,   0.0f, 1.0f,
	  -0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
	  -0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
	   0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
	   0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
	  -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,
	  -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,
	   0.5f, -0.5f, -0.5f,   1.0f, 0.0f
};

static unsigned int indices[] = {
	1, 2, 3, 1, 0, 3,
	3, 7, 0, 7, 4, 0,
	4, 6, 7, 4, 5, 6,
	1, 2, 6, 1, 5, 6,
	1, 5, 4, 1, 0, 4,
	3, 7, 2, 6, 2, 7
};


Cube::Cube() : Mesh(vertices, sizeof(vertices), indices, sizeof(indices)) { };
