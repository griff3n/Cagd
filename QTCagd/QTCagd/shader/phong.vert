#version 330
in vec3 position;
in vec3 normal;
in vec3 color;

uniform mat4 mvp;
uniform mat4 mv;
uniform mat3 nm;

out Data
{
    vec3 normal;
    vec3 color;
    vec4 eye;
} DataOut;

void main()
{
    DataOut.normal = normalize(nm * normal);
    DataOut.color  = color;
    DataOut.eye	   = -(mv * vec4(position, 1));

    gl_Position = mvp * vec4(position, 1);
}
