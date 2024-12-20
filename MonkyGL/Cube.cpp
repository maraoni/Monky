#include "Cube.h"
#include <vector>

Cube::Cube()
{
    // Define vertices and indices as std::vector
    std::vector<float> vertexData = {
        // Front face
        -0.5f, -0.5f,  0.5f,   0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,   1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,   0.0f, 0.0f,

        // Back face            
        -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,   1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,   0.0f, 0.0f,

         // Bottom face        
         -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,
          0.5f, -0.5f, -0.5f,   1.0f, 1.0f,
          0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
         -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,

         // Top face
         -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,
         -0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
          0.5f,  0.5f,  0.5f,   1.0f, 0.0f,
          0.5f,  0.5f, -0.5f,   0.0f, 0.0f,

          // Left face
          -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
          -0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
          -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
          -0.5f,  0.5f, -0.5f,  0.0f, 0.0f,

          // Right face
           0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
           0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
           0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
           0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    };

    std::vector<unsigned int> indexData = {
        // Front face
        0, 2, 1, 2, 0, 3,
        // Back face
        4, 6, 5, 6, 4, 7,
        // Bottom face
        8, 10, 9, 10, 8, 11,
        // Top face
        12, 14, 13, 14, 12, 15,
        // Left face
        16, 18, 17, 18, 16, 19,
        // Right face
        20, 22, 21, 22, 20, 23
    };

    // Pass std::vector data to the Mesh constructor
    Mesh(vertexData, indexData);
}