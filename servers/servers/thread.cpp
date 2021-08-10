#include"thread.h"
server serverObj;
UINT  recServerThread(LPVOID lParam)
{
	SOCKET recSocket = (SOCKET)lParam;
	while (1)
	{
		if (serverObj.RecFromClient(recSocket))
			break;
	}
	return 0;
}

UINT  listenServerThread(LPVOID lParam)
{
	while (1)
		serverObj.listenClient();
	return 0;
}