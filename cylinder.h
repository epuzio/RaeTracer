#ifndef CYLINDER_H
#define CYLINDER_H

#include "shape.h"
#include "vec3.h"
#include "interval.h"

class cylinder : public shape {
  public:
    cylinder(){}
    cylinder(point3 center, double radius, vec3 height) : center(center), radius(radius), height(height) {}
    bool hit(const ray& r, interval ray_t, hit_record& rec) const override{
        // Ray parameters
        vec3 P = r.origin();
        vec3 V = r.direction();

        // Solve for the intersection using quadratic equation coefficients
        float a = V.x() * V.x() + V.z() * V.z();
        float b = 2.0 * (P.x() * V.x() + P.z() * V.z() - center.x() * V.x() - center.z() * V.z());
        float c = P.x() * P.x() + P.z() * P.z() - center.x() * center.x() - center.z() * center.z() - radius * radius;

        // Solve quadratic equation: at^2 + bt + c = 0
        float discriminant = b * b - (4 * (a * c));
        if(fabs(discriminant) < 0.0001){ return false;}

        // Check if the ray intersects the cylinder
        if (discriminant >= 0) {
            float t1 = (-b - sqrt(discriminant)) / (2.0 * a);
            float t2 = (-b + sqrt(discriminant)) / (2.0 * a);

            // Check if the intersection is within the ray's interval
            if (t1 > ray_t.max || t2 < ray_t.min) {
                return false;
            }

            // Determine the closest intersection point
            float t = (t1 > ray_t.min) ? t1 : t2;

            // Compute the intersection point
            vec3 intersection_point = P + (t * V);

            // Set the hit record information (come back to later)
            // rec.t = t;
            // rec.point = intersection_point;
            // rec.normal = (intersection_point - center) / radius;

            return true; // Intersection found
        }

        return false; // No intersection
      }
  private:
    point3 center;
    double radius;
    vec3 height;
};

#endif