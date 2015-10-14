
#include <windows.h>
#include <GL/GL.h>
#include "glext.h"
#include <slib.h>

global b32 GlobalRunning;

internal LRESULT CALLBACK
MainWindowCallback(HWND Window,
                   UINT Message,
                   WPARAM wParam,
                   LPARAM lParam)
{
    LRESULT Result = 0;

    switch(Message)
    {
        case WM_CLOSE:
        case WM_DESTROY:
        {
            GlobalRunning = false;
        } break;
        default:
        {
            Result = DefWindowProcA(Window, Message, wParam, lParam);
        } break;
    }

    return Result;
}

internal void
ProcessPendingMessages()
{
    MSG Message;
    while(PeekMessageA(&Message, 0, 0, 0, PM_REMOVE))
    {
        switch(Message.message)
        {
            case WM_QUIT:
            {
                GlobalRunning = false;
            } break;

            case WM_SYSKEYDOWN:
            case WM_SYSKEYUP:
            case WM_KEYDOWN:
            case WM_KEYUP:
            {
                u32 VKCode = (u32)Message.wParam;
                b32 WasDown = (Message.lParam & (1 << 30)) != 0;
                b32 IsDown = (Message.lParam & (1 << 31)) == 0;
                if(WasDown != IsDown)
                {
                    if(VKCode == VK_UP)
                    {
                    }
                    else if(VKCode == VK_LEFT)
                    {
                    }
                    else if(VKCode == VK_DOWN)
                    {
                    }
                    else if(VKCode == VK_RIGHT)
                    {
                    }
                    else if(VKCode == VK_ESCAPE)
                    {
                    }
                    else if(VKCode == VK_SPACE)
                    {
                    }
                }

                if(IsDown)
                {
                    b32 AltKeyIsDown = (Message.lParam & (1 << 29));
                    if ((VKCode == VK_F4) && AltKeyIsDown)
                    {
                        GlobalRunning = false;
                    }
                }
            } break;
            default:
            {
                TranslateMessage(&Message);
                DispatchMessageA(&Message);
            } break;
        };
    }
}

internal HGLRC
InitializeOpenGL(HWND Window, HDC DeviceContext)
{
    PIXELFORMATDESCRIPTOR PixelFormatDescriptor =
        {
            sizeof(PIXELFORMATDESCRIPTOR),
            1,
            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
            PFD_TYPE_RGBA,            //The kind of framebuffer. RGBA or palette.
            32,                        //Colordepth of the framebuffer.
            0, 0, 0, 0, 0, 0,
            0,
            0,
            0,
            0, 0, 0, 0,
            24,                        //Number of bits for the depthbuffer
            8,                        //Number of bits for the stencilbuffer
            0,                        //Number of Aux buffers in the framebuffer.
            PFD_MAIN_PLANE,
            0,
            0, 0, 0
        };

    s32 PixelFormatIndex = ChoosePixelFormat(DeviceContext, &PixelFormatDescriptor);
    SetPixelFormat(DeviceContext, PixelFormatIndex, &PixelFormatDescriptor);

    HGLRC GLRenderContext = wglCreateContext(DeviceContext);
    wglMakeCurrent(DeviceContext, GLRenderContext);

    return GLRenderContext;
}

PFNGLGENBUFFERSPROC glGenBuffers;
PFNGLBINDBUFFERPROC glBindBuffer;
PFNGLBUFFERDATAPROC glBufferData;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;

internal void
LoadGLFunctions()
{
    glGenBuffers = (PFNGLGENBUFFERSPROC) wglGetProcAddress("glGenBuffers");
    glBindBuffer = (PFNGLBINDBUFFERPROC) wglGetProcAddress("glBindBuffer");
    glBufferData = (PFNGLBUFFERDATAPROC) wglGetProcAddress("glBufferData");;
    glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
    glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
    glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glDisableVertexAttribArray");
}

global GLfloat TriangleVerts[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f,  0.5f, 0.0f,
};
    
global GLuint VertexBuffer;

internal void
SetupTriangle()
{
    glGenBuffers(1, &VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(TriangleVerts),
                 TriangleVerts,
                 GL_STATIC_DRAW);
}

internal void
Render(HGLRC GLRenderContext)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,VertexBuffer);
    glVertexAttribPointer(
        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
                          );
 
    glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
 
    glDisableVertexAttribArray(0);
}

s32 CALLBACK
WinMain(HINSTANCE instance,
        HINSTANCE, LPSTR, s32)
{
    WNDCLASS WindowClass = {};
    WindowClass.style = CS_HREDRAW|CS_VREDRAW;
    WindowClass.lpfnWndProc = MainWindowCallback;
    WindowClass.hInstance = instance;
    WindowClass.lpszClassName = "Win32WindowClass";
    WindowClass.hCursor = LoadCursor(0, IDC_ARROW);
    WindowClass.hIcon = LoadIcon(0, IDI_APPLICATION);

    if(RegisterClassA(&WindowClass))
    {
        HWND Window =
            CreateWindowExA(
                0,
                WindowClass.lpszClassName,
                "Steven's Awesome App",
                WS_OVERLAPPEDWINDOW|WS_VISIBLE,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                0,0,instance,0);
        if(Window)
        {
            HDC DeviceContext = GetDC(Window);

            HGLRC GLRenderContext = InitializeOpenGL(Window, DeviceContext);

            GlobalRunning = true;

            LoadGLFunctions();
            SetupTriangle();
            glClearColor(0.0f,1.0f,0.0f,1.0f);

            while(GlobalRunning)
            {
                ProcessPendingMessages();
                Render(GLRenderContext);
                SwapBuffers(DeviceContext);
            }
            
            wglMakeCurrent(DeviceContext, 0);
            wglDeleteContext(GLRenderContext);

            ReleaseDC(Window, DeviceContext);
        }
    }
        
    return 0;
}
