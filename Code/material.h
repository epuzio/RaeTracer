#ifndef MATERIAL_H
#define MATERIAL_H 
#include <vector>
#include <memory>

#include "raytracer.h"
#include "scene.h"
#include "shape.h"
#include "vec3.h"
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
    vector<vector<vec3>> texture;

    material() {}
    material(double ks, double kd, double specularexponent, color diffusecolor, color specularcolor, bool isreflective, double reflectivity, bool isrefractive, double refractiveindex)
        : ks(ks), kd(kd), specularexponent(specularexponent), diffusecolor(diffusecolor), specularcolor(specularcolor), isreflective(isreflective), reflectivity(reflectivity), isrefractive(isrefractive), refractiveindex(refractiveindex) {}
    material(double ks, double kd, double specularexponent, color diffusecolor, color specularcolor, bool isreflective, double reflectivity, bool isrefractive, double refractiveindex, bool hastexture, vector<vector<vec3>> texture)
        : ks(ks), kd(kd), specularexponent(specularexponent), diffusecolor(diffusecolor), specularcolor(specularcolor), isreflective(isreflective), reflectivity(reflectivity), isrefractive(isrefractive), refractiveindex(refractiveindex), hastexture(hastexture), texture(texture) {}
    ~material() = default;
};

#endif
