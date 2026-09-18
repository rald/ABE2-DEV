#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define GL2D_IMPLEMENTATION
#define CANVAS_IMPLEMENTATION
#define KEYBOARD_IMPLEMENTATION

#include "gl2d.h"
#include "canvas.h"
#include "keyboard.h"

char levels[][12][21]={
  {
    "####################",
    "#+                 #",
    "# $.               #",
    "# *                #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "####################"
  },
};

int main(void) {
  bool quit=false;

  unsigned char *dbf = calloc(320*200,sizeof(*dbf));

  Canvas *sprites = Canvas_Load("sprites.cvs");

  keyboard_init();

  GL2D_Init();
  GL2D_SetSweetiePalette();

  while(!quit) {

    GL2D_VWait();
    
    memset(dbf,0,320*200);

    if(keystate[KEY_ESC]) quit=true;

    for(int j=0;j<12;j++) {
      for(int i=0;i<20;i++) {
        switch(levels[0][j][i]) {
          case ' ': Canvas_Draw(dbf,sprites,i*16,j*16,0); break;
          case '@': Canvas_Draw(dbf,sprites,i*16,j*16,1); break;
          case '+': Canvas_Draw(dbf,sprites,i*16,j*16,1); break;
          case '$': Canvas_Draw(dbf,sprites,i*16,j*16,2); break;
          case '*': Canvas_Draw(dbf,sprites,i*16,j*16,3); break;
          case '.': Canvas_Draw(dbf,sprites,i*16,j*16,4); break;
          case '#': Canvas_Draw(dbf,sprites,i*16,j*16,5); break;
        }
      }
    }

    memcpy(GL2D_VGA,dbf,320*200);

  }

  GL2D_Quit();

  keyboard_cleanup();
  
  return 0;
}

