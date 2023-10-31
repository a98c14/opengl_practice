
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
    vec4 u_glyph_bounds;
    vec4 u_color;
    vec4 u_outline_color;
    float u_softness;
    float u_thickness;
    float u_outline_thickness;
};

uniform mat4 u_mvp;
uniform sampler2D u_main_texture;

/* Vertex Data */
in vec2 v_tex_coord;

out vec4 color;

void main() {
    vec2 uv = v_tex_coord;
    vec2 dxy = fwidth(uv);
    float d = texture(u_main_texture, uv).r;
    float softness = u_softness * max(dxy.x, dxy.y);
    float alpha = smoothstep(1 - u_thickness - softness, 1 - u_thickness + softness, d);
    float outline_alpha = smoothstep(1 - u_thickness - softness - u_outline_thickness, 1 - u_thickness + softness - u_outline_thickness, d);
    vec4 outline_color = vec4(u_outline_color.xyz, outline_alpha * u_outline_color.a);
    vec4 inside_color = vec4(u_color.xyz, alpha * u_color.a);
    color = mix(inside_color, outline_color, clamp(outline_color.a - alpha, 0, 1));
}