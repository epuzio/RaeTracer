#ifndef SHAPE_H
#define SHAPE_H

#include "ray.h"
#include "interval.h"
#include "material.h"

class hit_record {
  public:
    point3 p;
    vec3 normal;
    shared_ptr<material> materialPtr;
    double t;
    bool front_face;

    void set_face_normal(const ray& r, const vec3& outward_normal) {
        // Sets the hit record normal vector.
        // NOTE: the parameter `outward_normal` is assumed to have unit length.

        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class shape {
  public:
    virtual ~shape() {};
    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};



#endif