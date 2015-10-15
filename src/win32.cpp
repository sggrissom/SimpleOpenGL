
#include <windows.h>
#include <GL/GL.h>
#include "glext.h"
#include <slib.h>

struct graphics_buffer
{
    void *Memory;
    u32 Width;
    u32 Height;
    u32 Pitch;
};

internal void
SetupBuffer(graphics_buffer *Buffer, u32 Width, u32 Height)
{
    if(Buffer->Memory)
    {
        VirtualFree(Buffer->Memory, 0, MEM_RELEASE);
    }

    Buffer->Width = Width;
    Buffer->Height = Height;

    u32 BytesPerPixel = 4;
    u32 BitmapMemorySize = (Buffer->Width*Buffer->Height)*BytesPerPixel;
    Buffer->Memory = VirtualAlloc(0, BitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);
    Buffer->Pitch = Width*BytesPerPixel;
}

internal void
RenderWeirdGradient(graphics_buffer *Buffer, s32 BlueOffset, s32 GreenOffset)
{
    u8 *Row = (u8 *)Buffer->Memory;    
    for(u32 Y = 0;
        Y < Buffer->Height;
        ++Y)
    {
        u32 *Pixel = (u32 *)Row;
        for(u32 X = 0;
            X < Buffer->Width;
            ++X)
        {
            u8 Blue = (u8)(X + BlueOffset);
            u8 Green = (u8)(Y + GreenOffset);

            *Pixel++ = ((Green << 8) | Blue);
        }
        
        Row += Buffer->Pitch;
    }
}

global b32 GlobalRunning;
global graphics_buffer GlobalGraphicsBuffer = {};

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

PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers;
PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer;
PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage;
PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer;
PFNGLBLITFRAMEBUFFERPROC glBlitFramebuffer;

internal void
LoadGLFunctions()
{
    glGenBuffers = (PFNGLGENBUFFERSPROC) wglGetProcAddress("glGenBuffers");
    glBindBuffer = (PFNGLBINDBUFFERPROC) wglGetProcAddress("glBindBuffer");
    glBufferData = (PFNGLBUFFERDATAPROC) wglGetProcAddress("glBufferData");;
    glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
    glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
    glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glDisableVertexAttribArray");
    glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)wglGetProcAddress("glGenFramebuffers");
    glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)wglGetProcAddress("glBindFramebuffer");
    glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)wglGetProcAddress("glGenRenderbuffers");
    glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)wglGetProcAddress("glBindRenderbuffer");
    glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)wglGetProcAddress("glRenderbufferStorage");
    glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)wglGetProcAddress("glFramebufferRenderbuffer");
    glBlitFramebuffer = (PFNGLBLITFRAMEBUFFERPROC)wglGetProcAddress("glBlitFramebuffer");
}

GLuint fbo, rbo;

internal void
Setup(u32 Width, u32 Height)
{
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, Width, Height);
    
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                              GL_COLOR_ATTACHMENT0,
                              GL_RENDERBUFFER,
                              rbo);
}


internal void
RenderToScreen(HGLRC GLRenderContext, u32 Width, u32 Height, GLvoid *Pixels)
{
    glClearColor(0.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glRasterPos2d(0,0);
    glDrawPixels(Width, Height, GL_RGBA8, GL_UNSIGNED_BYTE, Pixels);
    
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glDrawBuffer(GL_BACK);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
    glReadBuffer(GL_COLOR_ATTACHMENT0);

    glBlitFramebuffer(0,0,Width,Height,
                      0,0,Width,Height,
                      GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT,
                      GL_NEAREST);
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

    u32 Width = 100;
    u32 Height = 100;

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
            Setup(Width, Height);

            u32 XOffset = 0, YOffset = 0;

            while(GlobalRunning)
            {
                ProcessPendingMessages();

                RenderWeirdGradient(&GlobalGraphicsBuffer, XOffset, YOffset);
               
                RenderToScreen(GLRenderContext, Width, Height, GlobalGraphicsBuffer.Memory);
                SwapBuffers(DeviceContext);

                ++XOffset;
                YOffset += 2;
            }
            
            wglMakeCurrent(DeviceContext, 0);
            wglDeleteContext(GLRenderContext);

            ReleaseDC(Window, DeviceContext);
        }
    }
        
    return 0;
}
