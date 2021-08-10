#pragma once
#define _AFXDLL
#include<Afxwin.h>
#include<iostream>
#include "resource.h"
#include"thread.h"
#include"TypeMessage.h"
#include"ChatWithServer.h"
#include <mmsystem.h>
#include<fstream>
#include<list>
#include <codecvt>
using namespace std;
HWND hTextBox;
void loadata();
void savedata();