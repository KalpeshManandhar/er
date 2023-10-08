#include <math/math_er.h>
#include <Renderer/2D/renderer.h>
#define BMP_USE_VALUES
#include "bmp.h"
#include "timeMeasure.h"

#include <stdio.h>
#include <Windows.h>



#define RGB_TO_BMPHEX(r,g,b) ((0xff000000) | (uint8_t(b)) | (uint8_t(g)<<8) | (uint8_t(r)<<16))


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

    default:
        break;
    }
    return(DefWindowProc(windowHandle, message, wparam, lparam));
}

void win32_display(HWND windowHandle, er_Renderer2D *r, BITMAPINFO *bmpinfo){

    RECT windowRect = {};
    HDC deviceContext = GetWindowDC(windowHandle);
    GetWindowRect(windowHandle, &windowRect);
    int windowWidth = windowRect.right - windowRect.left;
    int windowHeight = windowRect.bottom - windowRect.top;


    StretchDIBits(deviceContext, 
                    0,0, windowWidth, windowHeight, 
                    0,0, r->buffer.w, r->buffer.h, 
                    r->buffer.buffer,
                    bmpinfo, DIB_RGB_COLORS, SRCCOPY);

    ReleaseDC(windowHandle, deviceContext);
}

void rendererTest(){
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
    float fps = 0;
    float framecounter = 0;
    float timecounter = 0;
    float angle = 0;

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
            r.buffer.clearAll(RGB_TO_BMPHEX(color.x, color.y, color.z));
            // r.buffer.clearAll(RGB_TO_BMPHEX(255,0,255));

            Mat3 m = rotateAboutPoint2D(angle, Vec2f{500,500});
            Vec3f transformed[3] = {
                m*triangle[0],
                m*triangle[1],
                m*triangle[2]
            };



            fillTriangle(&r, transformed[0].xy(), transformed[1].xy(), transformed[2].xy(), 0xffffff00);
            win32_display(windowHandle, &r, &bmpinfo);
            
            color.y = lerp(color.x, color.z, float(tics)/UINT32_MAX);
            angle += 0.5; 
            tics += 10;
            framecounter++;
            timecounter += t_diff(framestart, get_current_time());
            // printf("FPS : %0.4f  ", framecounter/timecounter);
        }
    }
    
    // BMP_File *f = newBMP(w,h, r.buffer.buffer, 32);
    // writeBMP(f, "before.bmp", BMP_WRITE_NONE);


    // writeBMP(f, "after.bmp", BMP_WRITE_NONE);

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


int main(){
    rendererTest();
    return 0;
    
}