#include "color.h"

const char *
colorname(int color) {
    switch (color) {
        /* Text colors */
    case 30:
        return "RED";
    case 31:
        return "GREEN";
    case 32:
        return "YELLOW";
    case 33:
        return "BLUE";
    case 34:
        return "PURPLE";
    case 35:
        return "YELLOW";
    case 36:
        return "DARKGREEN";
    case 37:
        return "WHITE";

        /* BG colors */
    case 40:
        return "BG_BLACK";
    case 41:
        return "BG_RED";
    case 42:
        return "BG_GREEN";
    case 43:
        return "BG_YELLOW";
    case 44:
        return "BG_BLUE";
    case 45:
        return "BG_PURPLE";
    case 46:
        return "BG_DARKGREEN";
    case 47:
        return "BG_WHITE";
    default:
        return "UNKNOWN";
    }
}
