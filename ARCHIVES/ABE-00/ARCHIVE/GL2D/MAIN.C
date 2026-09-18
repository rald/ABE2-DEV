#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define GL2D_IMPLEMENTATION
#include "gl2d.h"

int main() {
    int x = 160, y = 100, sx = 1, vx = sx;

    unsigned char *dbf=calloc(320*200,sizeof(*dbf));
    
    // Initialize Graphics
    GL2D_Init();

    GL2D_SetSweetiePalette();
    
    while (!kbhit()) {
        GL2D_VWait(); // Sync with monitor refresh

        memset(dbf,0,300*200);
        
        // Draw primitives
        GL2D_FillRect(dbf, 10, 10, 50, 50, 4);
        GL2D_DrawRect(dbf, 70, 10, 50, 50, 14);
        GL2D_DrawLine(dbf, 0, 199, 319, 0, 15);
        GL2D_DrawPoint(dbf, 160, 150, 13);

        GL2D_FillCircle(dbf, x, y, 20, 1);
        GL2D_DrawCircle(dbf, x, y, 20, 12);

        memcpy(gl2d_vga,dbf,320*200);

        x+=vx;

        if(x>=320-20) { x=320-20; vx=-sx; }
        if(x<20)      { x=20; vx=sx; }
        
    }
    
    getch(); // Clear key buffer
    
    // Return to text mode
    GL2D_Quit();
    
    return 0;
}
