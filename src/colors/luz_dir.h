#ifndef LUZ_DIR_H
#define LUZ_DIR_H

#include "../ray/ray.h"
#include "../objects/hittable.h"
#include "../colors/color.h"

color ray_color_dir(
    const ray& r,
    const hittable& world,
    const vec4& light_dir,   // assumindo direção Para a luz
    const color& I_A,
    const color& I_F
);

#endif