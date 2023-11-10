#include "raytracer.h"
#include "camera.h"
#include "color.h"
#include "shape.h"
#include "shapes_hit.h"
#include "sphere.h"

#include "interval.h"
#include "vec3.h"
#include "ray.h"

#include <iostream>
using namespace std;

int main() {
    shapes_hit world;

    world.add(make_shared<sphere>(point3(0, 0, -1), .5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

    camera camera;
    camera.aspect_ratio = 16.0/9.0;
    camera.image_width = 400;

    camera.render(world);
}