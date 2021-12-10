#include "color.h"

const char *
colorname(int color) {
    switch (color) {
        /* Text colors */
    case BLACK:
        return "BLACK";
    case RED:
        return "RED";
    case GREEN:
        return "GREEN";
    case YELLOW:
        return "YELLOW";
    case BLUE:
        return "BLUE";
    case PURPLE:
        return "PURPLE";
    case DARKGREEN:
        return "DARKGREEN";
    case WHITE:
        return "WHITE";

        /* BG colors */
    case BG_BLACK:
        return "BG_BLACK";
    case BG_RED:
        return "BG_RED";
    case BG_GREEN:
        return "BG_GREEN";
    case BG_YELLOW:
        return "BG_YELLOW";
    case BG_BLUE:
        return "BG_BLUE";
    case BG_PURPLE:
        return "BG_PURPLE";
    case BG_DARKGREEN:
        return "BG_DARKGREEN";
    case BG_WHITE:
        return "BG_WHITE";
    default:
        return "UNKNOWN";
    }
}
