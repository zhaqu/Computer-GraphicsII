#version 410

in vec3 Position;
in vec3 Normal;

uniform samplerCube envMap;
uniform vec3 cam_pos;
out vec4 outCol;

void main()
{
	vec3 I = normalize(Position - cam_pos);
	vec3 RLE = reflect(I, normalize(Normal));
	vec3 RA = refract(I, normalize(Normal), 1.2);
	vec3 half_R_RA = 0.5*RLE + 0.5* RA;
	float R0 = pow((1.2-1)/(1.2+1),2.0);
	float Schi_R = R0 + (1-R0) * pow(1-dot(-I,Normal), 2.0);
	vec3 Schi = Schi_R * RLE + (1-Schi_R) * RA;
	//outCol = vec4(texture(envMap, Schi).rgb, 1.0);
	outCol = vec4(Schi_R,Schi_R,Schi_R, 1.0);
}
