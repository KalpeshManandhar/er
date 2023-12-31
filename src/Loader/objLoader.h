#pragma once

#include <math/vec.h>
#include <vector>


#define NAME_ARRAY_SIZE 64


struct Material{
    char name[NAME_ARRAY_SIZE];
    float ns, Ni, d;
    Vec3f ka, kd, ks, ke;
};



struct MtlFileInfo{
    std::vector<Material> materials;
};



struct ObjMeshData{
    char name[NAME_ARRAY_SIZE];
    struct RenderInfo{
        char materialName[NAME_ARRAY_SIZE];
        uint32_t materialIndex;


        std::vector<uint32_t> vIndices;
        std::vector<uint32_t> tIndices;
        std::vector<uint32_t> nIndices;
    };
    std::vector<RenderInfo> renderInfo;

};

struct ObjFileInfo{

    char mtlFile[64];
    MtlFileInfo mtl;

    std::vector<ObjMeshData> meshes;

    std::vector<Vec3f> vertices;
    std::vector<Vec2f> texCoords;
    std::vector<Vec3f> normals;
    
    // bounding box
    Vec3f min, max;
};

ObjFileInfo loadObj(const char *path);
