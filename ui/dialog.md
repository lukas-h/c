# dialog-widget
yes-no dialog widget for the (unix) console, **without ncurses**.
  
# getting started
example:
```C
	// Create dialog
	dialog_t dialog;
	dialog_new(&dialog, "Title", "Subtitle");

	// Set text (again)
	dialog_set_title(&dialog, "Main Title");
	dialog_set_subtitle(&dialog, "Subtitle");

	// Set labels
	dialog_set_yes(&dialog, "Accept");
	dialog_set_no(&dialog, "Disallow");
	
	// display dialog
	int ret = dialog_show(&dialog);
	dialog_destroy(&dialog);
```

- with function `void dialog_set_colors(dialog_t *dialog, char *active, char *inactive)`, you can set fore- and background color, `active` and `inactive` require color codes, defined in `dialog.h`  

- other functions described by the example


##### Questions?
Open an issue!
