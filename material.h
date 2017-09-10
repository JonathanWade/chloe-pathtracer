#pragma once

#include <random>
#include "utility.h"
#include "hitable.h"

extern std::mt19937 gen;
extern std::uniform_real_distribution<double> rd;

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

// Dielectric (refactive) //////////////////////////////
class dielectric : public material {
public:
    dielectric(float ri) : refractiveIndex(ri) {}

    virtual bool scatter(const ray& r_in, const hitRecord& rec, vec3& attenuation, ray& scattered) const {
        vec3 outwardNormal;
        vec3 reflected = Reflect(r_in.Direction(), rec.normal);
        float niOverNt;
        attenuation = vec3(1.0, 1.0, 1.0f);
        vec3 refracted;
        float reflect_prob;
        float cosine;

        if(dot(r_in.Direction(), rec.normal) > 0) {
            outwardNormal = -rec.normal;
            niOverNt = refractiveIndex;
            cosine = refractiveIndex * dot(r_in.Direction(), rec.normal) / r_in.Direction().Length();
        } else {
            outwardNormal = rec.normal;
            niOverNt = 1.0 / refractiveIndex;
            cosine = -dot(r_in.Direction(), rec.normal) / r_in.Direction().Length();
        }

        if(Refract(r_in.Direction(), outwardNormal, niOverNt, refracted)) {
            reflect_prob = schlick(cosine, refractiveIndex);
        } else {
            reflect_prob = 1.0f;
        }

        if(rd(gen) < reflect_prob) {
            scattered = ray(rec.p, reflected);
        } else {
            scattered = ray(rec.p, refracted);
        }

        return true;
    }

    float refractiveIndex;
};
