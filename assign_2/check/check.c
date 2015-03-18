/*
 * Tijmen Zwaan
 * 10208917
 *
 * Checks whether the kings on a given chess board are in check.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include "check.h"

/*
 * The colors of the kings are also used to indicate the difference
 * between the lowercase and uppercase letters to quickly switch
 * between piece colors.
 */
#define WHITE 0
#define BLACK 32

#define PAWN 'P'
#define KNIGHT 'N'
#define BISHOP 'B'
#define ROOK 'R'
#define QUEEN 'Q'
#define KING 'K'

#define EMPTY '.'

int main() {
    char **board = init_board();
    int i;

    for (i=0; i<=BLACK; i+=BLACK) {
        king king = find_king(board, i);
        if (check_king(board, king)) {
            printf("\nHow is it possible that two kings are next to eachother? You screwed up guys.\n");
            return EXIT_SUCCESS;
        }

        if (
                check_bishop(board, king) |
                check_rook(board, king) |
                check_queen(board, king) |
                check_knight(board, king) |
                check_pawn(board, king)) {
            if (i) {
                printf("\nblack king is in check.\n");
                return EXIT_SUCCESS;
            }
            printf("\nwhite king is in check.\n");
            return EXIT_SUCCESS;
        }
    }
    printf("\nno king is in check\n");

    return EXIT_SUCCESS;
}

char **init_board() {
    char **board = malloc(8 * sizeof(char*));
    int i, j;
    for (i=0; i<8; i++) {
        board[i] = malloc(8 * sizeof(char));
        scanf("%s", board[i]);
        for (j=0; j<8; j++) {
            printf("%c", board[i][j]);
        }
        printf("\n");
    }
    return board;
}

/*
 * finds the king of the chosen color and
 * returns a king struct with the coordinates
 * and the color of the enemy.
 * This is usefull later on when checking against
 * the pieces of the other color.
 */
king find_king(char **board, int color) {
    king king = {.x = 0, .y = 0};
    int i, j, target = KING + color;
    if (color) {
        king.enemy = 0;
    }
    else {
        king.enemy = BLACK;
    }
    for (i=0; i<8; i++){
        for (j=0; j<8; j++) {
            if (board[i][j] == target) {
                king.y = i;
                king.x = j;
                return king;
            }
        }
    }
    return king;
}

/*
 * checks whether an index is within the board bounds.
 */
int in_bounds(int index) {
    return index >= 0 && index < 8;
}

/*
 * Checks whether the given king is in check by a pawn.
 */
int check_pawn(char **board, king king) {
    int dy = 1;
    if (king.enemy) {
        if (!king.y) {
            return 0;
        }
        dy = -1;
    }
    else if (king.y == 7) {
        return 0;
    }
    if (
            board[king.y + dy][king.x - 1] == (PAWN + king.enemy) ||
            board[king.y + dy][king.x + 1] == (PAWN + king.enemy)) {
        return 1;
    }
    return 0;
}

/*
 * Checks whether the given king is in check by a knight.
 */
int check_knight(char **board, king king) {
    int dx, dy, sx, sy;

    for (dx=2, dy=1; dy<3; dx--, dy++) {
        for (sy=-1; sy<2; sy+=2) {
            for (sx=-1; sx<2; sx+=2) {
                if (in_bounds(king.x + (sx * dx)) && in_bounds(king.y + (sy * dy))) {
                    if (board[king.y + (sy * dy)][king.x + (sx * dx)] == KNIGHT + king.enemy)
                        return 1;
                }
            }
        }
    }
    return 0;
}

/*
 * Checks whether the given king is in check by a bishop.
 */
int check_bishop(char **board, king king) {
    int i, j;

    for (i=-1; i<2; i++) {
        for (j=-1; j<2; j++) {
            if (i==0 || j==0)
                continue;
            if (check_dir(board, king, BISHOP, i, j))
                return 1;
        }
    }
    return 0;
}

/*
 * Checks whether the given king is in check by a rook.
 */
int check_rook(char **board, king king) {
    int i, j;

    for (i=-1; i<2; i++) {
        for (j=-1; j<2; j++) {
            if (i ^ j) {
                if (check_dir(board, king, ROOK, i, j))
                    return 1;
            }
        }
    }
    return 0;
}

/*
 * Checks whether the given king is in check by a queen.
 */
int check_queen(char **board, king king) {
    int i, j;

    for (i=-1; i<2; i++) {
        for (j=-1; j<2; j++) {
            if (!(i || j))
                continue;
            if (check_dir(board, king, QUEEN, i, j))
                return 1;
        }
    }
    return 0;
}

/*
 * Checks if there is a match with the given piece
 * in one straight line from the given king in the
 * given direction.
 * Breaks if it encounters another piece.
 */
int check_dir(char **board, king king, int piece, int dx, int dy) {
    int x = king.x,
        y = king.y;
    while (x >= 0 && x < 8 && y >= 0 && y < 8) {
        if (y == king.y && x == king.x) {
            x += dx;
            y += dy;
            continue;
        }
        if (board[y][x] == piece + king.enemy)
            return 1;
        if (board[y][x] != EMPTY)
            break;
        x += dx;
        y += dy;
    }
    return 0;
}

/*
 * Checks if the king is checked by another king.
 * This should never happen.
 */
int check_king(char **board, king king) {
    int i, j;
    for (i=-1; i<2; i++) {
        for (j=-1; j<2; j++) {
            if (!(i || j))
                continue;
            if (!(in_bounds(king.y+i) && in_bounds(king.x+j)))
                continue;
            if (board[king.y + i][king.x + j] == KING + king.enemy) {
                return 1;
            }
        }
    }
    return 0;
}
