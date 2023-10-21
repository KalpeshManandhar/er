#include "./renderer3D.h"
#include <math/math_er.h>
#include <thread>

#include "./projection.h"

#include <tests/debug.h>


// #include <>


#ifndef RGBA_TO_U32
#define RGBA_TO_U32_ARGB(r,g,b,a) ((uint8_t(b)) | (uint8_t(g)<<8) | (uint8_t(r)<<16) | (uint8_t(a)<<24))
#define RGB_TO_U32_ARGB(r,g,b)  RGBA_TO_U32_ARGB(r,g,b, 0xff)

#define RGBA_TO_U32(r,g,b,a)    RGBA_TO_U32_ARGB(r,g,b,a)

#define RGB_TO_U32(r,g,b)       RGBA_TO_U32(r,g,b,0xff)
#define nRGB_TO_U32(r,g,b)      RGB_TO_U32(uint8_t((r)*255),uint8_t((g)*255),uint8_t((b)*255))
#define nRGBA_TO_U32(r,g,b,a)   RGBA_TO_U32(uint8_t((r)*255),uint8_t((g)*255),uint8_t((b)*255),uint8_t((a)*255))
#endif


static bool visibleSurfaceDetection(Vec3f a, Vec3f b, Vec3f c, Vec3f view){
    Vec3f normal = crossProduct(b-a, c-a);
    return(dotProduct(normal, view) > 0);
}


static Point interpolateProperties(Point p1, Point p2, Point p3, Vec3f t){
    Point p;
    p.p = p1.p*t.x + p2.p*t.y + p3.p*t.z; 
    p.color = p1.color*t.x + p2.color*t.y + p3.color*t.z; 
    return p;
} 


static bool compareAndUpdateZ(er_Renderer3D *r, int x, int y, float z){
    bool withinRange = (BetweenInc(0.0f,1.0f, z));
    if (!withinRange) printf("Not in range 0-1: %f\n",z);
    bool infront =  ((1.0f - z) > r->zBuffer.getValue(x,y));
    if (!infront) printf("Not in front 0-1: %f\n",z);
    // bool visible = (BetweenInc(0.0f,1.0f, z)) && (1.0f - z > r->zBuffer.getValue(x,y));
    bool visible = withinRange && infront;
    if (visible)
        r->zBuffer.setValue(x,y,1.0f - z);
    return visible;

}


Vec4f shadePixel(Point p){
    return(Vec4f(p.color,1.0f));
}

//TODO: use a thread pool: currently way too much overhead on creating and joining threads
int rasterizeEdgeCheck_mt(er_Renderer3D *r, Point p1, Point p2, Point p3){
    Vec2f a = p1.p.xy();
    Vec2f b = p2.p.xy();
    Vec2f c = p3.p.xy();


    const int numThreads = 2;
    std::thread threads[numThreads];

    int ymax = Max(a.y, Max(b.y, c.y));
    int ymin = Min(a.y, Min(b.y, c.y));
    int xmax = Max(a.x, Max(b.x, c.x));
    int xmin = Min(a.x, Min(b.x, c.x));

    int h = ymax - ymin;
    h = (numThreads - (h-1)%numThreads) + h;    // round off to nearest multiple of numThreads
    ymax = ymin + h;

    float invAreaABC = 1.0f/signedArea(a,b,c);

        auto fillSpan = [&](int yStart, int yEnd){
            for (int y = yStart; y<yEnd; y++){
                for (int x = xmin; x<= xmax; x++){
                    Vec2f p = {float(x),float(y)};
                    // barycentric coordinates of P where w1 + w2 + w3 = signedArea of triangle ABC
                    int w1_pixel = signedArea(b,c,p);
                    int w2_pixel = signedArea(c,a,p);
                    int w3_pixel = signedArea(a,b,p);
                    
                    if ((w1_pixel | w2_pixel | w3_pixel) >= 0){
                        Vec3f tValues = invAreaABC * Vec3f{(float)w1_pixel,(float)w2_pixel,(float)w3_pixel};
                        Point interpolated = interpolateProperties(p1, p2, p3, tValues);

                        Vec4f pixelColor = shadePixel(interpolated);
                        uint32_t color = nRGBA_TO_U32(pixelColor.x, pixelColor.y, pixelColor.z, pixelColor.w);
                        r->framebuffer.setValue(x,y,color);
                    }
                }
            }
        };
        
        int span = h/(numThreads);
        
        for(int threadNo = 0; threadNo<numThreads; threadNo++){
            threads[threadNo] = std::thread(fillSpan, ymin + threadNo * span, span);
        }
        for(int threadNo = 0; threadNo<numThreads; threadNo++){
            threads[threadNo].join();
        }
    return 0;
    
}



/* 
referenced from https://fgiesen.wordpress.com/2013/02/10/optimizing-the-basic-rasterizer/
*/
int rasterizeEdgeCheck(er_Renderer3D *r, Point p1, Point p2, Point p3){
    // convert to window coordinates from normalized
    p1.p.x *= r->framebuffer.w;
    p1.p.y *= r->framebuffer.h;
    p2.p.x *= r->framebuffer.w;
    p2.p.y *= r->framebuffer.h;
    p3.p.x *= r->framebuffer.w;
    p3.p.y *= r->framebuffer.h;

    Vec2f a = p1.p.xy();
    Vec2f b = p2.p.xy();
    Vec2f c = p3.p.xy();
    
    // find bounding box and clip to window
    float ymax = Min(Max(a.y, Max(b.y, c.y)), r->framebuffer.h-1);
    float ymin = Max(Min(a.y, Min(b.y, c.y)), 0);
    float xmax = Min(Max(a.x, Max(b.x, c.x)), r->framebuffer.w-1);
    float xmin = Max(Min(a.x, Min(b.x, c.x)), 0);

    float invAreaABC = 1.0f/signedArea(a,b,c);

    Vec2f startPixel = {xmin, ymin};

    int w1_row = signedArea(b, c, startPixel);
    int w2_row = signedArea(c, a, startPixel);
    int w3_row = signedArea(a, b, startPixel);

    int stepX_w1 = (int(b.y) - int(c.y));
    int stepX_w2 = (int(c.y) - int(a.y));
    int stepX_w3 = (int(a.y) - int(b.y));
    int stepY_w1 = (int(c.x) - int(b.x));
    int stepY_w2 = (int(a.x) - int(c.x));
    int stepY_w3 = (int(b.x) - int(a.x));

    for (int y = ymin; y<=ymax; y++){
        // barycentric coordinates of P where w1 + w2 + w3 = signedArea of triangle ABC
        int w1_pixel = w1_row;
        int w2_pixel = w2_row;
        int w3_pixel = w3_row;

        int span = 0;
        int fillspan = 0;

        for (int x = xmin; x<=xmax; x++){

            // if w1,w2,w3 are all positive, pixel lies inside triangle
            if ((w1_pixel | w2_pixel | w3_pixel) >= 0){
                Vec3f tValues = invAreaABC * Vec3f{(float)w1_pixel,(float)w2_pixel,(float)w3_pixel};

                span++;    
                float z = tValues.x * p1.p.z + tValues.y * p2.p.z + tValues.z * p3.p.z;
                // check with depth buffer
                if (compareAndUpdateZ(r,x,y,z)){
                    Point interpolated = interpolateProperties(p1, p2, p3, tValues);

                    Vec4f pixelColor = shadePixel(interpolated);
                    uint32_t color = nRGBA_TO_U32(pixelColor.x, pixelColor.y, pixelColor.z, pixelColor.w);
                    r->framebuffer.setValue(x,y,color);
                    fillspan++;
                }
                else {
                    float sum = tValues.x+tValues.y+ tValues.z;
                    printf("z: %f, t1: %f, t2: %f, t3: %f SUM: %f\n ",z,tValues.x, tValues.y, tValues.z, sum);
                }
            }

            w1_pixel += stepX_w1;
            w2_pixel += stepX_w2;
            w3_pixel += stepX_w3;
        }

        printf("y: %d Span: %d Fillspan: %d\n",y, span, fillspan);
        w1_row += stepY_w1;
        w2_row += stepY_w2;
        w3_row += stepY_w3;
    }
    return 0;
}




int rasterizeScanline(er_Renderer3D *r, Point p1, Point p2, Point p3){
    Vec2f a = p1.p.xy();
    Vec2f b = p2.p.xy();
    Vec2f c = p3.p.xy();

    Vec2f start = a,end = b, third = c;
    // find points with largest y separation: start and end for sampling, third is remaining point
    // A:start
    // B:end
    // C:third
    if (fabsf(start.y - end.y) < fabsf(b.y - c.y)) { start = b; end = c; third = a; }
    if (fabsf(start.y - end.y) < fabsf(a.y - c.y)) { start = a; end = c; third = b; }

    // deltaY and deltaX for sampling iterations along the line AB
    float deltaY = fabsf(end.y - start.y)/(end.y - start.y);
    float deltaX = (end.x - start.x)/fabsf(end.y - start.y);

    // slope of line AB
    float invm1 = (end.x - start.x)/(end.y - start.y);
    float m1 = 1.0f/invm1;

    // inverse slopes of lines AC and BC
    float invm2 = (third.x - start.x)/(third.y - start.y);
    float invm3 = (end.x - third.x)/(end.y - third.y);

    for (float yLine = start.y, xLineStart = start.x; BetweenInc(start.y, end.y, yLine) || BetweenInc(end.y, start.y, yLine); yLine += deltaY, xLineStart += deltaX){
        Vec2i scanlineStart = {Round(xLineStart), Round(yLine)};

        // find point of intersection of scanline with AC and BC
        Vec2i scanlineEndA = {Round(invm2 * (yLine - start.y) + start.x), Round(yLine)};
        Vec2i scanlineEndB = {Round(invm3 * (yLine - third.y) + third.x), Round(yLine)};
        
        // calculate f(x) for intersection points and the one with least magnitude is actual end point
        float fA = ((int)invm1 == 0)? (scanlineEndA.y - start.y)*invm1 - (scanlineEndA.x - start.x):scanlineEndA.y - start.y - m1 * (scanlineEndA.x - start.x);
        float fB = ((int)invm1 == 0)? (scanlineEndB.y - start.y)*invm1 - (scanlineEndB.x - start.x):scanlineEndB.y - start.y - m1 * (scanlineEndB.x - start.x);

        Vec2i scanlineEnd = (fabsf(fA) < fabsf(fB))? scanlineEndA:scanlineEndB;
        
        int scanlineDeltaX = (scanlineStart.x < scanlineEnd.x)? 1:-1;


        // set pixels for scanline
        for (int xScan = scanlineStart.x; BetweenInc(scanlineStart.x, scanlineEnd.x,  xScan) || BetweenInc(scanlineEnd.x, scanlineStart.x,  xScan); xScan += scanlineDeltaX){
            Vec3f tvalues = Barycentric(a,b,c, Vec2f{(float)xScan, yLine});
            Point interpolated = interpolateProperties(p1, p2, p3, tvalues);

            Vec4f pixelColor = shadePixel(interpolated);
            uint32_t color = nRGBA_TO_U32(pixelColor.x, pixelColor.y, pixelColor.z, pixelColor.w);
            r->framebuffer.setValue(xScan, yLine, color);  
        }
    }

    return 0;
}


Point computeVertex(Point p){
    Vec4f p4(p.p, 1.0f);
    const float znear = 2.0f, zfar = 1000.0f;
    const float fovx = 90, fovy = 90;

    //apply transformations
    Mat4 m = identity<4>(); 
    m = scaleAboutOrigin(100,100,100);
    m = translate(100,100,200) * m;
    m = perspectiveProjection(fovx, fovy, znear, zfar) * m;

    p.p = (m * p4).xyz_h();
    return p;
}




// a,b,c in anticlockwise order
int computeTriangle(er_Renderer3D *r, Point a, Point b, Point c){
    // if (!visibleSurfaceDetection(a.p,b.p,c.p, {0,0,1})) 
        // return -1;


    // compute vertex     
    a = computeVertex(a);
    b = computeVertex(b);
    c = computeVertex(c);

    

    TIME(rasterizeStart);
    // rasterize
    rasterizeEdgeCheck(r, a, b, c);
    // rasterizeScanline(r, a, b, c);
    TIME(rasterizeEnd);
    printf("%f", TIME_DIFF(rasterizeStart, rasterizeEnd));

    return 0;

}


int displayMesh(er_Renderer3D *r, Point *points, size_t nPoints, uint32_t *indices, size_t nIndices){
    for (int i=0; i<nIndices; i+=3){
        computeTriangle(r, points[indices[i]], points[indices[i+1]], points[indices[i+2]]);
    }
    return 0;
}