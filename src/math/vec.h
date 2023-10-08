#pragma once


template <typename T>
struct Vec2{
    T x,y;
};

using Vec2f = Vec2<float>;
using Vec2i = Vec2<int>;

template <typename T>
struct Vec3{
    T x,y,z;

    Vec2<T> xy(){
        return Vec2<T>{x,y};
    }
};

using Vec3f = Vec3<float>;
using Vec3i = Vec3<int>;




template <typename T>
struct Vec4{
    T x,y,z,w;

    Vec3<T> xyz(){
        return Vec3<T>{x,y,z};
    }
};

using Vec4f = Vec4<float>;
using Vec4i = Vec4<int>;



// vec2
Vec2f operator+(Vec2f, Vec2f);
Vec2f operator-(Vec2f, Vec2f);
Vec2f operator*(Vec2f, float);
Vec2f operator*(float, Vec2f);
Vec2f operator/(Vec2f, float);
Vec2f operator-(Vec2f);
void operator+=(Vec2f&, Vec2f);
void operator-=(Vec2f&, Vec2f);
void operator*=(Vec2f&, float);
void operator/=(Vec2f&, float);
bool operator==(Vec2f, Vec2f);
bool operator!=(Vec2f, Vec2f);

float lenSquared(Vec2f);
float len(Vec2f);

float dotProduct(Vec2f, Vec2f);
float crossMag(Vec2f, Vec2f);

Vec2f normalize(Vec2f);
float projection(Vec2f of, Vec2f on);


// vec3
Vec3f operator+(Vec3f, Vec3f);
Vec3f operator-(Vec3f, Vec3f);
Vec3f operator*(Vec3f, float);
Vec3f operator*(float, Vec3f);
Vec3f operator/(Vec3f, float);
Vec3f operator-(Vec3f);
void operator+=(Vec3f&, Vec3f);
void operator-=(Vec3f&, Vec3f);
void operator*=(Vec3f&, float);
void operator/=(Vec3f&, float);
bool operator==(Vec3f, Vec3f);
bool operator!=(Vec3f, Vec3f);

float lenSquared(Vec3f);
float len(Vec3f);

float dotProduct(Vec3f, Vec3f);
Vec3f crossProduct(Vec3f, Vec3f);

Vec3f normalize(Vec3f);
float projection(Vec3f of, Vec3f on);












