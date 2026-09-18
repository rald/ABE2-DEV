#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define GL2D_IMPLEMENTATION
#define CANVAS_IMPLEMENTATION
#define KEYBOARD_IMPLEMENTATION

#include "gl2d.h"
#include "canvas.h"
#include "keyboard.h"

typedef struct {
    int x, y;
    int w, h;
} Rect;

bool checkCollision(Rect a, Rect b) {
    return (a.x < b.x + b.w &&
            a.x + a.w > b.x &&
            a.y < b.y + b.h &&
            a.y + a.h > b.y);
}

void updatePlayer(Rect *player, int vx, int vy, Rect obstacle) {
    // 1. Move and resolve on the X-axis
    player->x += vx;
    if (checkCollision(*player, obstacle)) {
        // Collision response: Push out / revert X movement
        if (vx > 0) {
            // Moving right, hit left side of obstacle
            player->x = obstacle.x - player->w;
        } else if (vx < 0) {
            // Moving left, hit right side of obstacle
            player->x = obstacle.x + obstacle.w;
        }
    }

    // 2. Move and resolve on the Y-axis
    player->y += vy;
    if (checkCollision(*player, obstacle)) {
        // Collision response: Push out / revert Y movement
        if (vy > 0) {
            // Moving down, hit top side of obstacle
            player->y = obstacle.y - player->h;
        } else if (vy < 0) {
            // Moving up, hit bottom side of obstacle
            player->y = obstacle.y + obstacle.h;
        }
    }
}

int main(void) {
  bool quit=false;
  int frame = 0;
  int x=0,y=0,vx=0,vy=0;
  Rect rectA,rectB;

  Canvas *abe = Canvas_Load("abe.cvs");
  unsigned char *dbf = calloc(320*200,sizeof(*dbf));

  keyboard_init();

  GL2D_Init();
  GL2D_SetSweetiePalette();

  while(!quit) {

    GL2D_VWait();
    
    memset(dbf,0,320*200);

    if(keystate[KEY_ESC]) quit=true;

    vx=0;
    vy=0;

    if(keystate[KEY_UP]) vy=-1;
    if(keystate[KEY_DOWN]) vy=1;
    if(keystate[KEY_LEFT]) vx=-1;
    if(keystate[KEY_RIGHT]) vx=1;

    rectA.x=x;
    rectA.y=y;
    rectA.w=16;
    rectA.h=16;
    
    rectB.x=100;
    rectB.y=100;
    rectB.w=16;
    rectB.h=16;

    updatePlayer(&rectA,vx,vy,rectB);

    x=rectA.x;
    y=rectA.y;
    
    if(x<0) x=0;
    if(y<0) y=0;
    if(x>320-16) x=320-16;
    if(y>200-16) y=200-16;

    Canvas_Draw(dbf,abe,x,y,(frame/10)%4);
    GL2D_DrawRect(dbf,rectB.x,rectB.y,rectB.w,rectB.h,12);
    
    memcpy(GL2D_VGA,dbf,320*200);

    frame++;
  }

  GL2D_Quit();

  keyboard_cleanup();
  
  return 0;
}

