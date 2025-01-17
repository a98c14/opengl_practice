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

layout (std140, binding = 2) uniform Custom
{
    vec4 u_color;
};

uniform mat4 u_mvp;
uniform sampler2D u_main_texture;

/* Vertex Data */
in vec2 v_tex_coord;

out vec4 color;

void main() {
    float softness = 3;
    vec2 uv = v_tex_coord;
    uv = uv * 2.0 - 1.0;
    vec2 dp = fwidth(uv);
    softness *= dp.x;
    float d = smoothstep(1 - softness * 2, 1 - softness, length(uv)) - smoothstep(1 - softness, 1.0, length(uv));
    color = vec4(u_color.xyz, d * u_color.a);
}