#ifndef CANVAS_H
#define CANVAS_H

#include <stdlib.h>

#define GL2D_IMPLEMENTATION

#include "gl2d.h"

typedef struct {
  int f,w,h;
  unsigned char *pixels;
} Canvas;

Canvas *Canvas_Load(char *path);
void Canvas_Free(Canvas **canvas);
void Canvas_Draw(unsigned char *srf,Canvas *canvas,int x,int y,int f);

#ifdef CANVAS_IMPLEMENTATION

Canvas *Canvas_Load(char *path) {
  Canvas *canvas=NULL;
  FILE *fin=NULL;
  int i,j,k,c;
  char *h="0123456789ABCDEF";

  canvas=malloc(sizeof(*canvas));
  if(!canvas) {
    fprintf(stderr,"Error: %s: cannot allocate canvas.",path);
    return NULL;
  }

  fin=fopen(path,"r");
  if(!fin) {
    fprintf(stderr,"Error: %s: cannot open file.",path);
    Canvas_Free(&canvas);
    return NULL;
  }

  if(fscanf(fin,"%d,%d,%d",&canvas->f,&canvas->w,&canvas->h)!=3) {
    fprintf(stderr,"Error: %s: invalid file header.",path);
    fclose(fin);
    Canvas_Free(&canvas);
    return NULL;
  }

  canvas->pixels=calloc(canvas->f*canvas->h*canvas->w,sizeof(*canvas->pixels));
  if(!canvas->pixels) {
    fprintf(stderr,"Error: %s: cannot allocate pixels.",path);
    fclose(fin);
    Canvas_Free(&canvas);
    return NULL;
  }

  k=0;
  while((c=fgetc(fin))!=EOF) {
    j=-1;
    for(i=0;i<16;i++) {
      if(c==h[i]) {
        j=i;
        break;
      } else if(c=='.')  {
        j=255;
        break;
      }
    }
    if(j!=-1) {
      canvas->pixels[k++]=j;
    }
  }
  fclose(fin);

  return canvas;
}

void Canvas_Free(Canvas **canvas) {
  free((*canvas)->pixels);
  (*canvas)->pixels=NULL;
  free(*canvas);
  (*canvas)=NULL;
}

void Canvas_Draw(unsigned char *srf,Canvas *canvas,int x,int y,int f) {
  int i,j,k;
  for(j=0;j<canvas->h;j++) {
    for(i=0;i<canvas->w;i++) {
      k=canvas->pixels[f*canvas->w*canvas->h+j*canvas->w+i];
      if(k!=255) {
        GL2D_DrawPoint(srf,x+i,y+j,k);
      }
    }
  }
}

#endif /* CANVAS_IMPLEMENTATION */

#endif /* CANVAS_H */

