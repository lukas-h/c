# menu-widget
menu widget for the (unix) console.
It is only a bit of code, nothing very good or professional but **without ncurses**.
  
# getting started
this code:
```C
	// Create menu
	menu_t menu;
	menu_new(&menu, "Title", "Subtitle");
	menu.show_numbers=true;
	
	// Add items
	menu_add_item(&menu, "item 1");
	menu_add_item(&menu, "item 2");
	menu_add_item(&menu, "item 3");
	menu_add_item(&menu, "item 4");
	menu_add_item(&menu, "item 5");
	menu_add_item(&menu, "item 6");
	
	// Show menu
	int ret = menu_show(&menu);
	menu_destroy(&menu);
```
produces this:
![](http://file.himsel.me/img/console-menu.png)

## reference
`menu_new(menu_t *menu, char *title, char *subtitle)` creates a new menu. You can activate/deactivate many options, like `show_border`, that activates
the two lines over and under the items list. It is a `bool` (and can be set to `true` or `false`).
Another option is `arrow-symb`, a `char`. It is the symbol at the begin of the line of a selected menu item.  
  
You can set the design; call the function `menu_set_colors(menu_t *menu, char *active, char *inactive);`
arguments:  
1. the context  
2. the color-sequence for the selected item  
3. the colorset for the not-selected items  
  
For viewing the menu, you need the function `menu_show(menu_t *menu)`.
Note: 
You should always call the function `menu_destroy(menu_t *menu)` to free the memory.  
To see all predefined functions, look into `menu.h`

## use in your projects
You need a C99 compatible compiler, preferred ones are *gcc* and *clang*. Easily add the files `menu.c` and `menu.h`
to your project and use the predefined menu API. It is incompatible with Microsoft Windows, but it can be compiled with cygwin or something similar

## license
MIT