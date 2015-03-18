/*
 * Tijmen Zwaan
 * 10208917
 *
 * Checks whether the kings on a given chess board are in check.
 */
typedef struct king king;
struct king {
    int x,
        y,
        enemy;
};

int main();
king find_king(char**, int);
int check_pawn(char**, king);
int check_knight(char**, king);
int check_bishop(char**, king);
int check_rook(char**, king);
int check_queen(char**, king);
int check_king(char**, king);
int check_dir(char**, king, int, int, int);
char **init_board();
