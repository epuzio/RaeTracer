#ifndef MATERIAL_H
#define MATERIAL_H 

#include "raytracer.h"
#include "scene.h"
#include "shape.h"
#include "vec3.h"
#include "color.h"
#include "ray.h"
#include "interval.h"

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
    bool hastexture = false;
    vector<vec3> image;
    vec3 texturedimensions = vec3(0, 0, 0);

    material() {}
    material(double ks, double kd, double specularexponent, color diffusecolor, color specularcolor, bool isreflective, double reflectivity, bool isrefractive, double refractiveindex)
        : ks(ks), kd(kd), specularexponent(specularexponent), diffusecolor(diffusecolor), specularcolor(specularcolor), isreflective(isreflective), reflectivity(reflectivity), isrefractive(isrefractive), refractiveindex(refractiveindex) {}
    material(double ks, double kd, double specularexponent, color diffusecolor, color specularcolor, bool isreflective, double reflectivity, bool isrefractive, double refractiveindex, bool hastexture, vector<vec3> image, vec3 texturedimensions)
        : ks(ks), kd(kd), specularexponent(specularexponent), diffusecolor(diffusecolor), specularcolor(specularcolor), isreflective(isreflective), reflectivity(reflectivity), isrefractive(isrefractive), refractiveindex(refractiveindex), hastexture(hastexture), image(image), texturedimensions(texturedimensions) {}
    ~material() = default;
};

#endif
