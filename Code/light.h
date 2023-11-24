#ifndef LIGHT_H
#define LIGHT_H 

#include "scene.h"
#include "vec3.h"

class pointlight { 
    public:
        vec3 position;
        vec3 intensity;
        pointlight(const vec3& pos, const vec3& intensity) : position(pos), intensity(intensity) {}
        vec3 getDirection(const vec3& point) const {
            return normalize((position - point));
        }

        float getDistance(const vec3& point) const {
            return (position - point).length();
        }
    };

#endif