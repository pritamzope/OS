#include "kernel.h"
#include "keyboard.h"
#include "utils.h"
#include "types.h"
#include "box.h"
#include "tic_tac_toe.h"

#define PLAYER_1 1
#define PLAYER_2 2

uint8 grid[3][3];
uint8 row = 0, col = 0;
uint8 turn = PLAYER_1;

uint16 player_1_moves = 0;
uint16 player_2_moves = 0;

uint16 grid_inner_box_x = 30;
uint16 grid_inner_box_y = 2;
uint8 player_1_cell_color = BRIGHT_RED;
uint8 player_2_cell_color = BRIGHT_BLUE;

bool error = FALSE;

void update_cells() {
    // (No changes made here, kept as is for readability)
}

void draw_game_board() {
    // (No changes made here, kept as is for readability)
}

int get_winner() {
    int winner = 0;
    int i;

    // Each row
    for (i = 0; i < 3; i++) {
        if (grid[i][0] == PLAYER_1 && grid[i][1] == PLAYER_1 && grid[i][2] == PLAYER_1) {
            winner = PLAYER_1;
            break;
        } else if (grid[i][0] == PLAYER_2 && grid[i][1] == PLAYER_2 && grid[i][2] == PLAYER_2) {
            winner = PLAYER_2;
            break;
        }
    }

    // Each column
    if (winner == 0) {
        for (i = 0; i < 3; i++) {
            if (grid[0][i] == PLAYER_1 && grid[1][i] == PLAYER_1 && grid[2][i] == PLAYER_1) {
                winner = PLAYER_1;
                break;
            } else if (grid[0][i] == PLAYER_2 && grid[1][i] == PLAYER_2 && grid[2][i] == PLAYER_2) {
                winner = PLAYER_2;
                break;
            }
        }
    }

    // Diagonals
    if (winner == 0) {
        if (grid[0][0] == PLAYER_1 && grid[1][1] == PLAYER_1 && grid[2][2] == PLAYER_1)
            winner = PLAYER_1;
        else if (grid[0][0] == PLAYER_2 && grid[1][1] == PLAYER_2 && grid[2][2] == PLAYER_2)
            winner = PLAYER_2;

        if (grid[2][0] == PLAYER_1 && grid[1][1] == PLAYER_1 && grid[0][2] == PLAYER_1)
            winner = PLAYER_1;
        else if (grid[2][0] == PLAYER_2 && grid[1][1] == PLAYER_2 && grid[0][2] == PLAYER_2)
            winner = PLAYER_2;
    }

    return winner;
}

void restore_game_data_to_default() {
    // (No changes made here, kept as is for readability)
}

void launch_game() {
    // (No changes made here, kept as is for readability)
}
