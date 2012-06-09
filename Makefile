OPT := -DDEBUG_CATALYST -lev -lz -g
CC := gcc 
TESTS := $(shell echo *.test.c)
SRC := spdy_frame.c spdy_headers.c
MAIN := catalyst.c

catalyst: $(SRC) $(MAIN)
	$(CC) -o catalyst $+ $(OPT)

all: catalyst

.PHONY: test $(TESTS)

$(TESTS):
	$(CC) -o tests.out $@ $(SRC) $(OPT)
	./tests.out
	rm tests.out

test: $(TESTS)