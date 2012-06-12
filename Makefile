OPT := -lev -lz -g -I.
#OPT += -DDEBUG_CATALYST
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
	@./tests.out > /tmp/test_stdout 2> /tmp/test_stderr || (cat /tmp/test_stdout; cat /tmp/test_stderr; false)
	@cat /tmp/test_stderr
	@rm tests.out /tmp/test_stdout /tmp/test_stderr


test: $(TESTS)