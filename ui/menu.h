/*
 * menu.h
 * 
 * Copyright 2016 Lukas Himsel <lukas@himsel.me>
 * 
 * license: MIT
 * 
 */

#ifndef CONSOLE_WIDGET_MENU_H
#define CONSOLE_WIDGET_MENU_H

#include <stdio.h>
#include <stdbool.h>

typedef struct menu_t
{

    /* titles */
    char title[64];
    char subtitle[64];

    /* highlight */
    char active[16];
    char inactive[16];

    /* items*/
    size_t itemc;
    char **itemv;

    /* options */
    bool show_border;
    bool list_wide;
    bool keys_active;
    bool show_numbers;
    char arrow_symb;

} menu_t;

#define STYLE_NONE "\033[0m\0"
#define STYLE_RED "\033[1;31m\0"
#define STYLE_GREEN "\033[1;32m\0"
#define STYLE_YELLOW "\033[1;33m\0"
#define STYLE_BLUE "\033[1;34m\0"
#define STYLE_PURPLE "\033[1;35m\0"
#define STYLE_CYAN "\033[1;36m\0"
#define STYLE_WHITE "\033[1;37m\0"

#define STYLE_B_BLACK "\033[1;37;40m\0"
#define STYLE_B_RED "\033[1;37;41m\0"
#define STYLE_B_GREEN "\033[1;37;42m\0"
#define STYLE_B_YELLOW "\033[1;37;43m\0"
#define STYLE_B_BLUE "\033[1;37;44m\0"
#define STYLE_B_PURPLE "\033[1;37;45m\0"
#define STYLE_B_CYAN "\033[1;37;46m\0"
#define STYLE_B_WHITE "\033[1;30;47m\0"

extern void menu_new(menu_t *menu, char *title, char *subtitle);

extern void menu_add_item(menu_t *menu, char *item);

extern void menu_set_title(menu_t *menu, char *title);

extern void menu_set_subtitle(menu_t *menu, char *subtitle);

extern void menu_set_colors(menu_t *menu, char *active, char *inactive);

extern void menu_set_arrow_symb(menu_t *menu, char arrow_symb);

extern int menu_show(menu_t *menu);

extern void menu_destroy(menu_t *menu);

#endif