#include "ChatWithClient.h"
#include "framework.h"
void client::ConnectServer(int port) {
	WSADATA wsa;
	WORD l = MAKEWORD(2, 2);
	WSAStartup(l, &wsa);
	_client = socket(AF_INET, SOCK_STREAM,0);
	SOCKADDR_IN add;
	add.sin_family = AF_INET;
	add.sin_port = htons(port);
	add.sin_addr.s_addr = inet_addr("192.168.100.2");
	connect(_client, (SOCKADDR*)&add, sizeof(add));
	_isConnected = true;
	return;
}
bool client::connected() {
	return _isConnected;
}
int client::SendToServer(WCHAR* mess, int len) {
	int istart;
	istart = send(_client, (char*)mess, len * 2 + 2, 0);
	return 0;
}
int client::ReceiveFromServer() {
	char recData[4096];
	int itstart;
	itstart = recv(_client, recData, 4096, 0);
	SendMessage(_hwnd,WM_COMMAND, IDC_RECEIVE, (LPARAM)recData);
	return 0;
}
void client::setHWND(HWND hwnd) {
	_hwnd = hwnd;
}