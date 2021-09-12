#ifndef OFFSET_H
#define OFFSET_H

struct offset
{
    unsigned left;
    unsigned top;
};

#define gotopos(offset)             gotoxy(offset.left + 1, offset.top + 1)
#define setpos(offset, x, y)        { offset.left = (x); offset.top = (y); }
#define transpos(offset, x, y)      { offset.left += (x); offset.top += (y); }
#define next_line(offset)           transpos(offset, 0, 1)

#endif /* OFFSET_H */