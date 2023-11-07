#version 430 core

struct ShaderData
{
    vec4 color;
    float fill_ratio;
};

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

layout (std140, binding = 2) buffer Matrices
{
    mat4 mvp[];
};

layout (std140, binding = 3) buffer Custom
{
    ShaderData data[];
};

// uniform mat4 u_mvp;
uniform sampler2D u_main_texture;

/* Vertex Data */
in vec2 v_tex_coord;
flat in int v_instance_id;

out vec4 color;

void main() {
    float softness = 3;
    vec2 uv = v_tex_coord;
    uv = uv * 2.0 - 1.0;
    vec2 dp = fwidth(uv);
    softness *= dp.x;
    ShaderData v_data = data[v_instance_id];
    float d = smoothstep((1 - v_data.fill_ratio) - softness * 2, (1 - v_data.fill_ratio) - softness, length(uv)) - smoothstep(1 - softness, 1.0, length(uv));
    color = vec4(v_data.color.xyz, d * v_data.color.a);
}