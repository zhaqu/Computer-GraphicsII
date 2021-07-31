#version 410

layout ( location = 0 ) in vec3 pos;
layout ( location = 2 ) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 skyCol;

void main()
{
   	gl_Position = projection * view * model * vec4(pos, 1.0);
	skyCol = vec3(pos);
}
