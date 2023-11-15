#ifndef INTERVAL_H
#define INTERVAL_H
#include <algorithm>
//rewrite

class interval {
  public:
    double min, max;

    interval() : min(+INT_MAX), max(-INT_MAX) {}
    interval(double _min, double _max) : min(_min), max(_max) {}

    bool contains(double x) const {
        return min <= x && x <= max;
    }

    bool surrounds(double x) const {
        return min < x && x < max;
    }

    static const interval empty, universe;
};

const static interval empty   (+INT_MAX, -INT_MAX);
const static interval universe(-INT_MAX, +INT_MAX);

#endif