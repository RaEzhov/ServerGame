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
#include <Windows.h>

#define SIZE_CONSOLE 55
#define ANCHOR_HEIGHT 23
#define ANCHOR_WEIGHT 37
#define STR_SIZE 50

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

void printStringInCenter(char* text) {
	int width = SIZE_CONSOLE;
	int padding = (width - strlen(text)) / 2;
	for (int i = 0; i < padding; i++) {
		printf(" ");
	}
	printf("%s\n",text);
}

int main()
{
	WSADATA wsd;
	if (WSAStartup(MAKEWORD(1, 1), &wsd) != 0)
	{
		printf("Can't connect to socket lib");
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


	setDefaultFieldMenu();
	srand(time(NULL));
	int id = rand() % 10000;
	char isRegistered = { 0 };
	char tempStr[STR_SIZE] = "";
	while (1) {
		for (int i = 0; i < ANCHOR_HEIGHT;  i++) {
				printf("%s\n",anchor[i]);
		}
		printf("\n\n\n");
		strcpy(tempStr,"Are you registered?(Y/N)");
		printStringInCenter(tempStr);
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
			strcpy(tempStr, "Invalid character, try again\n");
			printStringInCenter(tempStr);
			Sleep(2000);
		}
	}
	menu(id);
	printf("Session is closed\n");
	return 0;
}

