#include "luz_pontual.h"

color luz_pontual(const ray& r, const hittable& world, const point4& light_pos, const color& I_A, const color& I_F) {
    hit_record rec;

    double tmin = 0.001;
    if (!world.hit(r, tmin, std::numeric_limits<double>::infinity(), rec)) {
        return color(0.0, 0.0, 0.0, 0.0);
    }

    color base_color;
    if (rec.mat->tex) {
        base_color = rec.mat->tex->value(rec.u, rec.v, rec.p);
    } else {
        base_color = rec.mat->K_dif;
    }

    color ambient_color = I_A * base_color;

    vec4 l = unit_vector(light_pos - rec.p);
    double light_distance = (light_pos - rec.p).length();

    point4 shadow_origin = rec.p + rec.normal * tmin;
    ray shadow_ray(shadow_origin, l);
    hit_record shadow_rec;

    if (world.hit(shadow_ray, tmin, light_distance, shadow_rec)) {
        return ambient_color;
    }

    vec4 n = rec.normal;
    vec4 v = unit_vector(-r.direction());
    vec4 rfl = unit_vector(reflect(-l, n));

    double diff = std::max(0.0, dot(l, n));
    double spec = pow(std::max(0.0, dot(v, rfl)), rec.mat->shininess);

    color I_d = I_F * base_color * diff;
    color I_e = I_F * rec.mat->K_esp * spec;

    return ambient_color + I_d + I_e;
}
