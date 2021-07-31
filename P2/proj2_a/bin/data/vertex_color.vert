#version 410

layout ( location = 0 ) in vec3 pos;
layout ( location = 1 ) in vec4 tangent;
layout ( location = 2 ) in vec3 normal;
layout ( location = 3 ) in vec2 uv;

uniform mat4 pvm;
uniform mat4 vm;
uniform mat4 normal_matrix;

out vec3 v_normal, interp_eyePos;
out vec2 TexCoord;
out vec3 v_tangent;

void main()
{
	vec4 v_eye = vm * vec4(pos, 1.0);
	v_normal = (normal_matrix * vec4(normal, 0.0)).xyz;
	v_tangent = vec3(normalize(normal_matrix*tangent));
	interp_eyePos = vec3(v_eye) / v_eye.w;
   	gl_Position = pvm * vec4(pos, 1.0);
	TexCoord = vec2(uv.x*512, (1-uv.y)*512);
	//fragCol = color;
}
