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

    vec3 uvmap(const vec3& point, int textureWidth, int textureHeight) const { //all copilot
        // Calculate the triangle's local coordinates
        vec3 localCoord = point - v0;

        // Calculate the triangle's normal
        vec3 normal = normalize(cross(v1 - v0, v2 - v0));

        // Calculate the triangle's tangent
        vec3 tangent = normalize(v1 - v0);

        // Calculate the triangle's bitangent
        vec3 bitangent = normalize(cross(normal, tangent));

        // Calculate the triangle's UV coordinates
        double u = dot(localCoord, tangent);
        double v = dot(localCoord, bitangent);

        // Map the UV coordinates to the texture coordinates
        int textureX = (int)(u * textureWidth) % textureWidth;
        int textureY = (int)(v * textureHeight) % textureHeight;

        // Return the texture coordinates
        return vec3(textureX, textureY, 0);
  }
  
  vec3 triangleMin(){
    return vec3(vec3::min(vec3::min(v0, v1), v2));
  }

  vec3 triangleMax(){
    return vec3(vec3::max(vec3::max(v0, v1), v2));
  }

  vec3 centroid() const { //added by copilot
    return (v0 + v1 + v2) / 3;
  }

  private:
    point3 v0;
    point3 v1;
    point3 v2;
    shared_ptr<material> bp;
};

#endif