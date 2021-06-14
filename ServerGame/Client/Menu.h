#ifndef MENU_H
#define MENU_H

//Registration 
#define INPUT_STR_SIZE       50
#define SUCCESS        (void*)0

void* login (void* params);//function requires login and password for registred users
void* registration (void* params);//function registers new users

//Menu
#define SIZE_CONSOLE 55

void printStringInCenter (char* text);

void printStringWithPaddingLeft (char* text, int padding);

void newGame (SOCKET client);

void showFriends (SOCKET client);

void showRatings (SOCKET client);

void exit ();

void menu (SOCKET client);

void menuList ();

#endif //MENU_h