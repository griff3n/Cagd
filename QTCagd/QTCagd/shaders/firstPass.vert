#version 330

in vec3 position;
in uint id;

uniform mat4 mvp;

flat out uint fragment_id;

void main()
{
    fragment_id = id;
    gl_Position = mvp * vec4(position, 1.0);
}
