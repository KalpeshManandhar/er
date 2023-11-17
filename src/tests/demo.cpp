#include <math/math_er.h>

#include <Renderer/2D/renderer3D.h>
#include <Renderer/2D/camera.h>
#include <Renderer/2D/lights.h>

#include <Loader/objLoader.h>

#include "./shaders/shaders.h"

#include <Windows.h>

#define WINDOW_W 1280
#define WINDOW_H 720

struct Win32_windowInfo{
    HWND windowHandle;
    BITMAPINFO bitmapInfo;
};

static LRESULT CALLBACK win32_windowProc(HWND windowHandle, UINT message, WPARAM wparam, LPARAM lparam){
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



static void win32_display(HWND windowHandle, er_Buffer2D *b, BITMAPINFO *bmpinfo){
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


static void newFrame(er_Renderer3D *r){
    r->framebuffer.clearAll_memset(0x00);
    r->zBuffer.clearAll_memset(0x00);
}



static Win32_windowInfo initWindow(const char* title, int windowW, int windowH, int bufferW, int bufferH) {
    WNDCLASSA windowClass = {};
    windowClass.lpszClassName = "windowclass";
    windowClass.lpfnWndProc = win32_windowProc;
    windowClass.hInstance = 0;


    RegisterClassA(&windowClass);

    HWND windowHandle = CreateWindowExA(
        0, windowClass.lpszClassName,
        title,
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        windowW, windowH,
        0, 0,
        windowClass.hInstance,
        0);

    BITMAPINFO bmpinfo = {};
    bmpinfo.bmiHeader.biCompression = BI_RGB;
    bmpinfo.bmiHeader.biHeight = bufferH;
    bmpinfo.bmiHeader.biWidth = bufferW;
    bmpinfo.bmiHeader.biSize = sizeof(bmpinfo.bmiHeader);
    bmpinfo.bmiHeader.biBitCount = 32;
    bmpinfo.bmiHeader.biPlanes = 1;
    Win32_windowInfo win;
    win.windowHandle = windowHandle;
    win.bitmapInfo = bmpinfo;
    return win;

}






static Camera3D c;

static void displayObj(er_Renderer3D *r, ObjFileInfo *f){
    struct Scene{
        PointLight pLights;
        Cube lightCube;
    }scene;
    
    ObjRenderShader objShader;
    useShader(r, &objShader);

    Vec3f objDimensions = f->max - f->min;
    float maxDim = Max(Max(objDimensions.x, objDimensions.y), objDimensions.z);
    float scale = 200.0f/maxDim;

    Vec3f objCenter = 0.5f * (f->max + f->min);


    r->shader->model =  scaleAboutOrigin(scale,scale,scale) * translate3D(-objCenter.x, -objCenter.y, -objCenter.z);
    r->shader->view = c.lookAt();
    objShader.cameraPos = c.pos;
    objShader.nPointLights = 1;
    objShader.pointLights = &scene.pLights;

    scene.pLights = PointLight(Vec3f{150,500,150});
    scene.lightCube.center = scene.pLights.lightPos;
    scene.lightCube.radii[0] = 25.0f;
    scene.lightCube.radii[1] = 25.0f;
    scene.lightCube.radii[2] = 25.0f;


    
    for (auto mesh: f->meshes){
        for (auto submesh: mesh.renderInfo){
            objShader.color = f->mtl.materials[submesh.materialIndex].kd;
            displayMesh(r, &f->vertices[0], f->vertices.size(), 
                            &f->normals[0], f->normals.size(), 
                            &submesh.vIndices[0], &submesh.nIndices[0], submesh.vIndices.size());
        }
    } 
}


void er_DisplayModel(const char *path){
    const float scaleDown = 1.5; 
    const int w = WINDOW_W/scaleDown, h = WINDOW_H/scaleDown;
    
    er_Renderer3D r(w,h);
    
    Win32_windowInfo win = initWindow("er", WINDOW_W, WINDOW_H, w, h);

    float angle = 0;

    c.right = Preset::X_AXIS;
    c.up = Preset::Y_AXIS;
    c.front = Preset::Z_AXIS;
    
    float rad = 300.0f;
    c.pos = Vec3f{0,50,rad};
    Vec3f target = Vec3f{0,0,0};


    ObjFileInfo f = loadObj(path);



    if (win.windowHandle){
        bool isRunning = true;
        MSG message;
        while (isRunning){
            while (PeekMessageA(&message, 0, 0,0,PM_REMOVE)){
                if (message.message == WM_QUIT){
                    isRunning = false;
                }
                TranslateMessage(&message);
                DispatchMessageA(&message);
            }

            newFrame(&r);

            displayObj(&r, &f);

            c.pos = Vec3f{rad * cosf(Radians(angle)), 0, rad * sinf(Radians(angle))};
            c.updateOrientation(target, Vec3f{0,1,0});



            win32_display(win.windowHandle, &r.framebuffer, &win.bitmapInfo);
            
            
            angle += 0.5; 
        }
    }
}

#ifdef demo
#define demomain main
#endif

int demomain(int argc, char **argv){
    if (argc < 2){
        printf("Usage: ./demo.exe <path to obj>");
        return -1;
    }
    er_DisplayModel(argv[1]);
    // er_DisplayModel("./models/plant.obj");

    return 0;
}