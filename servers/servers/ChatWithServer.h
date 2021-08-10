#pragma once
#define _AFXDLL
#include<Afxwin.h>
#include<iostream>
#include<WinSock2.h>
#include "resource.h"
#include<fstream>
#include<list>
#include "json/json.h"
using namespace std;
struct User
{
	wstring username;
	wstring password;
};
struct Gold {
	Json::Value Gold;//Tất cả thông tin
	Json::Value key;//tất cár thông tin gold
	Json::Value element;//1 cục nhỏ
	Json::Value value; //thông tin khóa value
	Json::Value buy; //thông tin mua
	Json::Value sell;//thông tin bán
	Json::Value Area; // thông tin khu vực
	Json::Value type;//loại vàng
	Json::Value day;//thời gian cập nhật
	Json::Value company;
	Json::Value brand1;
};
class server {
public:
	void getJson();
	bool connected();
	void ConnectClient();
	int SendtoClient(SOCKET socket,WCHAR* mess, int len);
	int RecFromClient(SOCKET s);
	void listenClient();
	void addUser(User* user);
	void setHWND(HWND hwnd);
	bool SignUp(User* user);
	bool LogIn(User* user);
	list<User*>& getuser();
	list<SOCKET> getsocket();
private:
	bool _connected;
	SOCKET _toClient;
	SOCKET _ListenClient;
	HWND _hwnd;
	list<User*> _userData;
	list<SOCKET> _clientList;
};