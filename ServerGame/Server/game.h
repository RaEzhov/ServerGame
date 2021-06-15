#ifndef GAME_H
#define GAME_H

#ifndef GAME_FIELD_LINES
#define GAME_FIELD_LINES 10
#endif//GAME_FIELD_LINES

struct players{
	SOCKET player1;
	SOCKET player2;
	char login1[BUFFER / 2];
	char login2[BUFFER / 2];
};

void* startGame (void* param);

int shot (SOCKET shooter, SOCKET target, unsigned char fieldTarget[][GAME_FIELD_LINES], int* shipsPlayer);

#endif//GAME_H