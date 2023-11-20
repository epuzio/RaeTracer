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
    int numSamples = 3; //to prevent aliasing
    double bias = .001; //to prevent shadow acne
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
                writeColor(outputFile, pixelColor, maxDepth);
            }
        }
        clog << "\rDone in " << (clock() - c) / CLOCKS_PER_SEC << " ms.       \n";
    }



    //GET COLOR OF RAY
    //rayColor function //copilot
    color rayColor(const ray& r, const scene& world, int maxDepth) const {
        if (maxDepth <= 0) {
            return color(0,0,0);
        } else{
            hit_record rec;
            if(rendermode == "binary"){
                if(world.hit(r, interval(0, infinity), rec)) { //copilot autofill
                    return color(1, 0, 0);
                }
            } 

            if(rendermode == "phong"){
                hit_record rec;
                if (world.hit(r, interval(0, infinity), rec)) {
                    //Calculate local contribution based on the material's reflectivity
                    double localContribution = (rec.bp->isreflective) ? 1.0 - rec.bp->reflectivity : 1.0; 

                    //Ambient:
                    vec3 ambient = 0.5 * rec.bp->diffusecolor; // Ambient reflection
                    vec3 pixelColor = clamp(localContribution*ambient, 0.0, 1.0); // Initialize with ambient light
                            
                    // Iterate through each light source in the scene
                    for (const auto& light : world.lights) {
                        //Reflective
                        if (rec.bp->isreflective && rec.bp->reflectivity > 0.0f) {
                            vec3 reflectedDir = reflect(r.direction(), rec.normal); 
                            vec3 reflectionRayOrigin = rec.p + (rec.normal * bias); //slight bias along the normal
                            ray reflectionRay(reflectionRayOrigin, reflectedDir);
                            // Trace reflection and transmission rays recursively to get colors
                            color reflectedColor = rayColor(reflectionRay, world, maxDepth - 1);
                            pixelColor += clamp((reflectedColor * rec.bp->reflectivity), 0.0, 1.0);
                        }

                        //Refractive
                        if(rec.bp->isrefractive && rec.bp->refractiveindex > 0.0f){
                            vec3 refractedDir = snell(r.direction(), rec.normal, rec.bp->refractiveindex);
                            vec3 transmissionRayOrigin = rec.p + (rec.normal * bias); //slight bias along the normal
                            ray transmissionRay(transmissionRayOrigin, refractedDir);
                            double transmittance = 1.0 - rec.bp->reflectivity;
                            color transmittedColor = rayColor(transmissionRay, world, maxDepth - 1);
                            pixelColor += clamp((transmittedColor * transmittance), 0.0, 1.0);
                        }

                        // Shadow Calculation - don't calculate Diffuse or Specular if in shadow
                        vec3 lightDir = normalize(light->position - rec.p);
                        vec3 shadowRayOrigin = rec.p + (rec.normal * bias); //slight bias along the normal
                        vec3 directionToLight = normalize(light->position - shadowRayOrigin);
                        ray shadowRay(shadowRayOrigin, directionToLight);
                        if(dot(rec.normal, directionToLight) > 0) {
                            hit_record shadowRec;
                            if (world.hit(shadowRay, interval(0, infinity), shadowRec)) {
                                if (shadowRec.t < (light->position - rec.p).length()) {
                                    // The hit point is in shadow, return an appropriate shadow color
                                    return color(pixelColor); 
                                }
                            }//
                        }
                        
                        //Diffuse:
                        double diffuseFactor = dot(rec.normal, lightDir); // Diffuse reflection
                        if (diffuseFactor > 0) {
                            // Calculate diffuse contribution
                            vec3 diffuse = light->intensity * rec.bp->diffusecolor * diffuseFactor * rec.bp->kd;
                            pixelColor += clamp(localContribution*diffuse, 0.0, 1.0);            
                        }

                        //Specular:
                        vec3 viewDir = normalize(cameraPosition - rec.p);
                        vec3 halfway = normalize(lightDir + viewDir);
                        float specularIntensity = pow(max(0.0, dot(rec.normal, halfway)), rec.bp->specularexponent);
                        vec3 specular = light->intensity * rec.bp->specularcolor * (specularIntensity*.1) * rec.bp->ks;
                        float normalLightHalfway = dot(rec.normal, halfway);
                        if (normalLightHalfway > 0.0) {
                            // Calculate the specular contribution and add it to the pixel color
                            pixelColor += clamp(localContribution * specular, 0.0, 1.0);
                        }

                    }
                            
                    // Ensure final pixel color is within the valid range [0, 1]
                    pixelColor = clamp(pixelColor, 0.0, 1.0);
                    return color(pixelColor);
                }
                return world.backgroundcolor; // If no object hit, return background color
            }
        }
        throw std::runtime_error("Error: rendermode must match either binary or phong");
        return color(0, 0, 0);
    }
};


#endif