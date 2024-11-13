#version 330 core
out vec4 FragColor;
  
in vec4 vertexColor; 
in vec2 texcoord;

uniform sampler2D Texture;

void main()
{
	vec4 texColor = texture(Texture, texcoord);

    if(texColor.a < 0.2)
        discard;

    FragColor = texColor;
} 
