AUTOMAKE_OPTIONS=foreign

bin_PROGRAMS=ustb-clock

ustb_clock_SOURCES = \
    src/color.c \
    src/flow.c \
    src/main.c \
    src/net/http.c \
    src/net/tcp.c \
    src/net/socket.c \
    src/sched/task.c \
    src/sched/watcher.c \
    src/tui/calc.c \
    src/tui/date.c \
    src/tui/digits.c \
    src/tui/draw.c \
    src/tui/info.c \
    src/tui/tui.c

AM_CFLAGS = $(PTHREAD_CFLAGS) \
	-Wall \
    -Isrc