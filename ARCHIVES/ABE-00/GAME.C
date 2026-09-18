#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define GL2D_IMPLEMENTATION
#define CANVAS_IMPLEMENTATION
#define KEYBOARD_IMPLEMENTATION

#include "gl2d.h"
#include "canvas.h"
#include "keyboard.h"

int main(void) {
  bool quit=false;
  int frame = 0;

  Canvas *abe = Canvas_Load("abe.cvs");
  unsigned char *dbf = calloc(320*200,sizeof(*dbf));

  keyboard_init();

  GL2D_Init();
  GL2D_SetSweetiePalette();

  while(!quit) {
    memset(dbf,0,320*200);
    if(keystate[KEY_ESC]) quit=true;
    Canvas_Draw(dbf,abe,0,0,(frame/10)%2);
    memcpy(GL2D_VGA,dbf,320*200);
    frame++;
    GL2D_VWait();
  }

  GL2D_Quit();

  keyboard_cleanup();
  
  return 0;
}
