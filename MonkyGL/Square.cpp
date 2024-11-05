#include "Square.h"

static float Vertices[] =
{
	-0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
	0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
	0.5f,  -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
};

Square::Square() : Mesh(Vertices, sizeof(Vertices), nullptr, 0) { }
