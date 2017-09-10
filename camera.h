#pragma once

#include <cmath>

#include "ray.h"
#include "utility.h"

class camera {
    public:
        // TODO: change this to not be a lookat
        camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect, float aperture, float focusDistance) {
            lensRadius = aperture / 2.0f;
            float theta = vfov*pi/180.0f;
            float halfHeight = tan(theta/2.0);
            float halfWidth = aspect * halfHeight;
            origin = lookfrom;
            w = UnitVector(lookfrom - lookat);
            u = UnitVector(cross(vup, w));
            v = UnitVector(cross(w, u));
            //topLeft = vec3(-halfWidth, halfHeight, -1.0);
            topLeft = origin - halfWidth * focusDistance * u + halfHeight * focusDistance * v - w * focusDistance;
            horizontal = 2.0f * halfWidth * focusDistance * u;
            vertical = -2.0f * halfHeight * focusDistance * v;
        }

        ray GetRay(float s, float t) {
            vec3 rd = lensRadius * RandomInUnitDisk();
            vec3 offset = u * rd.x() + v * rd.y();
            return ray(origin + offset, topLeft + s * horizontal + t * vertical - origin - offset);
        }

        vec3 origin;
        vec3 topLeft;
        vec3 horizontal;
        vec3 vertical;
        vec3 u, w, v;
        float lensRadius;
};
