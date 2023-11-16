#ifndef SPHERE_H
#define SPHERE_H

#include "shape.h"
#include "vec3.h"

class sphere : public shape {
  public:
    sphere(point3 c, double r) : center(c), radius(r) {}

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
      vec3 oc = r.origin() - center;
      double a = dot(r.direction(), r.direction());
      double b = 2.0 * dot(oc, r.direction());
      double c = dot(oc, oc) - (radius * radius);
      double discriminant = b * b - 4 * a * c;

      if (discriminant < 0) {
          // No intersection
          return false;
      } else {
          // Ray intersects the sphere
          return true;
      }
    }

  private:
    point3 center;
    double radius;
};

#endif






// //original gpt code:
// vec3 oc = ray.originPoint - sphere.center;
//       double a = dot(ray.rayDirection, ray.rayDirection);
//       double b = 2.0 * dot(oc, ray.rayDirection);
//       double c = dot(oc, oc) - (sphere.radius * sphere.radius);
//       double discriminant = b * b - 4 * a * c;

//       if (discriminant < 0) {
//           // No intersection
//           return false;
//       } else {
//           // Ray intersects the sphere
//           return true;
//       }