#include "thread.h"
client clientObj;
UINT recThread(LPVOID lParam) {
	while (1) {
		clientObj.ReceiveFromServer();
	}
	return 0;
}