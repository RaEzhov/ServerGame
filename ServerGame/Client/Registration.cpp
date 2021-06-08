#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INPUT_STR_SIZE 50
#include "Registration.h"
#include "Client.h"

void login(int clientId) {
	char login[INPUT_STR_SIZE] = { 0 };
	char password[INPUT_STR_SIZE] = { 0 };
	printf("Login: ");
	scanf("%s",login);
	printf("Password: ");
	scanf("%s", password);
	strcat(login, " ");
	strcat(login, password);
	strcat(login, " -log");
	SendData2Server(clientId, login);
}

void registration(int clientId) {
	char login[INPUT_STR_SIZE] = { 0 };
	char password[INPUT_STR_SIZE] = { 0 };
	char verify_password[INPUT_STR_SIZE] = { 0 };
	printf("Login: ");
	scanf("%s", login);
	printf("Password: ");
	scanf("%s", password);
	printf("Repeat your password, please: ");
	scanf("%s", verify_password);
	if (strcmp(password, verify_password)) {
		printf("Passwords don't match, try again");
		registration(clientId);
	}

	strcat(login, " ");
	strcat(login, password);
	strcat(login, " -reg");
	SendData2Server(clientId, login);
}