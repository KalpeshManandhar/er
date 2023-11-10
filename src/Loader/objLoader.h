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

        std::vector<uint32_t> vIndices;
        std::vector<uint32_t> tIndices;
        std::vector<uint32_t> nIndices;
    };
    std::vector<RenderInfo> renderInfo;

};

struct ObjFileInfo{

    char mtlFile[64];
    std::vector<ObjMeshData> meshes;

    std::vector<Vec3f> vertices;
    std::vector<Vec2f> texCoords;
    std::vector<Vec3f> normals;
};

ObjFileInfo loadObj(const char *path);
