all: catalyst

OPT := -DDEBUG_CATALYST

catalyst: catalyst.c spdy_frame.c spdy_headers.c
	gcc  $+   -o catalyst -lev -lz -g $(OPT)
