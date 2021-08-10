#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>	
#include<WinSock2.h>
#include "resource.h"
using namespace std;
class client {
public:
	bool connected();
	void ConnectServer(int port);
	int SendToServer(WCHAR* mess, int len);
	int ReceiveFromServer();
	void setHWND(HWND hwnd);
private:
	bool _isConnected = false;
	SOCKET _client;
	HWND _hwnd;

};