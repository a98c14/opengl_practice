#include "boids.h"

internal BoidBucketHashMap*
boid_bucket_hash_map_new(Arena* arena, float32 width, float32 height, float32 visual_range)
{
    uint32 bucket_count_x = (uint32)(3 * (width / visual_range));
    uint32 bucket_count_y = (uint32)(3 * (height / visual_range));
    BoidBucketHashMap* hash_map = arena_push_struct(arena, BoidBucketHashMap);
    hash_map->bucket_count = bucket_count_x * bucket_count_y;
    hash_map->cell_size = visual_range;
    hash_map->position_buckets = arena_push_array(arena, BoidPositionBucket, hash_map->bucket_count);
    hash_map->direction_buckets = arena_push_array(arena, BoidDirectionBucket, hash_map->bucket_count);
    
    for(int i = 0; i < hash_map->bucket_count; i++)
    {
        hash_map->position_buckets[i].count = 0;
        hash_map->position_buckets[i].positions = arena_push_array(arena, Vec2, BUCKET_BOID_CAPACITY);
        hash_map->direction_buckets[i].count = 0;
        hash_map->direction_buckets[i].directions = arena_push_array(arena, Vec2, BUCKET_BOID_CAPACITY);
    }
    return hash_map;
}

internal void
calculate_bucket_indices(Arena* scratch_arena, BoidBucketHashMap* hash_map, Vec2* in_boid_positions, uint16* out_bucket_indices, BoidBucketLookupArray* out_lookup_buckets, uint16 boid_count)
{
    for(int i = 0; i < boid_count; i++)
    {
        Vec2 pos = in_boid_positions[i];
        int16 boid_cell_x = (int16)(pos.x / hash_map->cell_size) + (pos.x > 0 ? 0 : -1);
        int16 boid_cell_y = (int16)(pos.y / hash_map->cell_size) + (pos.y > 0 ? 0 : -1);
        out_bucket_indices[i] = hash_xy(boid_cell_x, boid_cell_y) % hash_map->bucket_count;

        // lookup bucket range
        int16 min_bucket_x = (int16)((pos.x - hash_map->cell_size) / hash_map->cell_size) + (pos.x - hash_map->cell_size > 0 ? 0 : -1);
        int16 max_bucket_x = (int16)((pos.x + hash_map->cell_size) / hash_map->cell_size) + (pos.x + hash_map->cell_size > 0 ? 0 : -1);
        int16 min_bucket_y = (int16)((pos.y - hash_map->cell_size) / hash_map->cell_size) + (pos.y - hash_map->cell_size > 0 ? 0 : -1);
        int16 max_bucket_y = (int16)((pos.y + hash_map->cell_size) / hash_map->cell_size) + (pos.y + hash_map->cell_size > 0 ? 0 : -1);

        // init lookup bucket array
        int8 lookup_bucket_count = (1 + max_bucket_x - min_bucket_x) * (1 + max_bucket_y - min_bucket_y);
        BoidBucketLookupArray* lookup_bucket = &out_lookup_buckets[i];
        lookup_bucket->count = 0;
        lookup_bucket->indices = arena_push_array(scratch_arena, uint16, lookup_bucket_count);

        // add all neighbouring cells to lookup buckets
        for(int16 y = min_bucket_y; y <= max_bucket_y; y++)
        {
            for(int16 x = min_bucket_x; x <= max_bucket_x; x++)
            {
                // TODO: test if linear probing improves performance or not, we currently skip
                // it because worst case it will be couple more distance checks
                uint16 bucket_index = hash_xy(x, y) % hash_map->bucket_count;

                // set the lookup index
                lookup_bucket->indices[lookup_bucket->count] = bucket_index;
                lookup_bucket->count++;
            }
        }
    }
}

internal void
fill_buckets(BoidBucketHashMap* hash_map, uint16* boid_bucket_indices, Vec2* boid_positions, Vec2* boid_directions, uint16 boid_count)
{
    for(int i = 0 ; i < boid_count; i++)
    {
        uint16 bucket_index = boid_bucket_indices[i];
        BoidPositionBucket* position_bucket = &hash_map->position_buckets[bucket_index];
        BoidDirectionBucket* direction_bucket = &hash_map->direction_buckets[bucket_index];
        position_bucket->positions[position_bucket->count] = boid_positions[i];
        direction_bucket->directions[direction_bucket->count] = boid_directions[i];
        position_bucket->count++;
        direction_bucket->count++;
    }
}