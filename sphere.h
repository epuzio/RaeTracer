#ifndef SPHERE_H
#define SPHERE_H

#include "shape.h"
#include "vec3.h"

class sphere : public shape {
  public:
    sphere(point3 c, double r, shared_ptr<material> bp) : center(c), radius(r), bp(bp) {}

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
      vec3 oc = r.origin() - center;
      auto a = r.direction().length_squared();
      auto half_b = dot(oc, r.direction());
      auto c = oc.length_squared() - radius * radius;
      auto discriminant = half_b * half_b - a * c;

      if (discriminant > 0) {
          auto root = sqrt(discriminant);
          auto temp = (-half_b - root) / a;
          if (temp < ray_t.max && temp > ray_t.min) {
              rec.t = temp;
              rec.p = r.at(rec.t);
              rec.set_face_normal(r, normalize((rec.p - center) / radius));
              rec.bp = bp;
              return true;
          }
      }
      return false;
    }

  private:
    point3 center;
    double radius;
    shared_ptr<material> bp;
};

#endif
