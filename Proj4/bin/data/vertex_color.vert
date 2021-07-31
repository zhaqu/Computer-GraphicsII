#version 410

layout ( location = 0 ) in vec3 pos;
layout ( location = 1 ) in vec3 color;
layout ( location = 2 ) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 cam_pos;

out vec3 v_normal, interp_eyePos, fragCol;

void main()
{
	mat4 vm = view * model;
	mat4 pvm = projection * view * model;
	mat4 normal_matrix = transpose(inverse(view*model));
	vec4 v_eye = vm * vec4(cam_pos, 1.0);
	v_normal = (normal_matrix * vec4(normal, 0.0)).xyz;
	interp_eyePos = vec3(v_eye) / v_eye.w;
   	gl_Position = pvm * vec4(pos, 1.0);
	fragCol = color;
}
