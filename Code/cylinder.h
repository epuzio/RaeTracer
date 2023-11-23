#ifndef CYLINDER_H
#define CYLINDER_H

#include "shape.h"
#include "vec3.h"

class cylinder : public shape {
  public:
    cylinder(){}
    cylinder(point3 center, vec3 axis, double radius, double height, shared_ptr<material> bp)
        : center(center), axis(axis), radius(radius), height(height), bp(bp){}

    bool hit(const ray& r, double ray_min, double ray_max, hit_record& rec) const override {
        vec3 oc = r.origin() - center;
        double a = dot(r.direction(), r.direction()) - dot(r.direction(), axis) * dot(r.direction(), axis);
        double b = 2.0 * (dot(r.direction(), oc) - dot(r.direction(), axis) * dot(oc, axis));
        double c = dot(oc, oc) - dot(oc, axis) * dot(oc, axis) - radius * radius;
        double discriminant = b * b - 4 * a * c;

        if (discriminant < 0)
            return false;

        double t = (-b - sqrt(discriminant)) / (2.0 * a);
        if (t < ray_min || t > ray_max)
            return false;

        point3 p = r.at(t);
        double projection = dot(p - center, axis);

        // Check intersection with the cylindrical body
        if (projection >= -height && projection <= height) {
            vec3 diff = p - center - projection * axis;
            vec3 outward_normal = normalize(diff - dot(diff, axis) * axis / (height * height));
            rec.set_face_normal(r, outward_normal);
            rec.t = t;
            rec.p = p;
            rec.bp = bp;
            return true;
        }

        // Check intersection with the bottom cap circle
        if (projection < -height) {
            if (intersectCaps(r, ray_min, ray_max, rec, false))
                return true;
        }

        // Check intersection with the top cap circle
        if (projection > height) {
            if (intersectCaps(r, ray_min, ray_max, rec, true))
                return true;
        }

        
                    
        return false;
    }

    // Function to map a point on the cylinder body to UV coordinates
    vec3 uvmap(const vec3& point, int textureWidth, int textureHeight) const {
        // Calculate the cylinder's local coordinates
        vec3 localCoord = point - center;

        // Calculate the cylinder's normal
        vec3 normal = normalize(localCoord - dot(localCoord, axis) * axis);

        // Calculate the cylinder's polar coordinates
        double theta = atan2(normal.y, normal.x); // Angle from the x-axis
        double radius = sqrt(localCoord.x * localCoord.x + localCoord.y * localCoord.y); // Distance from the origin

        // Map the polar coordinates to the UV space
        double u = (theta + pi) / (2 * pi); // Normalize theta to [0, 1]
        double v = (localCoord.z + height) / (2 * height); // Normalize radius to [0, 1] based on cylinder's height

        // Map normalized coordinates to texture space
        int texU = static_cast<int>(u * textureWidth) % textureWidth;
        int texV = static_cast<int>(v * textureHeight) % textureHeight;

        return vec3(texU, texV, 0);
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

    bool intersectCaps(const ray& r, double ray_min, double ray_max, hit_record& rec, bool isTopCap) const {
        vec3 normal = isTopCap ? axis : -axis;
        point3 capCenter = isTopCap ? center + axis * height : center - axis * height;

        // Check if the ray is parallel to the cap
        double t = dot((capCenter - r.origin()), (normal)) / dot(r.direction(), (normal));
        if (t < ray_min || t > ray_max)
            return false;

        // Check if the intersection point is within the cap
        point3 p = r.at(t);
        vec3 toCenter = p - capCenter;
        double distanceSquared = dot(toCenter, toCenter);
        if (distanceSquared <= radius * radius) {
            vec3 outward_normal = normalize(isTopCap ? axis : -axis); // Calculate the outward normal based on if the cap is top or bottom
            if(dot(r.direction(), outward_normal) < 0){ //if the ray is pointing towards the cap
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