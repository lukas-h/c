all: menu dialog

menu: menu.o
	cc  -c menu.c -D DEBUG
	cc -o menu menu.o
dialog: dialog.o
	cc -c dialog.c -D DEBUG
	cc -o dialog dialog.o
.PHONY: menu dialog
