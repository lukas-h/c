/*
 * dialog.c
 * 
 * Copyright 2016 Lukas Himsel <lukas@himsel.me>
 * 
 * license: MIT
 * 
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdbool.h>

#include "dialog.h"

static struct termios _term;
static int getch()
{
    int c = '\0';
    struct termios term, ttmp;

    tcgetattr(STDIN_FILENO, &term);
    tcgetattr(STDIN_FILENO, &_term);

    ttmp = term;

    ttmp.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &ttmp);

    c = getchar();

#define KEY_UP -1
#define KEY_DOWN -2
#define KEY_RIGHT -3
#define KEY_LEFT -4

    tcsetattr(STDIN_FILENO, TCSANOW, &term);

    if (c == 033)
    {
        c = getch();
        if (c == 91)
        {
            c = getch();
            switch (c)
            {
            case 65:
                return KEY_UP;
                break;
            case 66:
                return KEY_DOWN;
                break;
            case 67:
                return KEY_RIGHT;
                break;
            case 68:
                return KEY_LEFT;
                break;
            }
        }
        else
        {
            return c;
        }
    }

    return c;
}
void dialog_new(dialog_t *dialog, char *title, char *subtitle)
{
    dialog->show_border = true;
    dialog->list_wide = false;
    strcpy(dialog->title, title);
    strcpy(dialog->subtitle, subtitle);
    strcpy(dialog->yes, "OKAY");
    strcpy(dialog->no, "NO");
    strcpy(dialog->active, STYLE_B_BLUE);
    strcpy(dialog->inactive, STYLE_BLUE);
}
void dialog_set_title(dialog_t *dialog, char *title)
{
    strcpy(dialog->title, title);
}
void dialog_set_subtitle(dialog_t *dialog, char *subtitle)
{
    strcpy(dialog->subtitle, subtitle);
}
void dialog_set_yes(dialog_t *dialog, char *msg)
{
    strcpy(dialog->yes, msg);
}

void dialog_set_no(dialog_t *dialog, char *msg)
{
    strcpy(dialog->no, msg);
}
void dialog_set_colors(dialog_t *dialog, char *active, char *inactive)
{
    strcpy(dialog->active, active);
    strcpy(dialog->inactive, inactive);
}

static void list_border(bool list_wide)
{
    putchar('~');
    int loops = list_wide ? 68 : 30;
    while (loops--)
    {
        putchar('-');
    }
    putchar('~');
    putchar('\n');
}
int dialog_show(dialog_t *dialog)
{
    int x = 1, k;

    while (1)
    {
        printf("\033c%s%s%s\n\n%s\n", dialog->inactive, dialog->title, STYLE_NONE, dialog->subtitle);

        if (dialog->show_border)
        {
            list_border(dialog->list_wide);
        }

        if (x == 1)
        {
            printf("  %s%s\033[0m\t%s%s\n", dialog->active, dialog->no, dialog->inactive, dialog->yes);
        }
        else
        {
            printf("  %s%s\033[0m\t%s%s\n", dialog->inactive, dialog->no, dialog->active, dialog->yes);
        }
        printf("\033[0m");

        if (dialog->show_border)
        {
            list_border(dialog->list_wide);
        }

        k = getch();
        switch (k)
        {
        case '\n':
        case ' ':
            return x;
            break;
        case KEY_UP:
        case KEY_DOWN:
        case KEY_LEFT:
        case KEY_RIGHT:
        case '\t':
            if (x == 1)
            {
                x = 2;
            }
            else
            {
                x = 1;
            }
            break;
        default:
            continue;
            break;
        }
    }
}
void dialog_destroy(dialog_t *dialog)
{
    /* nothing */
}

#ifdef DEBUG
int main()
{
    // Create dialog
    dialog_t dialog;
    dialog_new(&dialog, "Title", "Subtitle");

	// Set text
    dialog_set_title(&dialog, "Main Title");
    dialog_set_subtitle(&dialog, "Subtitle");

	// Set labels
    dialog_set_yes(&dialog, "Accept");
    dialog_set_no(&dialog, "Disallow");
	
	// display dialog
    int ret = dialog_show(&dialog);
    dialog_destroy(&dialog);
    return 0;
}
#endif
