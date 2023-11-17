#ifndef MATERIAL_H
#define MATERIAL_H 

#include "raytracer.h"
#include "scene.h"
#include "vec3.h"
#include "color.h"

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

    material() {}
    material(double ks, double kd, double specularexponent, color diffusecolor, color specularcolor, bool isreflective, double reflectivity, bool isrefractive, double refractiveindex)
        : ks(ks), kd(kd), specularexponent(specularexponent), diffusecolor(diffusecolor), specularcolor(specularcolor), isreflective(isreflective), reflectivity(reflectivity), isrefractive(isrefractive), refractiveindex(refractiveindex) {}
};

#endif
