#define _CRT_SECURE_NO_WARNINGS
#include <winsock.h>
#include <stdio.h>
#include "../commonFunctions.h"
#include "game.h"

int shot (SOCKET shooter, SOCKET target, unsigned char fieldTarget[][GAME_FIELD_LINES], int* shipsTarget){
	char buf[BUFFER] = {0};
	buf[0] = 's'; //shoot
	int ret = sendData (shooter, buf, 1);
	if (ret < 0){
		return ret;
	}
	buf[0] = 'w'; //wait
	ret = sendData (target, buf, 1);
	if (ret < 0){
		return ret;
	}
	getField (shooter, fieldTarget);  //get field 2 after shot
	getData (shooter, buf, 1);   //get status of shot
	(buf[0] == 1) ? (*shipsTarget)-- : 1;
	sendField (target, fieldTarget);
	sendData (target, buf, 1);
	if (buf[0] == 2 || buf[0] == 1){
		shot (shooter, target, fieldTarget, shipsTarget);
	}
}

void* startGame (void* param){
	struct players* clients = (struct players*)(param);
	SOCKET client1 = clients->player1;
	SOCKET client2 = clients->player2;
	char log1[BUFFER / 2], log2[BUFFER / 2];
	strcpy (log1, clients->login1);
	strcpy (log2, clients->login2);
	char msg[] = "connectionOK";
	sendData (client1, msg, strlen (msg));
	sendData (client2, msg, strlen (msg));
	Sleep (1000);
	sendData (client1, log1, strlen (log1));
	sendData (client1, log2, strlen (log2));
	sendData (client2, log2, strlen (log2));	
	sendData (client2, log1, strlen (log1));

	char recieve[1024], transmit[1024];
	unsigned char field1[10][10] = {0};
	unsigned char field2[10][10] = {0};
	

	getField (client1, field1);
	getField (client2, field2);

	sendField (client1, field2);
	sendField (client2, field1);

	int ships1Player = 10, ships2Player = 10;
	int status = 0;
	while (ships1Player && ships2Player  && status >= 0){
		status = shot (client1, client2, field2, &ships2Player);
		if (ships2Player == 0 || status < 0){
			break;
		}
		shot (client2, client1, field1, &ships1Player);
	}

	return (void*)0;
}