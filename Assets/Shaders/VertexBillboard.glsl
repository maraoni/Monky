#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCord;
  
out vec4 vertexColor; 
out vec2 texcoord;

uniform mat4 transform;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    mat4 modelTransform = transform;

    // First colunm.
    modelTransform[0][0] = 1.0; 
    modelTransform[0][1] = 0.0; 
    modelTransform[0][2] = 0.0; 

  
    // Second colunm.
    modelTransform[1][0] = 0.0; 
    modelTransform[1][1] = 1.0; 
    modelTransform[1][2] = 0.0; 
  

  // Thrid colunm.
    modelTransform[2][0] = 0.0; 
    modelTransform[2][1] = 0.0; 
    modelTransform[2][2] = 1.0; 


    gl_Position = projection * view * modelTransform * vec4(aPos, 1.0); 
    vertexColor = vec4(1.0, 1.0, 1.0, 1.0);
    texcoord = aTexCord;
}









