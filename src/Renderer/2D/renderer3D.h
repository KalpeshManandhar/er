#pragma once

#include "../Buffer/er_buffer.h"
#include "./primitives.h"
#include <math/vec.h>
#include "./defines.h"

struct er_Renderer3D;
struct Shader;

struct er_Renderer3D{
    er_Buffer2D framebuffer;
    er_Buffer2Df zBuffer;

    Shader *shader;

    er_Renderer3D(size_t w, size_t h){
        framebuffer = er_Buffer2D(w,h);
        zBuffer = er_Buffer2Df(w,h);
    }

};


typedef Point (*VertexShader)(Point p);
typedef Vec4f (*PixelShader)(Point p);

struct Shader{
    VertexShader vs;
    PixelShader ps;    
};



int computeTriangle(er_Renderer3D *r, Point a, Point b, Point c);
int displayMesh(er_Renderer3D *r, Point *points, size_t nPoints, uint32_t *indices, size_t nIndices);






