#version 410

layout ( location = 0 ) in vec3 pos;
layout ( location = 2 ) in vec3 normal;

uniform mat4 pvm;
uniform mat4 vm;
uniform mat4 normal_matrix;

out vec3 v_normal, interp_eyePos;

void main()
{
	vec4 v_eye = vm * vec4(pos, 1.0);
	v_normal = (normal_matrix * vec4(normal, 0.0)).xyz;
	interp_eyePos = vec3(v_eye) / v_eye.w;
   	gl_Position = pvm * vec4(pos, 1.0);
	//fragCol = color;
}
