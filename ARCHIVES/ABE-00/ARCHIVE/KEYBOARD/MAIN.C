#define KEYBOARD_IMPLEMENTATION
#include "keyboard.h"

int main() {
    if (keyboard_init() != 0) {
        printf("Error: Failed to allocate DPMI IRET wrapper.\n");
        return 1;
    }

    printf("Custom keyboard handler installed. Press ESC to quit.\n");
    printf("Monitoring UP, DOWN, LEFT, RIGHT, Z, and X...\n\n");
    
    while (!keystate[KEY_ESC]) {
        printf("Active Keys: ");
        
        if (keystate[KEY_UP])    printf("[UP] ");
        if (keystate[KEY_DOWN])  printf("[DOWN] ");
        if (keystate[KEY_LEFT])  printf("[LEFT] ");
        if (keystate[KEY_RIGHT]) printf("[RIGHT] ");
        if (keystate[KEY_Z])     printf("[Z] ");
        if (keystate[KEY_X])     printf("[X] ");
        
        // Pad with spaces to clear trailing characters from previous longer lines, then return to start
        printf("                                                   \r");
        
        fflush(stdout);
    }

    keyboard_cleanup();
    printf("\nHandler successfully removed. Exiting.\n");
    return 0;
}
