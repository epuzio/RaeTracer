// #include "raytracer.h"
// #include "camera.h"
// #include "color.h"
// #include "shape.h"
// #include "shapes_hit.h"
// #include "sphere.h"

// #include "interval.h"
// #include "vec3.h"
// #include "ray.h"

#include <nlohmann/json.hpp>


#include <iostream>
using namespace std;
using json = nlohmann::json;

int main() {
    Camera camera;

    //JSON input
    ifstream inputFile("binary_sphere.json");
    json input;

    if (inputFile.is_open()) {
        try {
            inputFile >> input; // Parse JSON from file
            json cameraInput = input["camera"]
            cout << cameraInput << endl;

            //Set camera variables from JSON input:
            // camera = new Camera(cameraInput);
            // camera = new Camera("pinhole", cameraInput["width"], cameraInput["height"], 
            // cameraInput["position"], cameraInput["lookAt"], cameraInput["upVector"],
            // cameraInput["fov"], cameraInput["exposure"]);

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
    