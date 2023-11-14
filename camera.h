#ifndef CAMERA_H
#define CAMERA_H

#include "raytracer.h"

#include "color.h"
#include "shapes_hit.h"
#include "vec3.h"

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

class camera {
    private: 
        //set from JSON file in main:
        int width;
        int height;
        point3 position; //camera position as a point in x,y,z space
        vec3 lookAt; //vector pointing to spot camera is looking at
        vec3 upVector; //normalized y-axis vector [0, 1, 0]
        vec3 right, up, forward; //basis vectors for camera position
        double fov;
        double exposure;   

  public:
  Camera(int w, int h, const vec3& pos, const vec3& look, const vec3& upVec, double fieldOfView, double exp)
        : width(w), height(h), position(pos), lookAt(look), upVector(upVec), fov(fieldOfView), exposure(exp) {
        
        // Calculate basis vectors for camera orientation
        forward = (lookAt - position).normalized();
        right = forward.cross(upVector).normalized();
        up = right.cross(forward).normalized();
    }

    // Camera dimensions
    double aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    int samplesPerPixel;
   
   vec3 generateRays() const {
        vec3 rays;

        double aspectRatio = static_cast<double>(width) / height;
        double halfHeight = tan(fov * 0.5 * M_PI / 180.0);
        double halfWidth = aspectRatio * halfHeight;

        vec3 topLeft = position + forward - halfWidth * right + halfHeight * up;
        double pixelWidth = 2.0 * halfWidth / width;
        double pixelHeight = 2.0 * halfHeight / height;

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                double u = (x + 0.5) * pixelWidth - halfWidth;
                double v = (y + 0.5) * pixelHeight - halfHeight;

                vec3 rayDirection = (topLeft + u * right - v * up) - position;
                rays.push_back(normalize(rayDirection));
            }
        }

        return rays;
    }
};

#endif