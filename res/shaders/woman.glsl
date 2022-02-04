#shader vertex
#version 330 core

layout(location = 0) in vec4 i_position;
layout(location = 1) in vec3 i_normal;
layout(location = 2) in vec2 i_tex_coords;


out vec3 f_fragment_position;
out vec3 f_normal;
out vec2 f_tex_coords;

uniform mat4 u_model_view_matrix;
uniform mat4 u_projection_matrix;

void main() {
	vec4 world_space_position = u_model_view_matrix * i_position;
	gl_Position = u_projection_matrix * world_space_position;

	f_fragment_position = world_space_position.xyz;
	f_normal = mat3(transpose(inverse(u_model_view_matrix))) * i_normal;
	f_tex_coords = i_tex_coords;
}

#shader fragment
#version 330 core

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct PointLight {
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient_intensity;
	vec3 diffuse_intensity;
	vec3 specular_intensity;
};

layout(location = 0) out vec4 o_color;

in vec3 f_fragment_position;
in vec3 f_normal;
in vec2 f_tex_coords;

uniform int u_active_textures;
uniform sampler2D u_texture;
uniform Material u_material;
uniform PointLight u_light;

void main() {
	vec3 ambient = u_light.ambient_intensity * u_material.ambient;

	vec3 norm = normalize(f_normal);
	vec3 light_direction = normalize(u_light.position - f_fragment_position);
	float diff = max(dot(norm, light_direction), 0.0);
	vec3 diffuse = u_light.diffuse_intensity * diff * u_material.diffuse;

	vec3 view_direction = normalize(-f_fragment_position);
	vec3 reflection_direction = reflect(-light_direction, norm);
	float spec = pow(max(dot(view_direction, reflection_direction), 0.0), u_material.shininess);
	vec3 specular = u_light.specular_intensity * spec * u_material.specular;

	float distance = length(u_light.position - f_fragment_position);
	float attenuation = 1.0 / (u_light.constant + u_light.linear * distance + u_light.quadratic * (distance * distance));


	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	vec3 tex_color = vec3(0.0);
	if (u_active_textures > 0) {
		tex_color = texture(u_texture, f_tex_coords).xyz;
	}
	o_color = vec4((ambient + diffuse) * tex_color + specular, 1.0);
}
