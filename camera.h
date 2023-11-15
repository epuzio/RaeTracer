#ifndef CAMERA_H
#define CAMERA_H

#include "raytracer.h"

#include "color.h"
#include "shapes_hit.h"
#include "vec3.h"

#include <iostream>
#include <vector>
#include <ctime>
#include <cmath>

using namespace std;

class camera {

  public:
    //world from JSON input:
    double nbounces;
    double rendermode;
    //camera from json input
    string type; //it's always going to be pinhole?
    int width;
    int height;
    point3 cameraPosition; //camera cameraPosition as a point in x,y,z space
    vec3 lookAt; //vector pointing to spot camera is looking at
    vec3 upVector; //normalized y-axis vector [0, 1, 0]
    double fov;
    double exposure;
    
    //not from JSON input:
    int numSamples = 5; //to prevent aliasing
    vec3 right, up, forward; //basis vectors for camera cameraPosition

    Camera(double b, double rm, int w, int h, const vec3& pos, const vec3& look, const vec3& upVec, double fieldOfView, double exp)
        : nbounces(n), rendermode(rm), width(w), height(h), cameraPosition(pos), lookAt(look), upVector(upVec), fov(fieldOfView), exposure(exp) {
        // Calculate basis vectors for camera orientation
        forward = (lookAt - cameraPosition).normalized();
        right = forward.cross(upVector).normalized();
        up = right.cross(forward).normalized();
    }
   
   vec3 render(const shapes_hit& world) const {
        vec3 rays;
        // Calculate viewport dimensions //copilot
        double aspectRatio = static_cast<double>(width) / height;
        double viewportHeight = tan(fov * 0.5 * M_PI / 180.0) * 2.0;
        double viewportWidth = aspectRatio * viewportHeight;

        // Calculate basis vectors for camera orientation //copilot
        forward = (lookAt - cameraPosition).normalized();
        right = forward.cross(upVector).normalized();
        up = right.cross(forward).normalized();

        //Calculate the vectors across the horizontal and vertical axes of the viewport //c
        vec3 horizontal = viewportWidth * right;
        vec3 vertical = viewportHeight * up;

        //Calculate the horizontal and vertical vectors pointing to the next pixel //c
        vec3 horizontalStep = horizontal / width;
        vec3 verticalStep = vertical / height;

        // Calculate location of upper left pixel (starting pixel)
        topLeftPixel = (cameraPosition - (horizontal/2) - (vertical/2)) + (.5*(horizontalStep + verticalStep));

        //RENDER LOOP
        clock_t c = clock();
        for (int y = 0; y < height; ++y) {
            clog << "\r[" << (height - y) / y << "]'%' of lines complete" << flush;
            // calculate pixel color //copilot
            for (int x = 0; x < width; ++x) {
                color pixelColor = (0,0,0);
                for (int s = 0; s < numSamples; ++s) {
                    double u = (x + random_double()) * pixelWidth - halfWidth;
                    double v = (y + random_double()) * pixelHeight - halfHeight;
                    ray r(cameraPosition, topLeft + u * right - v * up - cameraPosition); //
                    pixelColor += rayColor(r, world);
                }
                writeColor(cout, pixelColor, numSamples);
            }

        }
        clog << "\rDone in " << (clock() - c) / CLOCKS_PER_SEC << " ms.       \n";
    }

//GET COLOR OF RAY
    //rayColor function //copilot
    color rayColor(const ray& r, const shapes_hit& world) const {
        if (nbounces <= 0) {
            return color(0,0,0);
        }
        if(rendermode == "binary"){
            return color(255, 0, 0);
        } 
        // else if (rendermode == "phong"){
        //     hit_record rec;
        //     if (world.hit(r, 0.001, infinity, rec)) {
        //         ray scattered;
        //         color attenuation;
        //         if (rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
        //             return attenuation * rayColor(scattered, world, nbounces - 1);
        //         }
        //         return color(0,0,0);
        //     }
        //     vec3 unit_direction = normalize(r.direction());
        //     double t = 0.5 * (unit_direction.y() + 1.0);
        //     return (1.0 - t) * color(1.0,1.0,1.0) + t * color(0.5,0.7,1.0);
        // }
    }
};

#endif