#include "config.h"

#include <stdio.h>
#include <string.h>

#include "tui.h"

void
print_help() {
    fprintf(stderr,
            "%s\n\n"
            "Copy 2021 Jason <jason23347@gmail.com>\n",
            PACKAGE_STRING);
}

int
main(int argc, const char *argv[]) {
    if (!strcmp(argv[0], "--help")) {
        print_help();
        return 0;
    }

    tui_t tuis[1], *tui = &tuis[0];

    tui_init(tui);

    return 0;
}
