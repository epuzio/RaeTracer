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

    virtual ~material() = default;
    virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;
};

#endif

// class PhongShader {
// public:
//     PhongShader() {
//         // Default constructor
//     }

//     // Setters for material properties
//     void setAmbientColor(float r, float g, float b) {
//         ambientColor[0] = r;
//         ambientColor[1] = g;
//         ambientColor[2] = b;
//     }

//     void setDiffuseColor(float r, float g, float b) {
//         diffuseColor[0] = r;
//         diffuseColor[1] = g;
//         diffuseColor[2] = b;
//     }

//     void setSpecularColor(float r, float g, float b) {
//         specularColor[0] = r;
//         specularColor[1] = g;
//         specularColor[2] = b;
//     }

//     void setShininess(float shininess) {
//         this->shininess = shininess;
//     }

//     // Getters for material properties
//     float* getAmbientColor() {
//         return ambientColor;
//     }

//     float* getDiffuseColor() {
//         return diffuseColor;
//     }

//     float* getSpecularColor() {
//         return specularColor;
//     }

//     float getShininess() {
//         return shininess;
//     }

// private:
//     float ambientColor[3] = {0.2f, 0.2f, 0.2f};
//     float diffuseColor[3] = {0.8f, 0.8f, 0.8f};
//     float specularColor[3] = {1.0f, 1.0f, 1.0f};
//     float shininess = 32.0f;
// };
