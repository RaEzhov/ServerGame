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

void playGame(){
	SOCKET client;
	client = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if(client == INVALID_SOCKET){
		printf("Error create socket.\n");
		return;
	}
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(5510); //the same as in server
	server.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); //special look-up address
	if(connect(client, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR){
		printf("Can't connect to server.\n");
		closesocket(client);
		return;
	}
	//connect - ok
	welcomePage();
	unsigned char playerField[GAME_FIELD_LINES][GAME_FIELD_LINES] = {0};
	unsigned char opponentFileld[GAME_FIELD_LINES][GAME_FIELD_LINES] = {0};
	for(int i = 0; i < GAME_FIELD_LINES; i++){
		for(int j = 0; j < GAME_FIELD_LINES; j++){
			playerField[i][j] = EMPTY;
			opponentFileld[i][j] = EMPTY;
		}
	}
	int counterYourShips = COUNT_OF_SHIPS;
	int counterOpponentShips = COUNT_OF_SHIPS;
	char message[1024];

	int genShips[COUNT_OF_SHIPS] = {4,3,3,2,2,2,1,1,1,1};
	for(int i = 0; i < COUNT_OF_SHIPS; i++){ 
		drawField(playerField, opponentFileld, counterYourShips, counterOpponentShips);
		generationShips(genShips[i], playerField, opponentFileld, counterYourShips, counterOpponentShips);
	}
	drawField(playerField, opponentFileld, counterYourShips, counterOpponentShips);
	sendField(client, playerField);
	printf ("Wait while opponent place his ships...\n");
	getField(client, opponentFileld);
	drawField (playerField, opponentFileld, counterYourShips, counterOpponentShips);
	int shotRes = 0;
	while (counterYourShips){
		getData (client, message, 1);
		if (message[0] == 's'){
			message[0] = shootToShip (opponentFileld);
			sendField (client, opponentFileld);
			sendData (client, message, 1);
			drawField (playerField, opponentFileld, counterYourShips, counterOpponentShips);
		} else{
			printf ("Expect the opponent to move...\n");
			getField (client, playerField);
			getData (client, message, 1);
			shotRes = message[0];
			counterYourShips -= shotRes;
			drawField (playerField, opponentFileld, counterYourShips, counterOpponentShips);
		}
	}
	printf ("The end!\n");
	shutdown (client,2);
	closesocket(client);
}

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
	playGame();
	printf("SESSION IS CLOSED.\n");
	Sleep(1000);
	return 0;
}