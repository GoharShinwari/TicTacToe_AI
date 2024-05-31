#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <time.h> 

void createBoard(char arr[3][3]);
void printBoard(char arr[3][3]);
char checkWin(char arr[3][3]);
int goesFirst();
std::vector<int> checkOpenMoves(char arr[3][3]);
void cpuMove(char arr[3][3]);
int highestTemp(std::vector<int> openMoves, char arr[3][3]);

void createBoard(char arr[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int g = 0; g < 3; g++) {
            arr[i][g] = '-';
        }
    }
    printBoard(arr);
}

void printBoard(char arr[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int g = 0; g < 3; g++) {
            printf("%c ", arr[i][g]);
        }
        printf("\n");
    }
    printf("\n");
}

char checkWin(char arr[3][3]) {
    for (int i = 0; i < 3; i++) {
        if (arr[i][0] == arr[i][1] && arr[i][1] == arr[i][2] && arr[i][0] != '-') {
            return arr[i][0];
        }
    }
    for (int i = 0; i < 3; i++) {
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
    bool tie = true;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (arr[i][j] == '-') {
                tie = false;
                break;
            }
        }
        if (!tie) break;
    }
    if (tie) return 'T';
    return '-';
}

int goesFirst() {
    srand(time(NULL));
    return rand() % 2;
}

std::vector<int> checkOpenMoves(char arr[3][3]) {
    std::vector<int> openMoves;
    for (int i = 0; i < 3; i++) {
        for (int g = 0; g < 3; g++) {
            if (arr[i][g] == '-') {
                openMoves.push_back(i * 3 + g + 1); 
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

    int selectedMove = highestTemp(openMoves, arr);
    int row = (selectedMove - 1) / 3;
    int col = (selectedMove - 1) % 3;
    arr[row][col] = 'O';
    printf("CPU selected move: %d\n", selectedMove);
    printBoard(arr);
}

int highestTemp(std::vector<int> openMoves, char arr[3][3]) {
    int weights[] = {5, 4, 3, 2, 1};
    int selectedMove = openMoves[0];
    int maxWeight = -1;

    // Check for winning moves
    for (int move : openMoves) {
        int row = (move - 1) / 3;
        int col = (move - 1) % 3;
        arr[row][col] = 'O';
        if (checkWin(arr) == 'O') {
            selectedMove = move;
            maxWeight = weights[0];
            arr[row][col] = '-'; 
            return selectedMove;
        }
        arr[row][col] = '-'; 
    }

    // Check for blocking moves
    for (int move : openMoves) {
        int row = (move - 1) / 3;
        int col = (move - 1) % 3;
        arr[row][col] = 'X';
        if (checkWin(arr) == 'X') {
            selectedMove = move;
            maxWeight = weights[1];
            arr[row][col] = '-'; 
            return selectedMove;
        }
        arr[row][col] = '-'; 
    }

    // Check for corner moves
    for (int move : openMoves) {
        if (move == 1 || move == 3 || move == 7 || move == 9) {
            selectedMove = move;
            maxWeight = weights[2];
            return selectedMove;
        }
    }

    // Check for middle move if opponent takes corner
    if (openMoves.size() >= 3) {
        int oppCorner = -1;
        for (int i = 0; i < openMoves.size(); ++i) {
            int move = openMoves[i];
            if ((move == 1 && arr[2][2] == 'X') || (move == 3 && arr[2][0] == 'X') ||
                (move == 7 && arr[0][2] == 'X') || (move == 9 && arr[0][0] == 'X')) {
                oppCorner = move;
                break;
            }
        }
        if (oppCorner != -1) {
            selectedMove = 5; 
            maxWeight = weights[3];
        }
    }

    // Select randomly from remaining moves
    if (maxWeight == -1) {
        selectedMove = openMoves[rand() % openMoves.size()];
        maxWeight = weights[4];
    }

    return selectedMove;
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

    do {
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
                if (winner == '-') {
                    turn = 1;
                }
            } else {
                printf("Invalid move. Please try again.\n");
            }
        } else {
            cpuMove(arr);
            winner = checkWin(arr);
            if (winner == '-') {
                turn = 0;
            }
        }
    } while (winner == '-');

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
