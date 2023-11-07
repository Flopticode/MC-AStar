#include "WindowDisplay.h"
#include <Windows.h> 
#include <iostream>
#include <thread>
#include <mutex>
#include <functional>
#include "GraphicsTypes.h"

#define CLASS_NAME L"MyWindowClass"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

static std::function<Bitmap* ()> onDraw;
static bool manualRepaint = false;
static HWND window;
static Bitmap* lastBitmap = nullptr;

BOOL CALLBACK GetWorkerW(HWND hwnd, LPARAM lParam) {

    HWND p = FindWindowEx(hwnd, NULL, L"SHELLDLL_DefView", NULL);
    HWND* ret = (HWND*)lParam;

    if (p && *ret == NULL)
    {
        *ret = FindWindowEx(NULL, hwnd, L"WorkerW", NULL);
    }
    return true;

}

int WindowHandler::init(InitMode initMode, bool pManualRepaint, std::function<Bitmap* ()> pOnDraw)
{
    manualRepaint = pManualRepaint;
    onDraw = pOnDraw;

    auto t = std::thread([initMode](std::string message) {
        HINSTANCE hInstance = (HINSTANCE)GetWindowLongPtr(
            GetConsoleWindow(),
            GWLP_HINSTANCE);

        WNDCLASS wc = {};
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = hInstance;
        wc.lpszClassName = CLASS_NAME;

        if (!RegisterClass(&wc))
        {
            std::cerr << "Failed to register window class\n";
            return -1;
        }

        window = CreateWindowEx(
            0,
            CLASS_NAME,
            L"My Window",
            WS_POPUP | WS_VISIBLE,
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
            GetDesktopWindow(),
            NULL,
            hInstance,
            NULL
        );
        if (!window)
        {
            std::cerr << "Failed to create window\n";
            return -1;
        }

        if (initMode == InitMode::WALLPAPER)
        {
            HWND hDesktop = FindWindow(L"Progman", NULL);
            if (!hDesktop)
            {
                return -1;
            }
            SendMessageTimeout(hDesktop, 0x052C, NULL, NULL, SMTO_NORMAL, 1000, NULL);
            HWND hWorkerW = NULL;
            EnumWindows(GetWorkerW, (LPARAM)&hWorkerW);

            if (!hWorkerW)
                return -1;

            SetParent(window, hWorkerW);
        }

        ShowWindow(window, SW_SHOWMAXIMIZED);

        SetWindowPos(window, HWND_TOP,
            0, 0,
            1920, 1080,
            SWP_SHOWWINDOW);

        if (!manualRepaint)
        {
            HDC hdc = GetDC(window);
            int vrefresh = GetDeviceCaps(hdc, VREFRESH);
            ReleaseDC(window, hdc);
            SetTimer(window, 1, 1000 / vrefresh, NULL);
        }

        MSG msg = {};
        while (GetMessage(&msg, NULL, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        }, "Hello");
    t.detach();

    return 0;
}

void WindowHandler::repaint()
{
    InvalidateRect(window, NULL, FALSE);
    UpdateWindow(window);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        auto bitmap = onDraw();
        if (bitmap != lastBitmap && lastBitmap != nullptr)
        {
            delete lastBitmap;
        }

        BITMAPINFO bmi;
        ZeroMemory(&bmi, sizeof(BITMAPINFO));
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = bitmap->width;
        bmi.bmiHeader.biHeight = -bitmap->height;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = bitmap->getBitsPerPixel();
        bmi.bmiHeader.biCompression = BI_RGB;

        HBITMAP hbm = CreateDIBitmap(hdc, &bmi.bmiHeader, CBM_INIT, bitmap->pixels, &bmi, DIB_RGB_COLORS);
        HDC hdcMem = CreateCompatibleDC(hdc);
        HGDIOBJ oldbm = SelectObject(hdcMem, hbm);

        BitBlt(hdc, 0, 0, bitmap->width, bitmap->height, hdcMem, 0, 0, SRCCOPY);

        SelectObject(hdcMem, oldbm);
        DeleteDC(hdcMem);
        DeleteObject(hbm);

        EndPaint(hwnd, &ps);

        lastBitmap = bitmap;
    } break;
    case WM_DESTROY:
    {
        PostQuitMessage(0);
    } return 0;
    case WM_CREATE:
    {
        HDC hdc = GetDC(hwnd);
        int vrefresh = GetDeviceCaps(hdc, VREFRESH);
        SetTimer(hwnd, 1, 1000 / vrefresh, NULL);
    } break;
    case WM_TIMER:
    {
        WindowHandler::repaint();
    } break;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

void WindowHandler::update()
{

}
