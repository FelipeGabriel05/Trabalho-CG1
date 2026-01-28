#ifndef HITTABLE_H
#define HITTABLE_H

#include "../ray/ray.h"
#include <memory>
#include "../material/material.h"

class hittable;

class hit_record {
  public:
    point4 p;
    vec4 normal;  
    double t;
    double u;
    double v;
    std::shared_ptr<material> mat = nullptr;
    const hittable* obj = nullptr;
};

enum class ObjectType {
    Sphere,
    Cilindro,
    Cone,
    Plane,
    Mesh,
    Triangle,
    Unknown
};

class hittable {
  public:
    virtual ~hittable() = default;
    virtual bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const = 0;
    virtual ObjectType type() const { return ObjectType::Unknown; }
    virtual std::shared_ptr<material> get_material() const { return nullptr; }
};

#endif