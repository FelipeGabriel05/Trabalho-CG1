#ifndef COLOR_H
#define COLOR_H

#include "../vectors/vec.h"

using color = vec4;

vec4 reflect(const vec4& v, const vec4& n) {
    return v - 2 * dot(v,n) * n;
}

#endif