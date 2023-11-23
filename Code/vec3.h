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

    bool operator<(const vec3& other) const {
        return length_squared() < other.length_squared();
    }

    bool operator<=(const vec3& other) const {
        return length_squared() <= other.length_squared();
    }

    bool operator>(const vec3& other) const {
        return length_squared() > other.length_squared();
    }

    bool operator>=(const vec3& other) const {
        return length_squared() >= other.length_squared();
    }

    //overload min and max for vec3
    static vec3 min(const vec3& a, const vec3& b) {
        return vec3(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));
    }

    static vec3 max(const vec3& a, const vec3& b) {
        return vec3(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));
    }

    void swap(vec3& v1, vec3& v2) {
        std::swap(v1.x, v2.x);
        std::swap(v1.y, v2.y);
        std::swap(v1.z, v2.z);
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

inline vec3 reflect(const vec3& incident, const vec3& normal) { //added by copilot from "inline vec3 reflect"
    return incident - 2.0 * dot(incident, normal) * normal;
}

inline vec3 clamp(vec3 v, double min, double max) {
    if (v.x < min) { v.x = min; }
    if (v.y < min) { v.y = min; }
    if (v.z < min) { v.z = min; }
    if (v.x > max) { v.x = max; }  
    if (v.y > max) { v.y = max; }
    if (v.z > max) { v.z = max; }
    return v;
}

vec3 refract(const vec3& incident, const vec3& normal, double refractiveIndex) { //all gpt
    double cosI = -dot(incident, normal);
    double etaI = 1.0; // Refractive index of the medium incident ray is in (e.g., air)
    double etaT = refractiveIndex; // Refractive index of the material the ray is entering

    if (cosI < 0) {
        // Ray is entering the material, swap the indices
        cosI = -cosI;
        std::swap(etaI, etaT);
    }

    double etaRatio = etaI / etaT;
    double k = 1.0 - etaRatio * etaRatio * (1.0 - cosI * cosI);

    if (k < 0) {
        // Total internal reflection, no refraction, return the reflected direction instead of zero vector
        return reflect(incident, normal); 
    } else {
        // Compute the refracted direction
        return etaRatio * incident + (etaRatio * cosI - sqrt(k)) * normal;
    }
}

vec3 snell(const vec3 &incident, const vec3 &normal, double refractiveIndex) {
    double cosI = dot(-incident, normal);
    double sinT2 = refractiveIndex * refractiveIndex * (1.0 - cosI * cosI);

    if (sinT2 > 1.0) {
        // Total internal reflection
        return reflect(incident, normal);
    }

    double cosT = sqrt(1.0 - sinT2);
    return refractiveIndex * incident + (refractiveIndex * cosI - cosT) * normal;
}

//add aliases for vec3
using point3 = vec3; // 3D point
using color = vec3 ; // RGB color

#endif