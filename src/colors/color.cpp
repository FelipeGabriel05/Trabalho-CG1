#include "color.h"

vec4 reflect(const vec4& v, const vec4& n) {
    return v - 2 * dot(v,n) * n;
}