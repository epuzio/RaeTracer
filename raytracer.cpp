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

void setCameraParameters(camera& cam, json input){
    string rendermode = input["rendermode"].get<string>();
    int nbounces = (rendermode == "phong") ? input["nbounces"].get<int>() : 1;
    json cameraInput = input["camera"];
    cam = camera( //gpt for formatting
            nbounces,
            rendermode,
            cameraInput["width"].get<int>(),
            cameraInput["height"].get<int>(),
            vec3(
                cameraInput["position"][0].get<double>(),
                cameraInput["position"][1].get<double>(),
                cameraInput["position"][2].get<double>()
            ),
            vec3(
                cameraInput["lookAt"][0].get<double>(),
                cameraInput["lookAt"][1].get<double>(),
                cameraInput["lookAt"][2].get<double>()
            ),
            vec3(
                cameraInput["upVector"][0].get<double>(),
                cameraInput["upVector"][1].get<double>(),
                cameraInput["upVector"][2].get<double>()
            ),
            cameraInput["fov"].get<double>(),
            cameraInput["exposure"].get<double>()
        );
}

shared_ptr<material> setMaterialParameters(scene&world, json s){
    json materialInput = s["material"];
    shared_ptr<material> mat(
        materialInput["ks"].get<double>(),
        materialInput["kd"].get<double>(),
        materialInput["specularexponent"].get<double>(),
        color(
            materialInput["diffusecolor"][0].get<double>(),
            materialInput["diffusecolor"][1].get<double>(),
            materialInput["diffusecolor"][2].get<double>()
        ),
        color(
            materialInput["specularcolor"][0].get<double>(),
            materialInput["specularcolor"][1].get<double>(),
            materialInput["specularcolor"][2].get<double>()
        ),
        materialInput["isreflective"].get<bool>(),
        materialInput["reflectivity"].get<double>(),
        materialInput["isrefractive"].get<bool>(),
        materialInput["refractiveindex"].get<double>()
    );
    return mat;
}

void setWorldParameters(scene& world, json input){
    json sceneInput = input["scene"];
    json backgroundInput = sceneInput["backgroundcolor"];
    json shapesInput = sceneInput["shapes"];

    //Set background color from JSON input:
    world.backgroundcolor = color((backgroundInput[0]).get<double>(), backgroundInput[1].get<double>(), backgroundInput[2].get<double>());

    //Push shapes from JSON input: (and materials (to-do))
    for (const auto& s : shapesInput) {
        shared_ptr<material> mat = setMaterialParameters(world, s);
        if (s["type"] == "sphere"){
            world.add(make_shared<sphere>(
                point3(
                    s["center"][0].get<double>(),
                    s["center"][1].get<double>(),
                    s["center"][2].get<double>()
                ),
                s["radius"].get<double>()
            ));
        }
        if (s["type"] == "triangle"){
            world.add(make_shared<triangle>(
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
        if (s["type"] == "cylinder"){
            world.add(make_shared<cylinder>(
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
}

int main() {
    scene world;
    camera cam;

    //JSON input
    ifstream inputFile("binary_debug.json");
    json input;

    if (inputFile.is_open()) {
        try {
            inputFile >> input; // Parse JSON from file
            setCameraParameters(cam, input);
            setWorldParameters(world, input);

            // To-do: push back materials to shape, lights to world.
            
        } catch (json::parse_error& e) {
            cerr << "Parse error: " << e.what() << endl;
        }
    } else {
        cerr << "Unable to open json file." << endl;
    }
    cam.render(world);
};


    