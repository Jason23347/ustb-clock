#include "config.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "tui/tui.h"

void
print_info() {
    printf("%s\n\n"
           "Copy 2021 Jason <jason23347@gmail.com>\n",
           PACKAGE_STRING);
}

int
main(int argc, const char *argv[]) {
    /* disable stderr */
    fclose(stderr);

    /* print copy info */
    print_info();
    sleep(1);

    tui_init();

    return 0;
}
