#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include <algorithm> 

void createBoard(char arr[3][3]);
void printBoard(const char arr[3][3]);
char checkWin(const char arr[3][3]);
int goesFirst();
std::vector<int> checkOpenMoves(const char arr[3][3]);
void cpuMove(char arr[3][3]);
int selectBestMove(const std::vector<int>& openMoves, char arr[3][3]);
void startGame(char arr[3][3], int random);

void createBoard(char arr[3][3]) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            arr[i][j] = '-';
        }
    }
    printBoard(arr);
}

void printBoard(const char arr[3][3]) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            printf("%c ", arr[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

char checkWin(const char arr[3][3]) {
    for (int i = 0; i < 3; ++i) {
        if (arr[i][0] == arr[i][1] && arr[i][1] == arr[i][2] && arr[i][0] != '-') {
            return arr[i][0];
        }
        if (arr[0][i] == arr[1][i] && arr[1][i] == arr[2][i] && arr[0][i] != '-') {
            return arr[0][i];
        }
    }
    if (arr[0][0] == arr[1][1] && arr[1][1] == arr[2][2] && arr[0][0] != '-') {
        return arr[0][0];
    }
    if (arr[0][2] == arr[1][1] && arr[1][1] == arr[2][0] && arr[0][2] != '-') {
        return arr[0][2];
    }
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (arr[i][j] == '-') {
                return '-';
            }
        }
    }
    return 'T'; // Tie
}

int goesFirst() {
    srand(time(NULL));
    return rand() % 2;
}

std::vector<int> checkOpenMoves(const char arr[3][3]) {
    std::vector<int> openMoves;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (arr[i][j] == '-') {
                openMoves.push_back(i * 3 + j + 1);
            }
        }
    }
    return openMoves;
}

void cpuMove(char arr[3][3]) {
    std::vector<int> openMoves = checkOpenMoves(arr);
    if (openMoves.empty()) {
        printf("No available moves.\n");
        return;
    }

    int selectedMove = selectBestMove(openMoves, arr);
    int row = (selectedMove - 1) / 3;
    int col = (selectedMove - 1) % 3;
    arr[row][col] = 'O';
    printf("CPU selected move: %d\n", selectedMove);
    printBoard(arr);
}

int selectBestMove(const std::vector<int>& openMoves, char arr[3][3]) {
    // Check for winning moves or blocking moves
    for (char player : {'O', 'X'}) {
        for (int move : openMoves) {
            int row = (move - 1) / 3;
            int col = (move - 1) % 3;
            arr[row][col] = player;
            if (checkWin(arr) == player) {
                arr[row][col] = '-';
                return move;
            }
            arr[row][col] = '-';
        }
    }

    // Prefer middle move
    if (std::find(openMoves.begin(), openMoves.end(), 5) != openMoves.end()) {
        return 5;
    }

    // Prefer corners over edges
    std::vector<int> corners = {1, 3, 7, 9};
    for (int corner : corners) {
        if (std::find(openMoves.begin(), openMoves.end(), corner) != openMoves.end()) {
            return corner;
        }
    }

    // Return any open move (should be an edge at this point)
    return openMoves[0];
}

void startGame(char arr[3][3], int random) {
    createBoard(arr);
    int turn = random;
    char winner = '-';

    if (turn == 1) {
        printf("CPU goes first!\n");
        cpuMove(arr);
        turn = 0;
    } else {
        printf("You go first!\n");
    }

    while (winner == '-') {
        if (turn == 0) {
            int userMove;
            printf("Enter your move (1-9): ");
            scanf("%d", &userMove);
            int row = (userMove - 1) / 3;
            int col = (userMove - 1) % 3;

            if (arr[row][col] == '-') {
                arr[row][col] = 'X';
                printBoard(arr);
                winner = checkWin(arr);
                turn = 1;
            } else {
                printf("Invalid move. Please try again.\n");
            }
        } else {
            cpuMove(arr);
            winner = checkWin(arr);
            turn = 0;
        }
    }

    if (winner == 'X') {
        printf("Congratulations! You win!\n");
    } else if (winner == 'O') {
        printf("CPU wins!\n");
    } else {
        printf("It's a tie!\n");
    }
}

int main(void) {
    char arr[3][3];
    int random = goesFirst();
    startGame(arr, random);
    return 0;
}
