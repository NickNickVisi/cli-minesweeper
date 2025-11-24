#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// These macros assure a simpler way of compiling on either Windows or Linux machines

#define CHARNEWLINE '\n'

#ifdef _WIN32
    #define NEWLINE "\r\n"
#else
    #define NEWLINE "\n"
#endif

typedef struct {
    int easy, medium, hard;
}bombs;

void refresh_screen(void)
{
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void clear_buffer(void)
{
    char c = '\0';

    while (c != CHARNEWLINE) {
        c = getchar();
    }
}

int main_menu(void)
{
    int selection;
    char newline = '\0';

    a:
    printf("Please select difficulty or action:"NEWLINE);
    printf("1 - Easy"NEWLINE"2 - Medium"NEWLINE"3 - Hard"NEWLINE"4 - Quit"NEWLINE);
    scanf(" %d%c", &selection, &newline);
    if (newline != CHARNEWLINE) {
        refresh_screen();
        clear_buffer();
        printf("Insert only a digit!"NEWLINE);
        goto a;
    }
    switch(selection) {
        case 1: {
        }
        case 2: {
        }
        case 3: {
            return selection;
        }
        case 4: {
            return 0;
        }
        default: {
            refresh_screen();
            printf("Please use a valid digit."NEWLINE);
            newline = '\0';
            goto a;
        }
    }
}

char **generate_map(int n)
{
    char **map = malloc(n * sizeof(char *));

    for (int i = 0; i < n; i++) {
        map[i] = malloc(n * sizeof(char));
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            map[i][j] = '0';
        }
    }
    return map;
}

int **generate_boolean(int n)
{
    int **fog = malloc(n * sizeof(int *));

    for (int i = 0; i < n; i++) {
        fog[i] = malloc(n * sizeof(int));
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fog[i][j] = (i == 0 || j == 0 || i == n - 1 || j == n - 1);
        }
    }
    return fog;
}

void place_bombs(char **map, int n, bombs diff)
{
    int bombs_count;

    srand(time(NULL));
    switch(n) {
        case 8: {
            bombs_count = diff.easy;
            break;
        }
        case 16: {
            bombs_count = diff.medium;
            break;
        }
        case 24: {
            bombs_count = diff.hard;
            break;
        }
    }
    while (bombs_count) {
        int x = rand() % (n - 2) + 1, y = rand() % (n - 2) + 1;

        if (map[x][y] != 'b') {
            map[x][y] = 'b';
            for (int i = -1; i < 2; i++) {
                for (int j = -1; j < 2; j++) {
                    if (map[x + i][y + j] != 'b') {
                        map[x + i][y + j] = map[x + i][y + j] + 1;
                    }
                }
            }
            bombs_count--;
        }
    }
    for (int i = 0; i < n + 2; i++) {
        map[i][n + 1] = map[n + 1][i] = '@';
        map[i][0] = map[0][i] = '@';
    }
}

void display_full(int n, char **map)
{
    printf("   ");
    for (int i = 0; i < n; i++) {
        if (i < 10) {
            printf(" ");
        } else {
            printf("%d", i / 10);
        }
        printf(" ");
    }
    printf(NEWLINE);
    printf("   ");
    for (int i = 0; i < n; i++) {
        printf("%d ", i % 10);
    }
    printf(NEWLINE);
    for (int i = 0; i < n; i++) {
        if (i < 10) {
            printf(" ");
        }
        printf("%d ", i);
        for (int j = 0; j < n ; j++) {
            printf("%c ", map[i][j]);
        }
        printf(NEWLINE);
    } 
}

void display(int n, char **map, int **fog)
{
    printf("   ");
    for (int i = 0; i < n; i++) {
        if (i < 10) {
            printf(" ");
        } else {
            printf("%d", i / 10);
        }
        printf(" ");
    }
    printf(NEWLINE);
    printf("   ");
    for (int i = 0; i < n; i++) {
        printf("%d ", i % 10);
    }
    printf(NEWLINE);
    for (int i = 0; i < n; i++) {
        if (i < 10) {
            printf(" ");
        }
        printf("%d ", i);
        for (int j = 0; j < n ; j++) {
            if (fog[i][j]) {
                printf("%c ", map[i][j]);
            } else {
                printf("= ");
            }
        }
        printf(NEWLINE);
    }
}

void free_mem(void **matrix, int n)
{
    for (int i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void fill(char **map, int **fog, int x, int y, int n, int *count)
{
    if (!fog[x][y]) {
        fog[x][y] = 1;
        (*count)++;
        if (map[x][y] == '0') {
            if (fog[x + 1][y] != 1) {
                fill(map, fog, x + 1, y, n, &(*count));
            }
            if (fog[x][y + 1] != 1) {
                fill(map, fog, x, y + 1, n, &(*count));
            }
            if (fog[x - 1][y] != 1) {
                fill(map, fog, x - 1, y, n, &(*count));
            }
            if (fog[x][y - 1] != 1) {
                fill(map, fog, x, y - 1, n, &(*count));
            }
        }
    }
}

void gameplay(int n, char **map, bombs diff)
{
    int x = 1, y = 1, loop = 1, bombs_count, count = 0;
    int **fog = generate_boolean(n);

    if (!fog) {
        fprintf(stderr, "Memory allocation has failed. Aborting."NEWLINE);
        return;
    }
    switch(n) {
        case 10: {
            bombs_count = diff.easy;
            break;
        }
        case 18: {
            bombs_count = diff.medium;
            break;
        }
        case 26: {
            bombs_count = diff.hard;
            break;
        }
    }
    while (loop) {
        display(n, map, fog);
        printf("Coordinates:");
        if (scanf("%d %d", &x, &y) != 2 || x < 1 || y < 1 || x > n - 2 || y > n - 2) {
            refresh_screen();
            printf("Invalid coordinates!"NEWLINE);
            clear_buffer();
            continue;
        }
        if (map[x][y] == 'b') {
            refresh_screen();
            loop = 0;
            display_full(n, map);
            printf("You died! There was a bomb at: x = %d, y = %d"NEWLINE, x, y);
            break;
        }
        if (fog[x][y]) {
            refresh_screen();
            printf("Cell already uncovered!"NEWLINE);
            continue;
        } else {
            fill(map, fog, x, y, n - 2, &count);
        }
        refresh_screen();
        if (count == (n - 2) * (n - 2) - bombs_count) {
            display_full(n, map);
            printf("You won!"NEWLINE);
            loop = 0;
        }
    }
    free_mem((void **)fog, n);
}

int main(void)
{
    b:
    int n = main_menu() * 8 + 2;
    if (n - 2) {
        char **map = generate_map(n);

        if (!map) {
            fprintf(stderr, "Memory allocation has failed. Aborting."NEWLINE);
            return -1;
        }
        bombs number_of_bombs;

//      --- Change these 3 parameters depending on difficulty ---
        number_of_bombs.easy = 10; // Maximum 63
        number_of_bombs.medium = 50; // Maximum 255
        number_of_bombs.hard = 100; // Maximum 575
//      ---                                                   ---
        place_bombs(map, n - 2, number_of_bombs);
        refresh_screen();
        gameplay(n, map, number_of_bombs);
        free_mem((void **)map, n);
        goto b;
    }
    return 0;
}
