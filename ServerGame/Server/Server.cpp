#pragma comment(lib, "ws2_32.lib")
#include <winsock.h>
#include <stdio.h>
#include "loginAndMenu.h"

int main() {
	WSADATA wsd;
	if (WSAStartup(MAKEWORD(1, 1), &wsd) == 0)
	{
		printf("Connected to socket lib.\n");
	} else {
		return 1;
	}
	return CreateServer();
}