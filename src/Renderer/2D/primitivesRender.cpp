#include "./primitives.h"
#include "./renderer3D.h"
#include <math/mat.h>

#define ARRAY_COUNT(x)  (sizeof(x)/sizeof(*x))

namespace CubeMesh{
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



// converts cube to a model matrix
Mat4 getModelMatCube(Cube &c){
    Mat4 scale = scaleAboutOrigin(c.radii[0], c.radii[1], c.radii[2]);
    Mat4 rotate = Mat4{
        c.axes[0].x,    c.axes[1].x,    c.axes[2].x,    0, 
        c.axes[0].y,    c.axes[1].y,    c.axes[2].y,    0, 
        c.axes[0].z,    c.axes[1].z,    c.axes[2].z,    0, 
        0,              0,              0,              1         
    };
    Mat4 translate = translate3D(c.center.x, c.center.y, c.center.z);

    Mat4 model = translate * rotate * scale;
    return model;
}



// sets the model matrix and renders cube with the current shader
int renderCube(er_Renderer3D *r, Cube c, Vec3f color){
    Mat4 model = getModelMatCube(c);

    Point points[ARRAY_COUNT(CubeMesh::vertices)];
    for (int i =0; i<ARRAY_COUNT(CubeMesh::vertices); i++){
        points[i].pos = CubeMesh::vertices[i];
        points[i].color = color;
    }

    r->shader->model = model;
    displayMesh(r, points, ARRAY_COUNT(CubeMesh::vertices), CubeMesh::indices, ARRAY_COUNT(CubeMesh::indices));
    return 0;
}

