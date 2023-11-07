#include "./camera.h"

Mat4 Camera3D::lookAt(){
    /* from view space to world space
    Mat4 translation = Mat4{
        1,  0,  0,  pos.x,
        0,  1,  0,  pos.y, 
        0,  0,  1,  pos.z,
        0,  0,  0,  1
    };
    Mat4 rotation = Mat4{
        right.x,    up.x,   front.x,    0, 
        right.y,    up.y,   front.y,    0, 
        right.z,    up.z,   front.z,    0, 
        0,          0,      0,          1
    };
    Mat4 viewToWorld = translation * rotation;
    */

    // from world space to view space, i.e, essentially the inverse of viewToWorld
    Mat4 translation = Mat4{
        1,  0,  0,  -pos.x,
        0,  1,  0,  -pos.y, 
        0,  0,  1,  -pos.z,
        0,  0,  0,  1
    };
    Mat4 rotation = Mat4{
        right.x,    right.y,    right.z,    0, 
        up.x,       up.y,       up.z,       0, 
        front.x,    front.y,    front.z,    0, 
        0,          0,          0,          1
    };
    Mat4 worldToView = rotation * translation;
    return worldToView;
}


void Camera3D::updatePosition(Vec3f movementDirection, float deltaTime){
    pos = pos + normalize(movementDirection) * (deltaTime * velocity);
}



void Camera3D::updateOrientaton(float xOffset, float yOffset){
    const float sensitivity = 1.05f;

}

void Camera3D::updateOrientation(Vec3f targetPos, Vec3f worldUp){
    front = normalize(pos - targetPos);
    right = normalize(crossProduct(worldUp, front));
    up = normalize(crossProduct(front, right));
}