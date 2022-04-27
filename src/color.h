#ifndef COLOR_H
#define COLOR_H

#include "config.h"

/* 默认颜色 */
#define NORMAL 0
#define BG_NORMAL ;NORMAL

/* 网上抄的，懒得找官方的颜色命名 */
#define BLACK     30
#define RED       31
#define GREEN     32
#define YELLOW    33
#define BLUE      34
#define PURPLE    35
#define DARKGREEN 36
#define WHITE     37

#define BG_BLACK     40
#define BG_RED       41
#define BG_GREEN     42
#define BG_YELLOW    43
#define BG_BLUE      44
#define BG_PURPLE    45
#define BG_DARKGREEN 46
#define BG_WHITE     47

#define STR(text)     #text
#define color(code)   "\033[" STR(code) "m"

#ifdef COLORFUL_OUTPUT
#define set_color(c)  printf(color(%d), c)
#define reset_color() printf(color(NORMAL;NORMAL))
#else
#define set_color(...)
#define reset_color(...)
#endif /* COLORFUL_OUTPUT */

const char *colorname(int color);

#endif /* COLOR_H */
