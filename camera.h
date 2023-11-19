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
#include <fstream>
#include <cstdlib>

using namespace std;
std::ofstream outputFile("img.ppm"); //move this somewhere better

class camera {
  public:
    //world from JSON input:
    int nbounces;
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
    int numSamples = 1; //to prevent aliasing
    vec3 right, up, forward; //basis vectors for camera cameraPosition

    camera() {}
    camera(double b, string rm, int w, int h, const vec3& pos, const vec3& look, const vec3& upVec, double fieldOfView, double exp)
        : nbounces(b), rendermode(rm), width(w), height(h), cameraPosition(pos), lookAt(look), upVector(upVec), fov(fieldOfView), exposure(exp) {
        // Calculate basis vectors for camera orientation //copilot (modified)
        forward = normalize(lookAt - cameraPosition);
        right = normalize(cross(forward, upVector));
        up = normalize(cross(right,forward));
    }
   
   void render(const scene& world, int maxDepth) const {
        outputFile << "P3\n" << width << ' ' << height << "\n255\n"; 
        vec3 rays;

        // Calculate viewport dimensions //copilot
        double aspectRatio = static_cast<double>(width) / height;
        double fovRadians = (fov * M_PI) / 180.0;
        double viewportHeight = 2.0 * tan(fovRadians/2);
        double viewportWidth = aspectRatio * viewportHeight;

        //Calculate the vectors across the horizontal and vertical axes of the viewport //c
        vec3 horizontal = viewportWidth * -right;
        vec3 vertical = viewportHeight * -up;

        //Calculate the horizontal and vertical vectors pointing to the next pixel //c
        vec3 horizontalStep = horizontal / width;
        vec3 verticalStep = vertical / height;

        // Calculate location of upper left pixel (starting pixel)
        vec3 topLeftPixel = cameraPosition + forward - (horizontal / 2) - (vertical / 2);
       
        //RENDER LOOP
        clock_t c = clock();
        for (int y = 0; y < height; ++y) {
            clog << "\r[" << static_cast<int>((100.00*(y + 1) / height)) << "] percent complete" << flush;
            // calculate pixel color //copilot
            for (int x = 0; x < width; ++x) {
                vec3 pixelColor(0.0, 0.0, 0.0); //modified from color pixelColor = (0,0,0)
                for (int s = 0; s < numSamples; ++s) {
                    double jitterpx = (rand() / static_cast<double>(RAND_MAX)) - 0.5; // x offset for anti-aliasing
                    double jitterpy = (rand() / static_cast<double>(RAND_MAX)) - 0.5; // y offset for anti-aliasing

                    // Calculate sample position within the pixel
                    double u = (x + (jitterpx / 2.0)) / (width - 1);
                    double v = (y + (jitterpy / 2.0)) / (height - 1);

                    vec3 samplePixel = topLeftPixel + (u * horizontal) + (v * vertical);
                    vec3 rayDirection = samplePixel - cameraPosition;
                    ray r(cameraPosition, rayDirection);
                    pixelColor += rayColor(r, world, maxDepth);
                }
                writeColor(outputFile, pixelColor, numSamples);
            }
        }
        clog << "\rDone in " << (clock() - c) / CLOCKS_PER_SEC << " ms.       \n";
    }



//GET COLOR OF RAY
    //rayColor function //copilot
    color rayColor(const ray& r, const scene& world, int maxDepth) const {
        if (maxDepth <= 0) {
            return color(0,0,0);
        }

        hit_record rec;
        if(rendermode == "binary"){
            if(world.hit(r, interval(0, infinity), rec)) { //copilot autofill
                return color(1, 0, 0);
            }
        } 

        if(rendermode == "phong"){
            hit_record rec;
            if (world.hit(r, interval(0, infinity), rec)) {
                // if(maxDepth == 1){
                //     if(rec.bp->diffusecolor.x == 0.8 &&
                //         rec.bp->diffusecolor.y == 0.5 &&
                //         rec.bp->diffusecolor.z == 0.5){
                //             cout << "SPHERE HIT" << endl;
                //     }
                //     vec3 ambient = world.backgroundcolor * rec.bp->diffusecolor; // Ambient reflection
                //     vec3 pixelColor = clamp(ambient, 0.0, 1.0); 
                //     return color(pixelColor);
                // }
                
                //Ambient:
                vec3 ambient = world.backgroundcolor * rec.bp->diffusecolor; // Ambient reflection
                vec3 pixelColor = clamp(ambient, 0.0, 1.0); // Initialize with ambient light
                        
                // Iterate through each light source in the scene
                for (const auto& light : world.lights) {
                    // Shadow Calculation - don't calculate Diffuse or Specular if in shadow
                        // cout << "rn:  " << rec.normal << endl;
                        vec3 shadowRayOrigin = rec.p + (0.1 * rec.normal); //slight bias along the normal
                        cout << shadowRayOrigin << endl;
                        vec3 directionToLight = normalize(light->position - shadowRayOrigin);
                        ray shadowRay(shadowRayOrigin, directionToLight);


                        if(dot(rec.normal, directionToLight) > 0) {
                            hit_record shadowRec;
                            if (world.hit(shadowRay, interval(0.001, infinity), shadowRec)) {
                                if (shadowRec.t > 0 && shadowRec.t < (light->position - rec.p).length()) {
                                    // The hit point is in shadow, return an appropriate shadow color
                                    return color(0.3, 0.1, 0.2); // Adjust this color as needed
                                }
                            }//
                        }
                    
                    //Diffuse:
                    vec3 lightDir = normalize(light->position - rec.p);
                    double diffuseFactor = dot(rec.normal, lightDir); // Diffuse reflection
                    if (diffuseFactor > 0) {
                        // Calculate diffuse contribution
                        vec3 diffuse = light->intensity * rec.bp->diffusecolor * diffuseFactor * rec.bp->kd;
                        pixelColor += clamp(diffuse, 0.0, 1.0);            
                    }

                    //Specular:
                    vec3 viewDir = normalize(cameraPosition - rec.p);
                    // vec3 reflectDir = reflect(-lightDir, rec.normal); // Calculate reflection direction
                    vec3 halfway = normalize(lightDir + viewDir);
                    float specularIntensity = pow(max(0.0, dot(rec.normal, halfway)), rec.bp->specularexponent);
                    vec3 specular = light->intensity * rec.bp->specularcolor * specularIntensity * rec.bp->ks;
                    pixelColor += clamp(specular, 0.0, 1.0);   
                
                if (rec.bp->isreflective && rec.bp->reflectivity > 0.0f) {
                        vec3 reflectedDir = reflect(r.direction(), rec.normal);
                        // Create a reflection ray
                        ray reflectionRay(rec.p, reflectedDir);
                        // Trace the reflection ray recursively to get reflected color
                        color reflectedColor = rayColor(reflectionRay, world, maxDepth - 1);
                        // Combine the reflected color with the final color using the reflectivity factor
                        pixelColor += reflectedColor * rec.bp->reflectivity;
                    }
                }
                        
                // Ensure final pixel color is within the valid range [0, 1]
                pixelColor = clamp(pixelColor, 0.0, 1.0);
                return color(pixelColor);
            }
            return world.backgroundcolor; // If no object hit, return background color
        }
        return color(0,0,0);
    }
};


#endif