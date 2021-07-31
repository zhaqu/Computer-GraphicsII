#version 410
uniform sampler2DRect diffuse_Tex;
uniform sampler2DRect normal_Tex;
uniform sampler2DRect specular_Tex;
in vec3 v_normal, interp_eyePos;
in vec2 TexCoord;
in vec3 v_tangent;
out vec4 outColor;



uniform mat4 pvm;
uniform mat4 vm;
uniform mat4 normal_matrix;
uniform vec3 l_diffuse, l_specular;
uniform vec3 m_diffuse, m_ambient, m_specular, m_emission;
uniform float shininess;
vec3 l_ambient = vec3(0.8,0.8,0.8);

void main()
{
	vec3 T,B,N;
	N = normalize(v_normal);
	T = normalize(v_tangent);
	B = cross(N,T);
	mat3 TBN = mat3(T,B,N);
	vec3 eye_vector, light_dir, half_vector, normal;
	//normal = normalize((texture(normal_Tex, TexCoord).rgb)*v_normal);
	normal = normalize((texture(normal_Tex, TexCoord).rgb));
	normal = normalize(normal * 2.0 -1.0);
	normal = normalize(TBN * normal);
	vec3 ambient, diffuse, specular;
	float intensity, NdotH;
	eye_vector = normalize(-interp_eyePos);
	light_dir = normalize(-vec3(0.5, -1.0, -1.0));
	ambient = vec3(texture(diffuse_Tex, TexCoord)) * l_ambient;
	intensity = max(dot(normal, light_dir), 0.0);
	if(intensity > 0.0)
	{
		diffuse = l_diffuse * vec3(texture(diffuse_Tex, TexCoord));
		half_vector = normalize(light_dir + eye_vector);
		NdotH = max(dot(normal, half_vector), 0.0);
		float tem_shine = pow(NdotH, shininess);
		//part1
		//specular =  tem_shine * l_specular;
		//part2
		specular =  tem_shine * vec3(texture(specular_Tex, TexCoord)) * l_specular;
	}
	vec3 temp_color;
	temp_color = ambient + diffuse * intensity + specular;
	outColor = vec4(temp_color , 1.0);
}