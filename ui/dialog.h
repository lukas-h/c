/*
 * dialog.h
 * 
 * Copyright 2016 Lukas Himsel <lukas@himsel.me>
 * 
 * license: MIT
 * 
 */

#ifndef CONSOLE_WIDGET_DIALOG_H
#define CONSOLE_WIDGET_DIALOG_H

#include <stdio.h>
#include <stdbool.h>

typedef struct dialog_t
{

    /* titles */
    char title[64];
    char subtitle[64];

    /* highlight */
    char active[16];
    char inactive[16];

    /* items*/
    char yes[32];
    char no[32];

    /* options */
    bool show_border;
    bool list_wide;

} dialog_t;

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

extern void dialog_new(dialog_t *dialog, char *title, char *subtitle);

extern void dialog_set_yes(dialog_t *dialog, char *msg);

extern void dialog_set_no(dialog_t *dialog, char *msg);

extern void dialog_set_title(dialog_t *dialog, char *title);

extern void dialog_set_subtitle(dialog_t *dialog, char *subtitle);

extern void dialog_set_colors(dialog_t *dialog, char *active, char *inactive);

extern int dialog_show(dialog_t *dialog);

extern void dialog_destroy(dialog_t *dialog);

#endif
