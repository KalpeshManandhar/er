#include <math/math_er.h>
#include <Renderer/2D/renderer.h>
#include <Renderer/2D/renderer3D.h>

#include <Renderer/2D/texture.h>
#include <Renderer/2D/camera.h>

#include <Loader/objLoader.h>


#define BMP_USE_VALUES
#include "bmp.h"
#include "debug.h"
#include "vertices.h"

#include "./shaders/shaders.h"


#include <stdio.h>
#include <Windows.h>




LRESULT CALLBACK win32_windowProc(HWND windowHandle, UINT message, WPARAM wparam, LPARAM lparam){
    switch (message)
    {
    case WM_PAINT:
        break;
    case WM_SIZE:
        break;
    case WM_DESTROY:
        printf("DESTROY?");
        break;
    case WM_CLOSE:{
        PostQuitMessage(0);
        printf("CLOSE?");
    }break;

    case WM_KEYDOWN:{
    }

    default:
        break;
    }
    return(DefWindowProc(windowHandle, message, wparam, lparam));
}

void win32_display(HWND windowHandle, er_Buffer2D *b, BITMAPINFO *bmpinfo){
    RECT windowRect = {};
    HDC deviceContext = GetWindowDC(windowHandle);
    GetWindowRect(windowHandle, &windowRect);
    int windowWidth = windowRect.right - windowRect.left;
    int windowHeight = windowRect.bottom - windowRect.top;


    StretchDIBits(deviceContext, 
                    0,0, windowWidth, windowHeight, 
                    0,0, b->w, b->h, 
                    b->buffer,
                    bmpinfo, DIB_RGB_COLORS, SRCCOPY);

    ReleaseDC(windowHandle, deviceContext);
}

void rendererTest2D(){
    WNDCLASSA windowClass = {};
    windowClass.lpszClassName = "windowclass";
    windowClass.lpfnWndProc = win32_windowProc;
    windowClass.hInstance = 0;


    RegisterClassA(&windowClass);

    const int w = 1280, h = 720;
    er_Renderer2D r(w,h);

    HWND windowHandle = CreateWindowExA(
        0, windowClass.lpszClassName, 
                "Window-er",
                WS_OVERLAPPEDWINDOW|WS_VISIBLE,
                CW_USEDEFAULT,CW_USEDEFAULT,
                CW_USEDEFAULT,CW_USEDEFAULT,
                0,0,
                windowClass.hInstance,
                0);

    BITMAPINFO bmpinfo = {};
    bmpinfo.bmiHeader.biCompression = BI_RGB;
    bmpinfo.bmiHeader.biHeight = h;
    bmpinfo.bmiHeader.biWidth = w;
    bmpinfo.bmiHeader.biSize = sizeof(bmpinfo.bmiHeader);
    bmpinfo.bmiHeader.biBitCount = 32;
    bmpinfo.bmiHeader.biPlanes = 1;

    Vec3i color;
    int * pcolor = &color.x;
    uint32_t tics = 0;
    float framecounter = 0;
    float timecounter = 0;
    float angle = 0;

    const float fpsCap = 60.0f;
    const float frameLimit = 1.0f/fpsCap;
    const float multiplier = 280/fpsCap;

    Vec3f triangle[3]={
        {400,400,1}, {450,400,1}, {400,450,1}
    };
    

    if (windowHandle){
        bool isRunning = true;
        MSG message;
        while (isRunning){
            n_time framestart = get_current_time();

            while (PeekMessageA(&message, 0, 0,0,PM_REMOVE)){
                if (message.message == WM_QUIT){
                    isRunning = false;
                }
                TranslateMessage(&message);
                DispatchMessageA(&message);
            }
            r.buffer.clearAll(RGB_TO_U32(color.x, color.y, color.z));
            // r.buffer.clearAll(RGB_TO_BMP_U32(255,0,255));

            Mat3 m = rotateAboutPoint2D(angle, Vec2f{500,500});
            Vec3f transformed[3] = {
                m*triangle[0],
                m*triangle[1],
                m*triangle[2]
            };



            fillCircle(&r, transformed[0].xy(), 0.5f*len(transformed[1].xy()), color.y);
            fillTriangle(&r, transformed[0].xy(), transformed[1].xy(), transformed[2].xy(), 0xffffff00);
            // fillCircle(&r, transformed[0].xy(), 50, RGB_TO_BMP_U32(0,255,255));

            win32_display(windowHandle, &r.buffer, &bmpinfo);
            
            color.y = lerp(color.x, color.z, float(tics)/UINT32_MAX);
            angle += 0.5 * multiplier; 
            tics += 1 * multiplier;
            framecounter++;
            float frametime = t_diff(framestart, get_current_time());
            timecounter += frameLimit;
            printf("FPS : %0.4f  ", framecounter/timecounter);

            if(frametime < frameLimit)
                Sleep((frameLimit - frametime)*1000.0f);
        }
    }
}

int interpolationTest(er_Renderer3D *r){
    Triangle3DEx t;
    t.a = {{500,500,500}, {1,0,0}};
    t.b = {{400,45,100}, {0,1,0}};
    t.c = {{750,300,900}, {0,0,1}};

    computeTriangle(r,t.a,t.b,t.c);
    t.a = {{900,351,500}, {1,0.54,0.67}};
    t.b = {{26,490,100}, {0,1,0.34}};
    t.c = {{15,35,900}, {0.55,0,1}};
    computeTriangle(r,t.a,t.b,t.c);
    return 0;
}   






void projectiontest(er_Renderer3D *r){
    Triangle3DEx t;
    t.a = {{100,100,200}, {1,0,0}};
    t.b = {{40,50,400}, {1,0,0}};
    t.c = {{220,40,200}, {1,0,0}};
    computeTriangle(r,t.a,t.b,t.c);
    
    t.a = {{500,500,1000}, {0,1,0}};
    t.b = {{400,500,1000}, {0,1,0}};
    t.c = {{500,400,1000}, {0,1,0}};
    computeTriangle(r,t.a,t.b,t.c);


}



void mathTest(){
    float start = 0.01;
    float end = 1000.0f;
    float t = start;
    float step = 0.001f;

    while (t < end){
        printf("t: %f\t QSQRT %f \t 1/SQRT %f\n", t, Q_rsqrt(t), 1.0f/sqrtf(t));
        t += step;
    }
}

void newFrame(er_Renderer3D *r){
    r->framebuffer.clearAll_memset(0x00);
    r->zBuffer.clearAll_memset(0x00);
}



Camera3D c;


void textureTests(er_Renderer3D *r){
    TextureShader a;
    a.view = c.lookAt();
    useShader(r, &a);
    Triangle3DEx t;
    t.a = {{-55,50,-100}, {1,0,0}, {0,0}};
    t.b = {{50,-50,-100}, {0,1,0}, {1,1}};
    t.c = {{500,500,-100}, {0,0,1}, {1,0}};
    computeTriangle(r,t.a,t.b,t.c);
    // t.a = {{-50,50,-600}, {0,1,0}, {0,0}};
    // t.b = {{50,-50,-600}, {0,1,0}, {1,1}};
    // t.c = {{50,50,-600}, {0,1,0}, {1,0}};
    // computeTriangle(r,t.a,t.b,t.c);
    
}

void perspectiveCorrectTests(er_Renderer3D *r){
    TestShader a;
    useShader(r, &a);
    Triangle3DEx t;
    t.a = {{-5.5,5,-10}, {1,0,0}, {0,0}};
    t.b = {{5,-5,-10}, {0,1,0}, {1,1}};
    t.c = {{50,50,-100}, {0,0,1}, {1,0}};
    computeTriangle(r,t.a,t.b,t.c);
    // t.a = {{-50,50,-600}, {0,1,0}, {0,0}};
    // t.b = {{50,-50,-600}, {0,1,0}, {1,1}};
    // t.c = {{50,50,-600}, {0,1,0}, {1,0}};
    // computeTriangle(r,t.a,t.b,t.c);
    
}


void cubeTest(er_Renderer3D *r){

    TestShader a;
    useShader(r, &a);
    a.view = c.lookAt();

    Point points[ARRAY_COUNT(CUBES::vertices)];
    for (int i =0; i<ARRAY_COUNT(CUBES::vertices); i++){
        points[i].pos = CUBES::vertices[i];
        points[i].color = colors[i%ARRAY_COUNT(colors)];
        // points[i].color = colors[0];
    }
    
    const int rows = 5;
    const int columns = 5;

    Mat4 s = scaleAboutOrigin(200,200,200);
    for (int y=0; y<rows; y++){
        for (int x=0; x<columns; x++){
            a.model = translate3D((x-columns/2)*100, (y-rows/2)*100,0) * s;
            displayMesh(r, points, ARRAY_COUNT(CUBES::vertices), CUBES::indices, ARRAY_COUNT(CUBES::indices));
        }
    }
    useShader(r, 0);
    // displayMesh(r, points, ARRAY_COUNT(CubeMesh::vertices), CubeMesh::indices, 6);
}


void lightingTest(er_Renderer3D *r){
    const int rows = 1;
    const int columns = 1;

    struct Scene{
        PointLight pLights;
        Cube cubes[rows][columns];
        Cube lightCube;
    }scene;


    TestShader defaultShader;
    useShader(r, &defaultShader);
    defaultShader.view = c.lookAt();    

    scene.pLights = PointLight(Vec3f{-100,0,-125});
    scene.lightCube.center = scene.pLights.lightPos;
    scene.lightCube.radii[0] = 25.0f;
    scene.lightCube.radii[1] = 25.0f;
    scene.lightCube.radii[2] = 25.0f;
    
    renderCube(r, scene.lightCube, Colors::WHITE);
    
    LightingShader a;
    // TestShader a;
    useShader(r, &a);
    a.cameraPos = c.pos;
    a.nPointLights = 1;
    a.pointLights = &scene.pLights;
    a.view = c.lookAt();    


    for (int y=0; y<rows; y++){
        for (int x=0; x<columns; x++){
            scene.cubes[y][x].center = Vec3f{(x-columns/2)*100.0f, (y-rows/2)*100.0f,0.0f};
            scene.cubes[y][x].radii[0] = 75.0f;
            scene.cubes[y][x].radii[1] = 75.0f;
            scene.cubes[y][x].radii[2] = 75.0f;
            renderCube(r, scene.cubes[y][x], Colors::GREEN);
        }
    }
    useShader(r, 0);

}

void modelTest(er_Renderer3D *r, ObjFileInfo *f){
    struct Scene{
        PointLight pLights;
        Cube lightCube;
    }scene;
    
    ObjRenderShader defaultShader;
    useShader(r, &defaultShader);

    r->shader->model = translate3D(0,-150,0) * scaleAboutOrigin(1,1,1);
    r->shader->view = c.lookAt();
    defaultShader.cameraPos = c.pos;
    defaultShader.nPointLights = 1;
    defaultShader.pointLights = &scene.pLights;

    scene.pLights = PointLight(Vec3f{150,500,150});
    scene.lightCube.center = scene.pLights.lightPos;
    scene.lightCube.radii[0] = 25.0f;
    scene.lightCube.radii[1] = 25.0f;
    scene.lightCube.radii[2] = 25.0f;


    
    for (auto mesh: f->meshes){
        for (auto submesh: mesh.renderInfo){
            defaultShader.color = f->mtl.materials[submesh.materialIndex].kd;
            displayMesh(r, &f->vertices[0], f->vertices.size(), 
                            &f->normals[0], f->normals.size(), 
                            &submesh.vIndices[0], &submesh.nIndices[0], submesh.vIndices.size());
        }
    } 
}


void cubeSs(er_Renderer3D *r){
    TestShader defaultShader;
    useShader(r, &defaultShader);

    r->shader->model = scaleAboutOrigin(100,100,100);
    r->shader->view = c.lookAt();
    displayMesh(r, CUBES::vertices,ARRAY_COUNT(CUBES::vertices),0,0, CUBES::indices,0, ARRAY_COUNT(CUBES::indices));
}




void BMPTests(){
    const int scaleDown = 2; 
    const int w = 1280/scaleDown, h = 720/scaleDown;
    er_Renderer3D r(w,h);
    BMP_File *f = newBMP(w,h,r.framebuffer.buffer,32);

    c.right = Vec3f{1,0,0};
    c.up = Vec3f{0,1,0};
    c.front = Vec3f{0,0,1};
    
    c.pos = Vec3f{0,0,50};

    newFrame(&r);

    // interpolationTest(&r);
    // projectiontest(&r);
    cubeTest(&r);
    // lightingTest(&r);
    // writeBMP(f, "rendercubetest.bmp", BMP_WRITE_NONE);
    // textureTests(&r);
    // writeBMP(f, "textureTest.bmp", BMP_WRITE_NONE);


}







void rendererTest3DEx(){
    WNDCLASSA windowClass = {};
    windowClass.lpszClassName = "windowclass";
    windowClass.lpfnWndProc = win32_windowProc;
    windowClass.hInstance = 0;


    RegisterClassA(&windowClass);

    const float scaleDown = 1.5; 
    const int w = 1280/scaleDown, h = 720/scaleDown;
    er_Renderer3D r(w,h);

    HWND windowHandle = CreateWindowExA(
        0, windowClass.lpszClassName, 
                "Window-er",
                WS_OVERLAPPEDWINDOW|WS_VISIBLE,
                CW_USEDEFAULT,CW_USEDEFAULT,
                CW_USEDEFAULT,CW_USEDEFAULT,
                0,0,
                windowClass.hInstance,
                0);

    BITMAPINFO bmpinfo = {};
    bmpinfo.bmiHeader.biCompression = BI_RGB;
    bmpinfo.bmiHeader.biHeight = h;
    bmpinfo.bmiHeader.biWidth = w;
    bmpinfo.bmiHeader.biSize = sizeof(bmpinfo.bmiHeader);
    bmpinfo.bmiHeader.biBitCount = 32;
    bmpinfo.bmiHeader.biPlanes = 1;

    Vec3i color;
    int * pcolor = &color.x;
    uint32_t tics = 0;
    float framecounter = 0;
    float timecounter = 0;
    float angle = 0;

    const float fpsCap = 60.0f;
    const float frameLimit = 1.0f/fpsCap;
    const float multiplier = 280/fpsCap;

    c.right = Vec3f{1,0,0};
    c.up = Vec3f{0,1,0};
    c.front = Vec3f{0,0,1};
    
    float rad = 300.0f;
    c.pos = Vec3f{0,50,rad};

    Vec3f target = Vec3f{0,0,0};

    ObjFileInfo f = loadObj(".\\models\\goose.obj");

    


    if (windowHandle){
        bool isRunning = true;
        MSG message;
        while (isRunning){
            n_time framestart = get_current_time();

            while (PeekMessageA(&message, 0, 0,0,PM_REMOVE)){
                if (message.message == WM_QUIT){
                    isRunning = false;
                }
                TranslateMessage(&message);
                DispatchMessageA(&message);
            }
            // r.framebuffer.clearAll(RGB_TO_U32(color.x, color.y, color.z));
            // r.zBuffer.clearAll_memset(0xff);

            newFrame(&r);

            c.pos = Vec3f{rad * cosf(Radians(angle)), 0, rad * sinf(Radians(angle))};
            c.updateOrientation(target, Vec3f{0,1,0});



            // cubeSs(&r);
            modelTest(&r, &f);
            // lightingTest(&r);
            // cubeTest(&r);
            // textureTests(&r);
            // fillCircle(&r, transformed[0].xy(), 50, RGB_TO_BMP_U32(0,255,255));

            win32_display(windowHandle, &r.framebuffer, &bmpinfo);
            
            
            color.y = lerp(color.x, color.z, float(tics)/UINT32_MAX);
            angle += 0.5 * multiplier; 
            tics += 1 * multiplier;
            framecounter++;
            float frametime = t_diff(framestart, get_current_time());
            timecounter += frametime;
            printf("FPS : %0.4f  ", framecounter/timecounter);

            if(frametime < frameLimit)
                Sleep((frameLimit - frametime)*1000.0f);
        }
    }
}





// #define test
#ifdef test
#define testMain main
#endif


int testMain(){
    // rendererTest2D();
    rendererTest3DEx();
    
    BMPTests();
    return 0;
    
}