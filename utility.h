#pragma once

#include <random>
#include "vec3.h"

const double pi = 3.1415926535897;
extern std::mt19937 gen;
extern std::uniform_real_distribution<double> rd;

inline vec3 Reflect(const vec3& v, const vec3& n) {
    return v - 2 * dot(v, n) * n;
}

vec3 RandomInUnitSphere() {
    vec3 p;
    do {
        p = 2.0f * vec3(rd(gen), rd(gen), rd(gen)) - vec3(1.0f, 1.0f, 1.0f);
    } while (p.SquaredLength() >= 1.0f);
    return p;
}

vec3 RandomInUnitDisk() {
    vec3 p;
    do {
        p = 2.0f * vec3(rd(gen), rd(gen), 0.0f) - vec3(1.0f, 1.0f, 0.0f);
    } while (dot(p,p) >= 1.0f);
    return p;
}

bool Refract(const vec3& v, const vec3& n, float niOverNt, vec3& refracted) {
    vec3 uv = UnitVector(v);
    float dt = dot(uv, n);
    float discriminant = 1.0 - niOverNt*niOverNt*(1-dt*dt);
    if(discriminant > 0) {
        refracted = niOverNt * (uv - n*dt) - n * sqrt(discriminant);
        return true;
    }
    return false;
}

float schlick(float cosine, float refidx) {
    float r0 = (1 - refidx) / (1 + refidx);
    r0 = r0 * r0;
    return r0 + (1-r0)*pow((1-cosine),5);
}
