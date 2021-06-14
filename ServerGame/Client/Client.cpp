#define _TIMESPEC_DEFINED
#define _CRT_SECURE_NO_WARNINGS

#pragma comment(lib, "ws2_32.lib")
#include <winsock.h>
#include <conio.h>
#include <stdio.h>
#include <Windows.h>
#include <pthread.h>
#include <stdlib.h>
#include "outGame.h"
#include "Menu.h"
#include "../commonFunctions.h"

#define ERROR_CREATE_THREAD -11
#define ERROR_JOIN_THREAD   -12
#define ANCHOR_HEIGHT        23
#define ANCHOR_WEIGHT        37
#define STR_SIZE             50
#define INPUT_STR_SIZE       50


int main(){
	setDefaultField ();
	WSADATA wsd;
	if(WSAStartup(MAKEWORD(1, 1), &wsd) != 0){
		printf("CAN'T CONNECT TO SOCKTE LIB.\n");
		return 1;
	}

	SOCKET client_socket;
	client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if (client_socket == INVALID_SOCKET)
	{
		printf("ERROR CREATE SOCKET.\n");
		return -1;
	}
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(5510); //the same as in server
	server.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); //special look-up address
	if (connect(client_socket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("CAN'T CONNECT TO SERVER.\n");
		closesocket(client_socket);
		return -1;
	}

	srand(time(NULL));
	int id = rand() % 10000;
	char isRegistered = { 0 };
	char tempStr[STR_SIZE] = "";
	void** value = 0;
	int invChar = 1;
	while (invChar) {
		for (int i = 0; i < ANCHOR_HEIGHT;  i++) {
				printf("%s\n",anchor[i]);
		}
		printf("\n\n\n");
		strcpy(tempStr,"ARE YOU REGISTERED? (Y/N)");
		printStringInCenter(tempStr);
		scanf("%c",&isRegistered);
		system ("cls");
		if (isRegistered == 'Y' || isRegistered == 'y') {
			login((void*)(client_socket));
			invChar = 0;
		}
		else if (isRegistered == 'N' || isRegistered == 'n') {
			registration((void*)(client_socket));
			invChar = 0;
		} 
		else {
			strcpy(tempStr, "INVALID CHARACTER, TRY AGAIN:\n");
			printf ("\n\n\n_______________________________________________________\n\n\n\n\n\n\n\n\n\n\n\n\n");
			printStringInCenter(tempStr);
			printf ("\n\n\n\n\n\n\n\n\n\n_______________________________________________________\n\n");
			Sleep(1000);
		}
	}
	printf("SESSION IS CLOSED.\n");
	Sleep(1000);
	return 0;
}