#define _CRT_SECURE_NO_WARNINGS
#include "outGame.h"
#include <stdio.h>
#define _WIN32_WINNT 0x0500
#include <Windows.h>
#include <conio.h>

void setDefaultField() {

    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 6;
    cfi.dwFontSize.X = 20;                   // Width of each character in the font
    cfi.dwFontSize.Y = 20;                  // Height
    cfi.FontFamily = BALTIC_CHARSET;
    cfi.FontWeight = FW_NORMAL;
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

    system("mode con cols=55 lines=28");
    HWND consoleWindow = GetConsoleWindow();
    SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);

   
}

void welcomePage() {

    for (int i = 0; i <= 21; i++) {
        printf("%s\n", ship[i]);
    }
    printf("%s\n", smile[0]);
    printf("%s", smile[1]);
    _getch();
    return;
}

void drawField(unsigned char player1Field[][10], unsigned char player2Field[][10], int yourNumberShips, int rivalsNumberShips) {  //доделать: функция должна по-разному рисовать поле для разных игроков
   //names of players
    char userName1[] = "VASYA";
    char userName2[] = "PETYA";
    char standartFieldForm[][SIZE_CONSOLE] =
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
    for (int i = 0; i < FIELD_LINES; i++) {
        for (int j = 0; j < strlen(standartFieldForm[i]); j++) {
            switch (standartFieldForm[i][j]) {
            case '$':
                standartFieldForm[i][j] = 249;
                break;
            case '#':
                standartFieldForm[i][j] = 178;
                break;
            case '%':
                standartFieldForm[i][j] = 176;
                break;
            case '&':
                standartFieldForm[i][j] = 88;
                break;
            case 'q':
                standartFieldForm[i][j] = 201;
                break;
            case 'w':
                standartFieldForm[i][j] = 187;
                break;
            case 'z':
                standartFieldForm[i][j] = 200;
                break;
            case 'x':
                standartFieldForm[i][j] = 188;
                break;
            case 'v':
                standartFieldForm[i][j] = 186;
                break;
            case 't':
                standartFieldForm[i][j] = 205;
                break;
            default:
                standartFieldForm[i][j] = standartFieldForm[i][j];
            }
        }
    }
    //----------
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

    printf("%s\n", standartFieldForm[0]);
    printf("%s\n", standartFieldForm[1]);

    for (int i = 2; i < GAME_FIELD_LINES + 2; i++) {
        for (int j = 0; j <= 6; j++) {
            printf("%c", standartFieldForm[i][j]);
        }
        for (int j = 0; j < GAME_FIELD_LINES; j++) {


            printf("%c", player1Field[i - 2][j]);
        }
        printf("%c", standartFieldForm[i][17]);

        for (int j = 18; j < 36; j++) {
            printf("%c", standartFieldForm[i][j]);
        }

        for (int j = 0; j < GAME_FIELD_LINES; j++) {

            //printf(" ");
            if (player2Field[i - 2][j] != SHIP){
                printf ("%c", player2Field[i - 2][j]);
            } else{
                printf (" ");
            }
        }

        printf("%c\n", standartFieldForm[i][46]);
    }

    printf("%s\n", standartFieldForm[FIELD_LINES - 1]);

    printf("\n");
    printf("\n");

    for (int i = 0; i < strlen(numberShips1); i++) {
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


int checkAround(unsigned char gameField[][10], int beginLetterCh, int beginFigure, int endLetterCh, int endFigure, int sizeOfShip, char flag) {

    int* checkX = NULL;
    int* checkY = NULL;


    int y = 0;
    int x = 0;
    switch (sizeOfShip) {
    case 4:
    {
        if (flag == 'v') {
            checkY = (int*)checkX4;
            checkX = (int*)checkY4;
            if (gameField[beginFigure][beginLetterCh] == SHIP
                || gameField[beginFigure + 1][beginLetterCh] == SHIP
                || gameField[beginFigure + 2][beginLetterCh] == SHIP
                || gameField[beginFigure + 3][beginLetterCh] == SHIP) {
                return 1;
            }
            if (endFigure - beginFigure + 1 != 4) {
                return 1;
            }
        } else {
            checkY = (int*)checkY4;
            checkX = (int*)checkX4;
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
            checkY = (int*)checkX3;
            checkX = (int*)checkY3;
            if (gameField[beginFigure][beginLetterCh] == SHIP
                || gameField[beginFigure + 1][beginLetterCh] == SHIP
                || gameField[beginFigure + 2][beginLetterCh] == SHIP) {
                return 1;
            }
            if (endFigure - beginFigure + 1 != 3) {
                return 1;
            }
        } else {
            checkY = (int*)checkY3;
            checkX = (int*)checkX3;
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
            checkY = (int*)checkX2;
            checkX = (int*)checkY2;
            if (gameField[beginFigure][beginLetterCh] == SHIP
                || gameField[beginFigure + 1][beginLetterCh] == SHIP) {
                return 1;
            }
            if (endFigure - beginFigure + 1 != 2) {
                return 1;
            }
        } else {
            checkY = (int*)checkY2;
            checkX = (int*)checkX2;
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

        checkY = (int*)checkY1;
        checkX = (int*)checkX1;
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
    } else if (arr[0] >= 'a' && arr[0] <= 'z') {
        *beginLetterCh = (int)arr[0] - 'a';
    }
    *beginFigure = (int)arr[1] - '0';

    if (arr[2] >= 'A' && arr[2] <= 'Z') {
        *endLetterCh = (int)arr[2] - 'A';
    } else if (arr[2] >= 'a' && arr[2] <= 'z') {
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
    } else if (arr[0] >= 'a' && arr[0] <= 'z') {
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

                } else {
                    printf("Unable to place the ship. Enter another coordinates\n");
                    generationShips(4, gameField, gameField2, yourNumberShips, rivalsNumberShips);
                    drawField(gameField, gameField2, 10, rivalsNumberShips);
                    return;

                }
            } else if (beginFigure == endFigure) {
                if (!checkAround(gameField, beginLetterCh, beginFigure, endLetterCh, endFigure, 4, 'h')) {

                    gameField[beginFigure][beginLetterCh] = SHIP;
                    gameField[beginFigure][beginLetterCh + 1] = SHIP;
                    gameField[beginFigure][beginLetterCh + 2] = SHIP;
                    gameField[beginFigure][beginLetterCh + 3] = SHIP;
                    return;

                } else {
                    printf("Unable to place the ship. Enter another coordinates\n");
                    generationShips(4, gameField, gameField2, yourNumberShips, rivalsNumberShips);
                    drawField(gameField, gameField2, 10, rivalsNumberShips);
                    return;
                }
            }
        } else {
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

                } else {
                    printf("Unable to place the ship. Enter another coordinates\n");
                    generationShips(3, gameField, gameField2, yourNumberShips, rivalsNumberShips);
                    drawField(gameField, gameField2, 10, rivalsNumberShips);
                    return;
                }
            } else if (beginFigure == endFigure) {
                if (!checkAround(gameField, beginLetterCh, beginFigure, endLetterCh, endFigure, 3, 'h')) {
                    gameField[beginFigure][beginLetterCh] = SHIP;
                    gameField[beginFigure][beginLetterCh + 1] = SHIP;
                    gameField[beginFigure][beginLetterCh + 2] = SHIP;
                    return;


                } else {
                    printf("Unable to place the ship. Enter another coordinates\n");
                    generationShips(3, gameField, gameField2, yourNumberShips, rivalsNumberShips);
                    drawField(gameField, gameField2, 10, rivalsNumberShips);
                    return;
                }
            }
        } else {
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

                } else {
                    printf("Unable to place the ship. Enter another coordinates\n");
                    generationShips(2, gameField, gameField2, yourNumberShips, rivalsNumberShips);
                    drawField(gameField, gameField2, 10, rivalsNumberShips);
                    return;
                }
            } else if (beginFigure == endFigure) {
                if (!checkAround(gameField, beginLetterCh, beginFigure, endLetterCh, endFigure, 2, 'h')) {

                    gameField[beginFigure][beginLetterCh] = SHIP;
                    gameField[beginFigure][beginLetterCh + 1] = SHIP;
                    return;

                } else {
                    printf("Unable to place the ship. Enter another coordinates\n");
                    generationShips(2, gameField, gameField2, yourNumberShips, rivalsNumberShips);
                    drawField(gameField, gameField2, 10, rivalsNumberShips);
                    return;
                }
            }
        } else {
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
        } else {
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

int shootToShip(unsigned char gameFieldRival[][10]) {
    int check = 0;

    printf("Enter the coordinates for the shot:\n");
    char arr[BUFFER] = { 0 };
    scanf("%s", arr);
    int x, y;
    if (arr[0] >= 'A' && arr[0] <= 'J') {
        x = (int)arr[0] - 'A';
    } else if (arr[0] >= 'a' && arr[0] <= 'j') {
        x = (int)arr[0] - 'a';
    } else {
        printf("Unable to shoot. Please enter another coordinates\n");
        shootToShip(gameFieldRival);
        return 0;
    }
    y = (int)arr[1] - '0';


    if (y > 9 || y < 0 || x > 9 || x < 0) {
        printf("Unable to shoot. Please enter another coordinates\n");
        shootToShip(gameFieldRival);
        return 0;
    }

    if (gameFieldRival[y][x] == EMPTY) {
        gameFieldRival[y][x] = SHOT;
        return 0;
    }
    if (gameFieldRival[y][x] == SHIP) {
        gameFieldRival[y][x] = INJURED;
        check = checkLifeOrDieShip('u', (y)-1, x, gameFieldRival) + checkLifeOrDieShip('r', y, (x)+1, gameFieldRival) + checkLifeOrDieShip('d', (y)+1, x, gameFieldRival) + checkLifeOrDieShip('l', y, (x)-1, gameFieldRival);
        if (check == 0) {
            
            makeDie(gameFieldRival, y, x);
            return 1;
        }
    }
    return 0;
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