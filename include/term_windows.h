#ifndef CURSES_TEST_H
#define CURSES_TEST_H
#include <stdint.h>
#include <curses.h>

#define KEY_ALT_BACKSPACE 127
#define KEY_ALT_ENTER_1 10
#define KEY_ALT_ENTER_2 13
#define ESC_SEQUENCE_START 27
#define KEY_UP_ARROW 65
#define KEY_DOWN_ARROW 66
#define KEY_RIGHT_ARROW 67
#define KEY_LEFT_ARROW 68
#define PAGE_SIZE 4096

typedef struct cursor {
    uint8_t cur_line;
    uint8_t cur_col;
} cursor;

typedef struct ext_window {
    WINDOW *window;

    uint16_t nlines;
    uint16_t ncols;
} ext_window;

typedef struct msg_window {
    WINDOW *window;

    uint16_t nlines;
    uint16_t ncols;

    cursor *read_curs;
    cursor *print_curs;
} msg_window;

typedef struct edit_window {
    WINDOW *window;
    
    uint8_t nlines;
    uint8_t ncols;

    cursor *print_curs;
} edit_window;

void term_mode_init();
void term_mode_reset();

ext_window *ext_window_create(uint16_t nlines, uint16_t ncols, uint16_t start_row, uint16_t start_col);
ext_window *ext_window_create_from_existing(WINDOW *win);

msg_window *msg_window_create(uint16_t nlines, uint16_t ncols, uint16_t start_row, uint16_t start_col);
msg_window *msg_window_create_from_existing(WINDOW *win);

edit_window *edit_window_create(uint16_t nlines, uint16_t ncols, uint16_t start_row, uint16_t start_col);
edit_window *edit_window_create_from_existing(WINDOW *win);

/**
 * Returns the actual number of lines moved
 */
int64_t msg_window_move_v(msg_window *win, int64_t nlines);

/**
 * Returns the actual number of columns moved
 */
int32_t msg_window_move_h(msg_window *win, int32_t ncols);

/**
 * Returns the actual number of lines moved
 */
int64_t edit_window_move_v(edit_window *win, int64_t nlines);

/**
 * Returns the actual number of columns moved
 */
int32_t edit_window_move_h(edit_window *win, int32_t ncols);

int8_t msg_window_set_row(msg_window *win, uint16_t line_num);
int8_t msg_window_set_col(msg_window *win, uint16_t col_num);

int8_t edit_window_set_row(edit_window *win, uint16_t line_num);
int8_t edit_window_set_col(edit_window *win, uint16_t col_num);

int8_t msg_window_scroll(msg_window *win, uint64_t nlines);

int8_t edit_window_backspace(edit_window *win);

int8_t msg_window_puts(msg_window *win, char *str);

int8_t edit_window_clrln(edit_window *win);

int8_t edit_window_putc(edit_window *win, char c);

int32_t edit_window_move_h(edit_window *win, int32_t ncols);

#endif