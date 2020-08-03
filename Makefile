CC = gcc -ggdb

OBJS_FLAGS = -Iinclude -ggdb -lncurses -c
BIN_FLAGS = -Iinclude -ggdb -lncurses

.PHONY: debug release clean test

debug:
	echo "To be implemented"

release:
	echo "To be implemented"

clean:
	rm objs/* bin/*

test: bin/chat_window_test

bin/chat_window_test: objs/chat_window_test.o objs/term_windows.o include/term_windows.h
	$(CC) $(BIN_FLAGS) objs/chat_window_test.o objs/term_windows.o -o bin/chat_window_test

objs/term_windows.o: src/term_windows.c include/term_windows.h
	$(CC) $(OBJS_FLAGS) src/term_windows.c -o objs/term_windows.o

objs/chat_window_test.o: tests/chat_window_test.c src/term_windows.c include/term_windows.h
	$(CC) $(OBJS_FLAGS) tests/chat_window_test.c -o objs/chat_window_test.o


