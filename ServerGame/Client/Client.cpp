#define _TIMESPEC_DEFINED

#pragma comment(lib, "ws2_32.lib")
#define _CRT_SECURE_NO_WARNINGS
#define ERROR_CREATE_THREAD -11
#define ERROR_JOIN_THREAD   -12
#define SUCCESS               0
#include <winsock.h>
#include <conio.h>
#include <stdio.h>
#include <time.h>
#include <Windows.h>
#include "pthread.h"
#include <stdlib.h>
#define SIZE_CONSOLE 55
#define ANCHOR_HEIGHT 23
#define ANCHOR_WEIGHT 37
#define STR_SIZE 50
#define INPUT_STR_SIZE 50
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

struct CLientSocket {
	SOCKET s;
};
void printStringInCenter(char* text) {
	int width = SIZE_CONSOLE;
	int padding = (width - strlen(text)) / 2;
	for (int i = 0; i < padding; i++) {
		printf(" ");
	}
	printf("%s\n", text);
}
void printStringWithPaddingLeft(char* text, int padding) {
	for (int i = 0; i < padding; i++) {
		printf(" ");
	}
	printf("%s\n", text);
}

void getData(SOCKET s, char *buf, int size) {
	int ret = recv(s, buf, size, 0);
	if (ret == 0 || ret == WSAECONNRESET) {
		printf("Connection closed.\n");
		exit(10);
	}
	if (ret < 0) {
		printf("Can't recieve data.%d\n",ret);
		closesocket(s);
		exit(10);
	}
}

void SendData2Server(SOCKET client,char* data)
{
	char message[1024];
	sprintf(message, "%s", data);

	int ret = send(client, message, strlen(message), 0);
	if (ret == SOCKET_ERROR)
	{
		printf("Can't send message\n");
		closesocket(client);
		return;
	}
	
	if (ret == 0 || ret == WSAECONNRESET)
	{
		printf("Connection closed\n");
	}
	if (ret < 0)
	{
		printf("Can't recieve message\n");
		closesocket(client);
		return;
	}
}

void newGame(SOCKET client) {
	char tempStr[INPUT_STR_SIZE];
	strcpy(tempStr, "Game executed:...\n");
	printStringInCenter(tempStr);
	char newGameFlag[INPUT_STR_SIZE] = " (ng)";
	SendData2Server(client, newGameFlag);
}
void showFriends(SOCKET client) {
	char tempStr[INPUT_STR_SIZE];
	strcpy(tempStr, "Friens:\n");
	printStringInCenter(tempStr);
	char friendsFlag[INPUT_STR_SIZE] = " (frds)";
	SendData2Server(client, friendsFlag);
}
void showRatings(SOCKET client) {
	char tempStr[INPUT_STR_SIZE];
	strcpy(tempStr, "Ratings:\n");
	printStringInCenter(tempStr);
	char ratingsFlag[INPUT_STR_SIZE] = " (rtgs)";
	SendData2Server(client, ratingsFlag);
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
	strcpy(tempStr, "1. NEW GAME\n");
	printStringWithPaddingLeft(tempStr, SIZE_CONSOLE / 2 - 10);
	strcpy(tempStr, "2. FRIENDS\n");
	printStringWithPaddingLeft(tempStr, SIZE_CONSOLE / 2 - 10);
	strcpy(tempStr, "3. RATINGS\n");
	printStringWithPaddingLeft(tempStr, SIZE_CONSOLE / 2 - 10);
	strcpy(tempStr, "4. EXIT\n");
	printStringWithPaddingLeft(tempStr, SIZE_CONSOLE / 2 - 10);
}

void menu(SOCKET client) {
	menuList();
	int input;
	printf("Input number, please: ");
	scanf("%d", &input);
	printf("\033[0d\033[2J");//clear console
	switch (input) {
	case 1:
		
		newGame(client);
		break;
	case 2:
		showFriends(client);
		break;
	case 3:
		showRatings(client);
		break;
	case 4:
		exit();
		break;
	default:
		printf("Invalid character, try again\n");
		menu(client);
		break;
	}
}

void* login(void* params) {
	char login[INPUT_STR_SIZE] = { 0 };
	char password[INPUT_STR_SIZE] = { 0 };
	char tempStr[INPUT_STR_SIZE] = { 0 };
	struct CLientSocket *param = (struct CLientSocket*)params;
	SOCKET client = param->s;
	strcpy(tempStr, "Login: ");
	printStringInCenter(tempStr);
	scanf("%s", login);
	strcpy(tempStr, "Password: ");
	printStringInCenter(tempStr);
	scanf("%s", password);
	strcat(login, " ");
	strcat(login, password);
	strcat(login, " (log)");
	SendData2Server(client, login);
	char checkLogin[INPUT_STR_SIZE] = "";
	getData(client, checkLogin, INPUT_STR_SIZE);
	if (!strcmp(checkLogin, "login-ok")) {
		printf("\033[0d\033[2J");//clear console
		menu(client);
		return 0;
	}

	while (strcmp(checkLogin, "login-ok")) {
		printf("\033[0d\033[2J");//clear console
		printf("Invalid username or password, please try again\n");
		strcpy(tempStr, "Login: ");
		printStringInCenter(tempStr);
		scanf("%s", login);
		strcpy(tempStr, "Password: ");
		printStringInCenter(tempStr);
		scanf("%s", password);
		strcat(login, " ");
		strcat(login, password);
		strcat(login, " (log)");
		SendData2Server(client, login);
		getData(client, checkLogin, INPUT_STR_SIZE);
	}
	printf("\033[0d\033[2J");//clear console
	menu(client);
	return 0;
}

void* registration(void* params) {
	struct CLientSocket* param = (struct CLientSocket*)params;
	SOCKET client = param->s;

	char login[INPUT_STR_SIZE] = { 0 };
	char password[INPUT_STR_SIZE] = { 0 };
	char verify_password[INPUT_STR_SIZE] = { 0 };
	char tempStr[INPUT_STR_SIZE] = { 0 };
	strcpy(tempStr, "Login: ");
	printStringInCenter(tempStr);
	scanf("%s", login);
	strcpy(tempStr, "Password: ");
	printStringInCenter(tempStr);
	scanf("%s", password);
	strcpy(tempStr, "Repeat your password, please: ");
	printStringInCenter(tempStr);
	scanf("%s", verify_password);
	if (strcmp(password, verify_password)) {
		printf("Passwords don't match, try again\n");
		registration(NULL);
	}

	strcat(login, " ");
	strcat(login, password);
	strcat(login, " (reg)");
	SendData2Server(client,login);
	menu(client);
	return SUCCESS;
}





void setDefaultFieldMenu() {

	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 6;
	cfi.dwFontSize.X = 20;                   // Width of each character in the font
	cfi.dwFontSize.Y = 20;                  // Height
	cfi.FontFamily = BALTIC_CHARSET;
	cfi.FontWeight = FW_NORMAL;
	//std::wcscpy(cfi.FaceName, L"Consolas"); // Choose your font
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

	system("mode con cols=55 lines=28");
	HWND consoleWindow = GetConsoleWindow();
	SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
}

int main(){
	WSADATA wsd;
	if(WSAStartup(MAKEWORD(1, 1), &wsd) != 0){
		printf("Can't connect to socket lib.");
		return 1;
	}

	char anchor[][SIZE_CONSOLE + 1] = {
		"                                  -*+*+:.              ",
		"                                 :-    .+.             ",
		"                                 *.     :-             ",
		"                                 .*::*::-              ",
		"                                  +=*+=+-              ",
		"                                  .:***.               ",
		"                                  .*-.:-               ",
		"                        .-:........**+*........--.     ",
		"Welcome to Sea Battle!  *=*++++++++%*+*++++++++=**-    ",
		"                         .:.       -*+:        .-      ",
		"                                   -*+:                ",
		"                                   -*+:                ",
		"                                   -*+*                ",
		"                                   -*+*                ",
		"                                   -*+*                ",
		"                   :+-             -***.            .*-",
		"                    ==*-.          :***.          .+**.",
		"                    .===::.        :***.        -++++- ",
		"                      *==*-.      .**:*-      .-*++*.  ",
		"                       .+=***:-..-:****+:..-:*+++*-    ",
		"                         .*==+++++++=++++++++++*-      ",
		"                            .:=====%%+======*-         ",
		"                                 .:+%=+*-.             "
	};

	SOCKET client_socket;
	client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if (client_socket == INVALID_SOCKET)
	{
		printf("Error create socket\n");
		return -1;
	}
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(5510); //the same as in server
	server.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); //special look-up address
	if (connect(client_socket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Can't connect to server\n");
		closesocket(client_socket);
		return -1;
	}
	struct CLientSocket client;
	client.s = client_socket;

	setDefaultFieldMenu();
	srand(time(NULL));
	int id = rand() % 10000;
	char isRegistered = { 0 };
	char tempStr[STR_SIZE] = "";
	void** value = 0;


	while (1) {
		for (int i = 0; i < ANCHOR_HEIGHT;  i++) {
				printf("%s\n",anchor[i]);
		}
		printf("\n\n\n");
		strcpy(tempStr,"Are you registered?(Y/N)");
		printStringInCenter(tempStr);
		scanf("%c",&isRegistered);
		printf("\033[0d\033[2J");//clear console
		if (isRegistered == 'Y' || isRegistered == 'y') {
			pthread_t signInThread;
			int status = pthread_create(&signInThread, NULL, login, (void*)(&client));
			pthread_join(signInThread, value);
			break;
		}
		else if (isRegistered == 'N' || isRegistered == 'n') {
			pthread_t signInThread;
			int status = pthread_create(&signInThread, NULL, registration, (void*)(&client));
			pthread_join(signInThread, value);
			break;
		}
		else {
			strcpy(tempStr, "Invalid character, try again\n");
			printStringInCenter(tempStr);
			Sleep(2000);
		}
	}
	playGame();
	printf("Session is closed.\n");
	Sleep(1000);
	return 0;
}