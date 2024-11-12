#include "Square.h"

static float Vertices[] =
{
	-0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f, 0.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
};

static unsigned int Indices[] =
{
	0, 1, 3,  
	2, 1, 3
};

Square::Square() : Mesh(Vertices, sizeof(Vertices), Indices, sizeof(Indices)) { }
