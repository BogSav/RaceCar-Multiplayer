#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 3) in vec3 vertex_color;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform vec3 CarPosition;
uniform float CurveCoefficient;

out vec3 world_position;
out vec3 world_normal;

void main()
{
    // Lighting calculations
    world_position = (Model * vec4(v_position, 1)).xyz;
    world_normal = normalize( mat3(Model) * vertex_normal );

    // Apply the curve
    vec3 newPosition = (Model * vec4(v_position, 1.0)).xyz;
    newPosition.y = newPosition.y - pow(length(CarPosition - newPosition), 2) * CurveCoefficient;

    gl_Position = Projection * View * vec4(newPosition, 1.0);
}