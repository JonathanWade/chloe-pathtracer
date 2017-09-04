#pragma once

#include "hitable.h"

class sphere : public hitable {
    public:
        sphere() {}
        sphere(vec3 center, float r) : center(center), radius(r) {};
        virtual bool hit(const ray& r, float tMin, float tMax, hitRecord& rec) const override;

        vec3 center;
        float radius;
};

bool sphere::hit(const ray& r, float tMin, float tMax, hitRecord& rec) const {
    vec3 oc = r.Origin() - center;
    float a = dot(r.Direction(), r.Direction());
    float b = dot(oc, r.Direction());
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b*b - a*c;
    if (discriminant > 0) {
        float temp = (-b - sqrt(b*b-a*c)) / a;
        if(temp < tMax && temp > tMin) {
            rec.t = temp;
            rec.p = r.PointAtParameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            return true;
        }
        temp = (-b + sqrt(b*b-a*c)) / a;
        if(temp < tMax && temp > tMin) {
            rec.t = temp;
            rec.p = r.PointAtParameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            return true;
        }
    }

    return false;
}
