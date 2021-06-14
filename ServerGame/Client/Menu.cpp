#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INPUT_STR_SIZE 50
#include "Registration.h"
#include "Client.h"


void newGame(int clientId) {
	char tempStr[INPUT_STR_SIZE];
	strcpy(tempStr, "Game executed:...\n");
	printStringInCenter(tempStr);
	char newGameFlag[INPUT_STR_SIZE] = "-ng";
	SendData2Server(clientId,newGameFlag);
}
void showFriends(int clientId) {
	char tempStr[INPUT_STR_SIZE];
	strcpy(tempStr, "Friens:\n");
	printStringInCenter(tempStr);
	char friendsFlag[INPUT_STR_SIZE] = "-frds";
	SendData2Server(clientId, friendsFlag);
}
void showRatings(int clientId) {
	char tempStr[INPUT_STR_SIZE];
	strcpy(tempStr, "Ratings:\n");
	printStringInCenter(tempStr);
	char ratingsFlag[INPUT_STR_SIZE] = "-rtgs";
	SendData2Server(clientId, ratingsFlag);
}
void exit() {
	char tempStr[INPUT_STR_SIZE];
	strcpy(tempStr, "Good bye!\n");
	printStringInCenter(tempStr);
}
void menuList() {
	char tempStr[INPUT_STR_SIZE];
	printf("\n\n");
	strcpy(tempStr, "MENU\n");
	printStringInCenter(tempStr);
	strcpy(tempStr, "1. New game\n");
	printStringInCenter(tempStr);
	strcpy(tempStr, "2. Friends\n");
	printStringInCenter(tempStr);
	strcpy(tempStr, "3. Ratings\n");
	printStringInCenter(tempStr);
	strcpy(tempStr, "4. Exit\n");
	printStringInCenter(tempStr);
}

void menu(int clientId) {
	menuList();
	int input;
	printf("Input number, please: ");
	scanf("%d", &input);
	switch (input) {
	case 1:
		newGame(clientId);
		break;
	case 2:
		showFriends(clientId);
		break;
	case 3:
		showRatings(clientId);
		break;
	case 4:
		exit();
		break;
	default:
		printf("Invalid character, try again\n");
		menu(clientId);
		break;
	}
}