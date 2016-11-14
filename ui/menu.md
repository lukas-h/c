# menu-widget
menu widget for the (unix) console, **without ncurses**.
  
# getting started
example:
```C
	// Create menu
	menu_t menu;
	menu_new(&menu, "Title", "Subtitle");
	menu.show_numbers=true;
	
	// Set text (again)
	menu_set_title(&menu, "Main title");
	menu_set_subtitle(&menu, "Subtitle");

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

## additional

- with function `void menu_set_colors(menu_t *menu, char *active, char *inactive)`, you can set fore- and background color, `active` and `inactive` require color codes, defined in `menu.h`  
- with `void menu_set_arrow_symb(menu_t *menu, char arrow_symb)`, you set an arrow symbol, pointing to the currently selected value in menu. It's a single `char`.  
- there is a boolean option `show_numbers` in `menu_t`, that can be activated to display numbers in front of every list item.  
- other functions described by the example


##### Questions?
Open an issue!
