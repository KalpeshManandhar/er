#include "./projection.h"
#include <math/math_er.h>

Mat4 parallelProjection(){
    return identity<4>();
}



// near and far planes are z=-znear and z=-zfar
Mat4 perspectiveProjection(float fovXDegree, float fovYDegree, float zNear, float zFar){
    /* 
    projects a given vec4 (x,y,z,1) to (x', y', z', w')
    where 
    x' = 0 to 1 at frustum borders 
    y' = 0 to 1 at frustum borders
    z' = 0 at -znear to 1 at -zfar
    w' = z


    z = 0 is reference point
    */


    float invtanX = 1.0f/tanf(Radians(fovXDegree*0.5f));
    float invtanY = 1.0f/tanf(Radians(fovYDegree*0.5f));

    float A = zFar/(zFar - zNear);
    float B = zNear * A;
    return Mat4{
        -0.5f*invtanX,   0,              0.5,    0,
        0,              -0.5f*invtanY,   0.5,    0,
        0,              0,              A,      B,
        0,              0,              1,      0

    };
}


