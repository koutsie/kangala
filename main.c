#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define BOARD_SIZE 14
#define PLAYER_ONE_MANCALA 6
#define PLAYER_TWO_MANCALA 13

void init_board(int *board);
void print_board(int *board);
void player_turn(int player, int *board);
int game_over(int *board);

int main()
{
    int b[BOARD_SIZE], p = 1;
    time_t start_time, current_time;
    double elapsed_time;

    init_board(b);

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    if (w.ws_row < 13 || w.ws_col < 38)
    {
        printf("13x38!\n");
        return 1;
    }

    start_time = time(NULL);

    while (!game_over(b))
    {
        print_board(b);
        current_time = time(NULL);
        elapsed_time = difftime(current_time, start_time);
        printf("Game time: %02d:%02d:%02d\n", ((int)elapsed_time / 3600), (((int)elapsed_time / 60) % 60), ((int)elapsed_time % 60));

        player_turn(p, b);
        p = (p == 1) ? 2 : 1;
    }
    return 0;
}

void print_board(int *board)
{
    printf("Player 2\n");
    printf("+----+----+----+----+----+----+\n");
    printf("| %2d | %2d | %2d | %2d | %2d | %2d |\n", board[12], board[11], board[10], board[9], board[8], board[7]);
    printf("|    |----|----|----|----|----|\n");
    printf("| %2d | %2d | %2d | %2d | %2d | %2d |\n", board[13], board[0], board[1], board[2], board[3], board[4]);
    printf("+----+----+----+----+----+----+\n");
    printf("      | %2d | %2d | %2d | %2d | %2d | %2d |\n", board[5], board[6], board[7], board[8], board[9], board[10]);
    printf("      +----+----+----+----+----+----+\n");
    printf("            Player 1\n");
}

void init_board(int *b)
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        b[i] = (i == PLAYER_ONE_MANCALA || i == PLAYER_TWO_MANCALA) ? 0 : 4;
    }
}

void player_turn(int player, int *board)
{
    int p, s, e;
    printf("P%d turn:\nEnter pit number (1-6): ", player);
    scanf("%d", &p);
    while (p < 1 || p > 6 || board[(player - 1) * 7 + p] == 0)
        printf("Invalid input, try again.\nEnter pit number (1-6): "), scanf("%d", &p);
    s = board[(player - 1) * 7 + p];
    board[(player - 1) * 7 + p] = 0;
    e = (player - 1) * 7 + p + s;
    while (s)
    {
        if (e == PLAYER_ONE_MANCALA && player == 1)
            e = PLAYER_TWO_MANCALA;
        else if (e == PLAYER_TWO_MANCALA && player == 2)
            e = PLAYER_ONE_MANCALA;
        board[e]++;
        s--;
        e++;
        if (e > BOARD_SIZE - 1)
            e = 0;
    }
    if (e == PLAYER_ONE_MANCALA && player == 1)
        printf("P1 gets another turn!\n"), player_turn(player, board);
    else if (e == PLAYER_TWO_MANCALA && player == 2)
        printf("P2 gets another turn!\n"), player_turn(player, board);
    else if (board[e] == 1 && ((e <= 6 && player == 2) || (e >= 7 && e < 13 && player == 1)))
    {
        int o = 12 - e, s = board[o];
        board[o] = 0;
        if (player == 1)
            board[PLAYER_ONE_MANCALA] += s + 1;
        else
            board[PLAYER_TWO_MANCALA] += s + 1;
        printf("Captured %d stones!\n", s + 1);
    }
}

int game_over(int *b)
{
    int p1s = 0, p2s = 0, i;
    for (i = 0; i < 14; i++)
    {
        if (i != 6 && i != 13)
            i < 7 ? (p2s += b[i]) : (p1s += b[i]);
    }
    if (!p1s || !p2s)
    {
        int p1 = b[6] + p1s, p2 = b[13] + p2s;
        printf("Game over!\nPlayer 1: %d\nPlayer 2: %d\n", p1, p2);
        if (p1 > p2)
            printf("P1 wins!\n");
        else if (p2 > p1)
            printf("P2 wins!\n");
        else
            printf("It's a tie!\n");
        return 1;
    }
    return 0;
}
