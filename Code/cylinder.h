#ifndef CYLINDER_H
#define CYLINDER_H

#include "shape.h"
#include "vec3.h"
#include "interval.h"

class cylinder : public shape {
  public:
    cylinder(){}
    cylinder(point3 center, vec3 axis, double radius, double height, shared_ptr<material> bp)
        : center(center), axis(axis), radius(radius), height(height), bp(bp){}

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        vec3 oc = r.origin() - center;
        double a = dot(r.direction(), r.direction()) - dot(r.direction(), axis) * dot(r.direction(), axis);
        double b = 2.0 * (dot(r.direction(), oc) - dot(r.direction(), axis) * dot(oc, axis));
        double c = dot(oc, oc) - dot(oc, axis) * dot(oc, axis) - radius * radius;
        double discriminant = b * b - 4 * a * c;

        if (discriminant < 0)
            return false;

        double t = (-b - sqrt(discriminant)) / (2.0 * a);
        if (t < ray_t.min || t > ray_t.max)
            return false;

        point3 p = r.at(t);
        double projection = dot(p - center, axis);

        // Check intersection with the bottom cap circle
        if (projection < -height) {
            if (intersectCaps(r, ray_t, rec, false))
                return true;
        }

        // Check intersection with the top cap circle
        if (projection > height) {
            if (intersectCaps(r, ray_t, rec, true))
                return true;
        }

        // Check intersection with the cylindrical body
        if (projection >= -height && projection <= height) {
            vec3 toCenter = p - center; // Vector from the cylinder center to the intersection point
            // Project 'toCenter' onto the cylinder's axis to find the projection vector
            vec3 projectionOntoAxis = dot(toCenter, axis) * axis;
            // Calculate the normal vector by subtracting the projection vector from 'toCenter'
            vec3 perpendicularComponent = toCenter - projectionOntoAxis;
            // Calculate the outward normal using the perpendicular component
            vec3 outward_normal = normalize(perpendicularComponent);

            rec.set_face_normal(r, outward_normal);
            rec.t = t;
            rec.p = p;
            rec.bp = bp;
            if(bp->hastexture){
                rec.texturecoordinate = uvmap(rec.p, rec.bp->texture.size(), rec.bp->texture[0].size());
            }
            return true;
        }
        return false;
    }

    vec3 uvmap(const vec3& point, int textureWidth, int textureHeight) const { //all gpt
        vec3 toCenter = point - center;
        vec3 projectionOntoAxis = dot(toCenter, axis) * axis;
        vec3 perpendicularComponent = toCenter - projectionOntoAxis;
        
        // Calculate phi angle based on the circular cross-section around the cylinder
        double phi = atan2(perpendicularComponent.y, perpendicularComponent.x);
        if (phi < 0.0)
            phi += 2.0 * M_PI; // Ensure phi is in the range [0, 2π]

        // Map the phi angle to the range of texture coordinates (0, 1)
        double u = phi / (2.0 * M_PI);

        // Map the height of the point to the range of texture coordinates (0, 1)
        double v = (dot(toCenter, axis) + height) / (2.0 * height);

        // Map the normalized coordinates to the texture size
        double textureU = u * textureWidth;
        double textureV = v * textureHeight;

        return vec3(textureU, textureV, 0.0f);
    }

    vec3 uvmapCaps(vec3 point, point3 capCenter, int textureWidth, int textureHeight) const {
        // Calculate polar coordinates of the point
        double theta = atan2(point.y, point.x); // Angle from the x-axis
        double radius = sqrt(point.x * point.x + point.y * point.y); // Distance from the origin

        // Map the polar coordinates to the UV space
        double u = (theta + pi) / (2 * pi); // Normalize theta to [0, 1]
        double v = (point - capCenter).length() / (radius); // Normalize radius to [0, 1] based on circle's radius

        // Map normalized coordinates to texture space
        int texU = static_cast<int>(u * textureWidth) % textureWidth;
        int texV = static_cast<int>(v * textureHeight) % textureHeight;

        return vec3(texU, texV, 0);
    }

  private:
    point3 center; //center of the cylinder
    vec3 axis; //line where the cylinder lies
    double radius;
    double height;
    shared_ptr<material> bp;

    bool intersectCaps(const ray& r, interval ray_t, hit_record& rec, bool isTopCap) const {
        vec3 normal = isTopCap ? axis : -axis;
        point3 capCenter = isTopCap ? center + axis * height : center - axis * height;

        double t = dot((capCenter - r.origin()), (normal)) / dot(r.direction(), (normal));
        if (t < ray_t.min || t > ray_t.max)
            return false;

        point3 p = r.at(t);
        vec3 toCenter = p - capCenter;
        double distanceSquared = dot(toCenter, toCenter);
        if (distanceSquared <= radius * radius) {
            vec3 outward_normal = normalize(isTopCap ? axis : -axis);
            if(dot(r.direction(), outward_normal) < 0){ //i added this line
                rec.set_face_normal(r, outward_normal);
                rec.t = t;
                rec.p = p;
                rec.bp = bp;
                if(bp->hastexture){
                    rec.texturecoordinate = uvmapCaps(rec.p, capCenter, rec.bp->texture.size(), rec.bp->texture[0].size());
                }
                return true;
            }
        }

        return false;
    }
};

#endif