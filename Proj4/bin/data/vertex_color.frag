#version 410

in vec3 v_normal, interp_eyePos, fragCol;

out vec4 outColor;

uniform mat4 pvm;
uniform mat4 vm;
uniform mat4 normal_matrix;
uniform vec3 l_diffuse, l_specular;
uniform vec3 m_diffuse, m_ambient, m_specular, m_emission;
uniform float shininess;
vec3 l_ambient = vec3(0.2,0.0,0.0);
vec3 l_intensity = vec3(1.0, 1.0, 1.0);
void main()
{
	vec3 eye_vector, light_dir, half_vector, normal;
	normal = normalize(v_normal);
	vec3 ambient, diffuse, specular;
	float intensity, NdotH;
	eye_vector = normalize(-interp_eyePos);
	light_dir = normalize(-vec3(1.0, 0.0, -1.0));
	ambient = l_ambient;
	intensity = max(dot(normal, light_dir), 0.0);
	if(intensity > 0.0)
	{
		diffuse = l_diffuse ;
		half_vector = normalize(light_dir + eye_vector);
		NdotH = max(dot(normal, half_vector), 0.0);
		specular =  pow(NdotH, shininess)  * l_specular;
	}
	vec3 temp_color;
	temp_color = ambient * l_intensity + diffuse * intensity * l_intensity + specular * l_intensity;
	outColor = vec4(fragCol , 1.0);
}