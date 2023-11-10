#include <math/vec.h>

#define ARRAY_COUNT(x)  (sizeof(x)/sizeof(*x))

namespace CUBES{
    Vec3f vertices[] = {
        {1.0f,1.0f,1.0f},
        {1.0f,1.0f,-1.0f},
        {1.0f,-1.0f,-1.0f},
        {1.0f,-1.0f,1.0f},
        {-1.0f,-1.0f,1.0f},
        {-1.0f,-1.0f,-1.0f},
        {-1.0f,1.0f,-1.0f},
        {-1.0f,1.0f,1.0f},
    };

    unsigned int indices[] = {
        2,1,0,
        2,0,3,
        5,6,1,
        5,1,2,
        6,5,4,
        6,4,7,
        0,7,3,
        3,7,4,
        0,1,7,
        1,6,7,
        3,4,5,
        2,3,5,
    };

} // namespace Cube


Vec3f colors []= {
    { 1.0,1.0,1.0 },
    { 0.75,0.75,0.75 },
    { 0.5,0.5,0.5 },
    { 0.0,0.0,0.0 },
    { 1.0,0.0,0.0 },
    { 0.5,0.0,0.0 },
    { 1.0,1.0,0.0 },
    { 0.5,0.5,0.0 },
    { 0.0,1.0,0.0 },
    { 0.0,0.5,0.0 },
    { 0.0,1.0,1.0 },
    { 0.0,0.5,0.5 },
    { 0.0,0.0,1.0 },
    { 0.0,0.0,0.5 },
    { 1.0,0.0,1.0 },
    { 0.5,0.0,0.5 }
};