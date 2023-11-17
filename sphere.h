#ifndef SPHERE_H
#define SPHERE_H

#include "shape.h"
#include "vec3.h"

class sphere : public shape {
  public:
    sphere(point3 c, double r) : center(c), radius(r) {}

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
              rec.normal = (rec.p - center) / radius;
              cout << "hit :~)" << endl;
              // Additional data assignment to hit_record
              // e.g., material, texture, etc.
              return true;
          }
      }
      return false;
    }

  private:
    point3 center;
    double radius;
};

#endif
