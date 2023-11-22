#include "raytracer.h"
#include "camera.h"
#include "shape.h"
#include "scene.h"
#include "sphere.h"
#include "triangle.h"  
#include "cylinder.h"
#include "material.h"
#include "light.h"
#include "box.h"

#include "vec3.h"
#include "ray.h"

#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
using json = nlohmann::json;
using point3 = vec3;

bool readPPM(const char* filename, std::vector<std::vector<vec3>>& image) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return false;
    }

    std::string format;
    int width, height, maxColorValue;
    file >> format >> width >> height >> maxColorValue;

    if (format != "P3" || maxColorValue != 255) {
        std::cerr << "Invalid or unsupported PPM format." << std::endl;
        return false;
    }

    image.resize(height);
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int r, g, b;
            file >> r >> g >> b;
            image[i].push_back(vec3(r, g, b) / 255.0); // Normalize to range [0, 1]
        }
    }

    file.close();
    return true;
}

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

void setLightParameters(scene&world, json lightsInput){
    for(const auto& l : lightsInput){ //copilot
        if (l["type"] == "pointlight"){
            world.addLight(make_shared<pointlight>(
                vec3(
                    l["position"][0].get<double>(),
                    l["position"][1].get<double>(),
                    l["position"][2].get<double>()
                ),
                vec3 (
                    l["intensity"][0].get<double>(),
                    l["intensity"][1].get<double>(),
                    l["intensity"][2].get<double>()
                )
            ));
        }
    } //
}

material setMaterialParameters(scene&world, json s){
    json materialInput = s["material"];
    if(s.find("material") == s.end()){
        cout << "not there" << endl;
        return material(
            0.0,
            0.0,
            0.0,
            color(0.0, 0.0, 0.0),
            color(0.0, 0.0, 0.0),
            false,
            0.0,
            false,
            0.0
        );
    }
    if (materialInput.find("hastexture") == materialInput.end() || !materialInput["hastexture"].get<bool>()) {
        return material(
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
    } else {
        vector<vector<vec3>> img;
        readPPM(materialInput["texturepath"].get<string>().c_str(), img);
        return material(
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
            materialInput["refractiveindex"].get<double>(),
            materialInput["hastexture"].get<bool>(),
            img
        );
    }
}

void setWorldParameters(scene& world, json input){
    vector<triangle> triangles;
    json sceneInput = input["scene"];
    json backgroundInput = sceneInput["backgroundcolor"];
    json lightsInput = sceneInput["lightsources"];
    json shapesInput = sceneInput["shapes"];

    //Set background color from JSON input:
    world.backgroundcolor = color((backgroundInput[0]).get<double>(), backgroundInput[1].get<double>(), backgroundInput[2].get<double>());

    //Set lights from JSON input:
    setLightParameters(world, lightsInput);

    //Push shapes from JSON input: (and materials (to-do))
    for (const auto& s : shapesInput) {
        shared_ptr<material> mat = make_shared<material>(setMaterialParameters(world, s));

        if (s["type"] == "sphere"){
            world.add(make_shared<sphere>(
                point3(
                    s["center"][0].get<double>(),
                    s["center"][1].get<double>(),
                    s["center"][2].get<double>()
                ),
                s["radius"].get<double>(),
                mat
            ));
        }
        if (s["type"] == "triangle"){
            triangles.push_back(triangle(
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
                ),
                mat
            ));
            // world.add(make_shared<triangle>(
            //     vec3(
            //         s["v0"][0].get<double>(),
            //         s["v0"][1].get<double>(),
            //         s["v0"][2].get<double>()
            //     ),
            //     vec3(
            //         s["v1"][0].get<double>(),
            //         s["v1"][1].get<double>(),
            //         s["v1"][2].get<double>()
            //     ),
            //     vec3(
            //         s["v2"][0].get<double>(),
            //         s["v2"][1].get<double>(),
            //         s["v2"][2].get<double>()
            //     ),
            //     mat
            // ));
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
                s["height"].get<double>(),
                mat
            ));
        }
        if (!triangles.empty()) {
            auto bvh = make_shared<BVHNode>();
            bvh->buildBVH(triangles, 3);
            world.add(bvh);
        }
    }
}

int main(int argc, char* argv[]) {
    if(argc != 2){
        std::cerr << "Please input: " << argv[0] << " relative-path-to/input.json" << std::endl;
        return 1; // Return indicating an error
    }

    cout << argv[1] << endl;
    
    scene world;
    camera cam;

    //JSON input
    std::string filename = argv[1];
    ifstream inputFile(filename);
    json input;

    if (inputFile.is_open()) {
        try {
            inputFile >> input; // Parse JSON from file
            setCameraParameters(cam, input);
            setWorldParameters(world, input);
        } catch (json::parse_error& e) {
            cerr << "Parse error: " << e.what() << endl;
        }
    } else {
        cerr << "Unable to open json file." << endl;
    }
    cam.render(world, cam.nbounces);
};