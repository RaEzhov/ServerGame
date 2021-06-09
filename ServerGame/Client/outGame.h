#ifndef OUTGAME_H
#define OUTGAME_H

#define FIELD_LINES 13
#define GAME_FIELD_LINES 10
#define COUNT_OF_SHIPS 10
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
    EMPTY = 32,
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

const char border[SIZE_CONSOLE + 2] =
"_______________________________________________________";

const char smile[][SIZE_CONSOLE + 2] =
{
"                       PRESS ANY KEY                   ",
"                   (> ^ _^) > < (^ _^ <)               "
};

const char numberShips1[20] = "Your life ships: ";
const char numberShips2[20] = "Rivals life ships: ";



const char ship[][SIZE_CONSOLE + 1] = {
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
           "   XX__________XX   EXAMPLE OF ENTER COORDS: A3B3 OR d2",
           "     XXXXXXXXXX                                        "
};

const int checkY4[14] = { -1, -1, -1, -1, -1, -1, 0, 1, 1, 1, 1, 1, 1, 0 };
const int checkX4[14] = { -1, 0, 1, 2, 3, 4, 4, 4, 3, 2, 1, 0, -1, -1 };

const int checkY3[12] = { -1, -1, -1, -1, -1, 0, 1, 1, 1, 1, 1, 0 };
const int checkX3[12] = { -1, 0, 1, 2, 3, 3, 3, 2, 1, 0, -1, -1 };

const int checkY2[10] = { -1, -1, -1, -1, 0, 1, 1, 1, 1, 0 };
const int checkX2[10] = { -1, 0, 1, 2, 2, 2, 1, 0, -1, -1 };

const int checkY1[8] = { -1, -1, -1, 0, 1, 1,  1,  0 };
const int checkX1[8] = { -1,  0,  1, 1, 1, 0, -1, -1 };


void setDefaultField();
void welcomePage();
void drawField(unsigned char player1Field[][10], unsigned char player2Field[][10], int yourNumberShips, int rivalsNumberShips);
int checkAround(unsigned char gameField[][10], int beginLetterCh, int beginFigure, int endLetterCh, int endFigure, int sizeOfShip, char flag);
void generationShips(int sizeOfShip, unsigned char gameField[][10], unsigned char gameField2[][10], int yourNumberShips, int rivalsNumberShips);
int shootToShip(unsigned char gameFieldRival[][10]);
int checkLifeOrDieShip(char direct, int  y, int x, unsigned char gameFieldRival[][10]);
void makeDie(unsigned char gameFieldRival[][10], int y, int x);


#endif //OUTGAME_H