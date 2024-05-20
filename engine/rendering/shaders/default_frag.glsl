#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

#define POINT_LIGHT_COUNT 64 // The light total light count must be less than this variable
uniform int point_light_count;

uniform vec3 view_pos;

struct Material
{
	vec3	diffuse;
	vec3	specular;

	sampler2D	diffuse_tex;
	sampler2D	specular_tex;

	float		shininess;
};

uniform Material material;

#define MATERIAL_TYPE_COLOR 0
#define MATERIAL_TYPE_TEXTURE 1
uniform int material_type;

struct PointLight
{
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct DirLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform PointLight point_lights[POINT_LIGHT_COUNT];
uniform DirLight directional_light;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 view_dir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 frag_pos, vec3 view_dir);

void main()
{
	vec3 norm = normalize(Normal);
	vec3 view_dir = normalize(view_pos - FragPos);

	// Directional lighting
	vec3 result = CalcDirLight(directional_light, norm, view_dir);

	// Loop over point lights
	for (int i = 0; i < point_light_count; i++)
		result += CalcPointLight(point_lights[i], norm, FragPos, view_dir);

	FragColor = vec4(result, 1.0f);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 view_dir)
{
	vec3 light_dir = normalize(-light.direction);

	// Diffuse
	float diff = max(dot(normal, light_dir), 0.0);

	// Specular
	vec3 reflect_dir = reflect(-light_dir, normal);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

	// Calculate the final color
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	switch (material_type)
	{
		case MATERIAL_TYPE_COLOR:
			ambient = light.ambient * material.diffuse;
			diffuse = light.diffuse * diff * material.diffuse;
			specular = light.specular * spec * material.specular;
			break;

		case MATERIAL_TYPE_TEXTURE:
			ambient = light.ambient * vec3(texture(material.diffuse_tex, TexCoords));
			diffuse = light.diffuse * diff * vec3(texture(material.diffuse_tex, TexCoords));
			specular = light.specular * spec * vec3(texture(material.specular_tex, TexCoords));
			break;
	}

	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 frag_pos, vec3 view_dir)
{
	vec3 light_dir = normalize(light.position - FragPos);

	// Diffuse
	float diff = max(dot(normal, light_dir), 0.0);

	// Specular
	vec3 reflect_dir = reflect(-light_dir, normal);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

	// Attenuation
	float distance = length(light.position - FragPos);
	float attenuation = 1 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	// Calculate the final color
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	switch (material_type)
	{
		case MATERIAL_TYPE_COLOR:
			ambient = light.ambient * material.diffuse;
			diffuse = light.diffuse * diff * material.diffuse;
			specular = light.specular * spec * material.specular;
			break;

		case MATERIAL_TYPE_TEXTURE:
			ambient = light.ambient * vec3(texture(material.diffuse_tex, TexCoords));
			diffuse = light.diffuse * diff * vec3(texture(material.diffuse_tex, TexCoords));
			specular = light.specular * spec * vec3(texture(material.specular_tex, TexCoords));
			break;
	}

	ambient		*= attenuation;
	diffuse		*= attenuation;
	specular	*= attenuation;


	return (ambient + diffuse + specular);
}
