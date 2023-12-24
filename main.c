#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

#define board_size 20

int isRunning;
char board[20][20];
int collected_food;
int high_score;

struct Body
{
    int x;
    int y;
};

struct Player
{
    int xpos;
    int ypos;
    int xvel;
    int yvel;
    struct Body body[128];
    int length;
};

struct Food
{
    int xpos;
    int ypos;
};

struct Player player;
struct Food food;

void print_board()
{
    for (int y = 0; y < board_size; y++)
    {
        for (int x = 0; x < board_size; x++)
        {
            putch(board[y][x]);
        }

        putch('\n');
    }
}

void draw_border()
{
    for (int y = 0; y < board_size; y++)
    {
        for (int x = 0; x < board_size; x++)
        {
            if (y == 0 || y == board_size - 1 || x == 0 || x == board_size - 1)
            {
                board[y][x] = '#';
            }
            else
            {
                board[y][x] = ' ';
            }
        }
    }
}

void draw_head()
{
    player.xpos += player.xvel;
    player.ypos += player.yvel;
    board[player.ypos][player.xpos] = '@';
}

void move_player(int xdir, int ydir)
{
    if (player.length > 0)
    {
        if (player.xvel * -1 == xdir || player.yvel * -1 == ydir)
        {
            isRunning = 0;
        }
    }
    
    player.xvel = xdir;
    player.yvel = ydir;
}

void get_input()
{
    if (kbhit())
    {
        int key_code = getch();

        switch (key_code)
        {
        case 'w':
            move_player(0, -1);
            break;
        case 's':
            move_player(0, 1);
            break;
        case 'a':
            move_player(-1, 0);
            break;
        case 'd':
            move_player(1, 0);
            break;
        default:
            break;
        }
    }
}

void delay(double number_of_seconds)
{
    double milli_seconds = 1000 * number_of_seconds;
 
    clock_t start_time = clock();
 
    while (clock() < start_time + milli_seconds);
}

void clear_screen()
{
    // printf("\e[1;1H\e[2J");
    system("cls");
}

void draw_food()
{
    if (collected_food)
    {
        food.xpos = 1 + rand() % (board_size-2);
        food.ypos = 1 + rand() % (board_size-2);
        collected_food = 0;
    }

    board[food.ypos][food.xpos] = '+';
}

int check_collect_food()
{
    if (player.xpos == food.xpos && player.ypos == food.ypos && collected_food == 0)
    {
        collected_food = 1;
        player.length += 1;
        player.body[player.length - 1].x = player.xpos;
        player.body[player.length - 1].y = player.ypos;
    }
}

void draw_tail()
{
    if (player.length > 0)
    {
        for (int i = player.length - 1; i >= 0; i--)
        {
            if (i == 0)
            {
                player.body[i].x = player.xpos;
                player.body[i].y = player.ypos;
            }
            else
            {
                player.body[i].x = player.body[i - 1].x;
                player.body[i].y = player.body[i - 1].y;
            }
            
            board[player.body[i].y][player.body[i].x] = '*';
        }
    }
}

void check_rules()
{
    if (player.xpos == 0 || player.xpos == board_size - 1 || player.ypos == 0 || player.ypos == board_size - 1)
    {
        isRunning = 0;
    }

    for (int i = 0; i < player.length; i++)
    {
        if (player.xpos == player.body[i].x && player.ypos == player.body[i].y)
        {
            isRunning = 0;
        }
    }
    
}

int read_high_score()
{
    FILE *file = fopen("score.txt", "r");

    if (file == NULL)
    {
        return 0;
    }
    else
    {
        int num;
        fscanf(file, "%d", &num);
        fclose(file);
        return num;
    }

}

void write_high_score(int new_high_score)
{
    FILE *file = fopen("score.txt", "w");
    fprintf(file, "%d", new_high_score);
    fclose(file);
}

int main()
{
    srand(time(0));

    player.xpos = player.ypos = board_size / 2 - 1;
    player.xvel = 0;
    player.yvel = 1;
    player.length = 0;

    isRunning = 1;
    collected_food = 1;
    high_score = read_high_score();

    while (isRunning)
    {
        get_input();
        draw_border();
        draw_tail();
        draw_head();
        check_rules();
        check_collect_food();
        draw_food();
        clear_screen();
        print_board();
        delay(0.1);
    }

    printf("Game Over\n");
    printf("High Score: %i\n", high_score);
    printf("Score: %i\n", player.length);
    
    if (player.length > high_score)
    {
        write_high_score(player.length);
    }

    return 0;
}