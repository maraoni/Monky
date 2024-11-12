#include "Square.h"

static float Vertices[] =
{
	-0.5f, 0.5f, 0.0f,  1.0f, 0.0f,
	0.5f,  0.5f, 0.0f,  0.0f, 0.0f,
	0.5f, -0.5f, 0.0f,  0.0f, 1.0f,
	-0.5f,  -0.5f, 0.0f, 0.0f, 0.0f,
};

static float Indices[] =
{
	0, 1, 3,  
	2, 3, 1
};

Square::Square() : Mesh(Vertices, sizeof(Vertices), nullptr, 0) { }
