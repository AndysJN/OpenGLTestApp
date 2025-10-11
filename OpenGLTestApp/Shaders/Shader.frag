#version 330

in vec4 VertexColour;
in vec2 TexCoord;

out vec4 colour;

uniform sampler2D TheTexture;

void main()
{
	colour = texture(TheTexture, TexCoord);
}