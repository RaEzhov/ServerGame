#include <winsock.h>
#include <stdio.h>
#include "../commonFunctions.h"
#include "game.h"

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

void* startGame (void* param){
	struct players* clients = (struct players*)(param);
	SOCKET client1 = clients->player1;
	SOCKET client2 = clients->player2;
	char recieve[1024], transmit[1024];
	unsigned char field1[10][10] = {0};
	unsigned char field2[10][10] = {0};
	char msg[] = "connectionOK";
	sendData (client1, msg, strlen (msg));
	sendData (client2, msg, strlen (msg));

	getField (client1, field1);
	getField (client2, field2);

	sendField (client1, field2);
	sendField (client2, field1);

	int ships1Player = 10, ships2Player = 10;
	while (ships1Player && ships2Player){
		shot (client1, client2, field2, &ships2Player);
		if (ships2Player == 0){
			break;
		}
		shot (client2, client1, field1, &ships1Player);
	}

	return (void*)0;
}