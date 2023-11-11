#include <engine/draw.h>

internal DrawContext*
draw_context_new(Arena* arena, Renderer* renderer)
{
    DrawContext* draw_context = arena_push_struct_zero(arena, DrawContext);
    draw_context->renderer = renderer;
    
    draw_context->geometry_quad = geometry_quad_create(renderer);
    draw_context->geometry_triangle = geometry_triangle_create(renderer);

    draw_context->material_basic = material_new(
        renderer,
        file_read_all_as_string(arena, string("..\\src\\shaders\\basic_instanced.vert")),
        file_read_all_as_string(arena, string("..\\src\\shaders\\basic_instanced.frag")),
        sizeof(ShaderDataBasic),
        true);

    draw_context->material_line = material_new(
        renderer,
        file_read_all_as_string(arena, string("..\\src\\shaders\\line.vert")),
        file_read_all_as_string(arena, string("..\\src\\shaders\\line.frag")),
        sizeof(ShaderDataLine),
        false);

    draw_context->material_basic_texture = material_new(
        renderer,
        file_read_all_as_string(arena, string("..\\src\\shaders\\basic_texture.vert")),
        file_read_all_as_string(arena, string("..\\src\\shaders\\basic_texture.frag")),
        sizeof(ShaderDataBasicTexture),
        false);

    draw_context->material_triangle = material_new(
        renderer,
        file_read_all_as_string(arena, string("..\\src\\shaders\\triangle.vert")),
        file_read_all_as_string(arena, string("..\\src\\shaders\\triangle.frag")),
        sizeof(ShaderDataTriangle),
        false);

    draw_context->material_rounded_rect = material_new(
        renderer,
        file_read_all_as_string(arena, string("..\\src\\shaders\\rect_rounded.vert")),
        file_read_all_as_string(arena, string("..\\src\\shaders\\rect_rounded.frag")),
        sizeof(ShaderDataRectRounded),
        false);


    draw_context->material_boid = material_new(
        renderer,
        file_read_all_as_string(arena, string("..\\src\\shaders\\boid.vert")),
        file_read_all_as_string(arena, string("..\\src\\shaders\\boid.frag")),
        sizeof(ShaderDataBoid),
        false);

    draw_context->material_circle = material_new(
        renderer,
        file_read_all_as_string(arena, string("..\\src\\shaders\\circle.vert")),
        file_read_all_as_string(arena, string("..\\src\\shaders\\circle.frag")),
        sizeof(ShaderDataCircle), 
        false);

    draw_context->material_circle_instanced = material_new(
        renderer,
        file_read_all_as_string(arena, string("..\\src\\shaders\\circle_instanced.vert")),
        file_read_all_as_string(arena, string("..\\src\\shaders\\circle_instanced.frag")),
        sizeof(ShaderDataCircle), 
        true);

    draw_context->material_text = material_new(
        renderer,
        file_read_all_as_string(arena, string("..\\src\\shaders\\text.vert")),
        file_read_all_as_string(arena, string("..\\src\\shaders\\text.frag")),
        sizeof(ShaderDataText), 
        true);

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
draw_line(DrawContext* dc, Vec2 start, Vec2 end, Color color, float32 thickness)
{
    DrawBuffer draw_buffer = renderer_buffer_request(dc->renderer, ViewTypeWorld, SORT_LAYER_INDEX_DEFAULT, FRAME_BUFFER_INDEX_DEFAULT, TEXTURE_INDEX_NULL, dc->geometry_quad, dc->material_line, 1);
    draw_buffer.model_buffer[0] = transform_line(start, end, thickness);
    ((ShaderDataLine*)draw_buffer.uniform_data_buffer)[0].color = color_to_vec4(color);
}

internal void
draw_triangle(DrawContext* dc, Vec2 position, float32 rotation, Color color, float32 size, SortLayerIndex sort_index)
{
    DrawBuffer draw_buffer = renderer_buffer_request(dc->renderer, ViewTypeWorld, sort_index, FRAME_BUFFER_INDEX_DEFAULT, TEXTURE_INDEX_NULL, dc->geometry_triangle, dc->material_basic, 1);
    draw_buffer.model_buffer[0] = transform_quad(position, vec2(size, size), rotation);
    ((ShaderDataBasic*)draw_buffer.uniform_data_buffer)[0].color = color_to_vec4(color);
}

internal void
draw_rect(DrawContext* dc, Rect rect, float32 rotation, SortLayerIndex sort_index, StyleRect style)
{
    DrawBuffer draw_buffer = renderer_buffer_request(dc->renderer, ViewTypeWorld, sort_index, FRAME_BUFFER_INDEX_DEFAULT, TEXTURE_INDEX_NULL, dc->geometry_quad, dc->material_rounded_rect, 1);
    // transform_quad_aligned is much faster so if there is no need for rotation, use aligned
    if(rotation == 0) draw_buffer.model_buffer[0] = transform_quad_aligned(vec3_xy_z(rect.center, 0), rect.size);
    else draw_buffer.model_buffer[0] = transform_quad(rect.center, rect.size, rotation);

    ShaderDataRectRounded* uniform_buffer = (ShaderDataRectRounded*)draw_buffer.uniform_data_buffer;
    uniform_buffer[0].color = style.color;
    uniform_buffer[0].round = style.border_radius.v;
    uniform_buffer[0].scale = rect.size;
    uniform_buffer[0].softness = style.softness;
}

internal void
draw_debug_line(DrawContext* dc, Vec2 start, Vec2 end, Color color)
{
    draw_line(dc, start, end, color, 4 * dc->renderer->pixel_per_unit);
}

internal void
draw_texture_aligned(DrawContext* dc, Vec3 pos, Vec2 scale, TextureIndex texture)
{
    DrawBuffer draw_buffer = renderer_buffer_request(dc->renderer, ViewTypeWorld, SORT_LAYER_INDEX_DEFAULT, FRAME_BUFFER_INDEX_DEFAULT, texture, dc->geometry_quad, dc->material_basic_texture, 1);
    ShaderDataBasicTexture uniform_data = (ShaderDataBasicTexture){0};
    draw_buffer_insert(&draw_buffer, transform_quad_aligned(pos, scale), &uniform_data);
}

internal void
draw_bounds(DrawContext* dc, float32 left, float32 right, float32 bottom, float32 top, Color color, float32 thickness)
{
    DrawBuffer draw_buffer = renderer_buffer_request(dc->renderer, ViewTypeWorld, SORT_LAYER_INDEX_DEFAULT, FRAME_BUFFER_INDEX_DEFAULT, TEXTURE_INDEX_NULL, dc->geometry_quad, dc->material_line, 4);
    ShaderDataLine* uniform_buffer = (ShaderDataLine*)draw_buffer.uniform_data_buffer;
    Vec4 color_vec4 = color_to_vec4(color);
    draw_buffer.model_buffer[0] = transform_line(vec2(left, top), vec2(left, bottom), thickness);
    draw_buffer.model_buffer[1] = transform_line(vec2(left, bottom), vec2(right, bottom), thickness);
    draw_buffer.model_buffer[2] = transform_line(vec2(right, bottom), vec2(right, top), thickness);
    draw_buffer.model_buffer[3] = transform_line(vec2(right, top), vec2(left, top), thickness);
    uniform_buffer[0].color = color_vec4;
    uniform_buffer[1].color = color_vec4;
    uniform_buffer[2].color = color_vec4;
    uniform_buffer[3].color = color_vec4;
}

internal Rect
draw_text(DrawContext* dc, Vec2 pos, String str, RectAlignmentType alignment, StyleText style)
{
    ShaderDataText shader_data = {0};
    shader_data.color = style.color;
    shader_data.outline_color = style.outline_color;
    shader_data.thickness = style.thickness;
    shader_data.softness = style.softness;
    shader_data.outline_thickness = style.outline_thickness;
    DrawBuffer db = renderer_buffer_request(dc->renderer, ViewTypeWorld, SORT_LAYER_INDEX_DEFAULT, FRAME_BUFFER_INDEX_DEFAULT, dc->font_open_sans->texture, dc->geometry_quad, dc->material_text, str.length);
    Rect bounds = text_calculate_transforms(dc->font_open_sans, str, style.font_size, pos, alignment, db.model_buffer, 0);
    ShaderDataText* shader_data_buffer = (ShaderDataText*)db.uniform_data_buffer;
    for(int i = 0; i < str.length; i++)
    {
        Glyph glyph = glyph_get(dc->font_open_sans, str.value[i]);
        shader_data.glyph_bounds = glyph.atlas_bounds.v;
        memcpy(&shader_data_buffer[i], &shader_data, sizeof(ShaderDataText));
    }
    return bounds;
}

internal void
draw_circle(DrawContext* dc, Vec2 position, float32 radius, Color color)
{
    DrawBuffer draw_buffer = renderer_buffer_request(dc->renderer, ViewTypeWorld, SORT_LAYER_INDEX_DEFAULT, FRAME_BUFFER_INDEX_DEFAULT, TEXTURE_INDEX_NULL, dc->geometry_quad, dc->material_circle, 1);
    draw_buffer.model_buffer[0] = transform_quad(position, vec2(radius, radius), 0);
    ((ShaderDataCircle*)draw_buffer.uniform_data_buffer)[0].color = color_to_vec4(color);
}

internal void
draw_circle_filled(DrawContext* dc, Vec2 position, float32 radius, Color color)
{
    DrawBuffer draw_buffer = renderer_buffer_request(dc->renderer, ViewTypeWorld, SORT_LAYER_INDEX_DEFAULT, FRAME_BUFFER_INDEX_DEFAULT, TEXTURE_INDEX_NULL, dc->geometry_quad, dc->material_circle, 1);
    draw_buffer.model_buffer[0] = transform_quad(position, vec2(radius, radius), 0);
    ((ShaderDataCircle*)draw_buffer.uniform_data_buffer)[0].color = color_to_vec4(color);
    ((ShaderDataCircle*)draw_buffer.uniform_data_buffer)[0].fill_ratio = 1;
}

internal void
draw_boid(DrawContext* dc, Vec2 position, Vec2 direction, float32 size, Color color)
{
    float32 rotation = angle_vec2(direction);
    DrawBuffer draw_buffer = renderer_buffer_request(dc->renderer, ViewTypeWorld, SORT_LAYER_INDEX_DEFAULT, FRAME_BUFFER_INDEX_DEFAULT, TEXTURE_INDEX_NULL, dc->geometry_quad, dc->material_boid, 1);
    draw_buffer.model_buffer[0] = transform_quad(position, vec2(size, size * 1.4), 90 + rotation);
    ((ShaderDataBoid*)draw_buffer.uniform_data_buffer)[0].color = color_to_vec4(color);
}
