#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "shape.h"
#include "vec3.h"
#include "ray.h"

class triangle : public shape {
  public:
    triangle(point3 v0, point3 v1, point3 v2, shared_ptr<material> bp) : v0(v0), v1(v1), v2(v2), bp(bp) {}

    bool hit(const ray& r, double ray_min, double ray_max, hit_record& rec) const override {
      //using Watertight Ray/Triangle Intersection:

        vec3 edge1 = v1 - v0;
        vec3 edge2 = v2 - v0;

        vec3 pvec = cross(r.direction(), edge2);
        float det = dot(edge1, pvec);

        if (det > -.001 && det < .001) {
            return false;
        }

        float inv_det = 1.0f / det;
        vec3 tvec = r.origin() - v0;
        float u = dot(tvec, pvec) * inv_det;

        if (u < 0.0f || u > 1.0f) {
            return false;
        }

        vec3 qvec = cross(tvec, edge1);
        float v = dot(r.direction(), qvec) * inv_det;

        if (v < 0.0f || u + v > 1.0f) {
            return false;
        }

        float t = dot(edge2, qvec) * inv_det;

        if (t < ray_min || t > ray_max) {
            return false;
        }

        rec.t = t;
        rec.set_face_normal(r, normalize(cross(edge1, edge2)));
        rec.p = r.at(t);
        rec.bp = bp;
        if(bp->hastexture){
          rec.texturecoordinate = uvmap(rec.p, rec.bp->texture.size(), rec.bp->texture[0].size());
        }

        return true;
    }

    vec3 uvmap(const vec3& point, int textureWidth, int textureHeight) const { //all gpt
        // Calculate the vectors representing the edges of the triangle
      vec3 edge1 = v1 - v0;
      vec3 edge2 = v2 - v0;

      // Calculate the normal of the triangle
      vec3 normal = normalize(cross(edge1, edge2));

      // Calculate the vectors from v0 to the given point
      vec3 diff = point - v0;

      // Calculate the dot products to determine the u and v coordinates
      float u = dot(diff, edge1);
      float v = dot(diff, edge2);

      // Normalize u and v
      u /= (edge1).length();
      v /= (edge2).length();

      // Map u and v to the range [0, 1]
      u = (u + 1.0f) / 2.0f;
      v = (v + 1.0f) / 2.0f;

      // Convert u and v to pixel coordinates based on texture size
      int pixelU = static_cast<int>(u * textureWidth) % textureWidth;
      int pixelV = static_cast<int>(v * textureHeight) % textureHeight;
      return vec3(static_cast<float>(pixelU), static_cast<float>(pixelV), 0.0f);
  }

  private:
    point3 v0;
    point3 v1;
    point3 v2;
    shared_ptr<material> bp;
};

#endif