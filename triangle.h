#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "shape.h"
#include "vec3.h"
#include "ray.h"

class triangle : public shape {
  public:
    triangle(point3 v0, point3 v1, point3 v2, shared_ptr<material> bp) : v0(v0), v1(v1), v2(v2), bp(bp) {}

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
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

        if (t < ray_t.min || t > ray_t.max) {
            return false;
        }

        rec.t = t;
        rec.set_face_normal(r, normalize(cross(edge1, edge2)));
        rec.p = r.at(t);
        rec.bp = bp;

        return true;
    }

    vec3 uvmap(const vec3& point, int textureWidth, int textureHeight) {
        // Calculate barycentric coordinates
        vec3 edge1 = v1 - v0;
        vec3 edge2 = v2 - v0;
        vec3 normal = cross(edge1, edge2);

        double area = normal.length() / 2.0f;
        vec3 edgeVec1 = v1 - point;
        vec3 edgeVec2 = v2 - point;
        double alpha = cross(edge2, edgeVec2).length() / (2.0 * area);
        double beta = cross(edgeVec1, edge1).length() / (2.0 * area);
        double gamma = 1.0f - alpha - beta;

        // Calculate texture coordinates using barycentric interpolation
        double u = (alpha * v0.x + beta * v1.x + gamma * v2.x);
        double v = (alpha * v0.y + beta * v1.y + gamma * v2.y);

        // Normalize texture coordinates to fit within the texture image
        u /= textureWidth;
        v /= textureHeight;

        // Map the normalized coordinates to the texture size
        double textureU = u * textureWidth;
        double textureV = v * textureHeight;

        return vec3(textureU, textureV, 0.0);
    }

  private:
    point3 v0;
    point3 v1;
    point3 v2;
    shared_ptr<material> bp;
};

#endif