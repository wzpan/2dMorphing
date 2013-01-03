/* ====================================================================== 
 * warp.c -  Warp a single image using mesh warping. 
 * Copyright (C) 1993 by George Wolberg 
 * 
 * Written by: George Wolberg, 1993 
 * Edited by: Joseph Pan, 2013
 * ====================================================================== 
 */
#include "meshwarp.h"
#include <iostream>

using namespace std;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
 * main: 
 * 
 * Main function to collect input image, source, and target meshes. 
 * Pass them to meshWarp() and save result in output file. 
 */ 
void main(int argc, char **argv) 
{ 
  imageP I1, I2; 
  imageP M1, M2;

  /* make sure the user invokes this program properly */ 
  if(argc != 5) { 
    fprintf(stderr, "Usage: warp in.bw src.XY dst.XY out.bw\n"); 
    exit(1); 
  }

  /* read input image and meshes */ 
  I1 = readImage(argv[1], BW); /* input image */ 
  M1 = readImage(argv[2], MESH); /* source mesh */ 
  M2 = readImage(argv[3], MESH); /* target mesh */

  /* allocate memory and init structure for output image */ 
  I2 = allocImage(I1->width, I1->height, BW);

  /* invoke the mesh warping function */ 
  meshWarp(I1, M1, M2, I2);

  /* save output to a file */ 
  saveImage(I2, argv[4], BW); 
} 
 
