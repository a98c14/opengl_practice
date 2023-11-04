#include "boids.h"

internal BoidBucketHashMap*
boid_bucket_new(Arena* arena, float32 width, float32 height, float32 visual_range)
{
    uint32 bucket_count_x = (uint32)(3 * (width / visual_range));
    uint32 bucket_count_y = (uint32)(3 * (height / visual_range));
    BoidBucketHashMap* hash_map = arena_push_struct_zero(arena, BoidBucketHashMap);
    hash_map->bucket_count = bucket_count_x * bucket_count_y;
    hash_map->cell_size = visual_range;
    hash_map->buckets = arena_push_array_zero(arena, BoidBucket, hash_map->bucket_count);
    for(int i = 0; i < hash_map->bucket_count; i++)
    {
        hash_map->buckets[i].key = -1;
        hash_map->buckets[i].boid_indices = arena_push_array(arena, int32, BUCKET_BOID_CAPACITY);
    }
    return hash_map;
}

internal BoidBucket*
get_bucket(BoidBucketHashMap* hash_map, Vec2 pos)
{
    int32 bucket_x = (int32)(pos.x / hash_map->cell_size) + (pos.x > 0 ? 0 : -1);
    int32 bucket_y = (int32)(pos.y / hash_map->cell_size) + (pos.y > 0 ? 0 : -1);
    int64 key = ((int64)bucket_x << 32) + ((int64)bucket_y);
    int32 index = hash_uint64(key) % hash_map->bucket_count;
    BoidBucket* bucket = &hash_map->buckets[index];
    uint32 probe_count = 0;
    while(bucket->key != key && bucket->key != -1)
    {
        index++;
        bucket = &hash_map->buckets[index];
        probe_count++;
        Assert(probe_count < 3, "Probe count exceeded maximum");
    }

    // initialize bucket if this is the first time
    // it is being fetched
    if(bucket->key == -1)
    {
        bucket->x = bucket_x;
        bucket->y = bucket_y;
        bucket->key = key;
    }
    return bucket;
}

internal void
insert_to_bucket(BoidBucketHashMap* hash_map, Vec2 pos, uint16 boid_index)
{
    BoidBucket* bucket = get_bucket(hash_map, pos);
    bucket->boid_indices[bucket->boid_count] = boid_index;
    bucket->boid_count++;
}