#include <curses.h>
#include <locale.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <term_windows.h>
#include <string.h>
#include <term_windows.h>

void setup_sig_handler();
void sig_handler(int signo);
void send_msg(msg_window *display_window, edit_window* edit_window, char* msg);

FILE *log_file;

int main(int argc, char* argv[]) {
    // Initialize ncurses and basic window setup.
    term_mode_init();

    // Note: We must set up signal handlers AFTER initializing curses
    setup_sig_handler();

    // Wrap stdscr in and ext window
    ext_window *std_scr = ext_window_create_from_existing(stdscr);

    // Create the edit window. This is where the user types the text they
    // intend to send
    edit_window* edit_window = edit_window_create(2, std_scr->ncols - 1, std_scr->nlines - 3, 0);
    whline(edit_window->window, 0, INT_MAX);
    wrefresh(edit_window->window);

    edit_window_set_row(edit_window, 1);

    // Create the display window. This is window that displays the text that
    // the user and whoever they were chatting with wrote
    msg_window *display_window;
    display_window = msg_window_create(std_scr->nlines - 3, std_scr->ncols - 1,0, 0);

    // Setup history file
    log_file = fopen("conv.log", "w");
    // Allocated a page for a memory buffer to store conversation history

    // TODO Implement read buffers
    // char *buffer_array[2], *cur_buffer, *receipt;
    // buffer_array[0] = malloc(PAGE_SIZE);
    // buffer_array[1] = malloc(PAGE_SIZE);

    int c, i = 0, line_ptr = 0;
    char *msg_buffer = malloc(256);
    for(;;) {
        c = wgetch(edit_window->window);

        switch(c) {
            // User hit an escape-sequence key. Find out what it is
            case ESC_SEQUENCE_START:
                if (getch() == 91) {
                    c = wgetch(edit_window->window);

                    switch(c) {
                        case KEY_UP_ARROW:
                            msg_window_scroll(display_window,-1);
                            break;
                        case KEY_DOWN_ARROW:
                            msg_window_scroll(display_window,-1);
                            break;
                        case KEY_RIGHT_ARROW:
                            break;
                        case KEY_LEFT_ARROW:
                            break;
                    }
                }
                break;
            case KEY_ALT_BACKSPACE:
            case KEY_BACKSPACE:
                // Move the cursor back one space from its determined location
                if (edit_window_backspace(edit_window) == 0) {
                    // Replace the corresponding character in the message buffer
                    // with a null character, then decrement the position pointer
                    *(msg_buffer + --i) = '\0';
                }
                break;
            // The user hit and enter key. Send the message in the message
            // buffer and update the display window
            case KEY_ALT_ENTER_1:
            case KEY_ALT_ENTER_2:
            case KEY_ENTER:
                // Append a null character to the message buffer
                *(msg_buffer + i) = '\0';
                send_msg(display_window, edit_window, (char*)msg_buffer);
                i = 0;
                break;
            // User is entering a regular, displayable character, so write it
            // to the edit window
            default:
                edit_window_putc(edit_window, c);
                *(msg_buffer + i++) = c;
                break;
        } 
    }

    return 0;
}

void sig_handler(int signo) {
    if (signo == SIGINT) {
        fclose(log_file);
        term_mode_reset();
        exit(1);
    }
}

void term_mode_init() {
    // Set our locale to be portable, since our ncurses instance will inherit
    // this
    setlocale(LC_ALL, "");

    // Initialqize curses in standard mode
    initscr(); // Intialize our screen; we need to do this before anything else
    cbreak(); // Disable line-buffering (and buffering in general)
    noecho(); // Disable echoing of characters read into stdin
    nonl(); // Do not automatically read a return key press as a newline0
    // Disable flushing of the tty output driver when an interrupt key is 
    // pressed (preventing curses from losing its place)
    intrflush(stdscr, FALSE);
    // Use single values for function keys
    // (making it easier to read key inputs)
    keypad(stdscr, TRUE);
}

void setup_sig_handler() {
    struct sigaction sig_action;
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);

    sig_action.sa_handler = sig_handler;
    sig_action.sa_mask = mask;
    sig_action.sa_flags = 0;

    sigaction(SIGINT, &sig_action, NULL);

    ext_window *std_scr = ext_window_create_from_existing(stdscr);
}

void send_msg(msg_window *display_window, edit_window *edit_window, char* msg) {
    if (display_window->print_curs->cur_col > 0) {
        if (display_window->print_curs->cur_line >= display_window->nlines - 1) {
            msg_window_scroll(
                display_window,
                display_window->print_curs->cur_line - (display_window->nlines - 1)
            );
        }

        msg_window_move_v(display_window, 1);
    }

    // Reset the display window cursor to the beggining of the line
    // so the next message (when it is printed) is properly
    // left-justified
    msg_window_set_col(display_window, 0);
    msg_window_puts(display_window, msg);

    edit_window_set_col(edit_window, 0);
    edit_window_set_row(edit_window, 1);
    edit_window_clrln(edit_window);
}

void term_mode_reset() {
    // Reset all terminal input and output options
    nocbreak();
    echo();
    nl();

    // end curses
    endwin();
    printf("Resetting Terminal\n");
}