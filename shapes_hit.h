#ifndef SHAPES_HIT
#define SHAPES_HIT

#include "shape.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class shapes_hit : public shape {
  public:
    std::vector<shared_ptr<shape>> objects;

    shapes_hit() {}
    shapes_hit(shared_ptr<shape> object) { add(object); }

    void clear() { objects.clear(); }

    void add(shared_ptr<shape> object) {
        objects.push_back(object);
    }

    bool hit(const ray& r, interval ray_t, shape& rec) const override {
        shape temp_rec;
        bool hit_anything = false;
        auto closest_so_far = ray_t.max;

        for (const auto& object : objects) {
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