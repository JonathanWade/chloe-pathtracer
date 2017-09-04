#pragma once

#include "ray.h"

class camera {
    public:
        camera() {
            topLeft = vec3(-2.0, 1.5, -1.0);
            horizontal = vec3(4.0, 0.0, 0.0);
            vertical = vec3(0.0, -3.0, 0.0);
            origin = vec3(0.0, 0.0, 0.0);
        }

        ray GetRay(float u, float v) {
            return ray(origin, topLeft + u * horizontal + v * vertical - origin);
        }

        vec3 origin;
        vec3 topLeft;
        vec3 horizontal;
        vec3 vertical;
};
