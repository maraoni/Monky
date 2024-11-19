#version 330 core
out vec4 FragColor;
  
in vec4 vertexColor; 
in vec2 texcoord;

uniform sampler2D Texture;

void main()
{
    FragColor = vec4(1, 0.2, 0.1, 1);
} 
