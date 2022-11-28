#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#define XSIZE 40
#define YSIZE 20

int x, y, food_x, food_y;
int game_over, score = 0, eatcount = 0;
int quit = 0;
int flag;

int kbhit(void)
{
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

void Boundary(int field[YSIZE][XSIZE]) {
    for(int i = 0; i < YSIZE; i++) {
        for(int j = 0; j < XSIZE; j++) {
            if (i == 0 || i == YSIZE - 1 || j == 0 || j == XSIZE - 1) {
                field[i][j] = 2;
            } else {
                field[i][j] = 0;
            }
        }
    }
}

void Setup() {
    game_over = 0;
    x = XSIZE/2;
    y = YSIZE/2;
    score = 0;

    while (food_x == 0 || food_x == 39) {
        food_x = rand() % 40;
    }
    while (food_y == 0 || food_y == 19) {
        food_y = rand() % 20;
    }
}

void FoodReset() {
    while (food_x == 0 || food_x == 39) {
        food_x = rand() % 40;
    }
    while (food_y == 0 || food_y == 19) {
        food_y = rand() % 20;
    }
}

void Input() {
    if(kbhit()) {
        switch(getchar()) {
            case 'w' : flag = 0; break;
            case 'a' : flag = 3; break;
            case 's' : flag = 2; break;
            case 'd' : flag = 1; break;
            case 'x' : game_over = 1; break; 
        }
    }
}

void Algorithm(int field[YSIZE][XSIZE]) {
    sleep(.1);
    field[y][x] = 0;
    switch (flag) {
        case 0 : y--; field[y][x] = 1; break;
        case 1 : x++; field[y][x] = 1; break;
        case 2 : y++; field[y][x] = 1; break;
        case 3 : x--; field[y][x] = 1; break;
        default: break;
    }

    if (x < 0 || x >= XSIZE || y < 0 || y >= YSIZE) game_over = 1;
    if (x == food_x && y == food_y) {
        field[food_y][food_x] = 0;
        FoodReset();
        field[food_y][food_x] = 3;
        eatcount++;
        score += 10 * eatcount;
    } 
}

void Print(int field[YSIZE][XSIZE]) {
    for(int i = 0; i < YSIZE; i++) {
        for(int j = 0; j < XSIZE; j++) {
            switch (field[i][j])
            {
            case 3 : printf("*");
                break;
            case 2 : printf("□");
                break;
            case 1 : printf("■");
                break;
            default: printf("  ");
                break;
            }
        }
        printf("\n");
    }
    printf("Score = %d", score);
}

int main(void) {
    int field[YSIZE][XSIZE];
    Setup();
    Boundary(field);
    field[food_y][food_x] = 3;
    while (!game_over) {
        system("reset");
        Input();
        Algorithm(field);
        Print(field);
    }

    puts("Game Over!!");
    return 0;
}
