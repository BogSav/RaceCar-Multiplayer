#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 3) in vec3 vertex_color;

uniform mat3 Model;
uniform mat3 TransformMatrix;

out vec3 frag_color;

void main()
{
    frag_color = vertex_color;
    gl_Position = vec4((TransformMatrix * Model * v_position).xy, 0, 1.0);
}