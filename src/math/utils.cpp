#include "./utils.h"

bool nearlyEqual(float a, float b) {
    return(Abs(a - b) < 0.00005);
}

// quick inverse square root from Quake III
float Q_rsqrt(float number)              
{
    int i;
    float x2, y;
    const float threehalfs = 1.5f;
    x2 = number * 0.5f;
    y  = number;
    i  = *(int*) &y;
    i  = 0x5f3759df - ( i >> 1 );
    y  = *(float*)&i;
    y  = y * (threehalfs - (x2 * y * y));   // 1st iteration
	//  y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed
    return y;
}


