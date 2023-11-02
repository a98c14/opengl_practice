#include "opus_draw.h"

internal DrawContext*
draw_context_new(Arena* arena, Renderer* renderer)
{
    DrawContext* draw_context = arena_push_struct_zero(arena, DrawContext);
    draw_context->renderer = renderer;

    // draw_context->material_basic = material_new(
    //     arena,
    //     file_read_all_as_string(arena, string("C:\\Users\\selim\\source\\practice\\opengl\\opengl_03_win\\src\\shaders\\basic.vert")),
    //     file_read_all_as_string(arena, string("C:\\Users\\selim\\source\\practice\\opengl\\opengl_03_win\\src\\shaders\\basic.frag")));

    draw_context->material_line = material_new(
        renderer,
        file_read_all_as_string(arena, string("..\\src\\shaders\\line.vert")),
        file_read_all_as_string(arena, string("..\\src\\shaders\\line.frag")),
        sizeof(ShaderDataLine));

    // draw_context->material_quad = material_new(
    //     arena,
    //     file_read_all_as_string(arena, string("C:\\Users\\selim\\source\\practice\\opengl\\opengl_03_win\\src\\shaders\\basic.vert")),
    //     file_read_all_as_string(arena, string("C:\\Users\\selim\\source\\practice\\opengl\\opengl_03_win\\src\\shaders\\basic.frag")));

    // draw_context->material_boid = material_new(
    //     arena,
    //     file_read_all_as_string(arena, string("C:\\Users\\selim\\source\\practice\\opengl\\opengl_03_win\\src\\shaders\\boid.vert")),
    //     file_read_all_as_string(arena, string("C:\\Users\\selim\\source\\practice\\opengl\\opengl_03_win\\src\\shaders\\boid.frag")));

    // draw_context->material_circle = material_new(
    //     arena,
    //     file_read_all_as_string(arena, string("C:\\Users\\selim\\source\\practice\\opengl\\opengl_03_win\\src\\shaders\\circle.vert")),
    //     file_read_all_as_string(arena, string("C:\\Users\\selim\\source\\practice\\opengl\\opengl_03_win\\src\\shaders\\circle.frag")));

    draw_context->material_text = material_new(
        renderer,
        file_read_all_as_string(arena, string("..\\src\\shaders\\text.vert")),
        file_read_all_as_string(arena, string("..\\src\\shaders\\text.frag")),
        sizeof(ShaderDataText));

    return draw_context;
}

internal void
draw_line(DrawContext* dc, Vec2 start, Vec2 end)
{
    float32 thickness = 0.55;
    DrawBuffer draw_buffer = renderer_buffer_request(dc->renderer, FRAME_BUFFER_INDEX_DEFAULT, dc->material_line, ViewTypeWorld, TEXTURE_INDEX_NULL, 1);
    draw_buffer.model_buffer[0] = transform_line(start, end, thickness);
    ((ShaderDataLine*)draw_buffer.uniform_data_buffer)[0].color = color_to_vec4(ColorBlack) ;
}

internal void
draw_bounds(DrawContext* dc, float32 left, float32 right, float32 bottom, float32 top)
{
    draw_line(dc, vec2(left, top), vec2(left, bottom));
    draw_line(dc, vec2(left, bottom), vec2(right, bottom));
    draw_line(dc, vec2(right, bottom), vec2(right, top));
    draw_line(dc, vec2(right, top), vec2(left, top));
}

internal void
draw_circle(DrawContext* dc, Vec2 position, float32 radius)
{
    // glUseProgram(dc->material_circle.program_id);
    // Mat4 transform = transform_quad(position, vec2(radius*2, radius*2), 0);
    // Mat4 mvp = mul_mat4(dc->camera->view, transform);
    // mvp = mul_mat4(dc->camera->projection, mvp);
    // glUniform4f(dc->material_line.location_color, 1, 1, 1, 1.0f);
    // glUniformMatrix4fv(dc->material_line.location_model, 1, GL_FALSE, mvp.v);
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
