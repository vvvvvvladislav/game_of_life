#include <ncurses.h>
#include <stdio.h>

#define N 80
#define M 25
#define FIELD_WIDTH 82
#define FIELD_HEIGTH 27
#define MIN_SPEED 1000
#define MAX_SPEED 50

void start_game();
void draw_field();
int change_speed(int speed, int control);
// void stop_game(int control, int* is_working);

int main() {
    initscr();
    int speed = 500;
    int control = 0;

    initscr();
    cbreak();
    noecho();
    napms(speed); // TODO: CHECK NAPMS

    draw_field();

    while (control != ' ') {
        control = getch();
        if (control == 'a' || control == 'z') {
            speed = change_speed(speed, control);
            napms(speed);
        }
        // calculate_new_matrix();
        draw_field();
        refresh();
    }

    endwin();

    // usleep(speed);
    return 0;
}

void start_game() {
    // TODO start game here. init matrix
    draw_field();
}

// void calculate_new_matrix(int **matrix) {
//     // TODO: return new state matrix
// }

void draw_field() {
    for (int i = 0; i < M + 2; i++) {
        for (int j = 0; j < N + 2; j++) {
            if (i == 0 && j > 0) mvaddch(0, j, '=');
            if (i == FIELD_HEIGTH - 1 && j > 0) mvaddch(i, j, '=');
            if ((j == 0) || j == FIELD_WIDTH - 1) mvaddch(i, j, '|');
            // else mvaddch(i, j, ' ');
        }
    }
}

int change_speed(int speed, int control) {
    int new_speed = speed;
    int step_speed = 50;

    if (control == 'A' || control == 'a') {
        new_speed -= step_speed;
    } else if (control == 'Z' || control == 'z') {
        new_speed += step_speed;
    }

    return new_speed;
}

// void stop_game(int control, int* is_working) {
//     // TODO: stop game
//     if (control == ' ') {
//         is_working = 0; // set to false
//     }
// }