#pragma once

#include "ray.h"

struct hitRecord {
    float t;
    vec3 p;
    vec3 normal;
};

class hitable {
    public:
        virtual bool hit(const ray& r, float tMin, float tMax, hitRecord& rec) const = 0;
};

