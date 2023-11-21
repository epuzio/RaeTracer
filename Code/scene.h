#ifndef SCENE_H
#define SCENE_H

#include "shape.h"
#include "light.h"
#include "material.h"
#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

//This class stores all the objects in a scene (lights, bkg color and shapes) :P
class scene : public shape {
  public:
    vector<shared_ptr<shape> > objects;
    vector<shared_ptr<pointlight> > lights;
    vec3 backgroundcolor;

    scene() {}
    scene(shared_ptr<shape> object) { add(object); }

    void clear() { objects.clear(); }

    void add(shared_ptr<shape> object) {
        objects.push_back(object);
    }

    void addLight(shared_ptr<pointlight> light) {
        lights.push_back(light);
    }

    bool hit(const ray& r, double ray_min, double ray_max, hit_record& rec) const override {
        hit_record temp_rec;
        bool hit_anything = false;
        auto closest_so_far = ray_max;

        for (const auto& object : objects) { 
            //iterate through all objects in scene, set rec to be the object closest to camera
            if (object->hit(r, ray_min, closest_so_far, temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }
        return hit_anything;
    }
};

#endif

