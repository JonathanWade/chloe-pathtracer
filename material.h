#pragma once

#include "utility.h"
#include "hitable.h"

class material {
    public:
        virtual bool scatter(const ray& r_in, const hitRecord& rec, vec3& attenuation, ray& scattered) const = 0;
};

// Lambertian diffuse //////////////////////////////
class lambertian : public material {
    public:
        lambertian(const vec3& a) : albedo(a) {}

        virtual bool scatter(const ray& r_in, const hitRecord& rec, vec3& attenuation, ray& scattered) const {
            vec3 target = rec.p + rec.normal + RandomInUnitSphere();
            scattered = ray(rec.p, target-rec.p);
            attenuation = albedo;
            return true;
        }

        vec3 albedo;
};

// Metal //////////////////////////////
class metal : public material {
    public:
        metal(const vec3& a, float fuzziness) : albedo(a) {
            if (fuzziness < 1) {
                fuzz = fuzziness;
            } else {
                fuzz = 1.0f;
            }
        }

        virtual bool scatter(const ray& r_in, const hitRecord& rec, vec3& attenuation, ray& scattered) const {
            vec3 reflected = Reflect(UnitVector(r_in.Direction()), rec.normal);
            scattered = ray(rec.p, reflected + fuzz * RandomInUnitSphere());
            attenuation = albedo;
            return (dot(scattered.Direction(), rec.normal) > 0);
        }

        vec3 albedo;
        float fuzz;
};
