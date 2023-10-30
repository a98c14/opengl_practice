#include "opus_file.h"

internal String
file_read_all_as_string(Arena* arena, String path)
{
    FILE *file = fopen(path.value, "rb"); // open in binary mode
    if (!file) {
        perror("Failed to open file");
        return string_null();
    }

    // Determine file size
    fseek(file, 0, SEEK_END);
    long fsize = ftell(file);
    fseek(file, 0, SEEK_SET);  // reset file position

    uint64 arena_initial_pos = arena->pos;
    String s = string_new(arena, fsize+1);   // +1 for null-terminator
    usize read = fread(s.value, 1, fsize, file);
    if (read != fsize) {
        printf("[ERROR] Error reading file");
        arena->pos = arena_initial_pos;
        fclose(file);
        return string_null();
    }
    
    s.value[fsize] = 0;  // null-terminate the content
    fclose(file);
    return s;
}

/* NOTE: Returned pointer needs to be freed with `file_free_texture` */
internal uint8*
file_read_texture(String texture_path, bool flip_vertical, bool pixel_perfect)
{
    stbi_set_flip_vertically_on_load(flip_vertical);

    int32 width, height, channels;
    uint8* data = stbi_load(texture_path.value, &width, &height, &channels, 0);
    if(!data)
    {
        printf("failed to load texture");
        return NULL;
    }

    
    stbi_image_free(data);
}
