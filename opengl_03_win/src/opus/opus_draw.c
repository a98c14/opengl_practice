#include "opus_draw.h"

internal DrawContext*
draw_context_new(Arena* arena, Camera* camera)
{
    DrawContext* draw_context = arena_push_struct_zero(arena, DrawContext);
    draw_context->camera = camera;

    draw_context->material_basic = material_new(
        arena, 
        file_read_all_as_string(arena, string("C:\\Users\\selim\\source\\practice\\opengl\\opengl_03_win\\src\\shaders\\basic.vert")),
        file_read_all_as_string(arena, string("C:\\Users\\selim\\source\\practice\\opengl\\opengl_03_win\\src\\shaders\\basic.frag")));

    draw_context->material_line = material_new(
        arena, 
        file_read_all_as_string(arena, string("C:\\Users\\selim\\source\\practice\\opengl\\opengl_03_win\\src\\shaders\\line.vert")),
        file_read_all_as_string(arena, string("C:\\Users\\selim\\source\\practice\\opengl\\opengl_03_win\\src\\shaders\\line.frag")));

    draw_context->material_quad = material_new(
        arena, 
        file_read_all_as_string(arena, string("C:\\Users\\selim\\source\\practice\\opengl\\opengl_03_win\\src\\shaders\\basic.vert")),
        file_read_all_as_string(arena, string("C:\\Users\\selim\\source\\practice\\opengl\\opengl_03_win\\src\\shaders\\basic.frag")));

    draw_context->material_boid = material_new(
        arena, 
        file_read_all_as_string(arena, string("C:\\Users\\selim\\source\\practice\\opengl\\opengl_03_win\\src\\shaders\\boid.vert")),
        file_read_all_as_string(arena, string("C:\\Users\\selim\\source\\practice\\opengl\\opengl_03_win\\src\\shaders\\boid.frag")));

    draw_context->material_circle = material_new(
        arena, 
        file_read_all_as_string(arena, string("C:\\Users\\selim\\source\\practice\\opengl\\opengl_03_win\\src\\shaders\\circle.vert")),
        file_read_all_as_string(arena, string("C:\\Users\\selim\\source\\practice\\opengl\\opengl_03_win\\src\\shaders\\circle.frag")));

    return draw_context;
}

internal void
draw_line(DrawContext* dc, Vec2 start, Vec2 end)
{
    float32 thickness = 0.55;
    glUseProgram(dc->material_line.program_id);
    Mat4 transform = transform_line(start, end, thickness);
    Mat4 mvp = mul_mat4(dc->camera->view, transform);
    mvp = mul_mat4(dc->camera->projection, mvp);
    glUniform4f(dc->material_line.location_color, 1, 1, 1, 1.0f);
    glUniformMatrix4fv(dc->material_line.location_model, 1, GL_FALSE, mvp.v);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

internal void
draw_circle(DrawContext* dc, Vec2 position, float32 radius)
{
    glUseProgram(dc->material_circle.program_id);
    Mat4 transform = transform_quad(position, vec2(radius, radius), 0);
    Mat4 mvp = mul_mat4(dc->camera->view, transform);
    mvp = mul_mat4(dc->camera->projection, mvp);
    glUniform4f(dc->material_line.location_color, 1, 1, 1, 1.0f);
    glUniformMatrix4fv(dc->material_line.location_model, 1, GL_FALSE, mvp.v);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

internal void
draw_boid(DrawContext* dc, Vec2 pos, Vec2 dir, float32 scale)
{
    glUseProgram(dc->material_boid.program_id);
    float32 angle = angle_vec2(dir);
    Mat4 transform = transform_quad(pos, vec2(2 * scale, 3 * scale), angle + 90);
    Mat4 mvp = mul_mat4(dc->camera->projection, transform);

    glUniform4f(dc->material_boid.location_color, 1, 1, 1, 1.0f);
    glUniformMatrix4fv(dc->material_boid.location_model, 1, GL_FALSE, mvp.v);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}