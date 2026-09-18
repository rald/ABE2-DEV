#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdio.h>  
#include <stdlib.h> 
#include <dpmi.h>   
#include <go32.h>   
#include <pc.h>     
#include <conio.h>  

enum KEY_CODES {
    KEY_ESC         = 1,
    KEY_1           = 2,
    KEY_2           = 3,
    KEY_3           = 4,
    KEY_4           = 5,
    KEY_5           = 6,
    KEY_6           = 7,
    KEY_7           = 8,
    KEY_8           = 9,
    KEY_9           = 10,
    KEY_0           = 11,
    KEY_MINUS       = 12,
    KEY_EQUALS      = 13,
    KEY_BACKSPACE   = 14,
    KEY_TAB         = 15,
    KEY_Q           = 16,
    KEY_W           = 17,
    KEY_E           = 18,
    KEY_R           = 19,
    KEY_T           = 20,
    KEY_Y           = 21,
    KEY_U           = 22,
    KEY_I           = 23,
    KEY_O           = 24,
    KEY_P           = 25,
    KEY_LBRACKET    = 26,
    KEY_RBRACKET    = 27,
    KEY_ENTER       = 28,
    KEY_LCTRL       = 29,
    KEY_A           = 30,
    KEY_S           = 31,
    KEY_D           = 32,
    KEY_F           = 33,
    KEY_G           = 34,
    KEY_H           = 35,
    KEY_J           = 36,
    KEY_K           = 37,
    KEY_L           = 38,
    KEY_SEMICOLON   = 39,
    KEY_APOSTROPHE  = 40,
    KEY_TILDE       = 41,
    KEY_LSHIFT      = 42,
    KEY_BACKSLASH   = 43,
    KEY_Z           = 44,
    KEY_X           = 45,
    KEY_C           = 46,
    KEY_V           = 47,
    KEY_B           = 48,
    KEY_N           = 49,
    KEY_M           = 50,
    KEY_COMMA       = 51,
    KEY_PERIOD      = 52,
    KEY_SLASH       = 53,
    KEY_RSHIFT      = 54,
    KEY_NUMPAD_STAR = 55,
    KEY_LALT        = 56,
    KEY_SPACE       = 57,
    KEY_CAPSLOCK    = 58,
    KEY_F1          = 59,
    KEY_F2          = 60,
    KEY_F3          = 61,
    KEY_F4          = 62,
    KEY_F5          = 63,
    KEY_F6          = 64,
    KEY_F7          = 65,
    KEY_F8          = 66,
    KEY_F9          = 67,
    KEY_F10         = 68,
    KEY_NUMLOCK     = 69,
    KEY_SCROLLLOCK  = 70,
    KEY_HOME        = 71,
    KEY_UP          = 72,
    KEY_PAGEUP      = 73,
    KEY_NUMPAD_MINUS= 74,
    KEY_LEFT        = 75,
    KEY_NUMPAD_5    = 76,
    KEY_RIGHT       = 77,
    KEY_NUMPAD_PLUS = 78,
    KEY_END         = 79,
    KEY_DOWN        = 80,
    KEY_PAGEDOWN    = 81,
    KEY_INSERT      = 82,
    KEY_DELETE      = 83,
    KEY_F11         = 87,
    KEY_F12         = 88
};

extern volatile unsigned char keystate[128]; 

int keyboard_init(void);
void keyboard_cleanup(void);

#endif // KEYBOARD_H

/* ========================================================================= */
/*                              IMPLEMENTATION                               */
/* ========================================================================= */

#ifdef KEYBOARD_IMPLEMENTATION

volatile unsigned char keystate[128] = {0}; 
_go32_dpmi_seginfo old_handler, new_handler;

void keyboard_handler(void) {
    unsigned char scancode = inportb(0x60);

    if (scancode & 0x80) {
        keystate[scancode & 0x7F] = 0; 
    } else {
        keystate[scancode] = 1;
    }

    outportb(0x20, 0x20); 
}

void keyboard_handler_end(void) {}

int keyboard_init(void) {
    int handler_size = (int)keyboard_handler_end - (int)keyboard_handler;

    _go32_dpmi_lock_code(keyboard_handler, handler_size > 0 ? handler_size : 1024);
    _go32_dpmi_lock_data((void *)keystate, sizeof(keystate));
    _go32_dpmi_get_protected_mode_interrupt_vector(0x09, &old_handler);

    new_handler.pm_offset = (unsigned long)keyboard_handler;
    new_handler.pm_selector = _go32_my_cs();
    
    if (_go32_dpmi_allocate_iret_wrapper(&new_handler) != 0) {
        return 1;
    }

    _go32_dpmi_set_protected_mode_interrupt_vector(0x09, &new_handler);
    return 0;
}

void keyboard_cleanup(void) {
    _go32_dpmi_set_protected_mode_interrupt_vector(0x09, &old_handler);
    _go32_dpmi_free_iret_wrapper(&new_handler);
}

#endif // KEYBOARD_IMPLEMENTATION
