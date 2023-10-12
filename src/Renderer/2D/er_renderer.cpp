#include "renderer.h"
#include <math/math_er.h>



int drawLine(er_Renderer2D *r, Vec2f a, Vec2f b, uint32_t color){
    // float deltaX = fabsf(b.x - a.x);
    // float deltaY = fabsf(b.y - a.y);
    


    // for (float y = a.y, x = a.x; BetweenInc(a.y, b.y, y) || BetweenInc(b.y, a.y, yLine); yLine += deltaY, xLineStart += deltaX){
        
    // }
    return 0;
}


int fillTriangle(er_Renderer2D *r, Vec2f a, Vec2f b, Vec2f c, uint32_t color){
    Vec2f *start = &a,*end = &b, *third = &c;
    // find points with largest y separation: start and end for sampling, third is remaining point
    // A:start
    // B:end
    // C:third
    if (fabsf(start->y - end->y) < fabsf(b.y - c.y)) { start = &b; end = &c; third = &a; }
    if (fabsf(start->y - end->y) < fabsf(a.y - c.y)) { start = &a; end = &c; third = &b; }

    // deltaY and deltaX for sampling iterations along the line AB
    float deltaY = fabsf(end->y - start->y)/(end->y - start->y);
    float deltaX = (end->x - start->x)/fabsf(end->y - start->y);

    // slope of line AB
    float invm1 = (end->x - start->x)/(end->y - start->y);
    float m1 = 1.0f/invm1;

    // inverse slopes of lines AC and BC
    float invm2 = (third->x - start->x)/(third->y - start->y);
    float invm3 = (end->x - third->x)/(end->y - third->y);

    for (float yLine = start->y, xLineStart = start->x; BetweenInc(start->y, end->y, yLine) || BetweenInc(end->y, start->y, yLine); yLine += deltaY, xLineStart += deltaX){
        Vec2i scanlineStart = {Round(xLineStart), Round(yLine)};

        // find point of intersection of scanline with AC and BC
        Vec2i scanlineEndA = {Round(invm2 * (yLine - start->y) + start->x), Round(yLine)};
        Vec2i scanlineEndB = {Round(invm3 * (yLine - third->y) + third->x), Round(yLine)};
        
        // calculate f(x) for intersection points and the one with least magnitude is actual end point
        float fA = ((int)invm1 == 0)? (scanlineEndA.y - start->y)*invm1 - (scanlineEndA.x - start->x):scanlineEndA.y - start->y - m1 * (scanlineEndA.x - start->x);
        float fB = ((int)invm1 == 0)? (scanlineEndB.y - start->y)*invm1 - (scanlineEndB.x - start->x):scanlineEndB.y - start->y - m1 * (scanlineEndB.x - start->x);

        Vec2i scanlineEnd = (fabsf(fA) < fabsf(fB))? scanlineEndA:scanlineEndB;
        
        int scanlineDeltaX = (scanlineStart.x < scanlineEnd.x)? 1:-1;

        // set pixels for scanline
        for (int xScan = scanlineStart.x; BetweenInc(scanlineStart.x, scanlineEnd.x,  xScan) || BetweenInc(scanlineEnd.x, scanlineStart.x,  xScan); xScan += scanlineDeltaX){
            r->buffer.setPixel(xScan, yLine, color);            
        }
    }

    return 0;

}


int fillRectangle(er_Renderer2D*r, Vec2f min, Vec2f max, uint32_t color){
    assert((min.x <= max.x) && (min.y <= max.y));
    for (int y=min.y; y<=max.y; y++){
        for (int x = min.x; x <=max.x; x++){
            r->buffer.setPixel(x,y, color);
        }
    }
    return 0;
}


int fillRectangle(er_Renderer2D*r, Vec2f pos, float w, float h, uint32_t color){
    for (int y=pos.y; y<=pos.y + h; y++){
        for (int x = pos.x; x <=pos.x + w; x++){
            r->buffer.setPixel(x,y, color);
        }
    }
    return 0;
}


int fillCircle(er_Renderer2D *r, Vec2f center, float radius, uint32_t color){
    for (int y= center.y - radius; y<= center.y + radius; y++){
        if (!BetweenInc(0,r->buffer.h - 1, y))
            continue;
        for (int x= center.x - radius; x<=center.x + radius; x++){
            if (!BetweenInc(0,r->buffer.w - 1, x))
                continue;
            if ((x - center.x)*(x - center.x) + (y-center.y)*(y-center.y) <= radius * radius)
                r->buffer.setPixel(x,y, color);
        }
    }
    return 0;
}


