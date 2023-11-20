#ifndef SHAPE_H
#define SHAPE_H

#include "ray.h"
#include "interval.h"
#include "material.h"
#include <memory>

class hit_record {
  public:
    point3 p;
    vec3 normal;
    shared_ptr<material> bp;
    vec3 texturecoordinate; //where on the 2D texture is the hit point mapped to?
    double t;
    bool front_face;

    void set_face_normal(const ray& r, const vec3& outward_normal) {
        // Sets the hit record normal vector.
        // NOTE: the parameter `outward_normal` is assumed to have unit length (edit: this is explicitly calculated now)

        front_face = dot(r.direction(), normalize(outward_normal)) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class shape {
  public:
    virtual ~shape() {};
    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
    virtual vec3 uvmap(const vec3& point, int textureWidth, int textureHeight) const = 0;
};



#endif