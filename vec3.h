#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>
using namespace std;
using std::sqrt;


class vec3 {
  public:
    double x, y, z;

    vec3() : x(0), y(0), z(0) {}
    vec3(double x, double y, double z) : x(x), y(y), z(z) {}

    //vector operator overloads //copilot
    vec3 operator-() const { return vec3(-x, -y, -z); }
    vec3 operator+() const { return *this; }
    vec3& operator+=(const vec3 &v) {
        x += v.x; 
        y += v.y; 
        z += v.z; 
        return *this; 
    }
    vec3& operator-=(const vec3 &v) {
        x -= v.x; 
        y -= v.y; 
        z -= v.z; 
        return *this;
    }
    vec3& operator*=(const double t) {
        x *= t; 
        y *= t; 
        z *= t; 
        return *this;
    }
    vec3& operator/=(const double t) {
        return *this *= 1/t; 
    }
    double length() const {
        return sqrt(length_squared()); 
    }
    double length_squared() const {
        return x*x + y*y + z*z; 
    }
};

//vector utility functions: //copilot
inline ostream& operator<<(ostream &out, const vec3 &v) {
    return out << v.x << ' ' << v.y << ' ' << v.z;
}
inline vec3 operator+(const vec3 &u, const vec3 &v) {
    return vec3(u.x + v.x, u.y + v.y, u.z + v.z); 
}
inline vec3 operator-(const vec3 &u, const vec3 &v) {
    return vec3(u.x - v.x, u.y - v.y, u.z - v.z); 
}
inline vec3 operator*(const vec3 &u, const vec3 &v) {
    return vec3(u.x * v.x, u.y * v.y, u.z * v.z); 
}
inline vec3 operator*(double t, const vec3 &v) {
    return vec3(t*v.x, t*v.y, t*v.z); 
}
inline vec3 operator*(const vec3 &v, double t) {
    return t * v; 
}
inline vec3 operator/(vec3 v, double t) {
    return (1/t) * v; 
}
inline double dot(const vec3 &u, const vec3 &v) {
    return u.x * v.x + u.y * v.y + u.z * v.z; 
}
inline vec3 cross(const vec3 &u, const vec3 &v) {
    return vec3(u.y * v.z - u.z * v.y, 
                u.z * v.x - u.x * v.z, 
                u.x * v.y - u.y * v.x);
}
inline vec3 normalize(vec3 v) {
    v.x /= v.length();
    v.y /= v.length();
    v.z /= v.length();
    return v;
}

//add aliases for vec3 //copilot
using point3 = vec3; // 3D point
using color = vec3 ; // RGB color

#endif

















//original copilot:
// vector utility functions: //copilot
// inline ostream& operator<<(ostream &out, const vec3 &v) {
//     return out << v.x[0] << ' ' << v.y[1] << ' ' << v.z[2];
// }
// inline vec3 operator+(const vec3 &u, const vec3 &v) {
//     return vec3(u.x[0] + v.x[0], u.y[1] + v.y[1], u.z[2] + v.z[2]); 
// }
// inline vec3 operator-(const vec3 &u, const vec3 &v) {
//     return vec3(u.x[0] - v.x[0], u.y[1] - v.y[1], u.z[2] - v.z[2]); 
// }
// inline vec3 operator*(const vec3 &u, const vec3 &v) {
//     return vec3(u.x[0] * v.x[0], u.y[1] * v.y[1], u.z[2] * v.z[2]); 
// }
// inline vec3 operator*(double t, const vec3 &v) {
//     return vec3(t*v.x[0], t*v.y[1], t*v.z[2]); 
// }
// inline vec3 operator*(const vec3 &v, double t) {
//     return t * v; 
// }
// inline vec3 operator/(vec3 v, double t) {
//     return (1/t) * v; 
// }
// inline double dot(const vec3 &u, const vec3 &v) {
//     return u.x[0] * v.x[0] + u.y[1] * v.y[1] + u.z[2] * v.z[2]; 
// }
// inline vec3 cross(const vec3 &u, const vec3 &v) {
//     return vec3(u.y[1] * v.z[2] - u.z[2] * v.y[1], 
//                 u.z[2] * v.x[0] - u.x[0] * v.z[2], 
//                 u.x[0] * v.y[1] - u.y[1] * v.x[0]);
// }
// inline vec3 unit_vector(vec3 v) {
//     return v / v.length(); 
// }

// //add aliases for vec3 //copilot
// using point3 = vec3; // 3D point
// using color = vec3; // RGB color