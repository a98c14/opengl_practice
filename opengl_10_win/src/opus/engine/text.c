#include "text.h"

internal Glyph
glyph_get(GlyphAtlas* atlas, char c)
{
    return atlas->glyphs[(int)c - 32];
}

internal GlyphAtlas*
glyph_atlas_load(Arena* arena, const GlyphAtlasInfo* atlas_info, const Glyph* glyphs, uint32 glyph_count, TextureIndex texture)
{
    GlyphAtlas* atlas = arena_push_struct_zero(arena, GlyphAtlas);
    atlas->glyphs = glyphs;
    atlas->glyph_count = glyph_count;
    atlas->atlas_info = atlas_info;
    atlas->texture = texture;
    return atlas;
}

internal Rect
text_calculate_bounds(GlyphAtlas* atlas, String str, float32 size_in_pixels)
{
    Vec2 size = vec2_zero();
    for(int i = 0; i < str.length; i++)
    {
        Glyph glyph = glyph_get(atlas, str.value[i]);
        float32 h = size_in_pixels * (glyph.plane_bounds.top - glyph.plane_bounds.bottom);
        size.x += glyph.advance * size_in_pixels;
        size.y = max(h, size.y);
    };

    return (Rect){.center = vec2_zero(), .size = size };
}

internal Rect
text_calculate_transforms(GlyphAtlas* atlas, String str, float32 size_in_pixels, Vec2 position, RectAlignmentType alignment, Mat4* dst_matrices, uint32 dst_index)
{
    Rect string_bounds = text_calculate_bounds(atlas, str, size_in_pixels);
    Glyph first_glyph = glyph_get(atlas, str.value[0]);
    Vec2 base_offset = { 
        .x = string_bounds.w * FontAlignmentMultiplierX[alignment] - size_in_pixels * first_glyph.plane_bounds.left,
        .y = string_bounds.h * FontAlignmentMultiplierY[alignment] 
    };

    uint32 index = dst_index;
    for(int i = 0; i < str.length; i++)
    {
        Glyph glyph = glyph_get(atlas, str.value[i]);
        float32 w = size_in_pixels * (glyph.plane_bounds.right - glyph.plane_bounds.left);
        float32 h = size_in_pixels * (glyph.plane_bounds.top - glyph.plane_bounds.bottom);
        Vec2 plane_offset = { 
            .x = size_in_pixels * glyph.plane_bounds.left, 
            .y = size_in_pixels * glyph.plane_bounds.bottom 
        };
        float32 x = position.x + base_offset.x + plane_offset.x + w / 2.0f;
        float32 y = position.y + base_offset.y + plane_offset.y + h / 2.0f;

        Mat4 transform = transform_quad_aligned(vec3(x, y, -10), vec2(w, h));
        memcpy(&dst_matrices[index], &transform, sizeof(transform));
        base_offset.x += glyph.advance * size_in_pixels;
        index++;
    }
    return string_bounds;
}