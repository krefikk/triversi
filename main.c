#include <stdlib.h>
#include <stdio.h>

#define N_MAX 23
/*
Ahmet Refik Şener
*/

int main() {
    // Initialize and assign necessary variables
    int n = 0; // Board size
    int i, j; // Loop variables
    int x, y; // Start coordinates
    int moveCount = 0, player = 0, filled = 0, gameOver = 0, found = 0, validMove = 1, atCenter = 1;
    int lastCord[2];
    int currentCord[2];
    int directions[8][2] = { {-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1} };
    char colors[] = { 'R', 'Y', 'B' };
    char emptyCell = '-'; // Default empty cell holder

    // Get board size from the user within game limits
    while (n < 3 || n > N_MAX) {
        printf("Enter the size of board (3-%d): ", N_MAX);
        scanf("%d", &n);
        if (n < 3 || n > N_MAX) {
            printf("Board size must be within the given limits (3-%d)\n\n", N_MAX);
        }
    }

    // Fill the board
    char board[n][n];
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            board[i][j] = emptyCell; // "-" represents empty cells
        }
    }

    // Main game loop
    while (!gameOver) {

        // Print the board
        printf("\nAfter %d. move:\n", moveCount);
        // Print the header (column numbers)
        printf("    "); // Space for row numbers
        for (int i = 0; i < n; i++) {
            printf("%3d", i); // Fixed width for column numbers
        }
        printf("\n");

        // Print the board
        for (int i = 0; i < n; i++) {
            printf("%3d ", i); // Fixed width for row numbers
            for (int j = 0; j < n; j++) {
                printf("%3c", board[i][j]); // Fixed width for board cells
            }
            printf("\n");
        }

        do {
            // Prompt the player for a move
            printf("\nPlayer %d (%c), make your move (Enter (x y) coordinates): ", player + 1, colors[player]);
            scanf("%d %d", &x, &y);

            // Reset validation flags
            found = 0;  // Used for adjacency check
            atCenter = 0; // Used for center check
            validMove = 1; // Assume the move is valid initially

            // Check if the move is within bounds
            if (x < 0 || x >= n || y < 0 || y >= n) {
                printf("Invalid move. Coordinates are out of bounds. Try again.\n");
                validMove = 0;
            }

            // Check if the cell is empty
            if (validMove == 1 && board[x][y] != emptyCell) {
                printf("Invalid move. Cell is already occupied. Try again.\n");
                validMove = 0;
            }

            // Check if the first move is at center
            if (validMove == 1 && moveCount == 0) {
                if (n % 2 == 0) {
                    if ((x == (n / 2) - 1 || x == n / 2) && (y == (n / 2) - 1 || y == n / 2)) {
                        atCenter = 1; // Valid first move
                    }
                }
                else {
                    if (x == n / 2 && y == n / 2) {
                        atCenter = 1; // Valid first move
                    }
                }

                // If first move is not at the center
                if (atCenter == 0) {
                    printf("Invalid move. First piece must be placed ");
                    if (n % 2 == 0) {
                        printf("in one of the center cells: (%d, %d), (%d, %d), (%d, %d), (%d, %d)\n",
                            (n / 2) - 1, (n / 2) - 1, (n / 2) - 1, (n / 2),
                            (n / 2), (n / 2) - 1, (n / 2), (n / 2));
                    }
                    else {
                        printf("at the center of the board: (%d, %d)\n", n / 2, n / 2);
                    }
                    validMove = 0;
                }
            }

            // For moves other than first move, check adjacency
            if (validMove == 1 && moveCount > 0) {
                j = 0;
                while (j < 8 && found == 0) {
                    if (x + directions[j][0] >= 0 && x + directions[j][0] < n && y + directions[j][1] >= 0 && y + directions[j][1] < n) {
                        if (board[x + directions[j][0]][y + directions[j][1]] != emptyCell) {
                            found = 1;
                        }
                    }
                    j++;
                }

                if (found == 0) {
                    printf("Invalid move. Cannot place a piece to a cell that's not adjacent to a filled cell. Try again.\n");
                    validMove = 0;
                }
            }
        } while (validMove == 0);

        // Place the piece
        board[x][y] = colors[player];
        filled++;

        // Make necessary assignments
        found = -1;

        // Check all directions (8 directions)
        for (i = 0; i < 8; i++) {
            // Assing current coordinates
            currentCord[0] = x;
            currentCord[1] = y;

            // Check if the adjacent cell in the checked direction is empty or not and of a different color than the current cell or not
            if (board[x + directions[i][0]][y + directions[i][1]] != emptyCell && board[x + directions[i][0]][y + directions[i][1]] != board[x][y]) {
                // If the adjacent cell in the checked direction is not empty and has a different color, keep checking
                found = 0;

                while (found == 0) {

                    // If a cell with the same color as the starting cell couldn't found in the checked direction because reached the end
                    if (currentCord[0] + directions[i][0] < 0 || currentCord[0] + directions[i][0] > n - 1 || currentCord[1] + directions[i][1] < 0 || currentCord[1] + directions[i][1] > n - 1) {
                        // No need further checking, should exit from while loop and start checking next direction
                        found = -1;
                    }
                    // If an empty cell is found in the checked direction
                    else if (board[currentCord[0] + directions[i][0]][currentCord[1] + directions[i][1]] == emptyCell) {
                        // No need further checking, should exit from while loop and start checking next direction
                        found = -1;
                    }
                    // If a cell with the same color as the starting cell is found in the checked direction
                    else if (board[currentCord[0] + directions[i][0]][currentCord[1] + directions[i][1]] == board[x][y]) {
                        // No need further checking, should exit from while loop and take necessary actions
                        found = 1;
                        // Assing last coordinates to same color cell's coordinates
                        lastCord[0] = currentCord[0] + directions[i][0];
                        lastCord[1] = currentCord[1] + directions[i][1];
                    }
                    // If a cell with the not same color as the starting cell or a not empty cell is found in the checked direction
                    // Continue checking further
                    currentCord[0] = currentCord[0] + directions[i][0];
                    currentCord[1] = currentCord[1] + directions[i][1];

                }

                // If cells of the same color are found without interruption
                if (found == 1) {

                    // Assing current coordinates as starting corrdinates
                    currentCord[0] = x;
                    currentCord[1] = y;

                    // For all the cells between starting and last coordinates
                    while (currentCord[0] != lastCord[0] || currentCord[1] != lastCord[1]) {
                        // Make it the same color as the start cell
                        board[currentCord[0]][currentCord[1]] = board[x][y];
                        // Update current coordinates
                        currentCord[0] = currentCord[0] + directions[i][0];
                        currentCord[1] = currentCord[1] + directions[i][1];
                    }

                }

            }

        }

        // Check all cells are filled or not
        if (filled == n * n) {
            // If filled, game is over
            printf("\nGame over! Piece counts:\n");
            int count[3] = { 0, 0, 0 };
            for (i = 0; i < n; i++) {
                for (j = 0; j < n; j++) {
                    for (int k = 0; k < 3; k++) {
                        if (board[i][j] == colors[k]) count[k]++;
                    }
                }
            }
            for (i = 0; i < 3; i++) printf("%c: %d\n", colors[i], count[i]);
            if (count[0] > count[1] && count[0] > count[2]) printf("Winner: RED (K)\n");
            else if (count[1] > count[0] && count[1] > count[2]) printf("Winner: YELLOW (S)\n");
            else if (count[2] > count[0] && count[2] > count[1]) printf("Winner: BLUE (M)\n");
            else printf("DRAW!\n");
            gameOver = 1;
        }

        // Move to the next player
        player = (player + 1) % 3;
        // Update move count
        moveCount++;

    }

    // Print the last board
    printf("\nBOARD, after %d moves:\n", moveCount);
    // Print the header (column numbers)
    printf("    "); // Space for row numbers
    for (int i = 0; i < n; i++) {
        printf("%3d", i); // Fixed width for column numbers
    }
    printf("\n");

    // Print the board
    for (int i = 0; i < n; i++) {
        printf("%3d ", i); // Fixed width for row numbers
        for (int j = 0; j < n; j++) {
            printf("%3c", board[i][j]); // Fixed width for board cells
        }
        printf("\n");
    }
    printf("\n");

    // Pause the console, so results can be seen
    system("pause");
	return 0;
}
