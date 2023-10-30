#include "opus_graphics_utils.h"

internal Texture
texture_load_from_file(String texture_path, bool32 pixel_perfect, bool32 flip_vertical)
{
    stbi_set_flip_vertically_on_load(flip_vertical);

    int32 width, height, channels;
    // uint8 *data = stbi_load(texture_path.value, &width, &height, &channels, 0);
    uint8 *data = stbi_load("C:/Users/selim/source/practice/opengl/opengl_04_win/assets/open_sans.png", &width, &height, &channels, 0);
    
    if(!data)
    {
        printf("failed to load texture");
        return (Texture){0};
    }

    uint32 filter = pixel_perfect ? GL_NEAREST : GL_LINEAR;
    Texture texture = texture_load(width, height, channels, filter, data);
    stbi_image_free(data);
    return texture;
}