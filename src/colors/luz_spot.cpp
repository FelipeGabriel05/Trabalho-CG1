#include "luz_spot.h"
#include <limits>
#include <cmath>

color luz_spot(
    const ray& r,
    const hittable& world,
    const point4& light_pos,        // posição da luz
    const vec4& light_dir,          // direção do cone
    double abertura,                // ângulo de arbertura
    const color& I_A,
    const color& I_F
) {
    hit_record rec;
    double tmin = 0.001;

    if (!world.hit(r, tmin, std::numeric_limits<double>::infinity(), rec)) {
        return color(0,0,0,0);
    }

    color base_color = rec.mat->tex
        ? rec.mat->tex->value(rec.u, rec.v, rec.p)
        : rec.mat->K_dif;

    color ambient = I_A * base_color;


    vec4 dF = unit_vector(light_dir);           // d_F
    vec4 l  = unit_vector(light_pos - rec.p);  // l̂

    double cosAlpha = dot(-l, dF);
    double cosTheta = cos(abertura);

    // Fora do spot
    if (cosAlpha < cosTheta) {
        return ambient;
    }

    color I_F_spot = I_F * cosAlpha;

    point4 shadow_origin = rec.p + rec.normal * tmin;
    ray shadow_ray(shadow_origin, l);
    hit_record shadow_rec;
    double light_distance = (light_pos - rec.p).length();

    if (world.hit(shadow_ray, tmin, light_distance, shadow_rec)) {
        return ambient;
    }

    vec4 n = unit_vector(rec.normal);
    vec4 v = unit_vector(-r.direction());
    vec4 rfl = unit_vector(reflect(-l, n));

    double diff = std::max(0.0, dot(n, l));
    double spec = pow(
        std::max(0.0, dot(v, rfl)),
        rec.mat->shininess
    );

    color I_d = I_F_spot * base_color * diff;
    color I_e = I_F_spot * rec.mat->K_esp * spec;

    return ambient + I_d + I_e;
}