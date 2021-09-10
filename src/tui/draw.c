#include "conf.h"

#include "draw.h"
#include <string.h>

void
draw_line(struct offset offset, const char *prompt, const char *value,
          int length) {
    gotoxy(offset.left, offset.top);
    printf("%s", prompt);
    goright(length);
    printf("%s", value);
}
