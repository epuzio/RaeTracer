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
                rec.texturecoordinate = uvmap(rec.p, rec.bp->texture.size(), rec.bp->texture[0].size());
              }
              return true;
          }
      }
      return false;
    }

     // Function to map a point on the sphere to UV coordinates
    vec3 uvmap(const vec3& point, int textureWidth, int textureHeight) const {
        int u = int((point.x + radius) / (2 * radius) * textureWidth) % textureWidth;
        int v = int((point.y + radius) / (2 * radius) * textureHeight) % textureHeight;

        return vec3(u, v, 0); // Return the color based on the mapped UV coordinates 
    }

  private:
    point3 center;
    double radius;
    shared_ptr<material> bp;
    // void getSphericalCoordinates(const vec3& point, double& phi, double& theta) const {
    //   theta = atan2(point.y, point.x);
    //   phi = acos(point.z / radius);
    //   cout << "phi " << phi << " theta " << theta << endl;
    // }
    
};

#endif
