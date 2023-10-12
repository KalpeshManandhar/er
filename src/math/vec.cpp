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

Vec3f crossProduct(Vec3f a, Vec3f b){
    return(Vec3f{a.y*b.z-a.z*b.y, a.z*b.x-b.z*a.x, a.x * b.y - b.x * a.y});
}

Vec3f normalize(Vec3f a){
    return(a * Q_rsqrt(lenSquared(a)));
}
float projection(Vec3f of, Vec3f on){
    return(dotProduct(of, normalize(on)));
}


float signedArea(Vec2f p1, Vec2f p2, Vec2f p3){
    return crossMag(p2-p1, p3-p1);
}



// returns a vec3 with (u,v,w)
Vec3f Barycentric(Vec2f a, Vec2f b, Vec2f c, Vec2f p){
    // P = A + v(B-A) + w(C-A)
    // P = uA + vB + wC
    // a = (x1,y1)
    // b = (x2,y2)
    // c = (x3,y3)
    // p = (xp,yp)

    float x21 = b.x - a.x;
    float x31 = c.x - a.x;
    float y21 = b.y - a.y;
    float y31 = c.y - a.y;
    float xp1 = p.x - a.x;
    float yp1 = p.y - a.y;

    float denom = y31 * x21 - x31 * y21;
    float v = -(yp1 * x31 - xp1 * y31)/denom;
    float w = (yp1 * x21 - xp1 * y21)/denom;
    float u = 1.0f - v - w;
    return(Vec3f{u,v,w}); 
}



