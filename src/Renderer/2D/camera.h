#pragma once

#include <math/vec.h>
#include <math/mat.h>

struct Camera3D{
    // basis vectors in a right handed system
    Vec3f front, up, right;
    Vec3f pos;

    Vec3f velocity;
    float zNear, zFar;


    void updateOrientation(Vec3f targetPos, Vec3f worldUp);
    void updateOrientaton(float xOffset, float yOffset);
    void updatePosition(Vec3f movementDirection, float deltaTime);

    Mat4 lookAt();
    
};
