OPT := -DDEBUG_CATALYST -lev -lz -g -I.
CC := gcc 
TESTS := $(shell echo test/*.test.c)
SRC := spdy_frame.c spdy_headers.c spdy_session.c
MAIN := catalyst.c

catalyst: $(SRC) $(MAIN)
	$(CC) -o catalyst $+ $(OPT)

all: catalyst

.PHONY: test $(TESTS)

$(TESTS):
	$(CC) -o tests.out $@ $(SRC) $(OPT)
	@./tests.out > test_stdout 2> test_stderr || cat test_stdout
	@cat test_stderr
	@rm tests.out test_stdout test_stderr

test: $(TESTS)