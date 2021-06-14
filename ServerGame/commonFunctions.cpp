#include <stdio.h>
#include <winsock.h>
#include "commonFunctions.h"

int sendField (SOCKET s, unsigned char field[][GAME_FIELD_LINES]){
	int ret = send (s, (char*)field, BUFFER, 0);
	if (ret == SOCKET_ERROR){
		printf ("Can't send message.\n");
		shutdown (s, 2);
		closesocket (s);
	}
	return ret;
}

int sendData (SOCKET s, char buf[], int size){
	int ret = send (s, buf, size, 0);
	if (ret == SOCKET_ERROR){
		printf ("Can't send message.\n");
		shutdown (s, 2);
		closesocket (s);
	}
	return ret;
}

int getField (SOCKET s, unsigned char field[][GAME_FIELD_LINES]){
	int ret = recv (s, (char*)field, BUFFER, 0);
	if (ret == 0 || ret == WSAECONNRESET){
		printf ("Connection closed.\n");
	}
	if (ret < 0){
		printf ("Can't resieve data.\n");
		shutdown (s, 2);
		closesocket (s);
	}
	return ret;
}

int getData (SOCKET s, char buf[], int size){
	int ret = recv (s, buf, size, 0);
	if (ret == 0 || ret == WSAECONNRESET){
		printf ("Connection closed.\n");
	}
	if (ret < 0){
		printf ("Can't resieve data.\n");
		shutdown (s, 2);
		closesocket (s);
	}
	return ret;
}