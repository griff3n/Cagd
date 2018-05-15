#version 330
in vec3 position;
in vec3 color;
in uint id;

uniform mat4 mvp;
uniform mat4 mv;
uniform mat3 nm;

out Data
{
    vec3 color;
    flat uint id;
} DataOut;

void main()
{
    DataOut.color   = color;
    DataOut.id      = id;

    gl_Position = mvp * vec4(position, 1);
}
