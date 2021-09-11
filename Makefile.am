AUTOMAKE_OPTIONS=foreign

bin_PROGRAMS=ustb-clock

ustb_clock_SOURCES = \
    src/calc.c \
    src/net/http.c \
    src/net/tcp.c \
    src/net/socket.c \
    src/tui/clock.c \
    src/tui/draw.c \
    src/tui/info.c \
    src/tui/tui.c \
    src/main.c

AM_CFLAGS = $(PTHREAD_CFLAGS) \
	-Wall \
    -Isrc \
    -Isrc/calc \
    -Isrc/net \
    -Isrc/tui \
    -Iinclude \
    -Llib
