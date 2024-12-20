#include "Square.h"

Square::Square() 
{
	std::vector<unsigned int> Indices =
	{
		0, 1, 3,
		2, 1, 3
	};
	std::vector<float> Vertices =
	{
		-0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
	};

	Mesh(Vertices, Indices);
}
