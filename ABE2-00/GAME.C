#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <conio.h>

#define GL2D_IMPLEMENTATION
#define CANVAS_IMPLEMENTATION
#define FONT_IMPLEMENTATION

#include "gl2d.h"
#include "canvas.h"
#include "font.h"

char levels[][11][21]={
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
    "####################"
  },
};

void draw(unsigned char *srf,char level[11][21],Canvas *sprites,int *px,int *py) {
  for(int j=0;j<11;j++) {
    for(int i=0;i<20;i++) {
      switch(level[j][i]) {
        case ' ': Canvas_Draw(srf,sprites,i*16,j*16,0); break;
        case '$': Canvas_Draw(srf,sprites,i*16,j*16,2); break;
        case '*': Canvas_Draw(srf,sprites,i*16,j*16,3); break;
        case '.': Canvas_Draw(srf,sprites,i*16,j*16,4); break;
        case '#': Canvas_Draw(srf,sprites,i*16,j*16,5); break;
      }
    }
  }
  Canvas_Draw(srf,sprites,*px*16,*py*16,1);
}


int main(void) {
  bool quit=false;
  unsigned char *dbf = calloc(320*200,sizeof(*dbf));
  Canvas *sprites = Canvas_Load("sprites.cvs");
  Canvas *font = Canvas_Load("font.cvs");

  int key;
  int px=0,py=0;
  int vx=0,vy=0;
  int ppx,ppy;
  int cpx,cpy;
  int npx,npy;
  int pch,cch,nch;
  int moves=0;

  char level[11][21];

  GL2D_Init();
  GL2D_SetSweetiePalette();

  for(int j=0;j<11;j++) {
    for(int i=0;i<21;i++) {
      level[j][i]=levels[0][j][i];
      switch(level[j][i]) {
        case '@':
        case '+':
          px=i; py=j;
          break;
      }
    }
  }

  while(!quit) {

    if(kbhit()) {
      key=getch();
      if(key==0) key=getch()+256;
//      printf("%d\n",key);
      vx=0; vy=0;
      switch(key) {
        case 27: quit=true; break;
        case 328: vy=-1; break;
        case 336: vy=+1; break;
        case 331: vx=-1; break;
        case 333: vx=+1; break;        
      }      

      if(vx || vy) {
        ppx=px;
        ppy=py;

        cpx=px+vx;
        cpy=py+vy;

        npx=cpx+vx;
        npy=cpy+vy;

        pch=level[ppy][ppx];
        cch=level[cpy][cpx];
        nch=level[npy][npx];

        if(pch=='@' && cch==' ')  {
          level[ppy][ppx]=' ';
          level[cpy][cpx]='@';
          px=cpx;
          py=cpy;
          moves++;
        } else if(pch=='@' && cch=='.')  {
          level[ppy][ppx]=' ';
          level[cpy][cpx]='+';
          px=cpx;
          py=cpy;
          moves++;
        } else if(pch=='+' && cch=='.')  {
          level[ppy][ppx]='.';
          level[cpy][cpx]='+';
          px=cpx;
          py=cpy;
          moves++;
        } else if(pch=='+' && cch==' ') {
          level[ppy][ppx]='.';
          level[cpy][cpx]='@';
          px=cpx;
          py=cpy;
          moves++;
        } else if(pch=='@' && cch=='$' && nch==' ') {
          level[ppy][ppx]=' ';
          level[cpy][cpx]='@';
          level[npy][npx]='$';
          px=cpx;
          py=cpy;
          moves++;
        } else if(pch=='@' && cch=='$' && nch=='.') {
          level[ppy][ppx]=' ';
          level[cpy][cpx]='@';
          level[npy][npx]='*';
          px=cpx;
          py=cpy;
          moves++;
        } else if(pch=='@' && cch=='*' && nch==' ') {
          level[ppy][ppx]=' ';
          level[cpy][cpx]='+';
          level[npy][npx]='$';
          px=cpx;
          py=cpy;
          moves++;
        } else if(pch=='@' && cch=='*' && nch=='.') {
          level[ppy][ppx]=' ';
          level[cpy][cpx]='+';
          level[npy][npx]='*';
          px=cpx;
          py=cpy;
          moves++;
        } else if(pch=='+' && cch=='$' && nch==' ') {
          level[ppy][ppx]='.';
          level[cpy][cpx]='@';
          level[npy][npx]='$';
          px=cpx;
          py=cpy;
          moves++;
        } else if(pch=='+' && cch=='$' && nch=='.') {
          level[ppy][ppx]='.';
          level[cpy][cpx]='@';
          level[npy][npx]='*';
          px=cpx;
          py=cpy;
          moves++;
        } else if(pch=='+' && cch=='*' && nch==' ') {
          level[ppy][ppx]='.';
          level[cpy][cpx]='+';
          level[npy][npx]='$';
          px=cpx;
          py=cpy;
          moves++;
        } else if(pch=='+' && cch=='*' && nch=='.') {
          level[ppy][ppx]='.';
          level[cpy][cpx]='+';
          level[npy][npx]='*';
          px=cpx;
          py=cpy;
          moves++;
        }
      }
    }

    memset(dbf,0,320*200);
    draw(dbf,level,sprites,&px,&py);
    char msg[256];
    sprintf(msg,"MOVES %4d",moves);
    Font_DrawText(dbf,font,0,16*11+4,msg);
    memcpy(GL2D_VGA,dbf,320*200);
    
  }

  GL2D_Quit();

  return 0;
}

