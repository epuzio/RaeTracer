#ifndef SPHERE_H
#define SPHERE_H

#include "shape.h"
#include "vec3.h"

class sphere : public shape {
  public:
    sphere(point3 c, double r, shared_ptr<material> bp) : center(c), radius(r), bp(bp) {}

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
      vec3 oc = r.origin() - center;
      auto a = r.direction().length_squared();
      auto half_b = dot(oc, r.direction());
      auto c = oc.length_squared() - radius * radius;
      auto discriminant = half_b * half_b - a * c;

      if (discriminant > 0) {
          auto root = sqrt(discriminant);
          auto temp = (-half_b - root) / a;
          if (temp < ray_t.max && temp > ray_t.min) {
              rec.t = temp;
              rec.p = r.at(rec.t);
              rec.set_face_normal(r, (rec.p - center) / radius);
              rec.bp = bp;
              if(bp->hastexture){
                rec.texturecoordinate = uvmap(rec.p, 2000, 2000);
              }
              return true;
          }
      }
      return false;
    }

    // Function to map a point on the sphere to UV coordinates
    vec3 uvmap(const vec3& point, int textureWidth, int textureHeight) {
        double phi, theta;
        getSphericalCoordinates(normalize(point - center), phi, theta);

        float u = theta / (2.0f * M_PI);
        float v = phi / M_PI;

        float textureU = u * textureWidth;
        float textureV = v * textureHeight;
        return vec3(textureU, textureV, 0.0);
    }

    void getSphericalCoordinates(const vec3& point, double& phi, double& theta) {
      phi = atan2(point.z, point.x);
      theta = acos(point.y / radius);
    }


  private:
    point3 center;
    double radius;
    shared_ptr<material> bp;
};

#endif
