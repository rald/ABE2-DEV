#ifndef GL2D_H
#define GL2D_H

#include <dos.h>
#include <pc.h>
#include <sys/nearptr.h>
#include <go32.h>

#define GL2D_SCREEN_WIDTH  320
#define GL2D_SCREEN_HEIGHT 200

#define GL2D_PALETTE_INDEX       0x03c8
#define GL2D_PALETTE_DATA        0x03c9

#define GL2D_DARK_LICORICE         0x00
#define GL2D_PLUM_FUDGE            0x01
#define GL2D_CHERRY_TART           0x02
#define GL2D_SPICED_PEACH          0x03
#define GL2D_BUTTERSCOTCH          0x04
#define GL2D_LIME_DROP             0x05
#define GL2D_MINT_LEAF             0x06
#define GL2D_BLUE_RASPBERRY        0x07
#define GL2D_GRAPE_SYRUP           0x08
#define GL2D_BLUEBERRY_POP         0x09
#define GL2D_COTTON_CANDY_BLUE     0x0A
#define GL2D_SODA_FROST            0x0B
#define GL2D_POWDERED_SUGAR        0x0C
#define GL2D_ICED_MINT             0x0D
#define GL2D_COOL_CHOCOLATE        0x0E
#define GL2D_DARK_TRUFFLE          0x0F

typedef struct {
    unsigned char r,g,b;
} GL2D_Color;

void GL2D_Init(void);
void GL2D_Quit(void);
void GL2D_DrawPoint(unsigned char *srf, int x, int y, unsigned char color);
void GL2D_DrawLine(unsigned char *srf, int x0, int y0, int x1, int y1, unsigned char color);
void GL2D_DrawRect(unsigned char *srf, int x, int y, int w, int h, unsigned char color);
void GL2D_FillRect(unsigned char *srf, int x, int y, int w, int h, unsigned char color);
void GL2D_DrawCircle(unsigned char *srf, int xc, int yc, int r, unsigned char color);
void GL2D_FillCircle(unsigned char *srf, int xc, int yc, int r, unsigned char color);
void GL2D_VWait(void);

#ifdef GL2D_IMPLEMENTATION

const GL2D_Color sweetie[] = {
  { 6,  7, 11}, /* Dark Licorice       (#1a1c2c) */
  {23, 10, 23}, /* Plum Fudge          (#5d275d) */
  {44, 15, 21}, /* Cherry Tart         (#b13e53) */
  {59, 31, 22}, /* Spiced Peach        (#ef7d57) */
  {63, 51, 29}, /* Butterscotch        (#ffcd75) */
  {41, 59, 28}, /* Lime Drop           (#a7f070) */
  {14, 45, 25}, /* Mint Leaf           (#38b764) */
  { 9, 28, 30}, /* Blue Raspberry      (#257179) */
  {10, 13, 27}, /* Grape Syrup         (#29366f) */
  {15, 23, 50}, /* Blueberry Pop       (#3b5dc9) */
  {16, 41, 61}, /* Cotton Candy Blue   (#41a6f6) */
  {28, 59, 61}, /* Soda Frost          (#73eff7) */
  {60, 60, 60}, /* Powdered Sugar      (#f4f4f4) */
  {37, 44, 48}, /* Iced Mint           (#94b0c2) */
  {21, 27, 33}, /* Cool Chocolate      (#566c86) */
  {13, 15, 22}  /* Dark Truffle        (#333c57) */
};

unsigned char *GL2D_VGA = NULL;

void GL2D_SetSweetiePalette() {
  int i;
  
  outportb(GL2D_PALETTE_INDEX, 0); /* tell the VGA that palette data is coming. */  
  
  for(i = 0; i < 16; i++) {
    outportb(GL2D_PALETTE_DATA, sweetie[i].r);
    outportb(GL2D_PALETTE_DATA, sweetie[i].g);
    outportb(GL2D_PALETTE_DATA, sweetie[i].b);
  }
  
  for(i = 16; i < 256; i++) {
    outportb(GL2D_PALETTE_DATA, sweetie[0].r);
    outportb(GL2D_PALETTE_DATA, sweetie[0].g);
    outportb(GL2D_PALETTE_DATA, sweetie[0].b);
  }
}  

void GL2D_Init(void) {
    union REGS regs;
    
    /* Set VGA Mode 13h (320x200, 256 colors) */
    regs.x.ax = 0x0013;
    int86(0x10, &regs, &regs);

    /* Enable direct memory access via near pointers in DJGPP */
    if (__djgpp_nearptr_enable() == 0) {
        /* Failed to enable near pointers, fallback or exit could go here */
        return; 
    }
    
    /* Set pointer to VGA memory (0xA0000) */
    GL2D_VGA = (unsigned char *)(0xA0000 + __djgpp_conventional_base);
}

void GL2D_Quit(void) {
    union REGS regs;
    
    /* Disable near pointers */
    __djgpp_nearptr_disable();
    
    /* Restore text mode 03h */
    regs.x.ax = 0x0003;
    int86(0x10, &regs, &regs);
}

void GL2D_DrawPoint(unsigned char *srf, int x, int y, unsigned char color) {
    if (x >= 0 && x < GL2D_SCREEN_WIDTH && y >= 0 && y < GL2D_SCREEN_HEIGHT) {
        srf[y * GL2D_SCREEN_WIDTH + x] = color;
    }
}

void GL2D_DrawLine(unsigned char *srf, int x0, int y0, int x1, int y1, unsigned char color) {
    int dx = (x1 > x0) ? (x1 - x0) : (x0 - x1);
    int dy = (y1 > y0) ? (y1 - y0) : (y0 - y1);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        GL2D_DrawPoint(srf, x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

static void gl2d_draw_hline(unsigned char *srf, int x1, int x2, int y, unsigned char color) {
    int i;
    if (x1 > x2) { int t = x1; x1 = x2; x2 = t; }
    if (y < 0 || y >= GL2D_SCREEN_HEIGHT) return;
    if (x1 < 0) x1 = 0;
    if (x2 >= GL2D_SCREEN_WIDTH) x2 = GL2D_SCREEN_WIDTH - 1;
    for (i = x1; i <= x2; i++) {
        srf[y * GL2D_SCREEN_WIDTH + i] = color;
    }
}

static void gl2d_draw_vline(unsigned char *srf, int x, int y1, int y2, unsigned char color) {
    int i;
    if (y1 > y2) { int t = y1; y1 = y2; y2 = t; }
    if (x < 0 || x >= GL2D_SCREEN_WIDTH) return;
    if (y1 < 0) y1 = 0;
    if (y2 >= GL2D_SCREEN_HEIGHT) y2 = GL2D_SCREEN_HEIGHT - 1;
    for (i = y1; i <= y2; i++) {
        srf[i * GL2D_SCREEN_WIDTH + x] = color;
    }
}

void GL2D_DrawRect(unsigned char *srf, int x, int y, int w, int h, unsigned char color) {
    if (w <= 0 || h <= 0) return;
    gl2d_draw_hline(srf, x, x + w - 1, y, color);
    gl2d_draw_hline(srf, x, x + w - 1, y + h - 1, color);
    gl2d_draw_vline(srf, x, y, y + h - 1, color);
    gl2d_draw_vline(srf, x + w - 1, y, y + h - 1, color);
}

void GL2D_FillRect(unsigned char *srf, int x, int y, int w, int h, unsigned char color) {
    int i;
    if (w <= 0 || h <= 0) return;
    for (i = 0; i < h; i++) {
        gl2d_draw_hline(srf, x, x + w - 1, y + i, color);
    }
}

void GL2D_DrawCircle(unsigned char *srf, int xc, int yc, int r, unsigned char color) {
    int x = 0;
    int y = r;
    int p = 3 - 2 * r;

    if (!r) return;

    while (y >= x) {
        GL2D_DrawPoint(srf, xc - x, yc - y, color);
        GL2D_DrawPoint(srf, xc - y, yc - x, color);
        GL2D_DrawPoint(srf, xc + y, yc - x, color);
        GL2D_DrawPoint(srf, xc + x, yc - y, color);
        GL2D_DrawPoint(srf, xc - x, yc + y, color);
        GL2D_DrawPoint(srf, xc - y, yc + x, color);
        GL2D_DrawPoint(srf, xc + y, yc + x, color);
        GL2D_DrawPoint(srf, xc + x, yc + y, color);
        
        if (p < 0) {
            p += 4 * x + 6;
        } else {
            p += 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

void GL2D_FillCircle(unsigned char *srf, int xc, int yc, int r, unsigned char color) {
    int x = 0;
    int y = r;
    int p = 3 - 2 * r;

    if (!r) return;

    while (y >= x) {
        gl2d_draw_hline(srf, xc - x, xc + x, yc - y, color);
        gl2d_draw_hline(srf, xc - y, xc + y, yc - x, color);
        gl2d_draw_hline(srf, xc - x, xc + x, yc + y, color);
        gl2d_draw_hline(srf, xc - y, xc + y, yc + x, color);
        
        if (p < 0) {
            p += 4 * x + 6;
        } else {
            p += 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

void GL2D_VWait(void) {
    /* Wait for the vertical retrace (VBLANK) */
    /* Wait until we are NOT in the retrace */
    while (inportb(0x3DA) & 0x08);
    /* Wait until we ARE in the retrace */
    while (!(inportb(0x3DA) & 0x08));
}

#endif /* GL2D_IMPLEMENTATION */

#endif /* GL2D_H */

