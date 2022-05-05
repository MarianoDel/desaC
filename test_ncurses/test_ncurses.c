//----------------------------------------
// Tests for ncurses lib
//----------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

int main()
{
	/* Determne terminal type and it's features like size */
	initscr();
	/* Do not echo things such as user key presses back to the 
	   screen */
	noecho();
	/* Disable line buffering and instead make characters instantly 
	   available to the program */
	cbreak();
	/* Enable reading of keys like F1, F2 and arrow keys */
	keypad(stdscr, TRUE);


	mvprintw(20, 26, "A");
	refresh();

	napms(1000);
	refresh();

	mvprintw(20, 34, "B");
	refresh();

	napms(1000);
	refresh();

	mvprintw(20, 30, "C");
	refresh();


	getch();
	endwin();
}

//--- end of file ---//
