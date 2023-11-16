#ifndef SPHERE_H
#define SPHERE_H

#include "shape.h"
#include "vec3.h"

class sphere : public shape {
  public:
    sphere(point3 c, double r) : center(c), radius(r) {}

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
      vec3 oc = ray.originPoint - sphere.center;
      double a = dot(ray.rayDirection, ray.rayDirection);
      double b = 2.0 * dot(oc, ray.rayDirection);
      double c = dot(oc, oc) - (sphere.radius * sphere.radius);
      double discriminant = b * b - 4 * a * c;

      if (discriminant < 0) {
          // No intersection
          return false;
      } else {
          // Ray intersects the sphere
          return true;
      }
        // vec3 oc = r.origin() - center;
        // auto a = r.direction().length_squared();
        // auto half_b = dot(oc, r.direction());
        // auto c = oc.length_squared() - radius*radius;

        // auto discriminant = half_b*half_b - a*c;
        // if (discriminant < 0) return false;
        // auto sqrtd = sqrt(discriminant);

        // // Find the nearest root that lies in the acceptable range.
        // auto root = (-half_b - sqrtd) / a;
        // if (!ray_t.surrounds(root)) {
        //     root = (-half_b + sqrtd) / a;
        //     if (!ray_t.surrounds(root))
        //         return false;
        // }

        // rec.t = root;
        // rec.p = r.at(rec.t);
        // vec3 outward_normal = (rec.p - center) / radius;
        // rec.set_face_normal(r, outward_normal);

        // return true;
    }

  private:
    point3 center;
    double radius;
};

#endif