#pragma once

#include <math/vec.h>

#include "../Buffer/er_buffer.h"
#include "./primitives.h"
#include "./shader.h"
#include "./defines.h"

struct er_Renderer3D;

struct er_Renderer3D{
    er_Buffer2D framebuffer;
    er_Buffer2Df zBuffer;

    Shader shader;

    er_Renderer3D(size_t w, size_t h){
        framebuffer = er_Buffer2D(w,h);
        zBuffer = er_Buffer2Df(w,h);
    }

};






int computeTriangle(er_Renderer3D *r, Point a, Point b, Point c);
int displayMesh(er_Renderer3D *r, Point points[], size_t nPoints, uint32_t indices[], size_t nIndices);
int displayMesh(er_Renderer3D *r, 
                Vec3f points[], size_t nPoints, 
                Vec3f normals[], size_t nNormals, 
                uint32_t indices[], 
                uint32_t normalIndices[], size_t nIndices);






