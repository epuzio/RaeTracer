#ifndef CYLINDER_H
#define CYLINDER_H

#include "shape.h"
#include "vec3.h"
#include "interval.h"

class cylinder : public shape {
  public:
    cylinder(){}
    cylinder(point3 center, vec3 axis, double radius, double height, shared_ptr<material> bp)
        : center(center), axis(axis), radius(radius), height(height), bp(bp){}

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        vec3 oc = r.origin() - center;
        double a = dot(r.direction(), r.direction()) - dot(r.direction(), axis) * dot(r.direction(), axis);
        double b = 2.0 * (dot(r.direction(), oc) - dot(r.direction(), axis) * dot(oc, axis));
        double c = dot(oc, oc) - dot(oc, axis) * dot(oc, axis) - radius * radius;
        double discriminant = b * b - 4 * a * c;

        if (discriminant < 0)
            return false;

        double t = (-b - sqrt(discriminant)) / (2.0 * a);
        if (t < ray_t.min || t > ray_t.max)
            return false;

        point3 p = r.at(t);
        double projection = dot(p - center, axis);

        // Check intersection with the bottom cap circle
        if (projection < 0) {
            if (intersectBottomCap(r, ray_t, rec))
                return true;
        }

        // Check intersection with the top cap circle
        if (projection > height) {
            if (intersectTopCap(r, ray_t, rec))
                return true;
        }

        // Check intersection with the cylindrical body
        if (projection >= 0 && projection <= height) {
            vec3 outward_normal = normalize(p - center - dot(p - center, axis) * axis);
            rec.set_face_normal(r, outward_normal);
            rec.t = t;
            rec.p = p;
            rec.bp = bp;
            return true;
        }

        return false;
    }

  private:
    point3 center; //center of the cylinder
    vec3 axis; //line where the cylinder lies
    double radius;
    double height;
    shared_ptr<material> bp;

    bool intersectBottomCap(const ray& r, interval ray_t, hit_record& rec) const {
        // Create the bottom cap circle
        vec3 normal = -axis;
        point3 centerBottom = center - axis * height;
        
        // Calculate intersection with the circle
        double t = dot((centerBottom - r.origin()), (normal)) / dot(r.direction(), (normal));
        if (t < ray_t.min || t > ray_t.max)
            return false;

        point3 p = r.at(t);
        vec3 toCenter = p - centerBottom;
        double distanceSquared = toCenter.length_squared();
        if (distanceSquared <= radius * radius) {
            vec3 outward_normal = normalize(axis);
            rec.set_face_normal(r, outward_normal);
            rec.t = t;
            rec.p = p;
            rec.bp = bp;
            return true;
        }

        return false;
    }

    // Helper function to check intersection with the top cap circle
    bool intersectTopCap(const ray& r, interval ray_t, hit_record& rec) const {
        // Create the top cap circle
        vec3 normal = axis;
        point3 centerTop = center + axis * height;

        // Calculate intersection with the circle
        double t = dot((centerTop - r.origin()), (normal)) / dot(r.direction(), (normal));
        if (t < ray_t.min || t > ray_t.max)
            return false;

        point3 p = r.at(t);
        vec3 toCenter = p - centerTop;
        double distanceSquared = toCenter.length_squared();
        if (distanceSquared <= radius * radius) {
            vec3 outward_normal = normalize(axis);
            rec.set_face_normal(r, outward_normal);
            rec.t = t;
            rec.p = p;
            rec.bp = bp;
            return true;
        }
        return false;
    }
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