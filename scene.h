#ifndef SCENE_H
#define SCENE_H

#include "shape.h"
#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

//This class stores all the objects in a scene (lights and shapes) :P
class scene : public shape {
  public:
    vector<shared_ptr<shape> > objects;

    scene() {}
    scene(shared_ptr<shape> object) { add(object); }

    void clear() { objects.clear(); }

    void add(shared_ptr<shape> object) {
        objects.push_back(object);
    }

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        hit_record temp_rec;
        bool hit_anything = false;
        auto closest_so_far = ray_t.max;

        for (const auto& object : objects) { 
            //iterate through all objects in scene, set rec to be the object closest to camera
            if (object->hit(r, interval(ray_t.min, closest_so_far), temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }
        return hit_anything;
    }
};

#endif