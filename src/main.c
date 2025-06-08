#include <ncurses.h>
#include <stdio.h>

#define ROWS 80
#define COLS 25
#define FIELD_WIDTH 82
#define FIELD_HEIGTH 27
#define MIN_SPEED 1000
#define MAX_SPEED 10

WINDOW* init_ncurses();
int init_field(int field[ROWS][COLS]);
void draw_field(int field[ROWS][COLS], WINDOW* win);
void calculate_new_matrix(int field[ROWS][COLS], int new_field[ROWS][COLS]);
int change_speed(int speed, int control);

int main() {
    int scan_result = 1;
    int field[ROWS][COLS];

    scan_result = init_field(field);

    if (!freopen("/dev/tty", "r", stdin)) {
        scan_result = 0;
        printf("n/a");
    }

    if (scan_result) {
        int new_field[ROWS][COLS];
        int speed = 300, control = 0;
        WINDOW* w = init_ncurses();
        draw_field(field, w);

        while ((control = wgetch(w)) != ' ') {
            if (control == 'a' || control == 'z') {
                speed = change_speed(speed, control);
            }
            calculate_new_matrix(field, new_field);
            draw_field(field, w);
            wrefresh(w);
            napms(speed);
        }

        delwin(w);
        endwin();
    } else {
        printf("n/a");
    }

    return 0;
}

WINDOW* init_ncurses() {
    initscr();  // TODO fix leaks
    WINDOW* win = newwin(ROWS + 2, COLS * 4 + 2, 0, 0);
    cbreak();
    noecho();
    nodelay(win, TRUE);

    return win;
}

int init_field(int field[ROWS][COLS]) {
    int is_ok = 1;
    int row = 0;

    for (int i = 0; i < (ROWS * COLS); i++) {
        if (i % 80 == 0) row++;
        int col = i % 80;
        if (scanf("%d", &field[col][row]) != 1) {
            is_ok = 0;
        }
    }

    return is_ok;
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

void draw_field(int field[ROWS][COLS], WINDOW* win) {
    clear();

    for (int i = 0; i < COLS + 2; i++) {
        for (int j = 0; j < ROWS + 2; j++) {
            if (i == 0 && j > 0) {
                mvwaddch(win, 0, j, (char)168);
            }
            if (i == FIELD_HEIGTH - 1 && j > 0) mvwaddch(win, i, j, (char)168);
            if ((j == 0) || j == FIELD_WIDTH - 1) mvwaddch(win, i, j, (char)168);
            if ((i > 0) && (i < COLS + 1) && (j > 0) && (j < ROWS + 1))
                mvwaddch(win, i, j, ((field[j - 1][i - 1])) == 0 ? ' ' : (char)168);
        }
    }
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
