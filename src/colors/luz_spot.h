#ifndef LUZ_SPOT_H
#define LUZ_SPOT_H
#include "../ray/ray.h"
#include "../objects/hittable.h"
#include "../colors/color.h"


color luz_spot(
    const ray& r,
    const hittable& world,
    const point4& light_pos,        // posição da luz
    const vec4& light_dir,          // direção do cone
    double abertura,                // ângulo de arbertura
    const color& I_A,
    const color& I_F
); 

#endif