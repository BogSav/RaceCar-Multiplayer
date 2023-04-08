#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 3) in vec3 vertex_color;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform vec3 UniformColor;

uniform vec3 CarPosition;
uniform float CurveCoefficient;

out vec3 frag_color;

void main()
{
    if(all(equal(UniformColor, vec3(0, 0, 0))))
        frag_color = vertex_color;
    else
        frag_color = UniformColor;

    vec3 newPosition = v_position;
    newPosition.y = v_position.y - pow(length(CarPosition - v_position), 2) * CurveCoefficient;

    gl_Position = Projection * View * Model * vec4(newPosition, 1.0);
}