#include "./lexer.h"
#include "./objLoader.h"


#define for_range(i, st, end) for(int i=st; i<end; i++)

void meshDump(const ObjMesh &m){
    printf("Mesh: %s\n", m.name);
    // printf("Vertices\n");
    // for_range(i,0,m.vertices.size()){
    //     printf("(%0.3f, %0.3f, %0.3f)\n", m.vertices[i].x, m.vertices[i].y, m.vertices[i].z);
    // }
    // printf("Texture coords\n");
    // for_range(i,0,m.texCoords.size()){
    //     printf("(%0.3f, %0.3f)\n", m.texCoords[i].x, m.texCoords[i].y);
    // }
    // printf("Normals\n");
    // for_range(i,0,m.normals.size()){
    //     printf("(%0.3f, %0.3f, %0.3f)\n", m.normals[i].x, m.normals[i].y, m.normals[i].z);
    // }

    for_range(j, 0, m.renderInfo.size()){
        printf("Material: %s\n", m.renderInfo[j].materialName);
        printf("Vertices: %d\n", m.renderInfo[j].vIndices.size());
        printf("Texcoords: %d\n", m.renderInfo[j].tIndices.size());
        printf("Normals: %d\n", m.renderInfo[j].nIndices.size());
    }

    
    printf("\n\n");
}







ObjMesh loadMesh(Lexer &l){
    ObjMesh m;
    while (l.cursor < l.fileSize){
        DataString identifier = l.skipAndParseString();
        
        // vertex
        if (compare(identifier, "v")){
            Vec3f v;
            v.x = l.skipAndParseFloat();
            v.y = l.skipAndParseFloat();
            v.z = l.skipAndParseFloat();
            m.vertices.push_back(v);
        }
        // texture coordinates
        else if (compare(identifier, "vt")){
            Vec2f tCoords;
            tCoords.x = l.skipAndParseFloat();
            tCoords.y = l.skipAndParseFloat();
            m.texCoords.push_back(tCoords);
        }
        // vertex normal
        else if (compare(identifier, "vn")){
            Vec3f normal;
            normal.x = l.skipAndParseFloat();
            normal.y = l.skipAndParseFloat();
            normal.z = l.skipAndParseFloat();
            m.normals.push_back(normal);
        }
        // 
        else if (compare(identifier, "s")){

        }
        // use material 
        else if (compare(identifier, "usemtl")){
            ObjMesh::RenderInfo info = {};
            DataString materialName = l.skipAndParseString();
            copyToArray(materialName, info.materialName, sizeof(info.materialName));

            m.renderInfo.push_back(info);
        }
        // faces data (indices)
        else if (compare(identifier, "f")){
            for_range(i, 0, 3){
                int vIndex = l.skipAndParseInt();
                int tIndex = l.skipAndParseInt();
                int nIndex = l.skipAndParseInt();
                m.renderInfo.back().vIndices.push_back(vIndex);
                m.renderInfo.back().tIndices.push_back(tIndex);
                m.renderInfo.back().nIndices.push_back(nIndex);
            }
        }
        else{
            l.revertToLineStart();
            break;
        }

        l.skipCurrentLine();
    }
    return(m);

}


ObjFileInfo loadObj(const char *path){
    ObjFileInfo f = {  };
    Lexer l;
    l.loadFile(path);

    while (l.cursor < l.fileSize){
        // skip commented line in .obj
        if(l.data[l.cursor] == '#'){
            l.skipCurrentLine();
            continue;
        }

        DataString identifier = l.skipAndParseString();

        // use material 
        if (compare(identifier, "mtllib")){

        }
        // mesh name 
        else if (compare(identifier, "o")){
            l.skipSpaces();
            DataString name = l.skipAndParseString();
            l.skipCurrentLine();
            ObjMesh m = loadMesh(l);
            copyToArray(name, m.name, sizeof(m.name));            
            meshDump(m);
            f.meshes.push_back(m);
            continue;
        }
        
        l.skipCurrentLine();
    }
    return(f);
}

