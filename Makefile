all: catalyst

catalyst: catalyst.c spdy.c
	gcc  $+   -o catalyst -lev