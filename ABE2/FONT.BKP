#ifndef FONT_H
#define FONT_H

#include "canvas.h"

void Font_DrawChar(unsigned char *srf, Canvas *canvas, int x, int y, char c);
void Font_DrawText(unsigned char *srf, Canvas *canvas, int x, int y, char *t);

#ifdef FONT_IMPLEMENTATION

void Font_DrawChar(unsigned char *srf, Canvas *canvas, int x, int y, char c) {
    static int font_map[256];
    static int map_inited = 0;
    int idx;

    if (!map_inited) {
        int i;
        char *let = "ABCDEFGHIJKLMNOPQRSTUVWXYZ 0123456789";
        for (i = 0; i < 256; i++) font_map[i] = -1;
        for (i = 0; let[i]; i++) {
            font_map[(unsigned char)let[i]] = i;
        }
        map_inited = 1;
    }

    idx = font_map[(unsigned char)c];
    if (idx != -1) {
        Canvas_Draw(srf, canvas, x, y, idx);
    }
}

void Font_DrawText(unsigned char *srf, Canvas *canvas, int x, int y, char *t) {
    int i;
    int start_x = x;
    int char_w = canvas->w + 1;
    int char_h = canvas->h + 1;

    for (i = 0; t[i]; i++) {
        if (t[i] == '\n') {
            x = start_x;
            y += char_h;
        } else {
            Font_DrawChar(srf, canvas, x, y, t[i]);
            x += char_w;
            if (x + char_w >= 320) {
                x = start_x;
                y += char_h;
            }
        }
    }
}

#endif /* FONT_IMPLEMENTATION */

#endif /* FONT_H */
