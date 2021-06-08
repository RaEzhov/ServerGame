#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INPUT_STR_SIZE 50
#include "Registration.h"
#include "Client.h"

void newGame(int clientId) {
	printf("Game executed:...\n");
	char newGameFlag[INPUT_STR_SIZE] = "-ng";
	SendData2Server(clientId,newGameFlag);
}
void showFriends(int clientId) {
	printf("Friens:\n");
	char friendsFlag[INPUT_STR_SIZE] = "-frds";
	SendData2Server(clientId, friendsFlag);
}
void showRatings(int clientId) {
	printf("Ratings:\n");
	char ratingsFlag[INPUT_STR_SIZE] = "-rtgs";
	SendData2Server(clientId, ratingsFlag);
}
void exit() {
	printf("Good bye!\n");
}
void menuList() {
	printf("1. New game\n");
	printf("2. Friends\n");
	printf("3. Ratings\n");
	printf("4. Exit\n");
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