#define HAVE_STRUCT_TIMESPEC
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <Winsock.h>
#include <pthread.h>
#include "../commonFunctions.h"
#include "loginAndMenu.h"
#include "game.h"

pthread_mutex_t mutex;
pthread_mutex_t mutex_file;

int searchInDB (char* login, char* password){
	printf ("searching %s with password %s\n", login, password);
	FILE* DB = fopen ("DATABASE.txt", "r");
	char readedLogin[INPUT_STR_SIZE] = {0};
	char readedPassword[INPUT_STR_SIZE] = {0};
	char dbString[INPUT_STR_SIZE * 2] = {0};
	while (!feof (DB)){
		fgets (dbString, INPUT_STR_SIZE * 2, DB);
		sscanf (dbString, "%s %s", readedLogin, readedPassword);
		if (!strcmp (login, readedLogin) && !strcmp (password, readedPassword)){
			fclose (DB);
			return 1;
		}
	}
	fclose (DB);
	return 0;
}

int loginClient (char* login, char* password){
	if (searchInDB (login, password)){
		return 1;
	}
	return 0;
}

void registrateClient (char* login, char* password){
	printf ("trying to registrate %s with password %s\n", login, password);
	FILE* DB = fopen ("DATABASE.txt", "a");
	fprintf (DB, "%s %s\n", login, password);
	fclose (DB);
}

void menuServ (SOCKET client){
	char menuCommand[INPUT_STR_SIZE] = {0};
	getData (client, menuCommand, INPUT_STR_SIZE);
	if (!strcmp (menuCommand, "(ng)")){

	}
}

void* logInAndMenu (void* params){
	SOCKET client = (SOCKET)params;
	char recieve[INPUT_STR_SIZE+1] = "";
	int ret;
	char login[INPUT_STR_SIZE] = {0};
	char password[INPUT_STR_SIZE] = {0};
	char flag[INPUT_STR_SIZE] = {0};
	char loginOk[] = "login-ok";
	char loginError[] = "login-er";

	ret = getData (client, recieve, INPUT_STR_SIZE);
	printf ("%d", ret);
	recieve[ret] = '\0';
	sscanf (recieve, "%s%s%s", login, password, flag);
	
	pthread_mutex_lock (&mutex);
	pthread_mutex_lock (&mutex_file);
	printf ("\n%s\n", recieve);
	fprintf (stdout, "accepted data: ");
	printf ("%s\n", recieve);
	fprintf (stdout, "flag: ");
	printf ("%s\n", flag);

	if (!strcmp (flag, "(log)")){
		if (loginClient (login, password)){
			sendData (client, loginOk, strlen (loginOk));
		} else{
			while (!loginClient (login, password)){
				printf ("not found %s\n", login);
				sendData (client, loginError, strlen (loginError));
				char recieve[INPUT_STR_SIZE] = {0};
				int ret;
				ret = getData (client, recieve, INPUT_STR_SIZE);
				recieve[ret] = '\0';

				sscanf (recieve, "%s%s%s", login, password, flag);
				fprintf (stdout, "accepted data: ");
				printf ("%s\n", recieve);
				fprintf (stdout, "flag: ");
				printf ("%s\n", flag);
				fprintf (stdout, "login: ");
				printf ("%s\n", login);
			}
			sendData (client, loginOk, strlen (loginOk));
		}
		menuServ (client);
	} else if (!strcmp (flag, "(reg)")){
		registrateClient (login, password);
		menuServ (client);
	}
	pthread_mutex_unlock (&mutex_file);
	pthread_mutex_unlock (&mutex);
	return (void*)0;
}

int CreateServer (){
	SOCKET server, client;
	struct sockaddr_in localaddr, clientaddr;
	int size;
	server = socket (AF_INET, SOCK_STREAM, IPPROTO_IP);
	if (server == INVALID_SOCKET){
		printf ("Error create server.\n");
		return 1;
	}
	localaddr.sin_addr.S_un.S_addr = htonl (INADDR_ANY);
	localaddr.sin_family = AF_INET;
	localaddr.sin_port = htons (5510);//port number is for example, must be more than 1024
	if (bind (server, (struct sockaddr*)&localaddr, sizeof (localaddr)) == SOCKET_ERROR){
		printf ("Can't start server.\n");
		return 2;
	} else{
		printf ("Server is started.\n");
	}
	listen (server, 50);
	pthread_mutex_init (&mutex, NULL);
	int status = 0;
	pthread_mutex_init (&mutex_file, NULL);
	struct players clients = {0,0};
	while (1){
		size = sizeof (clientaddr);
		client = accept (server, (struct sockaddr*)&clientaddr, &size);

		if (client == INVALID_SOCKET){
			printf ("Error accept client.\n");
			continue;
		} else{
			printf ("Client is accepted.\n");
		}
		//=========logInAndMenu========
		// кидаем всех клиентов в подтверждение логина и пароля (поток)
		// -- (клиент в меню выбирает действие)
		// если новая игра, то организовать запуск игры по двое для всех подряд кто выбрал ng
		// если список друзей, то найти соответствие в файле с друзьями этого пользователя и попытаться их соединить
		// если рейтинг, то тянем из файла уровень и предлагаем играть с тем же уровнем 
		//=============================
		pthread_t newClient;
		printf ("socket: %d", client);
		int status = pthread_create (&newClient, NULL, logInAndMenu, (void*)client);
		pthread_detach (newClient);

		/*tatus++;
		if (status % 2 == 0){
			clients.player1 = client1;
			clients.player2 = client2;
			pthread_t mythread;
			int status = pthread_create (&mythread, NULL, startGame, (void*)(&clients));
			pthread_detach (mythread);
		}
		client2 = client1;*/
	}
	pthread_mutex_destroy (&mutex_file);
	pthread_mutex_destroy (&mutex);
	printf ("Server is stopped.\n");
	shutdown (server, 2);
	closesocket (server);
	return 0;
}
