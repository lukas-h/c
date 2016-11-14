/*
 * menu.c
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

#include "menu.h"

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
void menu_new(menu_t *menu, char *title, char *subtitle)
{
    menu->itemc = 0;
    menu->itemv = (char **)malloc(sizeof(char *));
    menu->arrow_symb = ' ';
    menu->show_border = true;
    menu->list_wide = false;
    menu->keys_active = false;
    menu->show_numbers = false;
    strcpy(menu->title, title);
    strcpy(menu->subtitle, subtitle);
    strcpy(menu->active, STYLE_B_BLUE);
    strcpy(menu->inactive, STYLE_BLUE);
}
void menu_add_item(menu_t *menu, char *item)
{
    if (strlen(item) + 6 > 30)
    {
	menu->list_wide = true;
    }
    menu->itemc++;
    menu->itemv = (char **)realloc(menu->itemv, sizeof(char *) * menu->itemc);
    menu->itemv[menu->itemc - 1] = (char *)malloc(strlen(item) + 1);
    strcpy(menu->itemv[menu->itemc - 1], item);
}
void menu_set_title(menu_t *menu, char *title)
{
    strcpy(menu->title, title);
}
void menu_set_subtitle(menu_t *menu, char *subtitle)
{
    strcpy(menu->subtitle, subtitle);
}
void menu_set_colors(menu_t *menu, char *active, char *inactive)
{
    strcpy(menu->active, active);
    strcpy(menu->inactive, inactive);
}
void menu_set_arrow_symb(menu_t *menu, char arrow_symb)
{
    menu->arrow_symb = arrow_symb;
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

int menu_show(menu_t *menu)
{
    int x = 1, k;
    size_t i;

    if (menu->itemc < 9)
    {
	menu->keys_active = true;
    }

    while (1)
    {
	printf("\033c%s%s%s\n\n%s\n", menu->inactive, menu->title, STYLE_NONE, menu->subtitle);

	if (menu->show_border)
	{
	    list_border(menu->list_wide);
	}

	for (i = 0; i < menu->itemc; i++)
	{
	    if (x == i + 1)
	    {
		putchar(' ');
		printf(menu->active);
		if (menu->show_numbers)
		{
		    printf("%c%u.\t%s", menu->arrow_symb, (unsigned)i + 1, menu->itemv[i]);
		}
		else
		{
		    printf("%c%s", menu->arrow_symb, menu->itemv[i]);
		}
	    }
	    else
	    {
		printf("  ");
		printf(menu->inactive);
		if (menu->show_numbers)
		{
		    printf("%u.\t%s", (unsigned)i + 1, menu->itemv[i]);
		}
		else
		{
		    printf("%s", menu->itemv[i]);
		}
	    }
	    printf("\033[0m\n");
	}

	if (menu->show_border)
	{
	    list_border(menu->list_wide);
	}

	k = getch();
	switch (k)
	{
	case '\n':
	case ' ':
	    return x;
	    break;
	case KEY_UP:
	    if (x == 1)
	    {
		x = menu->itemc;
	    }
	    else
	    {
		x--;
	    }
	    break;
	case KEY_DOWN:
	    if (x == menu->itemc)
	    {
		x = 1;
	    }
	    else
	    {
		x++;
	    }
	    break;
	case '1' ... '9':
	    k -= '0';
	    if (menu->keys_active == true)
	    {
		x = k <= menu->itemc ? k : x;
	    }
	    break;
	default:
	    continue;
	    break;
	}
    }
}

void menu_destroy(menu_t *menu)
{
    free(menu->itemv);
    menu->itemv = NULL;
}

#ifdef DEBUG
int main()
{
    menu_t menu;
    menu_new(&menu, "Title", "Subtitle");
    menu.show_numbers = true;
    menu_add_item(&menu, "item 1");
    menu_add_item(&menu, "item 2");
    menu_add_item(&menu, "item 3");
    menu_add_item(&menu, "item 4");
    menu_add_item(&menu, "item 5");
    menu_add_item(&menu, "item 6");
    int ret = menu_show(&menu);
    menu_destroy(&menu);
    return 0;
}
#endif