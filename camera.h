#pragma once

#include <cmath>

#include "ray.h"
#include "utility.h"

class camera {
    public:
        // TODO: change this to not be a lookat
        camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect) {
            vec3 u, w, v;
            float theta = vfov*pi/180.0f;
            float halfHeight = tan(theta/2.0);
            float halfWidth = aspect * halfHeight;
            origin = lookfrom;
            w = UnitVector(lookfrom - lookat);
            u = UnitVector(cross(vup, w));
            v = UnitVector(cross(w, u));
            //topLeft = vec3(-halfWidth, halfHeight, -1.0);
            topLeft = origin - halfWidth * u + halfHeight * v - w;
            horizontal = 2.0f * halfWidth * u;
            vertical = -2.0f * halfHeight * v;
        }

        ray GetRay(float u, float v) {
            return ray(origin, topLeft + u * horizontal + v * vertical - origin);
        }

        vec3 origin;
        vec3 topLeft;
        vec3 horizontal;
        vec3 vertical;
};
