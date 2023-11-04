#pragma once
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

// #include "linmath.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "opus.h"
#include "opus.c"
#include "fonts.h"

#define BUCKET_BOID_CAPACITY 128

typedef struct 
{
    int32 x;
    int32 y;
    int64 key;

    int32* boid_indices;
    int32 boid_count;
} BoidBucket;

typedef struct
{
    float32 cell_size;
    uint32 bucket_count;
    BoidBucket* buckets;
    Arena* arena;
} BoidBucketHashMap;

internal BoidBucketHashMap*
boid_bucket_new(Arena* arena, float32 width, float32 height, float32 visual_range);

internal BoidBucket*
get_bucket(BoidBucketHashMap* hash_map, Vec2 pos);

internal void
insert_to_bucket(BoidBucketHashMap* hash_map, Vec2 pos, uint16 boid_index);