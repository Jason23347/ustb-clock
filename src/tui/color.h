#ifndef COLOR_H
#define COLOR_H

#define NORMAL          0

#define BLACK           30
#define RED             31
#define GREEN           32
#define YELLOW          33
#define BLUE            34
#define PURPLE          35
#define DARKGREEN       36
#define WHITE           37

#define BG_BLACK        40
#define BG_RED          41
#define BG_GREEN        42
#define BG_YELLOW       43
#define BG_BLUE         44
#define BG_PURPLE       45
#define BG_DARKGREEN    46
#define BG_WHITE        47

#define STR(text)       #text
#define color(code)     "\033[" STR(code) "m"
#define reset_color()   printf(color(NORMAL))

#endif /* COLOR_H */