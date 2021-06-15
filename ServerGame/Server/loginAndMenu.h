#ifndef LOGINANDMENU_H
#define LOGINANDMENU_H

#define INPUT_STR_SIZE 50

struct usersOnServer{
    SOCKET sock[INPUT_STR_SIZE];
    char login[INPUT_STR_SIZE][INPUT_STR_SIZE];
    int amount;
};

int searchInDB (char* login, char* password);

int loginClient (char* login, char* password);

void registrateClient (char* login, char* password);

void menuServ (SOCKET client);

void* logInAndMenu (void* params);

int CreateServer ();

#endif//LOGINANDMENU_H