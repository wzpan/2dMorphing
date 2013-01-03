/* ====================================================================== 
 * meshwarp.c -  Mesh warping program. 
 * Copyright (C) 1993 by George Wolberg 
 * 
 * Written by: George Wolberg, 1993 
 * Edited by: Joseph Pan, 2013
 * ====================================================================== 
 */
#include "meshwarp.h"
#include "catmullrom.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
 * meshWarp: 
 * 
 * Warp I1 with correspondence points given in meshes M1 and M2. 
 * Result goes in I2. 
 * 
 * See "Digital Image Warping" by George Wolberg (IEEE Computer Society 
 * Press, 1990) for details. 
 * Based on Douglas Smythe's algorithm (in "A Two-Pass Mesh Warping Algorithm 
 * for Object Transformation and Image Interpolation", ILM Technical Memo 
 * #1030, 1990). 
 */ 
void meshWarp(imageP I1, imageP M1, imageP M2, imageP I2) 
{ 
  int  I_w, I_h, M_w, M_h; 
  int  x, y, u, v, n; 
  float *x1, *y1, *x2, *y2; 
  float *xrow, *yrow, *xcol, *ycol, *coll, *indx, *map; 
  uchar *src, *dst; 
  imageP  Mx, My, I3;

  I_w = I1->width; 
  I_h = I1->height;

  M_w = M1->width; 
  M_h = M1->height;

  /* allocate enough memory for a scanline along the longest dimension */ 
  n = MAX(I_w, I_h); 
  indx = new float[n]; 
  xrow = new float[n];
  yrow = new float[n];
  map  = new float[n];

  /* create table of x-intercepts for source mesh's vertical splines */ 
  Mx = allocImage(M_w, I_h, MESH); 
  for(y=0; y < I_h; y++) indx[y] = y; 
  for(u=0; u < M_w; u++) { /* visit each vertical spline   */ 
    /* store column as row for spline fct */ 
    xcol = (float *) M1->ch[0] + u; 
    ycol = (float *) M1->ch[1] + u; 
    coll = (float *) Mx->ch[0] + u;

    /* scan convert vertical splines */ 
    for(v=0; v < M_h; v++, xcol+=M_w) xrow[v] = *xcol; 
    for(v=0; v < M_h; v++, ycol+=M_w) yrow[v] = *ycol; 
    catmullRom(yrow, xrow, M_h, indx, map, I_h);

    /* store resampled row back into column */ 
    for(y=0; y < I_h; y++, coll+=M_w) *coll = map[y]; 
  }

  /* create table of x-intercepts for dst mesh's vertical splines */ 
  for(u=0; u < M_w; u++) { /* visit each  vertical spline  */ 
    /* store column as row for spline fct */ 
    xcol = (float *) M2->ch[0] + u; 
    ycol = (float *) M2->ch[1] + u; 
    coll = (float *) Mx->ch[1] + u;

    /* scan convert vertical splines */ 
    for(v=0; v < M_h; v++, xcol+=M_w) xrow[v] = *xcol; 
    for(v=0; v < M_h; v++, ycol+=M_w) yrow[v] = *ycol; 
    catmullRom(yrow, xrow, M_h, indx, map, I_h);

    /* store resampled row back into column */ 
    for(y=0; y < I_h; y++, coll+=M_w) *coll = map[y]; 
  }

  /* first pass: warp x using tables in Mx */ 
  I3  = allocImage(I_w, I_h, BW); 
  x1  = (float *) Mx->ch[0]; 
  x2  = (float *) Mx->ch[1]; 
  src = (uchar *) I1->ch[0]; 
  dst = (uchar *) I3->ch[0]; 
  for(x=0; x < I_w; x++) indx[x] = x; 
  for(y=0; y < I_h; y++) { 
    /* fit spline to x-intercepts; resample over all cols */ 
    catmullRom(x1, x2, M_w, indx, map, I_w);

    /* resample source row based on map */ 
    resample(src, I_w, 1, map, dst);

    /* advance pointers to next row */ 
    src += I_w; 
    dst += I_w; 
    x1  += M_w; 
    x2  += M_w; 
  } 
  freeImage(Mx);

  /* create table of y-intercepts for intermediate mesh's hor splines */ 
  My = allocImage(I_w, M_h, MESH); 
  x1 = (float *) M2->ch[0]; 
  y1 = (float *) M1->ch[1]; 
  y2 = (float *) My->ch[0]; 
  for(x=0; x < I_w; x++) indx[x] = x; 
  for(v=0; v < M_h; v++) { /* visit each horizontal spline */ 
    /* scan convert horizontal splines */ 
    catmullRom(x1, y1, M_w, indx, y2, I_w);

    /* advance pointers to next row */ 
    x1 += M_w; 
    y1 += M_w; 
    y2 += I_w; 
  }

  /* create table of y-intercepts for dst mesh's horizontal splines */ 
  x1 = (float *) M2->ch[0]; 
  y1 = (float *) M2->ch[1]; 
  y2 = (float *) My->ch[1]; 
  for(v=0; v < M_h; v++) { /* visit each horizontal spline   */ 
    /* scan convert horizontal splines */ 
    catmullRom(x1, y1, M_w, indx, y2, I_w);

    /* advance pointers to next row */ 
    x1 += M_w; 
    y1 += M_w; 
    y2 += I_w; 
  }

  /* second pass: warp y */ 
  src = (uchar *) I3->ch[0]; 
  dst = (uchar *) I2->ch[0]; 
  for(y=0; y < I_h; y++) indx[y] = y; 
  for(x=0; x < I_w; x++) { 
    /* store column as row for spline fct */ 
    xcol = (float *) My->ch[0] + x; 
    ycol = (float *) My->ch[1] + x; 
    for(v=0; v < M_h; v++, xcol+=I_w) xrow[v] = *xcol; 
    for(v=0; v < M_h; v++, ycol+=I_w) yrow[v] = *ycol;

    /* fit spline to y-intercepts; resample over all rows */ 
    catmullRom(xrow, yrow, M_h, indx, map, I_h);

    /* resample source column based on map */ 
    resample(src, I_h, I_w, map, dst);

    /* advance pointers to next column */ 
    src++; 
    dst++; 
  } 
  freeImage(My); 
  freeImage(I3);

  delete [] indx;
  delete [] xrow;
  delete [] yrow;
  delete [] map;
} 
  
 

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
 * resample: 
 * 
 * Resample the len elements of src (with stride offst) into dst according 
 * to the spatial mapping given in xmap. 
 * Perform linear interpolation for magnification and box filtering 
 * (unweighted averaging) for minification. 
 * Based on Fant's algorithm (IEEE Computer Graphics & Applications, 1/86). 
 */ 
void resample(uchar *src, int len, int offst, float *xmap, uchar *dst) 
{ 
  int u, x, v0, v1; 
  double val, sizfac, inseg, outseg, acc, inpos[1024];

  /* precompute input index for each output pixel */ 
  for(u=x=0; x<len; x++) { 
    while(xmap[u+1]<x) u++; 
    inpos[x] = u + (double) (x-xmap[u]) / (xmap[u+1]-xmap[u]); 
  }

  inseg  = 1.0; 
  outseg = inpos[1]; 
  sizfac = outseg; 
  acc = 0.; 
  v0 = *src; src += offst; 
  v1 = *src; src += offst; 
  for(u=1; u<len; ) { 
    val = inseg*v0 + (1-inseg)*v1; 
    if(inseg < outseg) { 
      acc += (val * inseg); 
      outseg -= inseg; 
      inseg = 1.0; 
      v0 = v1; 
      v1 = *src; 
      src += offst; 
    } else { 
      acc += (val * outseg); 
      acc /= sizfac; 
      *dst = (int) MIN(acc, 0xff); 
      dst += offst; 
      acc = 0.; 
      inseg -= outseg; 
      outseg = inpos[u+1] - inpos[u]; 
      sizfac = outseg; 
      u++; 
    } 
  } 
} 
 
