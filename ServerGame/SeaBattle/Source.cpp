#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <conio.h>

#define FIELD_LINES 13
#define GAME_FIELD_LINES 10

typedef enum {
    EMPTY = 32,
    INJURED = 176,
    KILL = 88,
    SHIP = 178,
    MISS = 249
}moveStatus;

char field[][30] =
{
            "  ABCDEFGHIJ      ABCDEFGHIJ ",
            " qttttttttttw    qttttttttttw",
            "0v          v   0v          v",
            "1v          v   1v          v",
            "2v          v   2v          v",
            "3v          v   3v          v",
            "4v          v   4v          v",
            "5v          v   5v          v",
            "6v          v   6v          v",
            "7v          v   7v          v",
            "8v          v   8v          v",
            "9v          v   9v          v",
            " zttttttttttx    zttttttttttx"
};


void setDefaultField();
void drawField();

int main() {

    setDefaultField();
    drawField();

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

void drawField() {
   printf("%s\n", field[0]);
   printf("%s\n", field[1]);

   for (int i = 2; i < GAME_FIELD_LINES + 2; i++) {
        printf("%c%c", field[i][0], field[i][1]);

        for (int j = 0; j < GAME_FIELD_LINES; j++) {
            printf(" ");
        }

        for (int j = 12; j < 19; j++) {
            printf("%c", field[i][j]);
        }
        
        for (int j = 0; j < GAME_FIELD_LINES - 1; j++) {
            printf(" ");
        }

        printf("%c\n", field[i][28]);
    }

   printf("%s\n", field[FIELD_LINES - 1]);
    
}