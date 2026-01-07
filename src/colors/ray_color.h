#ifndef RAY_COLOR_H
#define RAY_COLOR_H

#include "../ray/ray.h"
#include "../objects/hittable.h"
#include "../colors/color.h"

#include "ray_color.h"
#include <limits>
#include <cmath>

vec4 reflect(const vec4& v, const vec4& n) {
    return v - 2 * dot(v,n) * n;
}

color ray_color(const ray& r, const hittable& world) {
    hit_record rec;

    double tmin = 0.001;
    if (!world.hit(r, tmin, std::numeric_limits<double>::infinity(), rec))
        return color(0, 0, 0, 0);

    point4 light_pos(-100, 140, -20, 1);

    color I_A(0.3, 0.3, 0.3, 0.0);
    color I_F(0.7, 0.7, 0.7, 0.0);

    color base_color;
    if (rec.mat->tex)
        base_color = rec.mat->tex->value(rec.u, rec.v, rec.p);
    else
        base_color = rec.mat->K_dif;

    color ambient_color = I_A * base_color;

    vec4 l = unit_vector(light_pos - rec.p);
    double light_distance = (light_pos - rec.p).length();

    point4 shadow_origin = rec.p + rec.normal * tmin;
    ray shadow_ray(shadow_origin, l);
    hit_record shadow_rec;

    if (world.hit(shadow_ray, tmin, light_distance, shadow_rec))
        return ambient_color;

    vec4 n = rec.normal;
    vec4 v = unit_vector(-r.direction());
    vec4 rfl = unit_vector(reflect(-l, n));

    double diff = std::max(0.0, dot(l, n));
    double spec = pow(std::max(0.0, dot(v, rfl)), rec.mat->shininess);

    color I_d = I_F * base_color * diff;
    color I_e = I_F * rec.mat->K_esp * spec;

    return ambient_color + I_d + I_e;
}

#endif