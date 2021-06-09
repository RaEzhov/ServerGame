#pragma comment(lib, "ws2_32.lib")
#define _CRT_SECURE_NO_WARNINGS
#include <winsock.h>
#include <conio.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "outGame.h"

void sendField(SOCKET s, unsigned char field[][GAME_FIELD_LINES]) {
	int ret = send(s, (char*)field, BUFFER, 0);
	if (ret == SOCKET_ERROR)
	{
		printf("Can't send message.\n");
		closesocket(s);
		exit(10);
	}
}
void sendData (SOCKET s, char buf[], int size){
	int ret = send (s, buf, size, 0);
	if (ret == SOCKET_ERROR){
		printf ("Can't send message.\n");
		closesocket (s);
		exit (10);
	}
}
void getField(SOCKET s, unsigned char field[][GAME_FIELD_LINES]) {
	int ret = recv(s, (char*)field, BUFFER, 0);
	if (ret == 0 || ret == WSAECONNRESET)
	{
		printf("Connection closed.\n");
		exit(10);
	}
	if (ret < 0)
	{
		printf("Can't resieve data.\n");
		closesocket(s);
		exit(10);
	}
}
void getData (SOCKET s, char buf[], int size){
	int ret = recv (s, buf, size, 0);
	if (ret == 0 || ret == WSAECONNRESET){
		printf ("Connection closed.\n");
		exit (10);
	}
	if (ret < 0){
		printf ("Can't resieve data.\n");
		closesocket (s);
		exit (10);
	}
}

void playGame(){
	setDefaultField();
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
	_getch ();
	closesocket(client);
}

int main(){
	WSADATA wsd;
	if(WSAStartup(MAKEWORD(1, 1), &wsd) != 0){
		printf("Can't connect to socket lib.");
		return 1;
	}
	playGame();
	printf("Session is closed.\n");
	Sleep(1000);
	return 0;
}