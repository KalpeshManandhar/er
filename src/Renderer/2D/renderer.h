#pragma once

#include "../Window/window_er.h"
#include <math/vec.h>


struct er_Renderer2D{
    er_Buffer buffer;

    er_Renderer2D(size_t w, size_t h){
        buffer = er_Buffer(w,h);
    }

};


int fillTriangle(er_Renderer2D *r, Vec2f a, Vec2f b, Vec2f c, uint32_t color);
int fillRectangle(er_Renderer2D*r, Vec2f min, Vec2f max, uint32_t color);
int fillRectangle(er_Renderer2D*r, Vec2f pos, float w, float h, uint32_t color);
int fillCircle(er_Renderer2D *r, Vec2f center, float radius, uint32_t color);

