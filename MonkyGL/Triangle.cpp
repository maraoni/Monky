#include "Triangle.h"

static float Vertices[] =
{
	-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
	0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
};

Triangle::Triangle() : Mesh(Vertices, sizeof(Vertices), nullptr, 0) { }


