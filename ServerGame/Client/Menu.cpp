#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock.h>
#include <conio.h>
#define INPUT_STR_SIZE 50
#include "Menu.h"
#include "outGame.h"
#include "../commonFunctions.h"

char myLogin[INPUT_STR_SIZE] = {0};
char opLogin[INPUT_STR_SIZE] = {0};

void playGame (SOCKET client){
	welcomePage ();
	int res = getData (client, myLogin, INPUT_STR_SIZE);
	if (res < 0 ){
		return;
	}
	myLogin[res] = '\0';
	res = getData (client, opLogin, INPUT_STR_SIZE);
	if (res < 0){
		return;
	}
	opLogin[res] = '\0';
	_getch ();
	unsigned char playerField[GAME_FIELD_LINES][GAME_FIELD_LINES] = {0};
	unsigned char opponentFileld[GAME_FIELD_LINES][GAME_FIELD_LINES] = {0};
	for (int i = 0; i < GAME_FIELD_LINES; i++){
		for (int j = 0; j < GAME_FIELD_LINES; j++){
			playerField[i][j] = EMPTY;
			opponentFileld[i][j] = EMPTY;
		}
	}
	int counterYourShips = COUNT_OF_SHIPS;
	int counterOpponentShips = COUNT_OF_SHIPS;
	char message[1024];

	int genShips[COUNT_OF_SHIPS] = {4,3,3,2,2,2,1,1,1,1};
	for (int i = 0; i < COUNT_OF_SHIPS; i++){
		drawField (playerField, opponentFileld, counterYourShips, counterOpponentShips, myLogin, opLogin);
		generationShips (genShips[i], playerField, opponentFileld, counterYourShips, counterOpponentShips);
	}
	drawField (playerField, opponentFileld, counterYourShips, counterOpponentShips, myLogin, opLogin);
	sendField (client, playerField);
	printf ("WAIT WHILE OPPONENT PLACE HIS  SHIPS...\n");
	getField (client, opponentFileld);
	drawField (playerField, opponentFileld, counterYourShips, counterOpponentShips, myLogin, opLogin);
	int shotRes = 0;
	while (counterYourShips && counterOpponentShips){
		getData (client, message, 1);
		if (message[0] == 's'){
			message[0] = shootToShip (opponentFileld);
			sendField (client, opponentFileld);
			sendData (client, message, 1);
			if (message[0] == 1){
				counterOpponentShips--;
			}
			drawField (playerField, opponentFileld, counterYourShips, counterOpponentShips, myLogin, opLogin);
		} else{
			printf ("EXPECT YOUR OPPONENT TO MOVE...\n");
			getField (client, playerField);
			getData (client, message, 1);
			if (message[0] == 1){
				counterYourShips--;
			}
			drawField (playerField, opponentFileld, counterYourShips, counterOpponentShips, myLogin, opLogin);
		}
	}
	system ("cls");
	printf ("\n\n\n\n\n                       THE  END!\n\n\n                 PRESS ANY KEY TO EXIT.\n\n\n");
	_getch ();
	shutdown (client, 2);
	closesocket (client);
}


void* login (void* params){
	char login[INPUT_STR_SIZE] = {0};
	char password[INPUT_STR_SIZE] = {0};
	char tempStr[INPUT_STR_SIZE] = {0};
	SOCKET client = (SOCKET)params;
	strcpy (tempStr, "Login: ");
	printf ("\n\n\n\n\n\n\n");
	printStringInCenter (tempStr);
	scanf ("%s", login);
	strcpy (tempStr, "Password: ");
	printStringInCenter (tempStr);
	scanf ("%s", password);
	strcat (login, " ");
	strcat (login, password);
	strcat (login, " (log)");
	int ret = 0;
	ret = sendData (client, login, strlen(login));
	char checkLogin[INPUT_STR_SIZE] = {0};
	getData (client, checkLogin, INPUT_STR_SIZE);
	if (!strcmp (checkLogin, "login-ok")){
		system ("cls");
		sscanf (login, "%s", myLogin);
		myLogin[strlen (myLogin)] = '\0';
		menu (client);
		return (void*)0;
	}

	while (strcmp (checkLogin, "login-ok")){
		system ("cls");
		printf ("\n\n\n INVALID USERNAME OR PASSWORD, PLEASE TRY AGAIN.\n");
		printf ("\n\n\n\n\n\n\n");
		strcpy (tempStr, "LOGIN: ");
		printStringInCenter (tempStr);
		scanf ("%s", login);
		strcpy (tempStr, "PASSWORD: ");
		printStringInCenter (tempStr);
		scanf ("%s", password);
		strcat (login, " ");
		strcat (login, password);
		strcat (login, " (log)");
		sendData (client, login, strlen (login));
		getData (client, checkLogin, INPUT_STR_SIZE);
	}
	system ("cls");
	sscanf (login, "%s", myLogin);
	myLogin[strlen (myLogin)] = '\0';
	menu (client);
	return (void*)0;
}

void* registration (void* params){
	SOCKET client = (SOCKET)params;
	char login[INPUT_STR_SIZE] = {0};
	char password[INPUT_STR_SIZE] = {0};
	char verify_password[INPUT_STR_SIZE] = {0};
	char tempStr[INPUT_STR_SIZE] = {0};
	printf ("\n\n\n\n\n\n\n");
	strcpy (tempStr, "Login: ");
	printStringInCenter (tempStr);
	scanf ("%s", login);
	strcpy (tempStr, "Password: ");
	printStringInCenter (tempStr);
	scanf ("%s", password);
	strcpy (tempStr, "Repeat your password, please: ");
	printStringInCenter (tempStr);
	scanf ("%s", verify_password);
	if (strcmp (password, verify_password)){
		printf ("Passwords don't match, try again\n");
		registration (params);
	}
	strcat (login, " ");
	strcat (login, password);
	strcat (login, " (reg)");
	int ret = sendData (client, login, strlen (login));
	sscanf (login, "%s", myLogin);
	myLogin[strlen (myLogin)] = '\0';
	menu (client);
	return SUCCESS;
}


void printStringInCenter (char* text){
	int width = SIZE_CONSOLE;
	int padding = (width - strlen (text)) / 2;
	for (int i = 0; i < padding; i++){
		printf (" ");
	}
	printf ("%s\n", text);
}

void printStringWithPaddingLeft (char* text, int padding){
	for (int i = 0; i < padding; i++){
		printf (" ");
	}
	printf ("%s\n", text);
}


void newGame (SOCKET client){
	char tempStr[INPUT_STR_SIZE];
	strcpy (tempStr, "GAME EXECUTED:...\n");
	printStringInCenter (tempStr);
	char newGameFlag[INPUT_STR_SIZE] = "(ng)";
	sendData (client, newGameFlag, strlen(newGameFlag));
	printf ("WHAIT FOR AN OPPONENT...\n");
	getData (client, tempStr, INPUT_STR_SIZE);
	system ("cls");
	playGame (client);
}

void showFriends (SOCKET client){
	char tempStr[INPUT_STR_SIZE];
	strcpy (tempStr, "Friens:\n");
	printStringInCenter (tempStr);
	char friendsFlag[INPUT_STR_SIZE] = "(frds)";
	sendData (client, friendsFlag, strlen(friendsFlag));
}

void showRatings (SOCKET client){
	char tempStr[INPUT_STR_SIZE];
	strcpy (tempStr, "Ratings:\n");
	printStringInCenter (tempStr);
	char ratingsFlag[INPUT_STR_SIZE] = "(rtgs)";
	sendData (client, ratingsFlag, strlen(ratingsFlag));
}

void exit (){
	char tempStr[INPUT_STR_SIZE];
	strcpy (tempStr, "Good bye!\n");
	printStringInCenter (tempStr);
}

void menuList (){
	char tempStr[INPUT_STR_SIZE];
	printf ("\n\n");
	strcpy (tempStr, "MENU\n");
	printStringInCenter (tempStr);
	strcpy (tempStr, "1. NEW GAME\n");
	printStringWithPaddingLeft (tempStr, SIZE_CONSOLE / 2 - 10);
	strcpy (tempStr, "2. FRIENDS\n");
	printStringWithPaddingLeft (tempStr, SIZE_CONSOLE / 2 - 10);
	strcpy (tempStr, "3. RATINGS\n");
	printStringWithPaddingLeft (tempStr, SIZE_CONSOLE / 2 - 10);
	strcpy (tempStr, "4. EXIT\n");
	printStringWithPaddingLeft (tempStr, SIZE_CONSOLE / 2 - 10);
}

void menu (SOCKET client){
	system ("cls");
	printf ("\nHELLO, %s!\n", myLogin);
	menuList ();
	int input;
	printf ("INPUT NUMBER, PLEASE: ");
	scanf ("%d", &input);
	system ("cls");
	switch (input){
	case 1:
		newGame (client);
		break;
	case 2:
		showFriends (client);
		break;
	case 3:
		showRatings (client);
		break;
	case 4:
		exit ();
		break;
	default:
		printf ("Invalid character, try again\n");
		menu (client);
		break;
	}
}
