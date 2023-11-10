#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

#include "color.h"
#include "shapes_hit.h"

#include <iostream>
using namespace std;

class camera {
  public:
    // Camera dimensions
    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
   
    void render(const hittable& world) {
        initialize();

        // Render
        cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        //Writing to output + progress bar
        for (int j = 0; j < image_height; ++j) {
            clog << "[" << ((j*1.0) / image_height) << "%]" << "\r REMAINING LINES: " << (image_height - j) << ' ' << flush;
            for (int i = 0; i < image_width; ++i) {
                auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
                auto ray_direction = pixel_center - camera_center;
                ray r(camera_center, ray_direction);

                color pixel_color = ray_color(r, world); 
                write_color(std::cout, pixel_color);
            }
        }
        clog << "\r[100%] DONE :~)                    \n" << flush;
    }

  private:
    /* Private Camera Variables Here */

    void initialize() {
        // Set height based on width and aspect ratio
        int image_height = static_cast<int>(image_width / aspect_ratio);
        if (image_height < 1){image_height = 1;} //make sure heignt is at least 1

        center = point3(0, 0, 0);

        auto focal_length = 1.0;
        auto viewport_height = 2.0;
        auto viewport_width = viewport_height * (static_cast<double>(image_width)/image_height);
        auto camera_center = point3(0, 0, 0);

        // Camera vectors from edge to edge
        auto viewport_u = vec3(viewport_width, 0, 0);
        auto viewport_v = vec3(0, -viewport_height, 0);

        auto pixel_delta_u = viewport_u / image_width;
        auto pixel_delta_v = viewport_v / image_height;

        auto viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
        auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    }

    color ray_color(const ray& r, const hittable& world) const {
        hit_record rec;
        if (world.hit(r, interval(0, infinity), rec){
            return 0.5*(rec.normal+color(1,1,1));
        }) 

        vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5*(unit_direction.y() + 1.0);
        return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
    }
};

#endif