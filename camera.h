#ifndef CAMERA_H
#define CAMERA_H

#include "raytracer.h"

#include "color.h"
#include "scene.h"
#include "vec3.h"

#include <iostream>
#include <vector>
#include <ctime>
#include <cmath>
#include <cstdlib>

using namespace std;

class camera {

  public:
    //world from JSON input:
    double nbounces;
    string rendermode;
    //camera from json input
    string type; //it's always going to be pinhole?
    int width;
    int height;
    point3 cameraPosition; //camera cameraPosition as a point in x,y,z space
    vec3 lookAt; //vector pointing to spot camera is looking at
    vec3 upVector; //normalize y-axis vector [0, 1, 0]
    double fov;
    double exposure;
    
    //not from JSON input:
    int numSamples = 3; //to prevent aliasing
    vec3 right, up, forward; //basis vectors for camera cameraPosition

    camera(double b, string rm, int w, int h, const vec3& pos, const vec3& look, const vec3& upVec, double fieldOfView, double exp)
        : nbounces(b), rendermode(rm), width(w), height(h), cameraPosition(pos), lookAt(look), upVector(upVec), fov(fieldOfView), exposure(exp) {
        // Calculate basis vectors for camera orientation //copilot (modified)
        forward = normalize(lookAt - cameraPosition);
        right = normalize(cross(forward, upVector));
        up = normalize(cross(right,forward));
    }
   
   vec3 render(const scene& world) const {
        vec3 rays;
        // Calculate viewport dimensions //copilot
        double aspectRatio = static_cast<double>(width) / height;
        double viewportHeight = tan(fov * 0.5 * M_PI / 180.0) * 2.0;
        double viewportWidth = aspectRatio * viewportHeight;

        //Calculate the vectors across the horizontal and vertical axes of the viewport //c
        vec3 horizontal = viewportWidth * right;
        vec3 vertical = viewportHeight * up;

        //Calculate the horizontal and vertical vectors pointing to the next pixel //c
        vec3 horizontalStep = horizontal / width;
        vec3 verticalStep = vertical / height;

        // Calculate location of upper left pixel (starting pixel)
        vec3 topLeftPixel = (cameraPosition - (horizontal/2) - (vertical/2)) + (.5*(horizontalStep + verticalStep));

        //RENDER LOOP
        clock_t c = clock();
        for (int y = 0; y < height; ++y) {
            clog << "\r[" << (height - y) / y << "]'%' of lines complete" << flush;
            // calculate pixel color //copilot
            for (int x = 0; x < width; ++x) {
                vec3 pixelColor(0.0, 0.0, 0.0); //modified from color pixelColor = (0,0,0)
                for (int s = 0; s < numSamples; ++s) {
                    vec3 pixelCenter = topLeftPixel + (x * horizontalStep) + (y * verticalStep);
                    double jitterpx = (rand() / (RAND_MAX)) - 0.5; //x offset for anti-aliasing
                    double jitterpy = (rand() / (RAND_MAX)) - 0.5; //y offset for anti-aliasing
                    vec3 rayDirection = pixelCenter - (jitterpx * horizontalStep * .5) + (jitterpy * verticalStep * .5);
                    ray r(cameraPosition, rayDirection);
                    pixelColor += rayColor(r, world);
                }
                writeColor(cout, pixelColor, numSamples); //todo: write to image file
            }
        }
        clog << "\rDone in " << (clock() - c) / CLOCKS_PER_SEC << " ms.       \n";
    }

//GET COLOR OF RAY
    //rayColor function //copilot
    color rayColor(const ray& r, const scene& world) const {
        if (nbounces <= 0) {
            return color(0,0,0);
        }

        hit_record rec;
        if(world.hit(r, interval(0, infinity), rec)) { //copilot autofill
        //rec is now eqivalent to closest object hit by ray btw
            if(rendermode == "binary"){
                return color(255, 0, 0);
            } 
            else if (rendermode == "phong"){
                cout << "Phong!" << endl;
                // return phongShade(r, rec);
            }
        }
    }

    // vec3 phongShade(const ray& ray, const scene& world) {
    //     vec3 ambientColor = {0.1f, 0.1f, 0.1f}; // Ambient light color
    //     vec3 lightDirection = {-1.0f, 1.0f, -1.0f}; // Example light direction

    //     vec3 finalColor = {0.0f, 0.0f, 0.0f};

    //     Object closestObject;
    //     if (scene.hit(ray, closestObject)) {
    //         vec3 normal = {...}; // Calculate surface normal at the intersection point

    //         // Ambient light contribution
    //         vec3 ambientContribution = {
    //             ambientColor.x * closestObject.material.ambientColor.x,
    //             ambientColor.y * closestObject.material.ambientColor.y,
    //             ambientColor.z * closestObject.material.ambientColor.z
    //         };

    //         // Diffuse light contribution
    //         float cosTheta = std::max(0.0f, normal.x * lightDirection.x + normal.y * lightDirection.y + normal.z * lightDirection.z);
    //         vec3 diffuseContribution = {
    //             cosTheta * closestObject.material.diffuseColor.x,
    //             cosTheta * closestObject.material.diffuseColor.y,
    //             cosTheta * closestObject.material.diffuseColor.z
    //         };

    //         // Specular light contribution
    //         vec3 viewDirection = {...}; // Calculate view direction
    //         vec3 reflectionDirection = {...}; // Calculate reflection direction
    //         float cosAlpha = std::max(0.0f, reflectionDirection.x * lightDirection.x + reflectionDirection.y * lightDirection.y + reflectionDirection.z * lightDirection.z);
    //         float specularIntensity = pow(cosAlpha, closestObject.material.shininess);
    //         vec3 specularContribution = {
    //             specularIntensity * closestObject.material.specularColor.x,
    //             specularIntensity * closestObject.material.specularColor.y,
    //             specularIntensity * closestObject.material.specularColor.z
    //         };

    //         // Final color calculation
    //         finalColor.x = ambientContribution.x + diffuseContribution.x + specularContribution.x;
    //         finalColor.y = ambientContribution.y + diffuseContribution.y + specularContribution.y;
    //         finalColor.z = ambientContribution.z + diffuseContribution.z + specularContribution.z;
    //     }
    // return finalColor;
    // }
};

#endif













// //initial copilot output
// clock_t c = clock();
//         for (int y = 0; y < height; ++y) {
//             clog << "\r[" << (height - y) / y << "]'%' of lines complete" << flush;
//             // calculate pixel color //copilot
//             for (int x = 0; x < width; ++x) {
//                 vec3 pixelColor(0.0, 0.0, 0.0); //modified from color pixelColor = (0,0,0)
//                 for (int s = 0; s < numSamples; ++s) {
//                     double u = (x + random_double()) * pixelWidth - halfWidth;
//                     double v = (y + random_double()) * pixelHeight - halfHeight;
//                     ray r(cameraPosition, topLeft + u * right - v * up - cameraPosition); 
//                     pixelColor += rayColor(r, world);
//                 }
//                 writeColor(cout, pixelColor, numSamples); //todo: write to image file
//             }

//         }
//         clog << "\rDone in " << (clock() - c) / CLOCKS_PER_SEC << " ms.       \n";
//     }