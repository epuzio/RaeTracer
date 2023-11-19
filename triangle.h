#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "shape.h"
#include "vec3.h"
#include "ray.h"

class triangle : public shape {
  public:
    triangle(point3 v0, point3 v1, point3 v2, shared_ptr<material> bp) : v0(v0), v1(v1), v2(v2), bp(bp) {}

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
      //using Watertight Ray/Triangle Intersection:

        vec3 edge1 = v1 - v0;
        vec3 edge2 = v2 - v0;

        vec3 pvec = cross(r.direction(), edge2);
        float det = dot(edge1, pvec);

        if (det > -.001 && det < .001) {
            return false;
        }

        float inv_det = 1.0f / det;
        vec3 tvec = r.origin() - v0;
        float u = dot(tvec, pvec) * inv_det;

        if (u < 0.0f || u > 1.0f) {
            return false;
        }

        vec3 qvec = cross(tvec, edge1);
        float v = dot(r.direction(), qvec) * inv_det;

        if (v < 0.0f || u + v > 1.0f) {
            return false;
        }

        float t = dot(edge2, qvec) * inv_det;

        if (t < ray_t.min || t > ray_t.max) {
            return false;
        }

        rec.t = t;
        rec.set_face_normal(r, normalize(cross(edge1, edge2)));
        rec.p = r.at(t);
        rec.bp = bp;

        return true;
    }

  private:
    point3 v0;
    point3 v1;
    point3 v2;
    shared_ptr<material> bp;
};

#endif