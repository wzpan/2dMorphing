/* ====================================================================== 
 * catmullRom.c - Catmull-Rom interpolating spline function. 
 * Copyright (C) 1993 by George Wolberg 
 * 
 * Written by: George Wolberg, 1993 
 * ====================================================================== 
 */

#include "meshwarp.h"
#include "catmullrom.h"
#include <iostream>

using namespace std;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
 * catmullRom: 
 * 
 * Compute a Catmull-Rom spline passing through the len1 points in arrays 
 * x1, y1, where y1 = f(x1) 
 * len2 positions on the spline are to be computed. Their positions are 
 * given in x2. The spline values are stored in y2. 
 */ 
void catmullRom(float *x1, float *y1, int len1, float *x2, float *y2, int len2) 
{ 
  int i, j, dir, j1, j2; 
  double x,  dx1, dx2; 
  double dx, dy, yd1, yd2, p1, p2, p3; 
  double a0y, a1y, a2y, a3y; 
 
  /* find direction of monotonic x1; skip ends */ 
  if(x1[0] < x1[1]) { /* increasing */ 
    if(x2[0]<x1[0] || x2[len2-1]>x1[len1-1]) dir=0; 
    else dir = 1; 
  } else {  /* decreasing */ 
    if(x2[0]>x1[0] || x2[len2-1]<x1[len1-1]) dir=0; 
    else dir = -1; 
  } 
  if(dir == 0) {   /* error */
    cerr << "catmullRom: Output x-coord out of range of input\n"; 
    return; 
  }

  /* p1 is first endpoint of interval 
   * p2 is resampling position 
   * p3 is second endpoint of interval 
   * j  is input index for current interval 
   */

  /* force coefficient initialization */ 
  if(dir==1) p3 = x2[0] - 1; 
  else  p3 = x2[0] + 1;

  for(i=0; i<len2; i++) { 
    /* check if in new interval */ 
    p2 = x2[i]; 
    if((dir==1 && p2>p3) || (dir== -1 && p2<p3)) { 
      /* find the interval which contains p2 */ 
      if(dir) { 
        for(j=0; j<len1 && p2>x1[j]; j++); 
        if(p2 < x1[j]) j--; 
      } else { 
        for(j=0; j<len1 && p2<x1[j]; j++); 
        if(p2 > x1[j]) j--; 
      }

      p1 = x1[j];  /* update 1st endpt */ 
      p3 = x1[j+1];  /* update 2nd endpt */

      /* clamp indices for endpoint interpolation */ 
      j1 = MAX(j-1, 0); 
      j2 = MIN(j+2, len1-1); 
  
      /* compute spline coefficients */ 
      dx  = 1.0 / (p3 - p1); 
      dx1 = 1.0 / (p3 - x1[j1]); 
      dx2 = 1.0 / (x1[j2] - p1); 
      dy  = (y1[j+1] - y1[ j ]) * dx; 
      yd1 = (y1[j+1] - y1[ j1]) * dx1; 
      yd2 = (y1[j2 ] - y1[ j ]) * dx2; 
      a0y =  y1[j]; 
      a1y =  yd1; 
      a2y =  dx *  ( 3*dy - 2*yd1 - yd2); 
      a3y =  dx*dx*(-2*dy +   yd1 + yd2); 
    } 
    /* use Horner's rule to calculate cubic polynomial */ 
    x = p2 - p1; 
    y2[i] = ((a3y*x + a2y)*x + a1y)*x + a0y; 
  } 
} 
 
