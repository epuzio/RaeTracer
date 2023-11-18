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
    int numSamples = 2; //to prevent aliasing
    vec3 right, up, forward; //basis vectors for camera cameraPosition

    camera() {}
    camera(double b, string rm, int w, int h, const vec3& pos, const vec3& look, const vec3& upVec, double fieldOfView, double exp)
        : nbounces(b), rendermode(rm), width(w), height(h), cameraPosition(pos), lookAt(look), upVector(upVec), fov(fieldOfView), exposure(exp) {
        // Calculate basis vectors for camera orientation //copilot (modified)
        forward = normalize(lookAt - cameraPosition);
        right = normalize(cross(forward, upVector));
        up = normalize(cross(right,forward));
    }
   
   void render(const scene& world) const {
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
        vec3 topLeftPixel = (cameraPosition + (forward * (cameraPosition - lookAt).length()) - (horizontal/2) - (vertical/2)) + (.5*(horizontalStep + verticalStep));
       
        //RENDER LOOP
        clock_t c = clock();
        for (int y = 0; y < height; ++y) {
            clog << "\r[" << static_cast<int>((100.00*(y + 1) / height)) << "] percent complete" << flush;
            // calculate pixel color //copilot
            for (int x = 0; x < width; ++x) {
                vec3 pixelColor(0.0, 0.0, 0.0); //modified from color pixelColor = (0,0,0)
                for (int s = 0; s < numSamples; ++s) {
                    double jitterpx = (rand() / (RAND_MAX)) - 0.5; //x offset for anti-aliasing
                    double jitterpy = (rand() / (RAND_MAX)) - 0.5; //y offset for anti-aliasing
                    vec3 pixelCenter = (topLeftPixel + (x * horizontalStep) + (y * verticalStep) - cameraPosition);
                    vec3 rayDirection = pixelCenter - (jitterpx * horizontalStep * .5) + (jitterpy * verticalStep * .5);
                    ray r(cameraPosition, rayDirection); //ray r normalizes direction
                    pixelColor += rayColor(r, world);
                }
                writeColor(outputFile, pixelColor, numSamples);
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
        if(rendermode == "binary"){
            if(world.hit(r, interval(0, infinity), rec)) { //copilot autofill
                return color(1, 0, 0);
            }
        } 

        if(rendermode == "phong"){
            hit_record rec;
            if (world.hit(r, interval(0, infinity), rec)) {
                //Ambient:
                vec3 normal = normalize(rec.normal);
                vec3 ambient = world.backgroundcolor * rec.bp->diffusecolor; // Ambient reflection
                vec3 pixelColor = clamp(ambient, 0.0, 1.0); // Initialize with ambient light
                        
                // Iterate through each light source in the scene
                for (const auto& light : world.lights) {
                    //Diffuse:
                    vec3 lightDir = normalize(light->position - rec.p);
                    double diffuseFactor = dot(normal, lightDir); // Diffuse reflection
                    
                    if (diffuseFactor > 0) {
                        // Calculate diffuse contribution
                        vec3 diffuse = light->intensity * rec.bp->diffusecolor * diffuseFactor * rec.bp->kd;
                        pixelColor += clamp(diffuse, 0.0, 1.0);
                        
                        vec3 viewDir = normalize(cameraPosition - rec.p);
                        vec3 reflectDir = reflect(-lightDir, normal); // Calculate reflection direction
                        
                        // Calculate specular contribution using the Phong equation
                        double specularFactor = dot(viewDir, reflectDir);
                        if (specularFactor > 0) {
                            specularFactor = pow(specularFactor, rec.bp->specularexponent);
                            vec3 specular = light->intensity * rec.bp->specularcolor * specularFactor * rec.bp->ks;
                            pixelColor += clamp(specular, 0.0, 1.0);
                        }                       
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





// //original gpt code for phong
// if (world.hit(r, interval(0, infinity), rec)) {
//                 vec3 normal = unit_vector(rec.normal); // Ensure the normal is unit length
//                 vec3 surfaceColor = world.colorAt(rec); // Get surface color from the object hit
                
//                 vec3 ambient = world.ambientColor * surfaceColor; // Ambient reflection
                
//                 vec3 pixelColor = ambient; // Initialize with ambient light
                        
//                         // Iterate through each light source in the scene
//                 for (const auto& light : world.lights) {
//                     vec3 lightDir = unit_vector(light.position - rec.p);
//                     double diffuseFactor = dot(normal, lightDir); // Diffuse reflection
                    
//                     if (diffuseFactor > 0) {
//                             // Calculate diffuse contribution
//                         vec3 diffuse = light.intensity * surfaceColor * diffuseFactor;
//                         pixelColor += diffuse;
                        
//                         vec3 viewDir = unit_vector(cameraPosition - rec.p);
//                         vec3 reflectDir = reflect(-lightDir, normal); // Calculate reflection direction
                        
//                         // Calculate specular contribution using the Phong equation
//                         double specularFactor = dot(viewDir, reflectDir);
//                         if (specularFactor > 0) {
//                             specularFactor = pow(specularFactor, world.shininess);
//                             vec3 specular = light.intensity * world.specularColor * specularFactor;
//                             pixelColor += specular;
//                         }                       
//                     }
//                 }
                        
//                 // Ensure final pixel color is within the valid range [0, 1]
//                 pixelColor = clamp(pixelColor, 0.0, 1.0);
//                 return color(pixelColor);
//             }
//             return world.backgroundcolor; // If no object hit, return background color
//             }