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
            if(rendermode == "binary"){
                //parse for nbounces, lightsources, material of objects
            }

            //Set camera variables from JSON input:
            json cameraInput = input["camera"];
            camera = new Camera(cameraInput);
            
            
            //Set scene variables from JSON input (wip):
            json sceneInput = input["scene"];
            json backgroundInput = sceneInput["backgroundcolor"];
            json shapesInput = sceneInput["shapes"];

            if(rendermode == "binary"){
                for (const auto& s : shapesInput) {
                    if (shapeType == "Sphere"){
                        world.add(make_shared<sphere>(<vec3>(shapesInput["center"]), shapesInput["radius"]));
                    }
                    if (shapeType == "Triangle"){
                        //create triangle (haven't gotten here yet)
                    }
                    if (shapeType == "Cylinder"){
                        //create cylinder (haven't gotten here yet either)
                    }
                }
            }
            else if(rendermode == "phong"){
                for (const auto& s : shapesInput) {
                    if (shapeType == "Sphere"){
                        //create sphere
                        world.add()
                    }
                    if (shapeType == "Triangle"){
                        //create triangle
                    }
                    if (shapeType == "Cylinder"){
                        //create cylinder
                    }
                }
            }
            

           
        } catch (json::parse_error& e) {
            cerr << "Parse error: " << e.what() << endl;
        }
    } else {
        cerr << "Unable to open json file." << endl;
    }


    // shapes_hit world;

    // world.add(make_shared<sphere>(point3(0, 0, -1), .5));
    // world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));


    // camera.aspect_ratio = 16.0/9.0;
    // camera.image_width = 400;

    // camera.render(world);

    
}
    