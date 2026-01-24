#ifndef LUZ_DIR_H
#define LUZ_DIR_H

#include "../ray/ray.h"
#include "../objects/hittable.h"
#include "../colors/color.h"

#include <limits>
#include <cmath>

color ray_color_dir(
    const ray& r,
    const hittable& world,
    const vec4& light_dir,   // assumindo direção PARA a luz
    const color& I_A,
    const color& I_F
) {
    hit_record rec;
    const double eps = 1e-4;

    if (!world.hit(r, eps, std::numeric_limits<double>::infinity(), rec)) {
        return color(0,0,0,0);
    }

    color base_color = rec.mat->tex 
        ? rec.mat->tex->value(rec.u, rec.v, rec.p) 
        : rec.mat->K_dif;

    color ambient = I_A * base_color;
    vec4 n = unit_vector(rec.normal);
    
    // mete l para a luz
    vec4 l = unit_vector(light_dir); 

    double nl = dot(n, l);

    // face oposta a luz apenas ambiente
    if (nl <= 0.0) return ambient; 

    // sombra dispara o raio de rec.p na direção da luz l
    point4 shadow_origin = rec.p + n * eps;
    ray shadow_ray(shadow_origin, l);
    hit_record shadow_rec;

    //  tmax deve ser grande, mas não deve atingir o céu se ele existir
    if (world.hit(shadow_ray, eps, 1e30, shadow_rec)) {
        return ambient;
    }

    // calculos de phong (difusa e especular)
    vec4 v = unit_vector(-r.direction());
    vec4 rfl = unit_vector(reflect(-l, n));

    double spec = pow(std::max(0.0, dot(v, rfl)), rec.mat->shininess);
    
    color I_d = I_F * base_color * nl;
    color I_e = I_F * rec.mat->K_esp * spec;

    return ambient + I_d + I_e;
}

#endif