#include "game.h"
#define _CRT_SECURE_NO_WARNINGS
#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>
#pragma comment(lib, "ws2_32.lib")
#include <winsock.h>
#include <stdio.h>
#include <Windows.h>
#include <conio.h>

#define INPUT_STR_SIZE 50

struct players {
	SOCKET player1;
	SOCKET player2;
};

pthread_mutex_t mutex;
pthread_mutex_t mutex_file;

void sendField (SOCKET s, unsigned char field[][GAME_FIELD_LINES]){
	int ret = send (s, (char*)field, BUFFER, 0);
	if (ret == SOCKET_ERROR){
		printf ("Can't send message.\n");
		closesocket (s);
		exit (10);
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

void getField (SOCKET s, unsigned char field[][GAME_FIELD_LINES]){
	int ret = recv (s, (char*)field, BUFFER, 0);
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

void shot (SOCKET shooter, SOCKET target, unsigned char fieldTarget[][GAME_FIELD_LINES], int* shipsPlayer){
	char buf[BUFFER] = {0};
	buf[0] = 's'; //shoot
	sendData (shooter, buf, 1);
	buf[0] = 'w'; //wait
	sendData (target, buf, 1);
	getField (shooter, fieldTarget);  //get field 2 after shot
	getData (shooter, buf, 1);   //get status of shot
	(buf[0] == 1) ? (*shipsPlayer)-- : 1;
	sendField (target, fieldTarget);
	sendData (target, buf, 1);
	if (buf[0] == 2 || buf[0] == 1){
		shot (shooter, target, fieldTarget, shipsPlayer);
	}
}

void* ClientStart(void* param) {
	SOCKET client1 = ((struct players*)param)->player1;
	SOCKET client2 = ((struct players*)param)->player2;
	char recieve[1024], transmit[1024];
	unsigned char field1[10][10] = { 0 };
	unsigned char field2[10][10] = { 0 };
	getField (client1, field1);
	getField (client2, field2);
	/*ret = recv(client1, (char*)field1, 100, 0);
	if (!ret || ret == SOCKET_ERROR)
	{
		pthread_mutex_lock(&mutex);
		pthread_mutex_lock(&mutex_file);
		printf("Error getting data\n");
		fprintf(stdout, "test");
		pthread_mutex_unlock(&mutex_file);
		pthread_mutex_unlock(&mutex);
		return (void*)1;
	}
	ret = recv(client2, (char*)field2, 100, 0);
	if (!ret || ret == SOCKET_ERROR)
	{
		pthread_mutex_lock(&mutex);
		pthread_mutex_lock(&mutex_file);
		printf("Error getting data\n");
		fprintf(stdout, "test");
		pthread_mutex_unlock(&mutex_file);
		pthread_mutex_unlock(&mutex);
		return (void*)1;
	}*/
	sendField (client1, field2);
	sendField (client2, field1);
	
	int ships1Player = 10, ships2Player= 10;
	while (ships1Player && ships2Player){
		shot (client1, client2, field2, &ships2Player);
		if (ships2Player == 0){
			break;
		}
		shot (client2, client1, field1, &ships1Player);
	}
	_getch ();
	/*
	ret = recv(client, recieve, 1024, 0);
=======
void sendField(SOCKET s, unsigned char field[][GAME_FIELD_LINES]) {
	int ret = send(s, (char*)field, BUFFER, 0);
	if (ret == SOCKET_ERROR) {
		printf("Can't send message.\n");
		closesocket(s);
		exit(10);
	}
}

void sendData(SOCKET s, char buf[], int size) {
	int ret = send(s, buf, size, 0);
	if (ret == SOCKET_ERROR) {
		printf("Can't send message.\n");
		closesocket(s);
		exit(10);
	}
}

void getField(SOCKET s, unsigned char field[][GAME_FIELD_LINES]) {
	int ret = recv(s, (char*)field, BUFFER, 0);
	if (ret == 0 || ret == WSAECONNRESET) {
		printf("Connection closed.\n");
		exit(10);
	}
	if (ret < 0) {
		printf("Can't recieve data.\n");
		closesocket(s);
		exit(10);
	}
}

void getData(SOCKET s, char buf[], int size) {
	int ret = recv(s, buf, size, 0);
	if (ret == 0 || ret == WSAECONNRESET) {
		printf("Connection closed.\n");
		exit(10);
	}
	if (ret < 0) {
		printf("Can't recieve data.\n");
		closesocket(s);
		exit(10);
	}
}

void shot(SOCKET shooter, SOCKET target, unsigned char fieldTarget[][GAME_FIELD_LINES], int* shipsPlayer) {
	char buf[BUFFER] = { 0 };
	buf[0] = 's'; //shoot
	sendData(shooter, buf, 1);
	buf[0] = 'w'; //wait
	sendData(target, buf, 1);
	getField(shooter, fieldTarget);  //get field 2 after shot
	getData(shooter, buf, 1);   //get status of shot
	(buf[0] == 1) ? (*shipsPlayer)-- : 1;
	sendField(target, fieldTarget);
	sendData(target, buf, 1);
	if (buf[0] == 2 || buf[0] == 1) {
		shot(shooter, target, fieldTarget, shipsPlayer);
	}
}

void* ClientStart(void* param) {
	SOCKET client1 = ((struct players*)param)->player1;
	SOCKET client2 = ((struct players*)param)->player2;
	char recieve[1024], transmit[1024];
	unsigned char field1[10][10] = { 0 };
	unsigned char field2[10][10] = { 0 };
	getField(client1, field1);
	getField(client2, field2);
	
	sendField(client1, field2);
	sendField(client2, field1);

	int ships1Player = 10, ships2Player = 10;
	while (ships1Player && ships2Player) {
		shot(client1, client2, field2, &ships2Player);
		if (ships2Player == 0) {
			break;
		}
		shot(client2, client1, field1, &ships1Player);
	}
	_getch();
	
	return (void*)0;
}

int searchInDB(char* login, char* password) {
	printf("searching %s with password %s\n",login,password);
	FILE* DB = fopen("DATABASE.txt","r");
	char readedLogin[INPUT_STR_SIZE] = {0};
	char readedPassword[INPUT_STR_SIZE] = {0};
	char dbString[INPUT_STR_SIZE];
	while (!feof(DB)) {
		fgets(dbString, INPUT_STR_SIZE*2, DB);
		sscanf(dbString,"%s %s", readedLogin, readedPassword);
		if (!strcmp(login, readedLogin) && !strcmp(password, readedPassword)) {
			fclose(DB);
			return 1;
		}
	}
	fclose(DB);
	return 0;
}

int loginClient(char* login, char* password) {
	if (searchInDB(login,password)) {
		return 1;
	}
	return 0;
}

void registrateClient(char* login, char* password) {
	printf("trying to registrate %s with password %s\n",login,password);
	FILE* DB = fopen("DATABASE.txt","a");
	fprintf(DB,"%s %s\n",login,password);
	fclose(DB);
}

void menu(SOCKET client) {
	char menuCommand[INPUT_STR_SIZE] = { 0 };
	getData(client,menuCommand, INPUT_STR_SIZE);
	if (!strcmp(menuCommand, "(ng)")) {

	}
}

void* logInAndMenu(void* params) {
	SOCKET client = (SOCKET)params;
	char recieve[INPUT_STR_SIZE] = "";
	int ret;
	char login[INPUT_STR_SIZE] = { 0 };
	char password[INPUT_STR_SIZE] = { 0 };
	char flag[INPUT_STR_SIZE] = { 0 };
	char loginOk[INPUT_STR_SIZE] = "login-ok";
	char loginError[INPUT_STR_SIZE] = "login-er";

	ret = recv(client, recieve, INPUT_STR_SIZE, 0);
>>>>>>> clientPart
	if (!ret || ret == SOCKET_ERROR)
	{
		pthread_mutex_lock(&mutex);
		pthread_mutex_lock(&mutex_file);
		printf("Error getting data\n");
		fprintf(stdout, "test");
		pthread_mutex_unlock(&mutex_file);
		pthread_mutex_unlock(&mutex);
		return (void*)1;
	}
	
	recieve[ret] = '\0';
	sscanf(recieve, "%s%s%s", login, password, flag);

	pthread_mutex_lock(&mutex);
	pthread_mutex_lock(&mutex_file);
	fprintf(stdout, "accepted data: ");
	printf("%s\n", recieve);
	fprintf(stdout, "flag: ");
	printf("%s\n", flag);
	if (!strcmp(flag, "(log)")) {
		
		if (loginClient(login, password)) {
			sendData(client,loginOk,strlen(loginOk));
		}
		else {
			while (!loginClient(login, password)) {
				printf("not found %s\n",login);
				sendData(client, loginError, strlen(loginError));
				char recieve[INPUT_STR_SIZE] = { 0 };
				int ret;
				ret = recv(client, recieve, INPUT_STR_SIZE, 0);
				if (!ret || ret == SOCKET_ERROR){
					pthread_mutex_lock(&mutex);
					pthread_mutex_lock(&mutex_file);
					printf("Error getting data\n");
					fprintf(stdout, "test");
					pthread_mutex_unlock(&mutex_file);
					pthread_mutex_unlock(&mutex);
					return (void*)1;
				}

				recieve[ret] = '\0';
				sscanf(recieve, "%s%s%s", login, password, flag);
				fprintf(stdout, "accepted data: ");
				printf("%s\n", recieve);
				fprintf(stdout, "flag: ");
				printf("%s\n", flag);
				fprintf(stdout, "login: ");
				printf("%s\n", login);
			}
			sendData(client, loginOk, strlen(loginOk));
		}
		menu(client);
	}
	else if (!strcmp(flag, "(reg)")) {
		registrateClient(login,password);
		menu(client);
	}
	pthread_mutex_unlock(&mutex_file);
	pthread_mutex_unlock(&mutex);
<<<<<<< HEAD
	sprintf_s(transmit, "%s %s %s\n", "Your data", recieve, "recieved");
	//Sleep(2000);
	ret = send(client, transmit, sizeof(transmit), 0);
	if (ret == SOCKET_ERROR)
	{
		pthread_mutex_lock(&mutex);
		pthread_mutex_lock(&mutex_file);
		printf("Error sending data\n");
		pthread_mutex_unlock(&mutex_file);
		pthread_mutex_unlock(&mutex);
		return (void*)2;
	}
	*/
	return (void*)0;
}

int CreateServer() {
	SOCKET server, client1, client2;
	struct sockaddr_in localaddr, clientaddr;
	int size;
	server = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if (server == INVALID_SOCKET)
	{
		printf("Error create server.\n");
		return 1;
	}
	localaddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	localaddr.sin_family = AF_INET;
	localaddr.sin_port = htons(5510);//port number is for example, must be more than 1024
	if (bind(server, (struct sockaddr*)&localaddr, sizeof(localaddr)) == SOCKET_ERROR)
	{
		printf("Can't start server.\n");
		return 2;
	}
	else {
		printf("Server is started.\n");
	}
	listen(server, 50);//50 ÃªÃ«Ã¨Ã¥Ã­Ã²Ã®Ã¢ Ã¢ Ã®Ã·Ã¥Ã°Ã¥Ã¤Ã¨ Ã¬Ã®Ã£Ã³Ã² Ã±Ã²Ã®Ã¿Ã²Ã¼
	pthread_mutex_init(&mutex, NULL);
	int status = 0;
	pthread_mutex_init(&mutex_file, NULL);
	struct players clients = { 0,0 };
	while (1)
	{
		size = sizeof(clientaddr);
		client1 = accept(server, (struct sockaddr*)&clientaddr, &size);

		if (client1 == INVALID_SOCKET)
		{
			printf("Error accept client.\n");
			continue;
		} else {
			printf("Client is accepted.\n");
		}

		/*
			pthread_t mythread;
			int status = pthread_create(&mythread, NULL, logInAndMenu, (void*)client);
			pthread_detach(mythread);
		*/
		//=========logInAndMenu========
		// êèäàåì âñåõ êëèåíòîâ â ïîäòâåðæäåíèå ëîãèíà è ïàðîëÿ (ïîòîê)
		// -- (êëèåíò â ìåíþ âûáèðàåò äåéñòâèå)
		// åñëè íîâàÿ èãðà, òî îðãàíèçîâàòü çàïóñê èãðû ïî äâîå äëÿ âñåõ ïîäðÿä êòî âûáðàë ng
		// åñëè ñïèñîê äðóçåé, òî íàéòè ñîîòâåòñòâèå â ôàéëå ñ äðóçüÿìè ýòîãî ïîëüçîâàòåëÿ è ïîïûòàòüñÿ èõ ñîåäèíèòü
		// åñëè ðåéòèíã, òî òÿíåì èç ôàéëà óðîâåíü è ïðåäëàãàåì èãðàòü ñ òåì æå óðîâíåì 
		//=============================

		/*status++;
		if (status % 2 == 0) {
			clients.player1 = client1;
			clients.player2 = client2;
			pthread_t mythread;
			int status = pthread_create(&mythread, NULL, ClientStart, (void*)(&clients));
			pthread_detach(mythread);
		}*/

		
			pthread_t newClient;
			printf("socket: %d",client1);
			int status = pthread_create(&newClient, NULL, logInAndMenu, (void*)client1);
			pthread_detach(newClient);
		
		//=========logInAndMenu========
		// ÃªÃ¨Ã¤Ã Ã¥Ã¬ Ã¢Ã±Ã¥Ãµ ÃªÃ«Ã¨Ã¥Ã­Ã²Ã®Ã¢ Ã¢ Ã¯Ã®Ã¤Ã²Ã¢Ã¥Ã°Ã¦Ã¤Ã¥Ã­Ã¨Ã¥ Ã«Ã®Ã£Ã¨Ã­Ã  Ã¨ Ã¯Ã Ã°Ã®Ã«Ã¿ (Ã¯Ã®Ã²Ã®Ãª)
		// -- (ÃªÃ«Ã¨Ã¥Ã­Ã² Ã¢ Ã¬Ã¥Ã­Ã¾ Ã¢Ã»Ã¡Ã¨Ã°Ã Ã¥Ã² Ã¤Ã¥Ã©Ã±Ã²Ã¢Ã¨Ã¥)
		// Ã¥Ã±Ã«Ã¨ Ã­Ã®Ã¢Ã Ã¿ Ã¨Ã£Ã°Ã , Ã²Ã® Ã®Ã°Ã£Ã Ã­Ã¨Ã§Ã®Ã¢Ã Ã²Ã¼ Ã§Ã Ã¯Ã³Ã±Ãª Ã¨Ã£Ã°Ã» Ã¯Ã® Ã¤Ã¢Ã®Ã¥ Ã¤Ã«Ã¿ Ã¢Ã±Ã¥Ãµ Ã¯Ã®Ã¤Ã°Ã¿Ã¤ ÃªÃ²Ã® Ã¢Ã»Ã¡Ã°Ã Ã« ng
		// Ã¥Ã±Ã«Ã¨ Ã±Ã¯Ã¨Ã±Ã®Ãª Ã¤Ã°Ã³Ã§Ã¥Ã©, Ã²Ã® Ã­Ã Ã©Ã²Ã¨ Ã±Ã®Ã®Ã²Ã¢Ã¥Ã²Ã±Ã²Ã¢Ã¨Ã¥ Ã¢ Ã´Ã Ã©Ã«Ã¥ Ã± Ã¤Ã°Ã³Ã§Ã¼Ã¿Ã¬Ã¨ Ã½Ã²Ã®Ã£Ã® Ã¯Ã®Ã«Ã¼Ã§Ã®Ã¢Ã Ã²Ã¥Ã«Ã¿ Ã¨ Ã¯Ã®Ã¯Ã»Ã²Ã Ã²Ã¼Ã±Ã¿ Ã¨Ãµ Ã±Ã®Ã¥Ã¤Ã¨Ã­Ã¨Ã²Ã¼
		// Ã¥Ã±Ã«Ã¨ Ã°Ã¥Ã©Ã²Ã¨Ã­Ã£, Ã²Ã® Ã²Ã¿Ã­Ã¥Ã¬ Ã¨Ã§ Ã´Ã Ã©Ã«Ã  Ã³Ã°Ã®Ã¢Ã¥Ã­Ã¼ Ã¨ Ã¯Ã°Ã¥Ã¤Ã«Ã Ã£Ã Ã¥Ã¬ Ã¨Ã£Ã°Ã Ã²Ã¼ Ã± Ã²Ã¥Ã¬ Ã¦Ã¥ Ã³Ã°Ã®Ã¢Ã­Ã¥Ã¬ 
		//=============================

		status++;
		if (status % 2 == 0) {
			clients.player1 = client1;
			clients.player2 = client2;
			pthread_t mythread;
			int status = pthread_create(&mythread, NULL, ClientStart, (void*)(&clients));
			pthread_detach(mythread);
		}
		client2 = client1;
	}
	pthread_mutex_destroy(&mutex_file);
	pthread_mutex_destroy(&mutex);
	printf("Server is stopped.\n");
	closesocket(server);
	return 0;
}

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