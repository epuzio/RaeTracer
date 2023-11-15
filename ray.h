#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
  private:
    point3 originPoint;
    vec3 rayDirection;

  public:
    ray(const point3& origin, const vec3& direction) : originPoint(origin), rayDirection(normalize(direction)) {}

    point3 origin() const  { return originPoint; }
    vec3 direction() const { return rayDirection; }

    point3 at(double t) const {
        return originPoint + t*rayDirection;
    }
};

#endif