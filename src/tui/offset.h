#ifndef OFFSET_H
#define OFFSET_H

#ifndef DRAW_H
#error "Never use offset.h directly; include draw.h instead."
#endif

typedef struct {
    unsigned left;
    unsigned top;
} offset_t;

#define gotopos(offset) gotoxy(offset.left + 1, offset.top + 1)
#define setpos(offset, x, y)                                                   \
    {                                                                          \
        offset.left = (x);                                                     \
        offset.top = (y);                                                      \
    }
#define transpos(offset, x, y)                                                 \
    {                                                                          \
        offset.left += (x);                                                    \
        offset.top += (y);                                                     \
    }
#define next_line(offset) transpos(offset, 0, 1)

#endif /* OFFSET_H */