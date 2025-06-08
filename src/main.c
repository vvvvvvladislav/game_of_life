#include <ncurses.h>
#include <stdio.h>

#define ROWS 80
#define COLS 25
#define FIELD_WIDTH 82
#define FIELD_HEIGTH 27
#define MIN_SPEED 1000
#define MAX_SPEED 50

void start_game();
void init_field(int field[ROWS][COLS]);
void draw_field(int field[ROWS][COLS], int speed);
void calculate_new_matrix(int field[ROWS][COLS], int new_field[ROWS][COLS]);
int change_speed(int speed, int control);

int main() {
    initscr();
    int speed = 50;
    int control = 0;

    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);

    int field[ROWS][COLS];
    int new_field[ROWS][COLS];
    init_field(field);
    draw_field(field, speed);

    while (control != ' ') {
        control = getch();
        if (control == 'a' || control == 'z') {
            speed = change_speed(speed, control);
        }
        calculate_new_matrix(field, new_field);
        clear();
        draw_field(field, speed);
        refresh();
        napms(speed);
    }

    endwin();
    return 0;
}

/*void start_game() {
    // TODO start game here. init matrix
    draw_field(field[ROWS][COLS]);
}*/

// void calculate_new_matrix(int **matrix) {
//     // TODO: return new state matrix
// }

void init_field(int field[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            field[i][j] = 0;
        }
    }
    // TODO: init state here. add presets from files
    field[10][10] = 1;
    field[11][10] = 1;
    field[12][10] = 1;
    field[12][9] = 1;
    field[11][8] = 1;
}

void calculate_new_matrix(int field[ROWS][COLS], int new_field[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            int alive_neighbors = 0;
            alive_neighbors +=
                field[(i - 1 + ROWS) % ROWS][j] + field[(i - 1 + ROWS) % ROWS][(j - 1 + COLS) % COLS] +
                field[i][(j - 1 + COLS) % COLS] + field[(i + 1) % ROWS][(j - 1 + COLS) % COLS] +
                field[(i + 1) % ROWS][j] + field[(i + 1) % ROWS][(j + 1) % COLS] + field[i][(j + 1) % COLS] +
                field[(i - 1 + ROWS) % ROWS][(j + 1) % COLS];
            if (field[i][j] == 1) {
                new_field[i][j] = (alive_neighbors == 2 || alive_neighbors == 3) ? 1 : 0;
            } else {
                new_field[i][j] = (alive_neighbors == 3) ? 1 : 0;
            }
        }
    }
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            field[i][j] = new_field[i][j];
        }
    }
}

void draw_field(int field[ROWS][COLS], int speed) {
    for (int i = 0; i < COLS + 2; i++) {
        for (int j = 0; j < ROWS + 2; j++) {
            if (i == 0 && j > 0) mvaddch(0, j, (char)168);
            if (i == FIELD_HEIGTH - 1 && j > 0) mvaddch(i, j, (char)168);
            if ((j == 0) || j == FIELD_WIDTH - 1) mvaddch(i, j, (char)168);
            if ((i > 0) && (i < COLS + 1) && (j > 0) && (j < ROWS + 1))
                mvaddch(i, j, ((field[j - 1][i - 1])) == 0 ? ' ' : (char)168);
            // else mvaddch(i, j, ' ');
        }
    }
    mvprintw(FIELD_HEIGTH, 2, " Speed: %dms (a/z to change, SPACE to exit) ",
             speed);  // TODO DELETE THIS LINE
}

int change_speed(int speed, int control) {
    int new_speed = speed;
    int step_speed = 50;

    if (control == 'a') {
        new_speed = (new_speed - step_speed < MAX_SPEED) ? MAX_SPEED : new_speed - step_speed;
    } else if (control == 'z') {
        new_speed = (new_speed + step_speed > MIN_SPEED) ? MIN_SPEED : new_speed + step_speed;
    }

    return new_speed;
}