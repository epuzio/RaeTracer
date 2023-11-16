#ifndef MATERIAL_H
#define MATERIAL_H 

#include "raytracer.h"
#include "scene.h"
#include "vec3.h"
#include "color.h"

class hit_record;

class material {
  public:
    double ks;
    double kd;
    double specularexponent;
    color diffusecolor;
    color specularcolor;
    bool isreflective;
    double reflectivity;
    bool isrefractive;
    double refractiveindex;

    virtual ~material() {}
    virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;
};

#endif
