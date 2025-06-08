#include <ncurses.h>
#include <stdio.h>

#define N 80
#define M 25
#define FIELD_WIDTH 82
#define FIELD_HEIGTH 27
#define MIN_SPEED 1000
#define MAX_SPEED 50

void start_game();
void init_field(int field[N][M]);
void draw_field(int field[N][M], int speed);
void calculate_new_matrix(int field[N][M], int new_field[N][M]);
int change_speed(int speed, int control);
// void stop_game(int control, int* is_working);

int main() {
    initscr();
    int speed = 500;
    int control = 0;

    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);

    int field[N][M];
    int new_field[N][M];
    init_field(field);
    // draw_field(field);

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
    draw_field(field[N][M]);
}*/

// void calculate_new_matrix(int **matrix) {
//     // TODO: return new state matrix
// }

void init_field(int field[N][M]) {
    for (int i = 0; i < N; i++) {
       for (int j = 0; j < M; j++) {
           field[i][j] = 0; 
        }
    }
    field[10][10] = 1;
    field[11][10] = 1;
    field[12][10] = 1;
    field[12][9] = 1;
    field[11][8] = 1;
}

//void preset(int mode, int field[N][M]) {}

void calculate_new_matrix(int field[N][M], int new_field[N][M]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            int alive_neighbors = 0;
            alive_neighbors += field[(i-1) % N][j % M] + field[(i-1) % N][(j-1) % M] + field[i % N][(j-1) % M] + field[(i+1) % N][(j-1) % M] + field[(i+1) % N][j % M] + field[(i+1) % N][(j+1) % M] + field[i % N][(j+1) % M] + field[(i-1) % N][(j+1) % M];
            if (field[i][j] == 1) {
                new_field[i][j] = (alive_neighbors == 2 || alive_neighbors == 3) ? 1 : 0;
            } else {
                new_field[i][j] = (alive_neighbors == 3) ? 1 : 0;
            }
        }
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            field[i][j] = new_field[i][j];
        }
    }
}
void draw_field(int field[N][M], int speed) {
    for (int i = 0; i < M + 2; i++) {
        for (int j = 0; j < N + 2; j++) {
            if (i == 0 && j > 0) mvaddch(0, j, '=');
            if (i == FIELD_HEIGTH - 1 && j > 0) mvaddch(i, j, '=');
            if ((j == 0) || j == FIELD_WIDTH - 1) mvaddch(i, j, '|');
            if ((i > 0) && (i < M + 1) && (j > 0) && (j < N + 1)) mvaddch(i, j, (field[i-1][j-1]) + '0');
            // else mvaddch(i, j, ' ');
        }
    }

    mvprintw(FIELD_HEIGTH-1, 2, " Speed: %dms (a/z to change, SPACE to exit) ", speed);
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

// void stop_game(int control, int* is_working) {
//     // TODO: stop game
//     if (control == ' ') {
//         is_working = 0; // set to false
//     }
// }