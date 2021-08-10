#pragma once
#define _AFXDLL
#include <Afxwin.h>
#include "resource.h"

#include <winsock2.h>
#include <list>
#include <iostream>
#include <mmsystem.h>
#include "ChatWithClient.h"
#include "thread.h"
#include "TypeMessage.h"
using namespace std;
BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
HWND user;
HWND pass;
HWND login;
HWND signup;
HWND a;
HWND b;
HWND hListView;
LVCOLUMN col;
LV_ITEM row;
HWND hcombo;
HWND hBrand;
int s = 0,d = 0, t = 0;