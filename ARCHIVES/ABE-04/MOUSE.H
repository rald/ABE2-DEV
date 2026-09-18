#ifndef MOUSE_H
#define MOUSE_H

#include <dos.h>

#define MOUSE_LEFT   1
#define MOUSE_RIGHT  2
#define MOUSE_MIDDLE 4

typedef struct {
    int x;
    int y;
    int buttons;
} mouse_state_t;

/* Initializes the mouse driver. Returns 1 if successful, 0 if not found. */
int mouse_init(void);

/* Displays the mouse cursor on screen. */
void mouse_show(void);

/* Hides the mouse cursor (must be called before drawing to the screen to avoid glitching). */
void mouse_hide(void);

/* Updates the state struct with the current X/Y coordinates and button bitmask. */
void mouse_update(mouse_state_t *state);

/* Force the mouse cursor to a specific X, Y coordinate. */
void mouse_set_pos(int x, int y);

/* Restricts the mouse cursor's horizontal movement. */
void mouse_set_x_range(int min_x, int max_x);

/* Restricts the mouse cursor's vertical movement. */
void mouse_set_y_range(int min_y, int max_y);

#ifdef MOUSE_IMPLEMENTATION

int mouse_init(void) {
    union REGS r;
    r.x.ax = 0x0000;
    int86(0x33, &r, &r);
    return (r.x.ax == 0xFFFF);
}

void mouse_show(void) {
    union REGS r;
    r.x.ax = 0x0001;
    int86(0x33, &r, &r);
}

void mouse_hide(void) {
    union REGS r;
    r.x.ax = 0x0002;
    int86(0x33, &r, &r);
}

void mouse_update(mouse_state_t *state) {
    union REGS r;
    r.x.ax = 0x0003;
    int86(0x33, &r, &r);
    
    if (state) {
        state->x = r.x.cx;
        state->y = r.x.dx;
        state->buttons = r.x.bx;
    }
}

void mouse_set_pos(int x, int y) {
    union REGS r;
    r.x.ax = 0x0004;
    r.x.cx = x;
    r.x.dx = y;
    int86(0x33, &r, &r);
}

void mouse_set_x_range(int min_x, int max_x) {
    union REGS r;
    r.x.ax = 0x0007;
    r.x.cx = min_x;
    r.x.dx = max_x;
    int86(0x33, &r, &r);
}

void mouse_set_y_range(int min_y, int max_y) {
    union REGS r;
    r.x.ax = 0x0008;
    r.x.cx = min_y;
    r.x.dx = max_y;
    int86(0x33, &r, &r);
}

#endif /* MOUSE_IMPLEMENTATION */

#endif /* MOUSE_H */
