#version 410

in vec3 edgeCol;
out vec4 outCol;

void main()
{
	outCol = vec4(edgeCol, 1.0);
}

