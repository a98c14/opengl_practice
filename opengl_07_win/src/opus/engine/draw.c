#include <engine/draw.h>

internal DrawContext*
draw_context_new(Arena* arena, Renderer* renderer)
{
    DrawContext* draw_context = arena_push_struct_zero(arena, DrawContext);
    draw_context->renderer = renderer;
    
    draw_context->geometry_quad = geometry_quad_create();

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

    draw_context->material_boid = material_new(
        renderer,
        file_read_all_as_string(arena, string("..\\src\\shaders\\boid.vert")),
        file_read_all_as_string(arena, string("..\\src\\shaders\\boid.frag")),
        sizeof(ShaderDataBoid));

    draw_context->material_circle = material_new(
        renderer,
        file_read_all_as_string(arena, string("..\\src\\shaders\\circle.vert")),
        file_read_all_as_string(arena, string("..\\src\\shaders\\circle.frag")),
        sizeof(ShaderDataCircle));

    draw_context->material_text = material_new(
        renderer,
        file_read_all_as_string(arena, string("..\\src\\shaders\\text.vert")),
        file_read_all_as_string(arena, string("..\\src\\shaders\\text.frag")),
        sizeof(ShaderDataText));

    /* Fonts */
    TextureIndex font_texture = texture_new_from_file(renderer, string("..\\assets\\open_sans.png"), 0, 1);
    draw_context->font_open_sans =  glyph_atlas_load(
        arena,
        &FONT_OPEN_SANS_ATLAS_INFO,
        FONT_OPEN_SANS_GLYPHS,
        countof(FONT_OPEN_SANS_GLYPHS),
        font_texture);
    log_debug("initialized draw context");
    return draw_context;
}

internal void
draw_line(DrawContext* dc, Vec2 start, Vec2 end, Color color)
{
    float32 thickness = 0.45;
    DrawBuffer draw_buffer = renderer_buffer_request(dc->renderer, FRAME_BUFFER_INDEX_DEFAULT, dc->material_line, ViewTypeWorld, TEXTURE_INDEX_NULL, 1);
    draw_buffer.model_buffer[0] = transform_line(start, end, thickness);
    ((ShaderDataLine*)draw_buffer.uniform_data_buffer)[0].color = color_to_vec4(color);
}

internal void
draw_bounds(DrawContext* dc, float32 left, float32 right, float32 bottom, float32 top, Color color)
{
    draw_line(dc, vec2(left, top), vec2(left, bottom), color);
    draw_line(dc, vec2(left, bottom), vec2(right, bottom), color);
    draw_line(dc, vec2(right, bottom), vec2(right, top), color);
    draw_line(dc, vec2(right, top), vec2(left, top), color);
}

internal void
draw_text(DrawContext* dc, Vec2 pos, String str)
{
    ShaderDataText shader_data = {0};
    shader_data.color = color_to_vec4(ColorBlack);
    shader_data.outline_color = color_to_vec4(ColorInvisible);
    shader_data.thickness = 0.50;
    shader_data.softness = 30;
    shader_data.outline_thickness = 0.2;
    DrawBuffer db = renderer_buffer_request(dc->renderer, FRAME_BUFFER_INDEX_DEFAULT, dc->material_text, ViewTypeWorld, dc->font_open_sans->texture, str.length);
    text_calculate_transforms(dc->font_open_sans, str, 3.5, pos, RectAlignmentTypeBottomLeft, db.model_buffer, 0);
    ShaderDataText* shader_data_buffer = (ShaderDataText*)db.uniform_data_buffer;
    for(int i = 0; i < str.length; i++)
    {
        Glyph glyph = glyph_get(dc->font_open_sans, str.value[i]);
        shader_data.glyph_bounds = glyph.atlas_bounds.v;
        memcpy(&shader_data_buffer[i], &shader_data, sizeof(ShaderDataText));
    }
}

internal void
draw_circle(DrawContext* dc, Vec2 position, float32 radius, Color color)
{
    DrawBuffer draw_buffer = renderer_buffer_request(dc->renderer, FRAME_BUFFER_INDEX_DEFAULT, dc->material_circle, ViewTypeWorld, TEXTURE_INDEX_NULL, 1);
    draw_buffer.model_buffer[0] = transform_quad(position, vec2(radius, radius), 0);
    ((ShaderDataCircle*)draw_buffer.uniform_data_buffer)[0].color = color_to_vec4(color);
}

internal void
draw_circle_filled(DrawContext* dc, Vec2 position, float32 radius, Color color)
{
    DrawBuffer draw_buffer = renderer_buffer_request(dc->renderer, FRAME_BUFFER_INDEX_DEFAULT, dc->material_circle, ViewTypeWorld, TEXTURE_INDEX_NULL, 1);
    draw_buffer.model_buffer[0] = transform_quad(position, vec2(radius, radius), 0);
    ((ShaderDataCircle*)draw_buffer.uniform_data_buffer)[0].color = color_to_vec4(color);
    ((ShaderDataCircle*)draw_buffer.uniform_data_buffer)[0].fill_ratio = 1;
}

internal void
draw_boid(DrawContext* dc, Vec2 position, Vec2 direction, float32 size, Color color)
{
    float32 rotation = angle_vec2(direction);
    DrawBuffer draw_buffer = renderer_buffer_request(dc->renderer, FRAME_BUFFER_INDEX_DEFAULT, dc->material_boid, ViewTypeWorld, TEXTURE_INDEX_NULL, 1);
    draw_buffer.model_buffer[0] = transform_quad(position, vec2(size, size * 1.4), 90 + rotation);
    ((ShaderDataBoid*)draw_buffer.uniform_data_buffer)[0].color = color_to_vec4(color);
}
