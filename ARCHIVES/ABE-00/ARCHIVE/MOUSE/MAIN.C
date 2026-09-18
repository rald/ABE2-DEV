#include <stdio.h>
#include <conio.h> /* For gotoxy() and clrscr() in DJGPP */

/* 
 * Define MOUSE_IMPLEMENTATION in EXACTLY one C file 
 * before including the header to create the definitions.
 */
#define MOUSE_IMPLEMENTATION
#include "mouse.h"

int main(void) {
    mouse_state_t mouse;

    /* 1. Initialize the mouse driver */
    if (!mouse_init()) {
        printf("Error: No DOS mouse driver found!\n");
        return 1;
    }

    clrscr();
    printf("DOS Mouse Library Initialized.\n");
    printf("Move the mouse around. Right-click to exit.\n");
    
    /* 2. Show the mouse cursor */
    mouse_show();

    /* 3. Main loop */
    while (1) {
        /* Update the mouse state struct */
        mouse_update(&mouse);

        /* 
         * Move cursor to row 4, col 1 so we overwrite the same line 
         * instead of scrolling the screen endlessly.
         */
        gotoxy(1, 4);
        printf("X: %-4d | Y: %-4d | Left: %d | Middle: %d | Right: %d   ", 
               mouse.x, 
               mouse.y,
               (mouse.buttons & MOUSE_LEFT) ? 1 : 0,
               (mouse.buttons & MOUSE_MIDDLE) ? 1 : 0,
               (mouse.buttons & MOUSE_RIGHT) ? 1 : 0);
    }

    /* 4. Hide the mouse before returning to the DOS prompt to prevent visual glitches */
    mouse_hide();
    
    gotoxy(1, 6);
    printf("Exiting... Goodbye!\n");

    return 0;
}
