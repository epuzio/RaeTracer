#ifndef RAYTRACING_H
#define RAYTRACING_H

#include <cmath>
#include <limits>
#include <memory>
#include <nlohmann/json.hpp>
#include <fstream>


// Usings
using std::shared_ptr;
using std::make_shared;
using std::sqrt;

//stuff from rt1w, might get rid of ngl
// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

//Random number generator for doubles from [0, 1), given min and max //copilot
inline double random_double() {
    return rand() / (RAND_MAX + 1.0);
}
inline double random_double(double min, double max) {
    // Returns a random real in [min,max).
    return min + (max - min) * (rand() / (RAND_MAX + 1.0));
} //




#endif