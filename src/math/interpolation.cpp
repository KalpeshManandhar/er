#include "./interpolation.h"
#include "./utils.h"

float smoothstep(float x){
    x = Clamp(0.0f,x,1.0f);
    return x * x * (3.0f - 2.0f * x);
}