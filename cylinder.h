#ifndef CYLINDER_H
#define CYLINDER_H

#include "shape.h"
#include "vec3.h"
#include "interval.h"

class cylinder : public shape {
  public:
    cylinder(){}
    cylinder(point3 center, vec3 axis, double radius, double height)
        : center(center), axis(axis), radius(radius), height(height) {}

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override{
        vec3 oc = r.origin() - center;
        double a = dot(r.direction(), r.direction()) - dot(r.direction(), axis)*dot(r.direction(), axis);
        double b = 2.0*(dot(r.direction(), oc) - dot(r.direction(), axis)*dot(oc, axis));
        double c = dot(oc, oc) - dot(oc, axis)*dot(oc, axis) - radius*radius;
        double discriminant = b*b - 4*a*c;
        if (discriminant < 0){
            return false;
        }

        double t = (-b - sqrt(discriminant)) / (2.0*a);
        if (t < ray_t.min || t > ray_t.max){
            return false;
        }

        point3 p = r.at(t);
        double projection = dot(p - center, axis);

        // Check if the intersection point is within the height of the cylinder
        if (projection < 0 || projection > height*2) {
            return false;
        }
        
        vec3 outward_normal = (p - center - dot(p - center, axis)*axis) / radius;
        rec.set_face_normal(r, outward_normal);
        rec.t = t;
        rec.p = p;
        return true;
    }
  private:
    point3 center; //center of the cylinder
    vec3 axis; //line where the cylinder lies
    double radius;
    double height;
    shared_ptr<material> materialPtr;
};

#endif









// //original copilot code:
// bool hit(const ray& r, interval ray_t, hit_record& rec) const override{
//             vec3 oc = r.origin() - center;
//             double a = dot(r.direction(), r.direction()) - dot(r.direction(), axis)*dot(r.direction(), axis);
//             double b = 2.0*(dot(r.direction(), oc) - dot(r.direction(), axis)*dot(oc, axis));
//             double c = dot(oc, oc) - dot(oc, axis)*dot(oc, axis) - radius*radius;
//             double discriminant = b*b - 4*a*c;
//             if (discriminant < 0) return false;
//             double t = (-b - sqrt(discriminant)) / (2.0*a);
//             if (t < ray_t.min() || t > ray_t.max()) return false;
//             point3 p = r.at(t);
//             if (p.y() < center.y() || p.y() > center.y() + height) return false;
//             vec3 outward_normal = (p - center - dot(p - center, axis)*axis) / radius;
//             rec.set_face_normal(r, outward_normal);
//             rec.t = t;
//             rec.p = p;
//             return true;
//         }