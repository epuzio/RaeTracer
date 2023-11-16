#include "raytracer.h"
#include "camera.h"
#include "color.h"
#include "shape.h"
#include "scene.h"
#include "sphere.h"
#include "triangle.h"  
#include "cylinder.h"

#include "interval.h"
#include "vec3.h"
#include "ray.h"

#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
using namespace std;
using json = nlohmann::json;
using point3 = vec3;

int main() {
    scene world;
    camera camera;

    //JSON input
    ifstream inputFile("binary_sphere.json");
    json input;

    if (inputFile.is_open()) {
        try {
            inputFile >> input; // Parse JSON from file
            int nbounces = input["nbounces"];
            string rendermode = input["rendermode"];

            //Set camera variables from JSON input:
            json cameraInput = input["camera"];
            camera = camera( //gpt for formatting
                        nbounces,
                        rendermode,
                        cameraInput["width"].get<int>(),
                        cameraInput["height"].get<int>(),
                        vec3(
                            cameraInput["cameraPosition"][0].get<double>(),
                            cameraInput["cameraPosition"][1].get<double>(),
                            cameraInput["cameraPosition"][2].get<double>()
                        ),
                        vec3(
                            cameraInput["lookat"][0].get<double>(),
                            cameraInput["lookat"][1].get<double>(),
                            cameraInput["lookat"][2].get<double>()
                        ),
                        vec3(
                            cameraInput["upvector"][0].get<double>(),
                            cameraInput["upvector"][1].get<double>(),
                            cameraInput["upvector"][2].get<double>()
                        ),
                        cameraInput["fov"].get<double>(),
                        cameraInput["exposure"].get<double>()
                    );
            
            //Get scene variables from JSON input:
            json sceneInput = input["scene"];
            json backgroundInput = sceneInput["backgroundcolor"];
            json shapesInput = sceneInput["shapes"];
            json lightsInput = sceneInput["lightsources"];
            json materialsInput = sceneInput["materials"];
            
            //Set background color from JSON input:
            world.backgroundcolor = color((backgroundInput[0]).get<double>(), backgroundInput[1].get<double>(), backgroundInput[2].get<double>());

            //Push shapes from JSON input: (and materials (to-do))
            for (const auto& s : shapesInput) {
                if (s["type"] == "Sphere"){
                    world.add(sphere(
                        point3(
                            s["center"][0].get<double>(),
                            s["center"][1].get<double>(),
                            s["center"][2].get<double>()
                        ),
                        s["radius"].get<double>()
                    ));
                }
                if (s["type"] == "Triangle"){
                    world.add(triangle(
                        vec3(
                            s["v0"][0].get<double>(),
                            s["v0"][1].get<double>(),
                            s["v0"][2].get<double>()
                        ),
                        vec3(
                            s["v1"][0].get<double>(),
                            s["v1"][1].get<double>(),
                            s["v1"][2].get<double>()
                        ),
                        vec3(
                            s["v2"][0].get<double>(),
                            s["v2"][1].get<double>(),
                            s["v2"][2].get<double>()
                        )
                    ));
                }
                if (s["type"] == "Cylinder"){
                    world.add(cylinder(
                        point3(
                            s["center"][0].get<double>(),
                            s["center"][1].get<double>(),
                            s["center"][2].get<double>()
                        ),
                        vec3(
                            s["axis"][0].get<double>(),
                            s["axis"][1].get<double>(),
                            s["axis"][2].get<double>()
                        ),
                        s["radius"].get<double>(),
                        s["height"].get<double>()
                    ));
                }
            }
            //Push lights from JSON input (to-do):
            
        } catch (json::parse_error& e) {
            cerr << "Parse error: " << e.what() << endl;
        }
    } else {
        cerr << "Unable to open json file." << endl;
    }
    camera.render(world);
}
    