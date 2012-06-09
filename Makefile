OPT := -DDEBUG_CATALYST -lev -lz -g
CC := gcc 
TESTS := test_frames.c
SRC := spdy_frame.c spdy_headers.c
MAIN := catalyst.c

catalyst: $(SRC) $(MAIN)
	$(CC) -o catalyst $+ $(OPT)

all: catalyst

test:
	$(CC) -o tests.out $(TESTS) $(SRC) $(OPT)
	./tests.out