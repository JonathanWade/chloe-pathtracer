#pragma once

#include <random>
#include "vec3.h"

inline vec3 Reflect(const vec3& v, const vec3& n) {
    return v - 2 * dot(v, n) * n;
}

vec3 RandomInUnitSphere() {
    vec3 p;
    std::random_device r;
    std::mt19937 gen(r());
    std::uniform_real_distribution<double> rd;
    do {
        p = 2.0f * vec3(rd(gen), rd(gen), rd(gen)) - vec3(1.0f, 1.0f, 1.0f);
    } while (p.SquaredLength() >= 1.0f);
    return p;
}

