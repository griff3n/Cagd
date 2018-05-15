#version 330

in vec3 position;
in vec3 color;

uniform mat4 mvp;

out Data {
    vec3 color;
} data_out;


void main()
{
    data_out.color = color;

    gl_Position  = mvp * vec4(position, 1);
}
