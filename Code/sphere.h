#ifndef SPHERE_H
#define SPHERE_H

#include "shape.h"
#include "vec3.h"

class sphere : public shape {
  public:
    sphere(point3 c, double r, shared_ptr<material> bp) : center(c), radius(r), bp(bp) {}

   bool hit(const ray& r, double ray_min, double ray_max, hit_record& rec) const override {
      vec3 oc = r.origin() - center;
      auto a = r.direction().length_squared();
      auto half_b = dot(oc, r.direction());
      auto c = oc.length_squared() - radius * radius;
      auto discriminant = half_b * half_b - a * c;
      
      if (discriminant > 0) {
          auto root = sqrt(discriminant);
          auto temp = (-half_b - root) / a;
          if (temp < ray_max && temp > ray_min) {
              rec.t = temp;
              rec.p = r.at(rec.t);
              rec.set_face_normal(r, (rec.p - center) / radius);
              rec.bp = bp;
              if(bp->hastexture){
                rec.texturecoordinate = uvmap(rec.p, rec.bp->texture.size(), rec.bp->texture[0].size());
              }
              return true;
          }
      }
      return false;
    }

     // Function to map a point on the sphere to UV coordinates 
    vec3 uvmap(const vec3& point, int textureWidth, int textureHeight) const {
        // Calculate the sphere's local coordinates
        vec3 localCoord = point - center;

        // Calculate the spherical coordinates
        double theta = acos(localCoord.y / radius);
        double phi = atan2(localCoord.x, localCoord.z);

        // Map the spherical coordinates to the UV coordinates
        double u = (phi + M_PI) / (2 * M_PI);
        double v = 1 - theta / M_PI;

        // Map the UV coordinates to the texture coordinates
        int textureX = (int)(u * textureWidth);
        int textureY = (int)(v * textureHeight);

        // Return the texture coordinates
        return vec3(textureX, textureY, 0);
    }

  private:
    point3 center;
    double radius;
    shared_ptr<material> bp;
};

#endif
