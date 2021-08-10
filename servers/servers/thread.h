#pragma once
#include"ChatWithServer.h"
extern server serverObj;
UINT  listenServerThread(LPVOID lParam);
UINT  recServerThread(LPVOID lParam);