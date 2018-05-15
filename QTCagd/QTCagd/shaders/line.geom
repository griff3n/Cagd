#version 330

uniform float thickness;

uniform vec4 viewport;      // 0, 0, width, height
uniform vec2 depthrange;    // near, far
uniform mat4 projection;

layout (lines) in;
layout (triangle_strip, max_vertices = 4) out;

in Data {
    vec3 color;
} data_in [];

out Data
{
    vec3 color;

    vec2 v0;
    vec2 v1;
} data;

void emitVertex(int i, vec4 position, vec2 wv0, vec2 wv1)
{
    data.color   = data_in[i].color;

    data.v0     = wv0;
    data.v1     = wv1;

    gl_Position = position;

    EmitVertex();
}

// PERSPECTIVE DIVIDE to get from CLIP SPACE to NDC (https://www.opengl.org/wiki/Vertex_Post-Processing#Perspective_divide)
vec3 fromClipToNDC(vec4 clip)
{
    return clip.xyz / clip.w;
}

// VIEWPORT TRANSFORMATION to get from NDC to WINDOW SPACE (https://www.opengl.org/wiki/Vertex_Post-Processing#Viewport_transform)
vec3 fromClipToWindow(vec4 clip)
{
    vec3 ndc    = fromClipToNDC(clip);
    vec3 window;

    float far  = gl_DepthRange.far;
    float near = gl_DepthRange.near;

    window.x = 0.5 * viewport.z * ndc.x + viewport.x + 0.5 * viewport.z;
    window.y = 0.5 * viewport.w * ndc.y + viewport.y + 0.5 * viewport.w;
    window.z = 0.5 * (far - near) * ndc.z + 0.5 * (far + near);

    return window;
}

// https://www.opengl.org/wiki/Compute_eye_space_from_window_space#From_window_to_ndc
vec3 fromWindowToNDC(vec3 window)
{
    vec3 ndc;

    ndc.x = (2.0f * window.x - 2.0f * viewport.x) / viewport.z; // - 1.0;
    ndc.y = (2.0f * window.y - 2.0f * viewport.y) / viewport.w; // - 1.0;
    ndc.z = (2.0f * window.z - gl_DepthRange.far - gl_DepthRange.near) / (gl_DepthRange.far - gl_DepthRange.near); // - 1.0;

    return ndc;
}

vec2 fromWindowToNDC(vec2 window)
{
    vec2 ndc;

    ndc.x = (2.0f * window.x - 2.0f * viewport.x) / viewport.z; // - 1.0;
    ndc.y = (2.0f * window.y - 2.0f * viewport.y) / viewport.w; // - 1.0;

    return ndc;
}

void main()
{
    // thickness of the line
    float t = thickness * 2.1;

    // vertices in CLIP SPACE
    vec4 v0 = gl_in[0].gl_Position;
    vec4 v1 = gl_in[1].gl_Position;

    // Step 1: Calculate a and b in WINDOW SPACE
    vec2 wv0 = fromClipToWindow(v0).xy;
    vec2 wv1 = fromClipToWindow(v1).xy;

    vec2 a = normalize(wv1 - wv0);
    vec2 b = vec2(-a.y, a.x);

    // Step 2: End points of the "line"-quad in NDC
    vec4 p0 = v0;
    vec4 p1 = v1;
    vec4 p2 = v1;
    vec4 p3 = v0;

    p0.xy += fromWindowToNDC(-a - b) * v0.w * t;
    p1.xy += fromWindowToNDC( a - b) * v1.w * t;
    p2.xy += fromWindowToNDC( a + b) * v1.w * t;
    p3.xy += fromWindowToNDC(-a + b) * v0.w * t;

    // Final Step: Emit new vertices
    emitVertex(0, p0, wv0, wv1);
    emitVertex(0, p3, wv0, wv1);
    emitVertex(1, p1, wv0, wv1);
    emitVertex(1, p2, wv0, wv1);

    EndPrimitive();
}
