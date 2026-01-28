#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "../vectors/vec.h"
#include "../material/material.h"
#include <memory>

// indica herança
class sphere : public hittable {
  public:
    sphere(
        const point4& center, 
        double radius,
        std::shared_ptr<material> m
    ) : center(center), radius(std::fmax(0,radius)), mat(m) {}

    std::shared_ptr<material> get_material() const override {
        return mat;
    }

    bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const override {
        vec4 oc = center - r.origin();
        auto a = r.direction().length_squared();
        auto h = dot(r.direction(), oc);
        auto c = oc.length_squared() - radius*radius;

        auto discriminant = h*h - a*c;
        if (discriminant < 0)
            return false;

        auto sqrtd = std::sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        auto root = (h - sqrtd) / a;
        if (root <= ray_tmin || ray_tmax <= root) {
            root = (h + sqrtd) / a;
            if (root <= ray_tmin || ray_tmax <= root)
                return false;
        }

        rec.t = root;
        rec.p = r.at(rec.t);    
        rec.normal = (rec.p - center) / radius;
        rec.mat = mat;
        rec.obj = this;

        return true;
    }

    ObjectType type() const override {
        return ObjectType::Sphere;
    }

  private:
    point4 center;
    double radius;
    std::shared_ptr<material> mat;
};

#endif