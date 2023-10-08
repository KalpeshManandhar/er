#include <math.h>
#include "vec.h"
#include "./utils.h"




Vec2f operator+(Vec2f a, Vec2f b){
    return(Vec2f{a.x + b.x, a.y + b.y});
}
Vec2f operator-(Vec2f a, Vec2f b){
    return(Vec2f{a.x - b.x, a.y - b.y});
}
Vec2f operator*(Vec2f a, float b){
    return(Vec2f{a.x * b, a.y *b});
}
Vec2f operator*(float b, Vec2f a){
    return(Vec2f{a.x * b, a.y *b});
}
Vec2f operator/(Vec2f a, float b){
    return(Vec2f{a.x/b, a.y/b});
}
Vec2f operator-(Vec2f a){
    return(Vec2f{-a.x, -a.y});
}
void operator+=(Vec2f& a, Vec2f b){
    a = a+b;
}
void operator-=(Vec2f& a, Vec2f b){
    a = a-b;
}
void operator*=(Vec2f& a, float b){
    a = a*b;
}
void operator/=(Vec2f& a, float b){
    a = a/b;
}
bool operator==(Vec2f a, Vec2f b) {
    return(nearlyEqual(a.x,b.x) && nearlyEqual(a.y,b.y));
}
bool operator!=(Vec2f a, Vec2f b) {
    return(!nearlyEqual(a.x, b.x) || !nearlyEqual(a.y, b.y));
}

float lenSquared(Vec2f a){
    return(a.x * a.x + a.y * a.y);
}
float len(Vec2f a){
    return(sqrt(lenSquared(a)));
}

float dotProduct(Vec2f a, Vec2f b){
    return(a.x * b.x + a.y * b.y);
}

// clockwise = +ve; and anticlockwise = -ve
float crossMag(Vec2f a, Vec2f b){
    return(a.x * b.y - b.x * a.y);
}

Vec2f normalize(Vec2f a){
    return(a*(Q_rsqrt(lenSquared(a))));
}
float projection(Vec2f of, Vec2f on){
    return(dotProduct(of, normalize(on)));
}




// vec3f
Vec3f operator+(Vec3f a, Vec3f b){
    return(Vec3f{a.x + b.x, a.y + b.y,a.z + b.z});
}
Vec3f operator-(Vec3f a, Vec3f b){
    return(Vec3f{a.x - b.x, a.y - b.y, a.z-b.z});
}
Vec3f operator*(Vec3f a, float b){
    return(Vec3f{a.x * b, a.y *b,a.z*b});
}
Vec3f operator*(float b, Vec3f a){
    return(Vec3f{a.x * b, a.y *b, a.z*b});
}
Vec3f operator/(Vec3f a, float b){
    return(Vec3f{a.x/b, a.y/b, a.z/b});
}
Vec3f operator-(Vec3f a){
    return(Vec3f{-a.x, -a.y, -a.z});
}
void operator+=(Vec3f& a, Vec3f b){
    a = a+b;
}
void operator-=(Vec3f& a, Vec3f b){
    a = a-b;
}
void operator*=(Vec3f& a, float b){
    a = a*b;
}
void operator/=(Vec3f& a, float b){
    a = a/b;
}
bool operator==(Vec3f a, Vec3f b) {
    return(nearlyEqual(a.x,b.x) && nearlyEqual(a.y,b.y)&& nearlyEqual(a.z,b.z));
}
bool operator!=(Vec3f a, Vec3f b) {
    return(!nearlyEqual(a.x, b.x) || !nearlyEqual(a.y, b.y) || !nearlyEqual(a.z,b.z));
}

float lenSquared(Vec3f a){
    return(a.x * a.x + a.y * a.y+a.z * a.z);
}
float len(Vec3f a){
    return(sqrt(lenSquared(a)));
}

float dotProduct(Vec3f a, Vec3f b){
    return(a.x * b.x + a.y * b.y + a.z *b.z);
}

// clockwise = +ve; and anticlockwise = -ve
Vec3f crossProduct(Vec3f a, Vec3f b){
    return(Vec3f{a.y*b.z-a.z*b.y, a.z*b.x-b.z*a.x, a.x * b.y - b.x * a.y});
}

Vec3f normalize(Vec3f a){
    return(a * Q_rsqrt(lenSquared(a)));
}
float projection(Vec3f of, Vec3f on){
    return(dotProduct(of, normalize(on)));
}




// quick inverse square root from quake III 




