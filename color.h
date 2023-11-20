#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include <iostream>
#include "color.h"
typedef vec3 color;
using namespace std;

//Write pixel color to output file

void writeColor(ostream &out, color pixel_color, int numSamples) {
    pixel_color /= numSamples; //average color
    out << static_cast<int>(256 * pixel_color.x) << ' '
        << static_cast<int>(256 * pixel_color.y) << ' '
        << static_cast<int>(256 * pixel_color.z) << '\n';
}

#endif