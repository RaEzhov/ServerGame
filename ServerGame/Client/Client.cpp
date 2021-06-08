// client.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#pragma comment(lib, "ws2_32.lib")
#define _CRT_SECURE_NO_WARNINGS
#include <winsock.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "Registration.h"
#include "Menu.h"

void SendData2Server(int clientId, char* data)
{
	SOCKET client;
	client = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if (client == INVALID_SOCKET)
	{
		printf("Error create socket\n");
		return;
	}
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(5510); //the same as in server
	server.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); //special look-up address
	if (connect(client, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Can't connect to server\n");
		closesocket(client);
		return;
	}
	char message[1024];
	sprintf(message, "<clientId: %d> %s", clientId, data);

	int ret = send(client, message, strlen(message), 0);
	if (ret == SOCKET_ERROR)
	{
		printf("Can't send message\n");
		closesocket(client);
		return;
	}
	/*printf("Sent: %s\nbytes: %d\n\n", message, ret);*/
	ret = SOCKET_ERROR;
	int i = 0;
	while (ret == SOCKET_ERROR)
	{
		//полчение ответа
		ret = recv(client, message, 1024, 0);
		//обработка ошибок
		if (ret == 0 || ret == WSAECONNRESET)
		{
			printf("Connection closed\n");
			break;
		}
		if (ret < 0)
		{
			//printf("Can't resieve message\n");
			/*closesocket(client);
			return;*/
			continue;
		}
		//вывод на экран количества полученных байт и сообщение
		/*printf("Recieve: %s\n bytes: %d\n", message, ret);*/
	}
	closesocket(client);
}

int main()
{
	WSADATA wsd;
	if (WSAStartup(MAKEWORD(1, 1), &wsd) != 0)
	{
		printf("Can't connect to socket lib");
		return 1;
	}
	srand(time(NULL));
	int id = rand() % 10000;
	char isRegistered = {0};
	while (1) {
		printf("Are you registered?(Y/N)");
		scanf("%c",&isRegistered);

		if (isRegistered == 'Y' || isRegistered == 'y') {
			login(id);
			break;
		}
		else if (isRegistered == 'N' || isRegistered == 'n') {
			registration(id);
			break;
		}
		else {
			printf("Invalid character, try again\n");
		}
	}
	menu(id);
	printf("Session is closed\n");
	return 0;
}

