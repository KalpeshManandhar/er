#pragma once

#include "../Window/window_er.h"
#include <math/vec.h>
#include "./defines.h"

struct er_Renderer3D{
    er_Buffer2D framebuffer;
    er_Buffer2Df zBuffer;

    er_Renderer3D(size_t w, size_t h){
        framebuffer = er_Buffer2D(w,h);
        zBuffer = er_Buffer2Df(w,h);
    }

};

struct Point{
    Vec3f p;
    Vec3f color;
};



typedef Point (*VertexShader)(Point p);
typedef Vec4f (*PixelShader)(Point p);

struct Shader{
    VertexShader vs;
    PixelShader fs;
};



int computeTriangle(er_Renderer3D *r, Point a, Point b, Point c);
int displayMesh(er_Renderer3D *r, Point *points, size_t nPoints, uint32_t *indices, size_t nIndices);




// --------------------------------------------
//          PRIMITIVES/GEOMETRY - 3D
// --------------------------------------------
struct Line3D{
    Vec3f start, end;
};

struct Sphere3D{
    Vec3f center;
    float radius;
};

template <typename T>
union Triangle{
    struct {
        T a,b,c;
    };
    T points[3];
};

typedef Triangle<Vec3f> Triangle3D;
typedef Triangle<Point> Triangle3DEx;





