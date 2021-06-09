#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define _WIN32_WINNT 0x0500
#include <Windows.h>
#include <conio.h>
#include <string.h>

#define FIELD_LINES 13
#define GAME_FIELD_LINES 10

#define PLAYER_1 0
#define PLAYER_2 ~PLAYER_1

#define KEY_ENTER   13
#define KEY_PRESSED 0xE0
#define KEY_ENTER   13
#define KEY_UP      72
#define KEY_RIGHT   77
#define KEY_DOWN    80
#define KEY_LEFT    75
#define SHOT        249 
#define SIZE_CONSOLE 55
#define BUFFER       100

typedef enum {
    EMPTY = 249,
    INJURED = 176,
    KILL = 88,
    SHIP = 178,
    MISS = 249
}eMoveStatus;

typedef enum {
    INIT = 0,
    DRAW,
    PROCESSING,
    EXIT
}eGameStatus;

const char gameName[SIZE_CONSOLE + 2] =
"____________________GAME BATTLESHIP____________________";

char border[SIZE_CONSOLE + 2] =
"_______________________________________________________";

char smile[][SIZE_CONSOLE + 2] =
{
"                       PRESS ANY KEY                   ",
"                   (> ^ _^) > < (^ _^ <)               "
};

char userName1[20] = "Vasya228";
char userName2[20] = "Petr Ivanovich";

char numberShips1[20] = "Your life ships: ";
char numberShips2[20] = "Rivals life ships: ";



char field[][SIZE_CONSOLE] =
{
"       ABCDEFGHIJ       SHIPS       ABCDEFGHIJ ",
"      qttttttttttw    BATTLESHIP   qttttttttttw",
"     0v          v    ####        0v          v",
"     1v          v                1v          v",
"     2v          v    CRUISER     2v          v",
"     3v          v    ###         3v          v",
"     4v          v                4v          v",
"     5v          v    DESTROYER   5v          v",
"     6v          v    ##          6v          v",
"     7v          v                7v          v",
"     8v          v    SUBMARINE   8v          v",
"     9v          v    #           9v          v",
"      zttttttttttx                 zttttttttttx"
};

//Названия кораблей :
//Однопалубный - submarine
//Двухпалубный - destroyer
//Трёхпалубный - cruiser
//Четырёхпалубный - battleship

char ship[][SIZE_CONSOLE + 1] = {
           "           ______             BATTLESHIP               ",
           "           X____X   STAGE 1 - THE PLACEMENT OF SHIPS.  ",
           "           X____X   EACH PLAYERS HAS 10 SHIPS OF VARI- ", 
           "           X          OUS SIZES. (1 PIECE - BATTLESHIP,",
           "     XXXXXXXXXXXX     2 PIECES -  CRUISERS, 3 PIECES - ",
           "     X           X       DESTROYERS AND 4 PIECES OF    ",
           "      X           X      SUBMARINES).YOU CANNOT PUT ANY",
           "      X           X      2 SHIPS SO THAT THEY TOUCH THE",
           "      X           X        SIDES AND CORNERS. STAGE 2 -",
           "      X           X         SHOOTING AT ENEMY SHIPS.   ",
           "     X           X          AFTER PLACING THE SHIPS,YOU",
           "     XXXXXXXXXXXX            WILL HAVE THE OPPORTUNITY ",
           "          X_X                   TO MAKE A SHOT AT THE  ",
           "          X_X           XXXX    ENEMY FIELD. IF YOU HIT",
           "XXX       X_X      XXXXXXX       AN ENEMY SHIP FOR THE ",
           " XXXXX    X_X     XXX__X      FIRST TIME,THEN THIS PART",
           " X__XXXXXXXXXXXXXXX___X     BECOMES WOUNDED. IF YOU HIT", 
           "  X_________________XX   A SUBMARINE,THEN IT IS KILLED.",
           "  X___XX__XX__XX___XX    YOUR MAIN GOAL IS TO DESTROY  ",                                         
           "  XX_____________XX     ALL ENEMY SHIPS. HAPPY BATTLES!",
           "   XX__________XX                                      ",
           "     XXXXXXXXXX                                        "
};

int checkY4[14] = { -1, -1, -1, -1, -1, -1, 0, 1, 1, 1, 1, 1, 1, 0 };
int checkX4[14] = { -1, 0, 1, 2, 3, 4, 4, 4, 3, 2, 1, 0, -1, -1 };

int checkY3[12] = { -1, -1, -1, -1, -1, 0, 1, 1, 1, 1, 1, 0 };
int checkX3[12] = { -1, 0, 1, 2, 3, 3, 3, 2, 1, 0, -1, -1 };

int checkY2[10] = { -1, -1, -1, -1, 0, 1, 1, 1, 1, 0 };
int checkX2[10] = { -1, 0, 1, 2, 2, 2, 1, 0, -1, -1 };

int checkY1[8] = { -1, -1, -1, 0, 1, 1,  1,  0 };
int checkX1[8] = { -1,  0,  1, 1, 1, 0, -1, -1 };

void setDefaultField();
void welcomePage();
void drawField(unsigned char player1Field[][10], unsigned char player2Field[][10], int yourNumberShips, int rivalsNumberShips);
int convertLetterToNumber(char letter);
int checkAround(unsigned char gameField[][10], int beginLetterCh, int beginFigure, int endLetterCh, int endFigure, int sizeOfShip, char flag);
void generationShips(int sizeOfShip, unsigned char gameField[][10], unsigned char gameField2[][10], int yourNumberShips, int rivalsNumberShips);
void shootToShip(unsigned char gameFieldRival[][10], int* y, int* x, int* counterForShips);
int checkLifeOrDieShip(char direct, int  y, int x, unsigned char gameFieldRival[][10]);
void makeDie(unsigned char gameFieldRival[][10], int y, int x);



int main() {

    int player = PLAYER_1;

    unsigned char p1Data[GAME_FIELD_LINES][GAME_FIELD_LINES] = { 0 };
    unsigned char p2Data[GAME_FIELD_LINES][GAME_FIELD_LINES] = { 0 };



    setDefaultField();

    welcomePage();

    for (int i = 0; i < GAME_FIELD_LINES; i++) {
        for (int j = 0; j < GAME_FIELD_LINES; j++) {
            p1Data[i][j] = EMPTY;
            p2Data[i][j] = EMPTY;
        }
    }


    p2Data[9][0] = SHIP;
    p2Data[9][1] = SHIP;
    p2Data[9][2] = SHIP;
    p2Data[9][3] = SHIP;

    p2Data[0][0] = SHIP;
    p2Data[0][1] = SHIP;
    p2Data[0][2] = SHIP;

    p2Data[0][9] = SHIP;
    p2Data[1][9] = SHIP;
    p2Data[2][9] = SHIP;

    p2Data[7][7] = SHIP;
    p2Data[7][8] = SHIP;

    p2Data[2][3] = SHIP;
    p2Data[2][4] = SHIP;

    p2Data[4][8] = SHIP;
    p2Data[4][9] = SHIP;

    p2Data[5][5] = SHIP;

    p2Data[5][2] = SHIP;

    p2Data[9][9] = SHIP;

    p2Data[2][6] = SHIP;

    int y = 0;
    int x = 0;
    int counterForShips = 10;

    drawField(p1Data, p2Data, 10, counterForShips);
    generationShips(4, p1Data, p2Data, 10, counterForShips);
    drawField(p1Data, p2Data, 10, counterForShips);
    generationShips(3, p1Data, p2Data, 10, counterForShips);
    drawField(p1Data, p2Data, 10, counterForShips);
    generationShips(3, p1Data, p2Data, 10, counterForShips);
    drawField(p1Data, p2Data, 10, counterForShips);
    generationShips(2, p1Data, p2Data, 10, counterForShips);
    drawField(p1Data, p2Data, 10, counterForShips);
    generationShips(2, p1Data, p2Data, 10, counterForShips);
    drawField(p1Data, p2Data, 10, counterForShips);
    generationShips(2, p1Data, p2Data, 10, counterForShips);
    drawField(p1Data, p2Data, 10, counterForShips);
    generationShips(1, p1Data, p2Data, 10, counterForShips);
    drawField(p1Data, p2Data, 10, counterForShips);
    generationShips(1, p1Data, p2Data, 10, counterForShips);
    drawField(p1Data, p2Data, 10, counterForShips);
    generationShips(1, p1Data, p2Data, 10, counterForShips);
    drawField(p1Data, p2Data, 10, counterForShips);
    generationShips(1, p1Data, p2Data, 10, counterForShips);




  


    while (counterForShips) {
        shootToShip(p2Data, &y, &x, &counterForShips);
        drawField(p1Data, p2Data, 10, counterForShips);
        printf("\n");
    }


    drawField(p1Data, p2Data, 10, counterForShips);



    return 0;
}

void setDefaultField() {

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

    for (int i = 0; i < FIELD_LINES; i++) {
        for (int j = 0; j < strlen(field[i]); j++) {
            switch (field[i][j]) {
            case '$':
                field[i][j] = 249;
                break;
            case '#':
                field[i][j] = 178;
                break;
            case '%':
                field[i][j] = 176;
                break;
            case '&':
                field[i][j] = 88;
                break;
            case 'q':
                field[i][j] = 201;
                break;
            case 'w':
                field[i][j] = 187;
                break;
            case 'z':
                field[i][j] = 200;
                break;
            case 'x':
                field[i][j] = 188;
                break;
            case 'v':
                field[i][j] = 186;
                break;
            case 't':
                field[i][j] = 205;
                break;
            default:
                field[i][j] = field[i][j];
            }
        }
    }
}

void welcomePage() {
    for (int i = 0; i < 256; i++){
        printf ("%c ", i);
    }
    /*for (int i = 0; i <= 21; i++) {
        printf("%s\n", ship[i]);
    }
    printf("%s\n", smile[0]);
    printf("%s", smile[1]);*/
    _getch();
    return;
}

void drawField(unsigned char player1Field[][10], unsigned char player2Field[][10], int yourNumberShips, int rivalsNumberShips) {  //доделать: функция должна по-разному рисовать поле для разных игроков
    system("cls");
    printf("%s", gameName);
    printf("\n");
    for (int i = 0; i < strlen(userName1); i++) {
        printf("%c", userName1[i]);
    }
    for (int i = 0; i < (SIZE_CONSOLE - strlen(userName2) - strlen(userName1)); i++) {
        printf(" ");
    }
    for (int i = 0; i < strlen(userName2); i++) {
        printf("%c", userName2[i]);
    }
    printf("\n");
    printf("\n");
    printf("\n");

    printf("%s\n", field[0]);
    printf("%s\n", field[1]);

    for (int i = 2; i < GAME_FIELD_LINES + 2; i++) {
        for (int j = 0; j <= 6; j++) {
            printf("%c", field[i][j]);
        }

        
          
        

        for (int j = 0; j < GAME_FIELD_LINES; j++) {

            
            printf("%c", player1Field[i - 2][j]);
        }
        printf("%c", field[i][17]);

        for (int j = 18; j < 36; j++) {
            printf("%c", field[i][j]);
        }

        for (int j = 0; j < GAME_FIELD_LINES; j++) {

            //printf(" ");
            printf("%c", player2Field[i - 2][j]);
        }

        printf("%c\n", field[i][46]);
    }

    printf("%s\n", field[FIELD_LINES - 1]);

    printf("\n");
    printf("\n");

    for (int i = 0; i < strlen(numberShips1); i++){
        printf("%c", numberShips1[i]);
    }
    
    printf("%d", yourNumberShips);

    for (int i = 0; i < (SIZE_CONSOLE - strlen(numberShips1) - strlen(numberShips2) - 4); i++) {
        printf(" ");
    }

    for (int i = 0; i < strlen(numberShips2); i++) {
        printf("%c", numberShips2[i]);
    }

    printf("%d", rivalsNumberShips);

    printf("\n");

    printf("\n");
    printf("%s", border);
    printf("\n");

    
    printf("\n");

}

void swap(int* a, int* b) {
    int c = 0;
    c = *a;
    *a = *b;
    *b = c;
}

int convertLetterToNumber(char letter) {
    int figureAfterChanges = 0;

    switch (letter) {
    case 'A': {
        figureAfterChanges = 0;
        break;
    }
    case 'B': {
        figureAfterChanges = 1;
        break;
    }
    case 'C': {
        figureAfterChanges = 2;
        break;
    }
    case 'D': {
        figureAfterChanges = 3;
        break;
    }
    case 'E': {
        figureAfterChanges = 4;
        break;
    }
    case 'F': {
        figureAfterChanges = 5;
        break;
    }
    case 'G': {
        figureAfterChanges = 6;
        break;
    }
    case 'H': {
        figureAfterChanges = 7;
        break;
    }
    case 'I': {
        figureAfterChanges = 8;
        break;
    }
    case 'J': {
        figureAfterChanges = 9;
        break;
    }
    }

    return figureAfterChanges;
}

int checkAround(unsigned char gameField[][10], int beginLetterCh, int beginFigure, int endLetterCh, int endFigure, int sizeOfShip, char flag) {

    int* checkX = NULL;
    int* checkY = NULL;


    int y = 0;
    int x = 0;
    switch (sizeOfShip) {
    case 4:
    {
        if (flag == 'v') {
            checkY = checkX4;
            checkX = checkY4;
            if (gameField[beginFigure][beginLetterCh] == SHIP
                || gameField[beginFigure + 1][beginLetterCh] == SHIP
                || gameField[beginFigure + 2][beginLetterCh] == SHIP
                || gameField[beginFigure + 3][beginLetterCh] == SHIP) {
                return 1;
            }
            if (endFigure - beginFigure + 1 != 4) {
                return 1;
            }
        }
        else {
            checkY = checkY4;
            checkX = checkX4;
            if (gameField[beginFigure][beginLetterCh] == SHIP
                || gameField[beginFigure][beginLetterCh + 1] == SHIP
                || gameField[beginFigure][beginLetterCh + 2] == SHIP
                || gameField[beginFigure][beginLetterCh + 3] == SHIP) {
                return 1;
            }
            if (endLetterCh - beginLetterCh + 1 != 4) {
                return 1;
            }
        }

        for (int i = 0; i < 14; i++) {
            y = beginFigure + checkY[i];
            x = beginLetterCh + checkX[i];
            if (x < 0 || x > 9 || y < 0 || y > 9) {
                continue;
            }
            if (gameField[y][x] == SHIP) {
                return 1;
            }
        }
        return 0;
    }
    case 3:
    {
        if (flag == 'v') {
            checkY = checkX3;
            checkX = checkY3;
            if (gameField[beginFigure][beginLetterCh] == SHIP
                || gameField[beginFigure + 1][beginLetterCh] == SHIP
                || gameField[beginFigure + 2][beginLetterCh] == SHIP) {
                return 1;
            }
            if (endFigure - beginFigure + 1 != 3) {
                return 1;
            }
        }
        else {
            checkY = checkY3;
            checkX = checkX3;
            if (gameField[beginFigure][beginLetterCh] == SHIP
                || gameField[beginFigure][beginLetterCh + 1] == SHIP
                || gameField[beginFigure][beginLetterCh + 2] == SHIP) {
                return 1;
            }
            if (endLetterCh - beginLetterCh + 1 != 3) {
                return 1;
            }
        }
        for (int i = 0; i < 12; i++) {
            y = beginFigure + checkY[i];
            x = beginLetterCh + checkX[i];
            if (x < 0 || x > 9 || y < 0 || y > 9) {
                continue;
            }
            if (gameField[y][x] == SHIP) {
                return 1;
            }
        }
        return 0;
    }
    case 2:
    {
        if (flag == 'v') {
            checkY = checkX2;
            checkX = checkY2;
            if (gameField[beginFigure][beginLetterCh] == SHIP
                || gameField[beginFigure + 1][beginLetterCh] == SHIP) {
                return 1;
            }
            if (endFigure - beginFigure + 1 != 2) {
                return 1;
            }
        }
        else {
            checkY = checkY2;
            checkX = checkX2;
            if (gameField[beginFigure][beginLetterCh] == SHIP
                || gameField[beginFigure][beginLetterCh + 1] == SHIP) {
                return 1;
            }
            if (endLetterCh - beginLetterCh + 1 != 2) {
                return 1;
            }

        }
        for (int i = 0; i < 10; i++) {
            y = beginFigure + checkY[i];
            x = beginLetterCh + checkX[i];
            if (x < 0 || x > 9 || y < 0 || y > 9) {
                continue;
            }
            if (gameField[y][x] == SHIP) {
                return 1;
            }
        }
        return 0;
    }
    case 1:
    {

        checkY = checkY1;
        checkX = checkX1;
        if (gameField[beginFigure][beginLetterCh] == SHIP) {
            return 1;
        }
        if (endLetterCh - beginLetterCh + 1 != 1) {
            return 1;
        }
        for (int i = 0; i < 8; i++) {
            y = beginFigure + checkY[i];
            x = beginLetterCh + checkX[i];
            if (x < 0 || x > 9 || y < 0 || y > 9) {
                continue;
            }
            if (gameField[y][x] == SHIP) {
                return 1;
            }
        }
        return 0;
    }
    }
}

void readCoordinates4(int* beginLetterCh, int* beginFigure, int* endLetterCh, int* endFigure) {
    char arr[BUFFER] = { 0 };
    scanf("%s", arr);
    if (arr[0] >= 'A' && arr[0] <= 'Z') {
        *beginLetterCh = (int)arr[0] - 'A';
    }
    else if (arr[0] >= 'a' && arr[0] <= 'z') {
        *beginLetterCh = (int)arr[0] - 'a';
    }
    *beginFigure = (int)arr[1] - '0';

    if (arr[2] >= 'A' && arr[2] <= 'Z') {
        *endLetterCh = (int)arr[2] - 'A';
    }
    else if (arr[2] >= 'a' && arr[2] <= 'z') {
        *endLetterCh = (int)arr[2] - 'a';
    }
    *endFigure = (int)arr[3] - '0';



    if (*beginLetterCh > 9 || *beginLetterCh < 0 || *beginFigure > 9 || *beginFigure < 0 ||
        *endLetterCh > 9 || *endLetterCh < 0 || *endFigure > 9 || *endFigure < 0) {
        printf("Unable to place the ship. Enter another coordinates\n");
        readCoordinates4(beginLetterCh, beginFigure, endLetterCh, endFigure);
    }
    return;
}

void readCoordinates2(int* beginLetterCh, int* beginFigure) {
    char arr[BUFFER] = { 0 };
    scanf("%s", arr);
    if (arr[0] >= 'A' && arr[0] <= 'Z') {
        *beginLetterCh = (int)arr[0] - 'A';
    }
    else if (arr[0] >= 'a' && arr[0] <= 'z') {
        *beginLetterCh = (int)arr[0] - 'a';
    }
    *beginFigure = (int)arr[1] - '0';

    if (*beginLetterCh > 9 || *beginLetterCh < 0 || *beginFigure > 9 || *beginFigure < 0) {
        printf("Unable to place the ship. Enter another coordinates\n");
        readCoordinates2(beginLetterCh, beginFigure);
    }
    return;
}

void generationShips(int sizeOfShip, unsigned char gameField[][10], unsigned char gameField2[][10], int yourNumberShips, int rivalsNumberShips) {

    char beginLetter = 0;
    int beginLetterCh = 0;
    int beginFigure = 0;
    char endLetter = 0;
    int endLetterCh = 0;
    int endFigure = 0;




    switch (sizeOfShip) {
    case 4:
    {
        printf("Enter the start and end coordinates for a battleship:\n");
        readCoordinates4(&beginLetterCh, &beginFigure, &endLetterCh, &endFigure);
        if (beginLetterCh > endLetterCh) {
            swap(&beginLetterCh, &endLetterCh);
        }
        if (beginFigure > endFigure) {
            swap(&beginFigure, &endFigure);
        }
        if (beginLetterCh == endLetterCh || beginFigure == endFigure) {
            if (beginLetterCh == endLetterCh) {
                if (!checkAround(gameField, beginLetterCh, beginFigure, endLetterCh, endFigure, 4, 'v')) {

                    gameField[beginFigure][beginLetterCh] = SHIP;
                    gameField[beginFigure + 1][beginLetterCh] = SHIP;
                    gameField[beginFigure + 2][beginLetterCh] = SHIP;
                    gameField[beginFigure + 3][beginLetterCh] = SHIP;
                    return;

                }
                else {
                    printf("Unable to place the ship. Enter another coordinates\n");
                    generationShips(4, gameField, gameField2, yourNumberShips, rivalsNumberShips);
                    drawField(gameField, gameField2, 10, rivalsNumberShips);
                    return;

                }
            }
            else if (beginFigure == endFigure) {
                if (!checkAround(gameField, beginLetterCh, beginFigure, endLetterCh, endFigure, 4, 'h')) {

                    gameField[beginFigure][beginLetterCh] = SHIP;
                    gameField[beginFigure][beginLetterCh + 1] = SHIP;
                    gameField[beginFigure][beginLetterCh + 2] = SHIP;
                    gameField[beginFigure][beginLetterCh + 3] = SHIP;
                    return;

                }
                else {
                    printf("Unable to place the ship. Enter another coordinates\n");
                    generationShips(4, gameField, gameField2, yourNumberShips, rivalsNumberShips);
                    drawField(gameField, gameField2, 10, rivalsNumberShips);
                    return;
                }
            }
        }
        else {
            printf("Unable to place the ship. Enter another coordinates\n");
            generationShips(4, gameField, gameField2, yourNumberShips, rivalsNumberShips);
            drawField(gameField, gameField2, 10, rivalsNumberShips);
            return;
        }
        break;
    }
    case 3:
    {
        printf("Enter the start and end coordinates for a cruiser:\n");
        readCoordinates4(&beginLetterCh, &beginFigure, &endLetterCh, &endFigure);

        if (beginLetterCh > endLetterCh) {
            swap(&beginLetterCh, &endLetterCh);
        }
        if (beginFigure > endFigure) {
            swap(&beginFigure, &endFigure);
        }

        if (beginLetterCh == endLetterCh || beginFigure == endFigure) {
            if (beginLetterCh == endLetterCh) {
                if (!checkAround(gameField, beginLetterCh, beginFigure, endLetterCh, endFigure, 3, 'v')) {

                    gameField[beginFigure][beginLetterCh] = SHIP;
                    gameField[beginFigure + 1][beginLetterCh] = SHIP;
                    gameField[beginFigure + 2][beginLetterCh] = SHIP;
                    return;

                }
                else {
                    printf("Unable to place the ship. Enter another coordinates\n");
                    generationShips(3, gameField, gameField2, yourNumberShips, rivalsNumberShips);
                    drawField(gameField, gameField2, 10, rivalsNumberShips);
                    return;
                }
            }
            else if (beginFigure == endFigure) {
                if (!checkAround(gameField, beginLetterCh, beginFigure, endLetterCh, endFigure, 3, 'h')) {
                    gameField[beginFigure][beginLetterCh] = SHIP;
                    gameField[beginFigure][beginLetterCh + 1] = SHIP;
                    gameField[beginFigure][beginLetterCh + 2] = SHIP;
                    return;


                }
                else {
                    printf("Unable to place the ship. Enter another coordinates\n");
                    generationShips(3, gameField, gameField2, yourNumberShips, rivalsNumberShips);
                    drawField(gameField, gameField2, 10, rivalsNumberShips);
                    return;
                }
            }
        }
        else {
            printf("Unable to place the ship. Enter another coordinates\n");
            generationShips(3, gameField, gameField2, yourNumberShips, rivalsNumberShips);
            drawField(gameField, gameField2, 10, rivalsNumberShips);
            return;
        }
        break;
    }
    case 2:
    {
        printf("Enter the start and end coordinates for a destroyer:\n");
        readCoordinates4(&beginLetterCh, &beginFigure, &endLetterCh, &endFigure);

        if (beginLetterCh > endLetterCh) {
            swap(&beginLetterCh, &endLetterCh);
        }
        if (beginFigure > endFigure) {
            swap(&beginFigure, &endFigure);
        }

        if (beginLetterCh == endLetterCh || beginFigure == endFigure) {
            if (beginLetterCh == endLetterCh) {
                if (!checkAround(gameField, beginLetterCh, beginFigure, endLetterCh, endFigure, 2, 'v')) {

                    gameField[beginFigure][beginLetterCh] = SHIP;
                    gameField[beginFigure + 1][beginLetterCh] = SHIP;
                    return;

                }
                else {
                    printf("Unable to place the ship. Enter another coordinates\n");
                    generationShips(2, gameField, gameField2, yourNumberShips, rivalsNumberShips);
                    drawField(gameField, gameField2, 10, rivalsNumberShips);
                    return;
                }
            }
            else if (beginFigure == endFigure) {
                if (!checkAround(gameField, beginLetterCh, beginFigure, endLetterCh, endFigure, 2, 'h')) {

                    gameField[beginFigure][beginLetterCh] = SHIP;
                    gameField[beginFigure][beginLetterCh + 1] = SHIP;
                    return;

                }
                else {
                    printf("Unable to place the ship. Enter another coordinates\n");
                    generationShips(2, gameField, gameField2, yourNumberShips, rivalsNumberShips);
                    drawField(gameField, gameField2, 10, rivalsNumberShips);
                    return;
                }
            }
        }
        else {
            printf("Unable to place the ship. Enter another coordinates\n");
            generationShips(2, gameField, gameField2, yourNumberShips, rivalsNumberShips);
            drawField(gameField, gameField2, 10, rivalsNumberShips);
            return;
        }
        break;
    }
    case 1:
    {
        int temp = 49;
        printf("Enter the coordinates for a submarine:\n");
        readCoordinates2(&beginLetterCh, &beginFigure);
        if (!checkAround(gameField, beginLetterCh, beginFigure, beginLetterCh, beginFigure, 1, 'h')) {
            gameField[beginFigure][beginLetterCh] = SHIP;
            return;
        }
        else {
            printf("Unable to place the ship. Enter another coordinates\n");
            generationShips(1, gameField, gameField2, yourNumberShips, rivalsNumberShips);
            drawField(gameField, gameField2, 10, rivalsNumberShips);
            return;
        }
    }
    break;
    }
}

void makeDie(unsigned char gameFieldRival[][10], int y, int x) {
    gameFieldRival[y][x] = KILL;
    if (y - 1 >= 0 && y - 1 <= 9 && (gameFieldRival[y - 1][x] == INJURED)) {
        makeDie(gameFieldRival, y - 1, x);
    }
    if (x + 1 >= 0 && x + 1 <= 9 && (gameFieldRival[y][x + 1] == INJURED)) {
        makeDie(gameFieldRival, y, x + 1);
    }
    if (y + 1 >= 0 && y + 1 <= 9 && (gameFieldRival[y + 1][x] == INJURED)) {
        makeDie(gameFieldRival, y + 1, x);
    }
    if (x - 1 >= 0 && x - 1 <= 9 && (gameFieldRival[y][x - 1] == INJURED)) {
        makeDie(gameFieldRival, y, x - 1);
    }
}

void shootToShip(unsigned char gameFieldRival[][10], int* y, int* x, int* counterForShips) {
    int check = 0;

    printf("Enter the coordinates for the shot:\n");
    char arr[4] = { 0 };
    scanf("%s", arr);

    if (arr[0] >= 'A' && arr[0] <= 'J') {
        *x = (int)arr[0] - 'A';
    }
    else if (arr[0] >= 'a' && arr[0] <= 'j') {
        *x = (int)arr[0] - 'a';
    }
    else {
        printf("Unable to shoot. Please enter another coordinates\n");
        shootToShip(gameFieldRival, y, x, counterForShips);
        return;
    }
    *y = (int)arr[1] - '0';


    if (*y > 9 || *y < 0 || *x > 9 || *x < 0) {
        printf("Unable to shoot. Please enter another coordinates\n");
        shootToShip(gameFieldRival, y, x, counterForShips);
        return;
    }

    if (gameFieldRival[*y][*x] == EMPTY) {
        gameFieldRival[*y][*x] = SHOT;
        return;
    }
    if (gameFieldRival[*y][*x] == SHIP) {
        gameFieldRival[*y][*x] = INJURED;
        check = checkLifeOrDieShip('u', *(y)-1, *x, gameFieldRival) + checkLifeOrDieShip('r', *y, *(x)+1, gameFieldRival) + checkLifeOrDieShip('d', *(y)+1, *x, gameFieldRival) + checkLifeOrDieShip('l', *y, *(x)-1, gameFieldRival);
        if (check == 0) {
            (*counterForShips)--;
            makeDie(gameFieldRival, *y, *x);
        }
    }
}

int checkLifeOrDieShip(char direct, int  y, int x, unsigned char gameFieldRival[][10]) {
    if (y < 0 || y > 9 || x < 0 || x > 9) {
        return 0;
    }
    if (gameFieldRival[y][x] == EMPTY || gameFieldRival[y][x] == SHOT) {
        return 0;
    }
    if (gameFieldRival[y][x] == SHIP) {
        return 1;
    }
    if (gameFieldRival[y][x] == INJURED) {
        switch (direct)
        {
        case 'u':
        {
            return checkLifeOrDieShip('u', y - 1, x, gameFieldRival);
        }
        case 'r':
        {
            return checkLifeOrDieShip('r', y, x + 1, gameFieldRival);
        }
        case 'd':
        {
            return checkLifeOrDieShip('d', y + 1, x, gameFieldRival);
        }
        case 'l':
        {
            return checkLifeOrDieShip('l', y, x - 1, gameFieldRival);
        }

        }
    }
}