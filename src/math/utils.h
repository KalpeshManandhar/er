#pragma once

#define Min(a, b)           ((a<b)?a:b)
#define Max(a, b)           ((a>b)?a:b)
#define Clamp(a, val, b)    (Max(a, Min(val, b)))
#define Abs(a)				((a<0)?-a:a)
#define Radians(x)          (2.0f*3.1415f*x/360.f)
#define BetweenInc(a,b,x)   ((x>=a)&&(x<=b))
#define BetweenExc(a,b,x)   ((x>a)&&(x<b))
#define Round(x)            ((int)(x+0.5))



float Q_rsqrt(float number);
bool nearlyEqual(float, float);
