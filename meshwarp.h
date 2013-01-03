/* ====================================================================== 
 * meshwarp.h -  Mesh warping header file. 
 * Copyright (C) 1993 by George Wolberg 
 * 
 * Written by: George Wolberg, 1993 
 * ====================================================================== 
 */

#ifndef MESHWARP_H
#define MESHWARP_H

#define BW  0 
#define MESH  1 

//#define MAX(A,B) ((A) > (B) ? (A) : (B)) 
//#define MIN(A,B) ((A) < (B) ? (A) : (B))

inline int MAX(const int A, const int B)
{
  return ((A) > (B) ? (A) : (B)); 
}

inline int MIN(const int A, const int B)
{
  return ((A) < (B) ? (A) : (B));
}

typedef unsigned char uchar;

typedef struct {  /* image data structure  */ 
  int width;  /* image width  (# cols) */ 
  int height;  /* image height (# rows) */ 
  void *ch[2];  /* pointers to channels  */ 
} imageS, *imageP;

/* extern declarations for functions in meshwarp.c */ 
extern void meshWarp(imageP I1, imageP M1, imageP M2, imageP I2); 
extern void resample(uchar *src, int len, int offst, float *xmap, uchar *dst);

/* extern declarations for functions in util.c */ 
extern imageP readImage(char *file, int type); 
extern int saveImage(imageP I, char *file, int type); 
extern imageP allocImage(int w, int h, int type); 
extern void freeImage(imageP I); 
 
#endif
