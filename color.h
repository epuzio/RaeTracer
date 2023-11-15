#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include <iostream>
#include "color.h"
using color = vec3;
using namespace std;

void writeColor(ostream &out, color pixel_color, int numSamples) {
    pixel_color /= numSamples; //average color
    out << static_cast<int>(256 * pixel_color.x()) << ' '
        << static_cast<int>(256 * pixel_color.y()) << ' '
        << static_cast<int>(256 * pixel_color.z()) << '\n';
}

bool bounds(double x) const {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

#endif