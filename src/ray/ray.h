#ifndef RAY_H
#define RAY_H

#include "../vectors/vec.h"

class ray {
  public:
    ray() {}

    ray(const point4& origin, const vec4& direction) : orig(origin), dir(direction) {}

    const point4& origin() const  { return orig; }
    const vec4& direction() const { return dir; }

    point4 at(double t) const {
        return orig + t*dir;
    }

  private:
    point4 orig;
    vec4 dir;
};

#endif