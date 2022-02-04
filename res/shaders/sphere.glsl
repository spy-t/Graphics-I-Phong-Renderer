#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 tex_coord;

uniform mat4 u_model_view_matrix;
uniform mat4 u_projection_matrix;

void main() {
	gl_Position = u_projection_matrix * u_model_view_matrix * position;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

void main() {
	color = vec4(1.0);
}
