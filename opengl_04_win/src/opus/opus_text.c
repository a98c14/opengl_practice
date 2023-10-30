#include "opus_text.h"

internal GlyphAtlas*
glyph_atlas_load(Arena* arena, const GlyphAtlasInfo* atlas_info, const Glyph* glyphs, uint32 glyph_count, Texture texture)
{
    GlyphAtlas* atlas = arena_push_struct_zero(arena, GlyphAtlas);
    atlas->glyphs = glyphs;
    atlas->glyph_count = glyph_count;
    atlas->atlas_info = atlas_info;
    atlas->texture = texture;
    return atlas;
}