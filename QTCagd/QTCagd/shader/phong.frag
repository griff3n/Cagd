#version 330
out vec3 fragColor;

uniform vec3 lightDirection;
uniform vec3 ambient;
uniform vec3 specular;
uniform float shininess;

in Data
{
    vec3 normal;
    vec3 color;
    vec4 eye;
} DataIn;

void main()
{
    vec3 diffuse = DataIn.color;
    vec3 ambient2 = ambient;
    vec3 normal  = DataIn.normal;

    if (!gl_FrontFacing)
    {
        diffuse  = vec3(1.0, 0.0, 1.0);
        ambient2 = vec3(0.5, 0.0, 0.5);

        normal *= -1.0;
    }

    float spec = 0.0;

    vec3 n = normalize(normal);
    vec3 e = normalize(vec3(DataIn.eye));

    float lambert = max(dot(n, normalize(lightDirection)), 0.0);

    if (lambert > 0.0)
    {
        vec3 h = normalize(lightDirection + e);

        float intSpec = max(dot(h, n), 0.0);
        spec = pow(intSpec, shininess);
    }

    fragColor = ambient2 + lambert * diffuse + specular * spec;
}
