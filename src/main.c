#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#if 0
typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
} rgba;
#endif

typedef struct {
    unsigned char b;
    unsigned char g;
    unsigned char r;
    unsigned char a;
} bgra;

static BITMAPINFO g_bitmapinfo;
static int g_width;
static int g_height;
static bgra* g_backbuffer;

// Write a gradient to the back buffer
void DrawGradient(int offset) {
    for (int row = 0; row < g_height; row++) {
        for (int col = 0; col < g_width; col++) {
            int index = row * g_width + col;
            g_backbuffer[index].r = (int8_t)(row + offset);
            g_backbuffer[index].g = (int8_t)(col + offset);
            g_backbuffer[index].b = 0;
            g_backbuffer[index].a = 0;
        }
    }
}

void WindowResize(HWND window) {
    // Delete the old backbuffer if we have one already
    if (g_backbuffer) {
        VirtualFree(g_backbuffer, 0, MEM_RELEASE);
    }

    // Get the size of the "client" area of the window (i.e. does not include borders and decorations)
    RECT client_rect;
    GetClientRect(window, &client_rect);
	g_width = client_rect.right - client_rect.left;
	g_height = client_rect.bottom - client_rect.top;

    const int bytes_per_pixel = 4;
    const int buffer_size = g_width * g_height * bytes_per_pixel;

    g_bitmapinfo = (BITMAPINFO){0};
    g_bitmapinfo.bmiHeader.biSize = sizeof(g_bitmapinfo.bmiHeader);
    g_bitmapinfo.bmiHeader.biWidth = g_width;
    g_bitmapinfo.bmiHeader.biHeight = -g_height;
    g_bitmapinfo.bmiHeader.biPlanes = 1;
    g_bitmapinfo.bmiHeader.biBitCount = 8 * bytes_per_pixel;
    g_bitmapinfo.bmiHeader.biCompression = BI_RGB; // i.e. don't compress (in fact we waste 8 bits to maintain alignment)

    // https://msdn.microsoft.com/en-us/library/windows/desktop/aa366887(v=vs.85).aspx
    // TODO: check result
    g_backbuffer = VirtualAlloc(0, buffer_size, MEM_COMMIT, PAGE_READWRITE);

    DrawGradient(100);
}

void WindowPaint(HWND window, bool force) {
    HDC context;
    PAINTSTRUCT state;
    if (force) {
        context = GetDC(window);
    } else {
        context = BeginPaint(window, &state);
    }
#if 0
    // Painting to a window using the ancient GDI api
	int x = state.rcPaint.left;
	int y = state.rcPaint.top;
	int width = state.rcPaint.right - state.rcPaint.left;
	int height = state.rcPaint.bottom - state.rcPaint.top;
    int operation = WHITENESS;
	PatBlt(context, x, y, width, height, operation);
#else
    // Painting to a window using a back buffer in main memory
    // we ignore the update region specified by windows and always paint the entire back buffer
    RECT client_rect;
    GetClientRect(window, &client_rect);
	int width = client_rect.right - client_rect.left;
	int height = client_rect.bottom - client_rect.top;

    // https://docs.microsoft.com/en-us/windows/desktop/api/wingdi/nf-wingdi-stretchdibits
    StretchDIBits(
        context,
        0, 0, width, height,
        0, 0, g_width, g_height,
        g_backbuffer,
        &g_bitmapinfo,
        DIB_RGB_COLORS,
        SRCCOPY);
#endif
    if (force) {
        ReleaseDC(window, context);
    } else {
        EndPaint(window, &state);
    }
}

LRESULT CALLBACK WindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
    LRESULT result = 0;

    // See: https://docs.microsoft.com/en-us/windows/desktop/winmsg/about-messages-and-message-queues#system_defined
    switch(message) {
        case WM_DESTROY:
            //printf("WM_DESTROY: %u\n", message);
            PostQuitMessage(0);
            break;

        case WM_PAINT:
            //printf("WM_PAINT: %u\n", message);
            WindowPaint(window, false);
            goto use_default_handler;

        case WM_ACTIVATEAPP:
            //printf("WM_ACTIVATEAPP: %u\n", message);
            goto use_default_handler;

        case WM_CLOSE:
            // User requested a close (e.g. top right X)
            // TODO: in the future we may not want the default and to instead issue a confirmation
            //printf("WM_CLOSE: %u\n", message);
            goto use_default_handler;

        case WM_QUIT:
            //printf("WM_QUIT: %u\n", message);
            goto use_default_handler;

        case WM_SIZE:
            //printf("WM_SIZE: %u\n", message);
            WindowResize(window);
            break;

        case WM_WINDOWPOSCHANGED:
            //printf("WM_WINDOWPOSCHANGED: %u\n", message);
            goto use_default_handler;

        default:
            use_default_handler:
            result = DefWindowProc(window, message, wParam, lParam);
    }
    return result;
}

bool ProcessAllMessages() {
    MSG message;
    while (PeekMessage(&message, 0, 0, 0, PM_REMOVE)) {

		// Continue case
		TranslateMessage(&message);
		DispatchMessage(&message);

		if (message.message == WM_QUIT)
            return true;
	}
	return false;
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // https://docs.microsoft.com/en-us/windows/desktop/api/winuser/ns-winuser-tagwndclassexa
    WNDCLASSEX wc = {0};

    //char buffer[256];
    //const char* foo = itoa(nCmdShow, buffer, 10);
    //MessageBox(0, lpCmdLine, "Command Line", MB_OK|MB_ICONINFORMATION);
    //MessageBox(0, foo, "CmdShow", MB_OK|MB_ICONINFORMATION);

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

    // https://docs.microsoft.com/en-us/windows/desktop/api/winuser/nf-winuser-peekmessagea
    int offset = 0;
    for (;;) {
        // handle all message triggered updates
        if (ProcessAllMessages())
            break;

        // handle our time triggered updates
        DrawGradient(offset);
        WindowPaint(window, true);
        Sleep(33); // TODO: sleep based on time remaining after processing updates
        offset += 2;
    }

    return 0;
}

int main(int argc, char* argv[]) {
    printf("Hello Windows\n");
    return WinMain(GetModuleHandle(0), 0, GetCommandLine(), 10);
}
