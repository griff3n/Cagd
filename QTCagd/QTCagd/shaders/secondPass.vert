#version 330

in vec3 position;
in vec3 color;
in uint id1;
in uint id0;

uniform mat4 mvp;

out Data {
    vec3 color;

    flat uint id0;
    flat uint id1;
} data_out;


void main()
{
    data_out.color = color;
    data_out.id0 = id0;
    data_out.id1 = id1;

    gl_Position  = mvp * vec4(position, 1);
}
