#include <windows.h>
#include <stdio.h>

LRESULT CALLBACK WindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
    LRESULT result = 0;

    // See: https://docs.microsoft.com/en-us/windows/desktop/winmsg/about-messages-and-message-queues#system_defined
    switch(message) {
        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        case WM_PAINT:
        case WM_ACTIVATEAPP:
        case WM_CLOSE:
        case WM_QUIT:
        case WM_SIZE:
        case WM_WINDOWPOSCHANGED:
        default:
            result = DefWindowProc(window, message, wParam, lParam);
    }
    return result;
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX wc = {0};

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = &WindowProc;
    wc.hInstance = hInstance;
    //wc.hIcon = TODO;
    //wc.hCursor = TODO;
    wc.lpszClassName = "MainWindow";

    // TODO: check result
    RegisterClassEx(&wc);

    // TODO: check result
    HWND window = CreateWindowEx(
        0, // dwExStyle,
        wc.lpszClassName, // lpClassName,
        "Quest",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE, // dwStyle, https://docs.microsoft.com/en-us/windows/desktop/winmsg/window-styles
        CW_USEDEFAULT, // X,
        CW_USEDEFAULT, // Y,
        CW_USEDEFAULT, // nWidth,
        CW_USEDEFAULT, // nHeight,
        0, // hWndParent,
        0, // hMenu,
        hInstance, // hInstance,
        0);
    (void)window;

    // https://docs.microsoft.com/en-us/windows/desktop/api/winuser/nf-winuser-getmessage
    for (;;) {
        MSG message;
        BOOL result = GetMessage(&message, 0, 0, 0);

        if (result == 0) {
            // TODO: Quit case
            break;
        }

        if (result == -1) {
            // TODO: Error case
            break;
        }

        // Continue case
        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    return 0;
}

int main(int argc, char* argv[]) {
    printf("Hello Windows\n");
    return WinMain(GetModuleHandle(0), 0, GetCommandLine(), 10);
}

