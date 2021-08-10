// servers.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "servers.h"
#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SERVERS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SERVERS));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SERVERS));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SERVERS);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        char buf[4096];
        serverObj.ConnectClient();
        if(serverObj.connected() == false)
        {
            MessageBox(0, L"\nFailed to initialise server socket.", 0, 0);
            return 1;
        }
        serverObj.setHWND(hWnd);
        AfxBeginThread(listenServerThread, 0);
        loadata();
        serverObj.getJson();
        hTextBox = CreateWindow(L"edit", L"", WS_VISIBLE | WS_VSCROLL | WS_CHILD | ES_AUTOVSCROLL | ES_MULTILINE | ES_READONLY, 0, 0, 480, 500, hWnd, 0, hInst, 0);
        break;
    }
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case ID_USER_CONNECT: {
                WCHAR buffer[10000];
                GetWindowText(hTextBox, buffer, 10000);
                if (buffer == NULL)
                {
                    break;
                }
                wcscat_s(buffer, L"\r\n");
                wcscat_s(buffer, L"1 user has just connected to server");
                SetWindowText(hTextBox, buffer);

                SendMessageA(hTextBox, EM_SETSEL, 0, -1);
                SendMessageA(hTextBox, EM_SETSEL, -1, -1);
                SendMessageA(hTextBox, EM_SCROLLCARET, 0, 0);
                break;
            }
            case IDC_LOG_SU: {
                WCHAR buffer[10000];
                GetWindowText(hTextBox, buffer, 10000);
                if (buffer == NULL)
                {
                    break;
                }
                wcscat_s(buffer, L"\r\n");
                wcscat_s(buffer, L"1 user just log in to server");
                SetWindowText(hTextBox, buffer);

                SendMessageA(hTextBox, EM_SETSEL, 0, -1);
                SendMessageA(hTextBox, EM_SETSEL, -1, -1);
                SendMessageA(hTextBox, EM_SCROLLCARET, 0, 0);
                break;
            }
            case IDC_SIGN_SU: {
                WCHAR buffer[10000];
                GetWindowText(hTextBox, buffer, 10000);
                if (buffer == NULL)
                {
                    break;
                }
                wcscat_s(buffer, L"\r\n");
                wcscat_s(buffer, L"1 user just sign up success");
                SetWindowText(hTextBox, buffer);

                SendMessageA(hTextBox, EM_SETSEL, 0, -1);
                SendMessageA(hTextBox, EM_SETSEL, -1, -1);
                SendMessageA(hTextBox, EM_SCROLLCARET, 0, 0);
                break;
            }
            case IDC_RESTORE: {
                savedata();
                break;
            }
            case IDC_EXIT: {
                WCHAR buffer[10000];
                GetWindowText(hTextBox, buffer, 10000);
                if (buffer == NULL)
                {
                    break;
                }
                wcscat_s(buffer, L"\r\n");
                wcscat_s(buffer, L"1 user just leave");
                SetWindowText(hTextBox, buffer);

                SendMessageA(hTextBox, EM_SETSEL, 0, -1);
                SendMessageA(hTextBox, EM_SETSEL, -1, -1);
                SendMessageA(hTextBox, EM_SCROLLCARET, 0, 0);
                break;
            }
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                savedata();
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY: {
        WCHAR message[50];
        message[0] = messtype::EXIT;
        for (auto socket : serverObj.getsocket()) {
            serverObj.SendtoClient(socket,message, 1);
        }
        PostQuitMessage(0);
        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
void loadata() {
    wifstream f(L"data.txt");
    WCHAR username[100];
    WCHAR password[100];
    while (!f.eof()) {
        f >> username;
        f >> password;
        auto user = new User;
        user->username = username;
        user->password = password;
        serverObj.addUser(user);
    }
    f.close();
}
void savedata() {
    wofstream f(L"data.txt");
    for (auto user : serverObj.getuser())
    {
        f << user->username << L" ";
        f << user->password << L"\n";
    }
    f.close();
}