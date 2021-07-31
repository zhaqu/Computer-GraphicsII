#version 410

in vec3 skyCol;
uniform samplerCube envMap;
out vec4 outCol;

void main()
{
	outCol = texture(envMap, skyCol);
}

