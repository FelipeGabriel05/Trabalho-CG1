#ifndef LUZ_PONTUAL_H
#define LUZ_PONTUAL_H

#include "../ray/ray.h"
#include "../objects/hittable.h"
#include "../colors/color.h"

#include <limits>
#include <cmath>

color luz_pontual(const ray& r, const hittable& world, const point4& light_pos, const color& I_A, const color& I_F);

#endif