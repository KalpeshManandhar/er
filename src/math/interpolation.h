#pragma once

template <typename T>
T lerp(T a,T b, float t){
    return a+(b-a)*t;
}

template <typename T>
T bilinearInterpolate(T a00,T a10, T a11, T a01, float x, float y){
    T a1 = lerp(a00, a10, x);
    T a2 = lerp(a01, a11, x);
    return lerp(a1, a2, y)
}


float smoothstep(float x);