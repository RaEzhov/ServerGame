#include "game.h"
#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>
#pragma comment(lib, "ws2_32.lib")
#include <winsock.h>
#include <stdio.h>
#include <Windows.h>

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
		printf ("Can't resieve dsta.\n");
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
		printf ("Can't resieve dsta.\n");
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
	/*
	ret = recv(client, recieve, 1024, 0);
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

	pthread_mutex_lock(&mutex);
	pthread_mutex_lock(&mutex_file);
	fprintf(stdout, "test");
	printf("%s\n", recieve);
	pthread_mutex_unlock(&mutex_file);
	pthread_mutex_unlock(&mutex);
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

int CreateServer(){
	SOCKET server, client1, client2;
	struct sockaddr_in localaddr, clientaddr;
	int size;
	server = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if (server == INVALID_SOCKET)
	{
		printf("Error create server\n");
		return 1;
	}
	localaddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	localaddr.sin_family = AF_INET;
	localaddr.sin_port = htons(5510);//port number is for example, must be more than 1024
	if (bind(server, (struct sockaddr*)&localaddr, sizeof(localaddr)) == SOCKET_ERROR)
	{
		printf("Can't start server\n");
		return 2;
	} else {
		printf("Server is started\n");
	}
	listen(server, 50);//50 клиентов в очереди могут стоять
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
			printf("Error accept client\n");
			continue;
		} else {
			printf("Client is accepted\n");
		}

		/*
			pthread_t mythread;
			int status = pthread_create(&mythread, NULL, logInAndMenu, (void*)client);
			pthread_detach(mythread);
		*/
		//=========logInAndMenu========
		// кидаем всех клиентов в подтверждение логина и пароля (поток)
		// -- (клиент в меню выбирает действие)
		// если новая игра, то организовать запуск игры по двое для всех подряд кто выбрал ng
		// если список друзей, то найти соответствие в файле с друзьями этого пользователя и попытаться их соединить
		// если рейтинг, то тянем из файла уровень и предлагаем играть с тем же уровнем 
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
	printf("Server is stopped\n");
	closesocket(server);
	return 0;
}

int main(){
	WSADATA wsd;
	if (WSAStartup(MAKEWORD(1, 1), &wsd) == 0)
	{
		printf("Connected to socket lib\n");
	} else {
		return 1;
	}
	return CreateServer();
}