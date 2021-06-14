#ifndef COMMONFUNCTIONS_H
#define COMMONFUNCTIONS_H

#define GAME_FIELD_LINES 10
#define BUFFER 100

int sendField (SOCKET s, unsigned char field[][GAME_FIELD_LINES]);
int sendData (SOCKET s, char buf[], int size);
int getField (SOCKET s, unsigned char field[][GAME_FIELD_LINES]);
int getData (SOCKET s, char buf[], int size);

#endif//COMMONFUNCTIONS_H