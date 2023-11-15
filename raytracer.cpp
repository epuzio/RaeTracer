#include "raytracer.h"
#include "camera.h"
#include "color.h"
#include "shape.h"
#include "shapes_hit.h"
#include "sphere.h"

#include "interval.h"
#include "vec3.h"
#include "ray.h"

#include <nlohmann/json.hpp>
#include <iostream>
using namespace std;
using json = nlohmann::json;

int main() {
    shapes_hit world;
    Camera camera;

    //JSON input
    ifstream inputFile("binary_sphere.json");
    json input;

    if (inputFile.is_open()) {
        try {
            inputFile >> input; // Parse JSON from file
            string rendermode = input["rendermode"];

            //Set camera variables from JSON input:
            json cameraInput = input["camera"];
            camera = new Camera(cameraInput["width"], cameraInput["height"], <point3>(cameraInput["cameraPosition"]), <vec3>(cameraInput["lookat"]), <vec3>(cameraInput["upvector"]), cameraInput["fov"], cameraInput["exposure"]);
            
            //Get scene variables from JSON input:
            json sceneInput = input["scene"];
            json backgroundInput = sceneInput["backgroundcolor"];
            json shapesInput = sceneInput["shapes"];
            json lightsInput = sceneInput["lightsources"];
            json materialsInput = sceneInput["materials"];
            
            //Set background color from JSON input:
            background_color = color(<double>(backgroundInput[0]), <double>(backgroundInput[1]), <double>(backgroundInput[2]));

            //Push shapes from JSON input:
            for (const auto& s : shapesInput) {
                if (shapeType == "Sphere"){
                    world.add(sphere(<vec3>(shapesInput["center"]), shapesInput["radius"]));
                }
                if (shapeType == "Triangle"){
                    world.add(triangle(<vec3>(shapesInput["v0"]), <vec3>(shapesInput["v1"]), <vec3>(shapesInput["v2"])));
                }
                if (shapeType == "Cylinder"){
                    world.add(cylinder(<point3>(shapesInput["center"]), <vec3>(shapesInput["axis"]), shapesInput["radius"], shapesInput["height"]));
                }
            }
            
        } catch (json::parse_error& e) {
            cerr << "Parse error: " << e.what() << endl;
        }
    } else {
        cerr << "Unable to open json file." << endl;
    }

    camera.render(world);
}
    