#version 430 core

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_color;
layout(location = 2) in vec2 a_tex_coord;

layout (std140, binding = 0) uniform Global
{
    vec4 g_time;
};

layout (std140, binding = 1) uniform Texture
{
    vec2 texture_size;
    float texture_layer_count;
};

layout (std140, binding = 2) uniform Camera
{
    mat4 projection;
    mat4 view;
};

layout (std140, binding = 4) uniform Custom
{
    vec4 u_color;
    vec4 u_round;
    vec2 u_scale;
    float u_softness;
};

uniform mat4 u_model;
uniform sampler2D u_main_texture;

/* Vertex Data */
in vec2 v_tex_coord;

out vec4 color;

float sdRoundedBox(in vec2 p, in vec2 b, in vec4 r)
{
    r.xy = (p.x>0.0)?r.xy : r.zw;
    r.x  = (p.y>0.0)?r.x  : r.y;
    vec2 q = abs(p)-b+r.x;
    return min(max(q.x,q.y),0.0) + length(max(q,0.0)) - r.x;
}

void main() {
    vec2 uv = v_tex_coord * 2 - 1;
    float aspect = u_scale.x / u_scale.y;
    uv.x *= aspect;
    vec2 dxy = fwidth(uv);

    // calculate corner roundness
    vec4 r = u_round;
    r.xy = (uv.x>0.0) ? r.xy : r.zw;
    r.x  = (uv.y>0.0) ? r.x  : r.y;
    
    vec2 q = abs(uv) - (vec2(1 * aspect, 1) - dxy * 10) + r.x;
    float d = min(max(q.x,q.y),0.0) + length(max(q,0.0)) - r.x;
    d = 1-smoothstep(0.0, u_softness, d);

    // float d = sdRoundedBox(uv, vec2(0.5, 0.5) , vec4(r));
    color = vec4(u_color.xyz, u_color.a * d);
}
