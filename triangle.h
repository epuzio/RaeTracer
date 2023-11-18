#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "shape.h"
#include "vec3.h"
#include "ray.h"

class triangle : public shape {
  public:
    triangle(point3 v0, point3 v1, point3 v2, shared_ptr<material> bp) : v0(v0), v1(v1), v2(v2), bp(bp) {}

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        //Using Möller-Trumbore algorithm:
        vec3 edge1 = v1 - v0;
        vec3 edge2 = v2 - v0;
        vec3 h = cross(normalize(r.direction()), edge2);
        float a = dot(edge1, h);

        //if a is very close to 0, ray either misses or is parallel to triangle
        if (a > -0.00001f && a < 0.00001f) {
          return false;
        }

        float f = 1.0f / a;
        vec3 s = r.origin() - v0; //ray from origin to v0
        float u = f * dot(s, h); //barycentric coordinate

        if (u < 0.0f || u > 1.0f) {
          return false;
        }

        vec3 q = cross(s, edge1);
        float v = f * dot(normalize(r.direction()), q);

        if (v < 0.0f || u + v > 1.0f) {
            return false;
        }

        float t = f * dot(edge2, q);

        if (t < ray_t.min || t > ray_t.max) {
            return false;
        }
        rec.t = t; //i don't know if this is correct
        rec.p = r.at(t);
        rec.bp = bp;
        return true; // Ray intersects the triangle
    }

  private:
    point3 v0;
    point3 v1;
    point3 v2;
    shared_ptr<material> bp;
};

#endif