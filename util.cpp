/* ====================================================================== 
 * util.c - Utility functions: image I/O and memory management. 
 * Copyright (C) 1993 by George Wolberg 
 * 
 * Written by: George Wolberg, 1993 
 * Editted by: Joseph Pan, 2013
 * ====================================================================== 
 */

#include "util.h"
#include <iostream>
using namespace std;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
 * readImage: 
 * 
 * Read an image (type=BW) or mesh (type=MESH) from file. 
 * File Format for BW image: 
 * two integers to specify width and height, followed by uchar data. 
 * File Format for MESH image: 
 * two integers to specify width and height, followed by float data 
 for x-coordinates, followed by float data for y-coordinates. 
 * Return image structure pointer. 
 */ 
imageP readImage(char *file, int type) 
{ 
  int  sz[2]; 
  FILE *fp; 
  imageP  I;

  /* open file for reading */ 
  if((fp = fopen(file, "r")) == NULL) {
    cerr<< "readImage: Can't open file " << file << endl; 
    return ((imageP) NULL); 
  }

  /* read dimensions into I */ 
  fread(sz, sizeof(int), 2, fp); 
  I = allocImage(sz[0], sz[1], type);

  /* read data into I */ 
  switch(type) { 
  case BW: 
    fread(I->ch[0], sz[0]*sz[1], 1, fp); 
    break; 
  case MESH: 
    fread(I->ch[0], sz[0]*sz[1], 2*sizeof(float), fp); 
    break; 
  default:
    cerr << "readImage: Bad type "  << type << endl; 
    return((imageP) NULL); 
  } 
  fclose(fp);

  return I; 
} 
  
 

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
 * saveImage: 
 * 
 * Save image/mesh I into file. 
 */ 
int saveImage(imageP I, char *file, int type) 
{ 
  int  sz[2]; 
  FILE *fp;

  if((fp=fopen(file, "w")) == NULL) {
    cerr << "saveImage: Can't create file " << file <<endl; 
    return (NULL); 
  }

  /* write dimensions to file */ 
  sz[0] = I->width; 
  sz[1] = I->height; 
  fwrite(sz, sizeof(int), 2, fp);

  /* write data to file */ 
  switch(type) { 
  case BW: 
    fwrite(I->ch[0], sz[0]*sz[1], 1, fp); 
	return 0;
    break; 
  case MESH: 
    fwrite(I->ch[0], sz[0]*sz[1], 2*sizeof(float), fp); 
    break; 
	return 0;
  default:
    cerr << "saveImage: Bad type "<< type << endl; 
    return(NULL); 
  } 
  fclose(fp); 
} 
  
 

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
 * allocImage: 
 * 
 * Allocate space for a multi-channel image of width w and height h. 
 * If type = BW  : image has 1 channel of type unsigned char. 
 * If type = MESH: image has 2 channels of type float. 
 * Return image structure pointer. 
 */ 
imageP allocImage(int w, int h, int type) 
{ 
  //uchar *memptr; 
  imageP  I;

  /* allocate memory for image data structure */ 
  I = (imageP) malloc(sizeof(imageS)); 
  if(I == NULL) {
    cerr << "allocImage: Insufficient memory\n"; 
    return ((imageP) NULL); 
  }

  /* init dimensions */ 
  I->width  = w; 
  I->height = h;

  /* init channel pointers */ 
  switch(type) { 
  case BW: 
    I->ch[0] = (uchar *) malloc(w*h);	
    break; 
  case MESH: 
    I->ch[0] = (float *) malloc(2*w*h*sizeof(float)); 
    I->ch[1] = (float *) I->ch[0] + w*h;			  
    break; 
  default:
    cerr << "allocImage: Bad type" << type << endl;
    return((imageP) NULL); 
  }

  return(I); 
} 
  
 

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
 * freeImage: 
 * 
 * Free image/mesh memory. 
 */ 
void freeImage(imageP I) 
{ 
  free((char *) I->ch[0]); 
  free((char *) I); 
} 
 

