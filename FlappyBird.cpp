#include <iostream>
#include <windows.h>
#include <math.h>
#include <time.h>
#include <conio.h>
using namespace std;

void moveCursorTo(int col, int row) {
    COORD position = { col, row };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
}

const int totalBarriers = 4;

class ConsoleBirdGame {
    int barrierData[totalBarriers][2];
    int width = 79;
    int height = 25;
    int openingSize = 8;
    int barrierSpacing;

    int playerX = 17;
    int playerY = 15;

    int liftPower = 4;
    int playerScore = 0;

public:
    ConsoleBirdGame() {
        srand(time(NULL));

        barrierSpacing = (width / totalBarriers) + 10;
        for (int i = 0; i < totalBarriers; i++) {
            barrierData[i][0] = barrierSpacing * (i + 1);
            int gapStart = rand() % (height / 3) + openingSize;
            barrierData[i][1] = gapStart;
        }
    }

    void drawBarriers() {
        int gapCounter = 0;
        for (int i = 0; i < totalBarriers; i++) {
            for (int j = 0; j < height; j++) {
                if (barrierData[i][1] == j) gapCounter = openingSize;

                if (gapCounter == 0) {
                    if (barrierData[i][0] < width) {
                        moveCursorTo(barrierData[i][0] + 1, j);
                        cout << " ";
                        moveCursorTo(barrierData[i][0], j);
                        cout << i;
                    }
                } else {
                    if ((gapCounter == 1 || gapCounter == openingSize) && barrierData[i][0] < width) {
                        if (barrierData[i][0] + 1 > 0) {
                            moveCursorTo(barrierData[i][0] + 1, j);
                            cout << "   ";
                        }

                        if (barrierData[i][0] - 1 > 0) {
                            moveCursorTo(barrierData[i][0] - 1, j);
                            cout << "===";
                        }
                    }
                    gapCounter--;
                }
            }

            barrierData[i][0]--;

            if (barrierData[i][0] == -1) {
                int previousIndex = (i == 0) ? totalBarriers - 1 : i - 1;
                barrierData[i][0] = barrierData[previousIndex][0] + barrierSpacing;
                barrierData[i][1] = rand() % (height / 3) + openingSize;

                for (int clr = 0; clr < height; clr++) {
                    moveCursorTo(0, clr);
                    cout << " ";
                }
            }
        }
    }

    bool detectCollision() {
        if (playerY == 0 || playerY + 3 == height) return true;

        for (int i = 0; i < totalBarriers; i++) {
            if (
                barrierData[i][0] == playerX &&
                (
                    playerY >= barrierData[i][1] ||
                    playerY + 3 <= (barrierData[i][1] + openingSize)
                )
            ) {
                playerScore++;
            }

            if (
                barrierData[i][0] >= playerX - 5 &&
                barrierData[i][0] <= playerX &&
                (
                    playerY <= barrierData[i][1] ||
                    playerY + 3 >= (barrierData[i][1] + openingSize)
                )
            ) {
                return true;
            }
        }

        return false;
    }

    void erasePlayer() {
        moveCursorTo(playerX - 5, playerY);     cout << "   ";
        moveCursorTo(playerX - 5, playerY + 1); cout << "      ";
        moveCursorTo(playerX - 5, playerY + 2); cout << "       ";
    }

    void showScore() {
        moveCursorTo(0, height + 4);
        cout << "Score: " << playerScore;
    }

    void drawPlayer() {
        moveCursorTo(playerX - 5, playerY);     cout << " __";
        moveCursorTo(playerX - 5, playerY + 1); cout << "/-/o\\";
        moveCursorTo(playerX - 5, playerY + 2); cout << "\\_\\-/";
    }

    void drawGround() {
        for (int i = 0; i <= width; i++) {
            moveCursorTo(i, height);     cout << "_";
            moveCursorTo(i, height + 1); cout << "/";
            moveCursorTo(i, height + 2); cout << "=";
        }
    }

    void runGame() {
        drawGround();
        int frameDelay = 0;

        while (true) {
            if (GetAsyncKeyState(VK_SPACE)) {
                playerY -= liftPower;
            }

            drawBarriers();
            drawPlayer();
            showScore();

            if (detectCollision()) break;

            Sleep(100);
            erasePlayer();
            playerY += 1;
        }
    }
};

int main() {
    while (true) {
        ConsoleBirdGame flappyConsole;
        flappyConsole.runGame();
        moveCursorTo(30, 30);
        cout << "Do you want to play again? (Y/N)";

        char choice;
        cin >> choice;
        if (choice == 'N' || choice == 'n') {
            break;
        }
        system("cls");
    }

    return 0;
}
            