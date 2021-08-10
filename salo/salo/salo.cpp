// salo.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "salo.h"
#include <Windowsx.h>
#include <CommCtrl.h>
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
    LoadStringW(hInstance, IDC_SALO, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SALO));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SALO));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SALO);
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

   HWND hWnd = CreateWindowW(szWindowClass,L"Client", WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
       CW_USEDEFAULT, 0, 1000, 1000, nullptr, nullptr, hInstance, nullptr);

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
    HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
    HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
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
        message[1] = NULL;
        clientObj.SendToServer(message, 1);
        PostQuitMessage(0);
        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
    clientObj.ConnectServer(8888);
    if (clientObj.connected() == false) {
        MessageBox(0, L"\nFailed to initialise server socket.", 0, 0);
        return 1;
    }
    a = CreateWindowEx(NULL, L"BUTTON", L"Username:",
        WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
        80, 80, 200, 70, hwnd,
        NULL, lpCreateStruct->hInstance, NULL);
    user = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", NULL,
        WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
        100, 100, 150, 35, hwnd,
        0, lpCreateStruct->hInstance, NULL);
    b = CreateWindowEx(NULL, L"BUTTON", L"Password:",
        WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
        80, 140, 200, 70, hwnd,
        NULL, lpCreateStruct->hInstance, NULL);
    pass = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", NULL,
        WS_CHILD | WS_VISIBLE | ES_PASSWORD,
        100, 160, 150, 35, hwnd,
        0, lpCreateStruct->hInstance, NULL);
    login = CreateWindowEx(NULL, L"BUTTON", L"Đăng nhập",
        WS_CHILD | WS_VISIBLE | WS_VISIBLE | WS_CHILD, 60, 270, 100, 30, hwnd, 
        (HMENU)IDC_LOGIN, hInst, 0);
    signup = CreateWindowEx(NULL, L"BUTTON", L"Đăng ký",
        WS_CHILD | WS_VISIBLE | WS_VISIBLE | WS_CHILD, 200, 270, 100, 30, hwnd,
        (HMENU)IDC_SIGNUP, hInst, 0);
    clientObj.setHWND(hwnd);
    AfxBeginThread(recThread, 0);
    return TRUE;
}
void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {
    switch (id)
    {
    case IDC_LOGIN: {
        WCHAR username[50];
        WCHAR password[50];
        WCHAR message[100];
        message[0] = messtype::LOGIN;
        message[1] = NULL;
        GetWindowText(user, username, 50);
        if (username[0] == NULL)
        {
            MessageBox(hwnd, L"Username is empty.", 0, 0);
            return;
        }
        GetWindowText(pass, password, 50);
        if (password[0] == NULL)
        {
            MessageBox(hwnd, L"Password is empty.", 0, 0);
            return;
        }
        wcscat_s(message, username);
        int len = wcslen(message);
        len++;
        int i;
        for (i = 0; password[i] != NULL; i++)
        {
            message[len + i] = password[i];
        }
        len += i + 1;
        message[len - 1] = NULL;
        clientObj.SendToServer(message, len);
        Edit_SetText(user, L"");
        break;
    }
    case IDC_SIGNUP: {
        WCHAR username[50];
        WCHAR password[50];
        WCHAR message[150];
        message[0] = messtype::SIGNUP;
        message[1] = NULL;
        GetWindowText(user, username, 50);
        if (username[0] == NULL)
        {
            MessageBox(hwnd, L"Username is empty.",0, 0);
            return;
        }
        GetWindowText(pass, password, 50);
        if (password[0] == NULL)
        {
            MessageBox(hwnd, L"Password is empty.", 0, 0);
            return;
        }
        wcscat_s(message, username);
        int len = wcslen(message);
        len++;
        int i;
        for (i = 0; password[i] != NULL; i++)
        {
            message[len + i] = password[i];
        }
        len += i;
        message[len] = NULL;
        clientObj.SendToServer(message, len);
        break;
    }
    case IDC_RECEIVE: {
        WCHAR* message = (WCHAR*)hwndCtl;
        switch (message[0]) {
        case messtype::SIGN_SUCESS: {
            MessageBox(hwnd, L"Sign Up Successfull!", L"Congratulations", 0);
            break;
        }
        case messtype::SIGN_FAIL: {
            MessageBox(hwnd, L"Sign Up Failed", 0, 0);
            break;
        }
        case messtype::LOG_FAIL: {
            MessageBox(hwnd, L"Username or password is incorrect.", 0, 0);
            break;
        }
        case messtype::LOG_SUCCESS: {
            MessageBox(hwnd, L"Log in success.", L"Congratulations", 0);
            DestroyWindow(user);
            DestroyWindow(pass);
            DestroyWindow(signup);
            DestroyWindow(login);
            DestroyWindow(a);
            DestroyWindow(b);
            CreateWindowEx(NULL, L"BUTTON", L"Tra cứu theo loại vàng:",
                WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
                540, 30, 10, 300, hwnd,
                NULL, hInst, NULL);
            hcombo = CreateWindow(WC_COMBOBOX, TEXT(""),
                CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
                560, 50, 200, 300, hwnd, NULL, hInst,
                NULL);
            CreateWindowEx(NULL, L"BUTTON", L"Tra cứu",
                WS_CHILD | WS_VISIBLE | WS_VISIBLE | WS_CHILD, 600, 400, 60, 50, hwnd,
                (HMENU)IDC_SEARCH, hInst, 0);
            hListView = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTVIEW, NULL,
                WS_CHILD | WS_VISIBLE | WS_VSCROLL | LVS_REPORT,
                50, 0, 500, 440, hwnd,
                (HMENU)IDC_LISTWIEW, hInst, NULL);
            col.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
            col.fmt = LVCFMT_LEFT;
            col.cx = 150;
            col.pszText = (LPWSTR)L"Tên vàng";
            ListView_InsertColumn(hListView, 0, &col);

            // Cột thứ hai
            col.cx = 130;
            col.pszText = (LPWSTR)L"Địa phương/nơi bán";
            ListView_InsertColumn(hListView, 1, &col);

            // Cột thứ ba
            col.cx = 100;
            col.pszText = (LPWSTR)L"Giá mua";
            ListView_InsertColumn(hListView, 2, &col);

            // Cột thứ tư
            col.cx = 100;
            col.pszText = (LPWSTR)L"Giá bán";
            ListView_InsertColumn(hListView, 3, &col);
            break;
        }
        case messtype::EXIT: {
            MessageBox(hwnd, L"Server has disconnected.", L"Error", 0);
            DestroyWindow(hwnd);
            break;
        }
        case messtype::ADD1: {
            WCHAR* type, *brand;
            type = message + 1;
            brand = message + wcslen(message) + 1;
            row.mask = LVIF_TEXT | LVIF_PARAM;
            row.iItem = s;
            row.iSubItem = 0; // Cột đầu tiên - Tên
            row.pszText = type;
            row.lParam = (LPARAM)type;
            ListView_InsertItem(hListView, &row);
            ListView_SetItemText(hListView, s, 1, (LPWSTR)brand);
            s++;
            break;
        } 
        case messtype::ADD2: {
            WCHAR* buy,*sell;
            buy = message + 1;
            sell = message + wcslen(message) + 1;
            ListView_SetItemText(hListView, d, 2, (LPWSTR)buy);
            ListView_SetItemText(hListView, d, 3, (LPWSTR)sell);
            d++;
            break;
        }
        }
        break;
    }
    case IDC_SEARCH: {
        s = 0;
        d = 0;
        t = 0;
        ListView_DeleteAllItems(hListView);
        WCHAR add1[50], add2[50];
        GetWindowText(hcombo,add1, 50);
        WCHAR message[150];
        message[0] = messtype::ADD;
        message[1] = NULL;
        wcscat_s(message, add1);
        int len = wcslen(message);
        message[len] = NULL;
        clientObj.SendToServer(message, len);
        break;
    }
    case IDM_ABOUT:
        DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hwnd, About);
        break;
    case IDM_EXIT: {
        WCHAR message[50];
        message[0] = messtype::EXIT;
        message[1] = NULL;
        clientObj.SendToServer(message, 1);
        DestroyWindow(hwnd);
        break;
    }
    }
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