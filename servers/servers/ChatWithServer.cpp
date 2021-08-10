#include "ChatWithServer.h"
#include"thread.h"
#include"TypeMessage.h"
#include <codecvt>
#include <locale>
#pragma warning(disable : 4996)
void server::ConnectClient() {
	cout << "Starting up TCP Chat server\n";
	WSADATA wsa;
	WORD l = MAKEWORD(2, 2);
	WSAStartup(l, &wsa);
	SOCKADDR_IN add;
	add.sin_family = AF_INET;
	add.sin_addr.s_addr = INADDR_ANY;
	add.sin_port = htons(8888);
	_ListenClient = socket(AF_INET, SOCK_STREAM, 0);
	if (_ListenClient == INVALID_SOCKET) return;
	if (bind(_ListenClient, (sockaddr*)&add, sizeof(add)) != 0) return;
	if (listen(_ListenClient, 10) != 0) return;
	_connected = true;
	return;
}
int server::SendtoClient(SOCKET _socket,WCHAR* mess, int len) {
	int istart;
	istart = send(_socket, (char*)mess, len * 2 + 2, 0);
	return 0;
}
int server::RecFromClient(SOCKET _socket) {
	WCHAR* message;
	WCHAR temp[4096];
	char* sem;
	int iStat;
	int len;
	iStat = recv(_socket, (char*)temp, 4096, 0);
	message = temp;
	sem = (char*)temp;
	switch (message[0])
	{
	case messtype::SIGNUP: {
		WCHAR* username;
		WCHAR* password;
		username = message + 1;
		password = message + wcslen(message) + 1;
		auto user = new User;
		user->username = username;
		user->password = password;
		bool result = SignUp(user);
		if (result) {
			message[0] = messtype::SIGN_SUCCESS;
			SendMessage(_hwnd, WM_COMMAND, IDC_RESTORE, 0);
			SendMessage(_hwnd, WM_COMMAND, IDC_SIGN_SU, 0);
		}
		else {
			message[0] = messtype::SIGN_FAIL;
		}
		SendtoClient(_socket,(WCHAR*)message,1);
		break;
	}
	case messtype::LOGIN: {
		WCHAR* username;
		WCHAR* password;
		username = message + 1;
		password = message + wcslen(message) + 1;
		auto user = new User;
		user->username = username;
		user->password = password;
		bool result = LogIn(user);
		if (result) {
			message[0] = messtype::LOG_SUCCESS;
			SendMessage(_hwnd, WM_COMMAND, IDC_LOG_SU, 0);
		}
		else {
			message[0] = messtype::LOG_FAIL;
		}
		SendtoClient(_socket, (WCHAR*)message,2);
		break;
	}
	case messtype::EXIT: {
		SendMessage(_hwnd, WM_COMMAND, IDC_EXIT, 0);
		break;
	}
	case messtype::ADD: {
		WCHAR *typegold = message + 1;
		Json::Value Data;
		ifstream people_file("gold.json");
		people_file >> Data;
		Json::Value a = Data["golds"];
		Json::Value b = a[0];
		Json::Value c = b["value"];
		int sz = b["value"].size();
		Json::Value d;
		for (int i = 0; i < sz; i++) {
			string buy1, sell1, company1, brand1;
			wstring buy2, sell2, company2, brand2;
			d = c[i];
			buy1 = d["buy"].asString();
			sell1 = d["sell"].asString();
			company1 = d["type"].asString();
			brand1 = d["brand"].asString();
			buy2 = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(buy1);
			sell2 = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(sell1);
			company2 = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(company1);
			brand2 = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(brand1);
			if (wcsstr(company2.c_str(), typegold) != NULL) {
				WCHAR message1[200],message2[100],message3[100];
				message1[0] = messtype::ADD1;
				message1[1] = NULL;
				wcscat(message1, company2.c_str());
				int len = wcslen(message1);
				len++;
				int j;
				for (j = 0; brand2[j] != NULL;j++) {
					message1[len + j] = brand2[j];
				}
				len += j;
				message1[len] = NULL;
				serverObj.SendtoClient(_socket, message1, len);

				message2[0] = messtype::ADD2;
				message2[1] = NULL;
				wcscat(message2, buy2.c_str());
				len = wcslen(message2);
				len++;
				for (j = 0; sell2[j] != NULL;j++) {
					message2[len + j] = sell2[j];
				}
				len += j;
				message2[len] = NULL;
				serverObj.SendtoClient(_socket, message2, len);
			}
		}
		break;
	}
	}
	return 0;
}
void server::addUser(User* user)
{
	_userData.push_back(user);
}
void server::listenClient() {
	sockaddr_in from;
	int fromlen = sizeof(from);

	_toClient = accept(_ListenClient,
		(sockaddr*)&from, &fromlen);
	SOCKET packet;
	packet = _toClient;
	if (_toClient != INVALID_SOCKET)
	{
		SendMessage(_hwnd, WM_COMMAND, ID_USER_CONNECT, 0);
		_clientList.push_back(packet);

	}
	AfxBeginThread(recServerThread, (void*)_toClient);
}
void server::setHWND(HWND hwnd) {
	_hwnd = hwnd;
}
bool server::connected() {
	return _connected;
}
bool server::SignUp(User* user) {
	for (auto userdata : _userData)
	{
		if (user->username == userdata->username)
		{
			return false;
		}
	}
	_userData.push_back(user);
	return true;
}
bool server::LogIn(User* user) {
	for (auto userdata : _userData)
	{
		if (user->username == userdata->username && user->password == userdata->password)
		{
			return true;
		}
	}
	return false;
}
list<User*>& server::getuser() {
	return _userData;
}
list<SOCKET> server::getsocket() {
	return _clientList;
}
void mParseUrl(char* mUrl, string& serverName, string& filepath, string& filename)
{
	string::size_type n;
	string url = mUrl;

	if (url.substr(0, 7) == "http://")
		url.erase(0, 7);

	if (url.substr(0, 8) == "https://")
		url.erase(0, 8);

	n = url.find('/');
	if (n != string::npos)
	{
		serverName = url.substr(0, n);
		filepath = url.substr(n);
		n = filepath.rfind('/');
		filename = filepath.substr(n + 1);
	}

	else
	{
		serverName = url;
		filepath = "/";
		filename = "";
	}
}
SOCKET connectToServer(char* szServerName, WORD portNum)
{
	struct hostent* hp;
	unsigned int addr;
	struct sockaddr_in server;
	SOCKET conn;

	conn = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (conn == INVALID_SOCKET)
		return NULL;

	if (inet_addr(szServerName) == INADDR_NONE)
	{
		hp = gethostbyname(szServerName);
	}
	else
	{
		addr = inet_addr(szServerName);
		hp = gethostbyaddr((char*)&addr, sizeof(addr), AF_INET);
	}

	if (hp == NULL)
	{
		closesocket(conn);
		return NULL;
	}

	server.sin_addr.s_addr = *((unsigned long*)hp->h_addr);
	server.sin_family = AF_INET;
	server.sin_port = htons(portNum);
	if (connect(conn, (struct sockaddr*)&server, sizeof(server)))
	{
		closesocket(conn);
		return NULL;
	}
	return conn;
}
int getHeaderLength(char* content)
{
	const char* srchStr1 = "\r\n\r\n", * srchStr2 = "\n\r\n\r";
	char* findPos;
	int ofset = -1;

	findPos = strstr(content, srchStr1);
	if (findPos != NULL)
	{
		ofset = findPos - content;
		ofset += strlen(srchStr1);
	}

	else
	{
		findPos = strstr(content, srchStr2);
		if (findPos != NULL)
		{
			ofset = findPos - content;
			ofset += strlen(srchStr2);
		}
	}
	return ofset;
}
char* readUrl2(char* szUrl, long& bytesReturnedOut, char** headerOut)
{
	const int bufSize = 512;
	char readBuffer[bufSize], sendBuffer[bufSize], tmpBuffer[bufSize];
	char* tmpResult = NULL, * result;
	SOCKET conn;
	string server, filepath, filename;
	long totalBytesRead, thisReadSize, headerLen;

	mParseUrl(szUrl, server, filepath, filename);

	///////////// step 1, connect //////////////////////
	conn = connectToServer((char*)server.c_str(), 80);

	///////////// step 2, send GET request /////////////
	sprintf(tmpBuffer, "GET %s HTTP/1.0", filepath.c_str());
	strcpy(sendBuffer, tmpBuffer);
	strcat(sendBuffer, "\r\n");
	sprintf(tmpBuffer, "Host: %s", server.c_str());
	strcat(sendBuffer, tmpBuffer);
	strcat(sendBuffer, "\r\n");
	strcat(sendBuffer, "\r\n");
	send(conn, sendBuffer, strlen(sendBuffer), 0);

	//    SetWindowText(edit3Hwnd, sendBuffer);
	//printf("Buffer being sent:\n%s", sendBuffer);

	///////////// step 3 - get received bytes ////////////////
	// Receive until the peer closes the connection
	totalBytesRead = 0;
	while (1)
	{
		memset(readBuffer, 0, bufSize);
		thisReadSize = recv(conn, readBuffer, bufSize, 0);

		if (thisReadSize <= 0)
			break;

		tmpResult = (char*)realloc(tmpResult, thisReadSize + totalBytesRead);

		memcpy(tmpResult + totalBytesRead, readBuffer, thisReadSize);
		totalBytesRead += thisReadSize;
	}

	headerLen = getHeaderLength(tmpResult);
	long contenLen = totalBytesRead - headerLen;
	result = new char[contenLen + 1];
	memcpy(result, tmpResult + headerLen, contenLen);
	result[contenLen] = 0x0;
	char* myTmp;

	myTmp = new char[headerLen + 1];
	strncpy(myTmp, tmpResult, headerLen);
	myTmp[headerLen] = NULL;
	delete(tmpResult);
	*headerOut = myTmp;

	bytesReturnedOut = contenLen;
	closesocket(conn);
	return(result);
}
void server::getJson() {
	const int bufLen = 1024;
	char szUrl[] = "https://tygia.com/json.php?ran=0&rate=0&gold=1&bank=VIETCOM&date=now";
	long fileSize;
	char* memBuffer, * headerBuffer;
	FILE* fp;
	WSAData wsaData;
	memBuffer = headerBuffer = NULL;

	if (WSAStartup(0x101, &wsaData) != 0)
		return;


	memBuffer = readUrl2(szUrl, fileSize, &headerBuffer);


	//printf("returned from readUrl\n");
	//printf("data returned:\n%s", memBuffer);
	if (fileSize != 0)
	{
		//printf("Got some data\n");
		fp = fopen("gold.json", "wb");
		fwrite(memBuffer, 1, fileSize, fp);
		fclose(fp);
		delete(memBuffer);
		delete(headerBuffer);
	}
	WSACleanup();
}