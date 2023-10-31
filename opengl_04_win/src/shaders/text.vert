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
out vec2 v_tex_coord;

void main() 
{
    gl_Position = u_mvp * vec4(a_pos, 1.0);
    float w = u_glyph_bounds.z - u_glyph_bounds.x;
    float h = u_glyph_bounds.w - u_glyph_bounds.y;
    float x = ((a_tex_coord.x * w) + u_glyph_bounds.x) / texture_size.x;
    float y = ((a_tex_coord.y * h) + u_glyph_bounds.y) / texture_size.y;
    v_tex_coord = vec2(x, y);
}