/* ====================================================================== 
* util.h - Utility functions: image I/O and memory management. 
* Copyright (C) 1993 by George Wolberg 
* 
* Written by: George Wolberg, 1993 
* Editted by: Joseph Pan, 2013
* ====================================================================== 
*/

#ifndef UTIL_H
#define UTIL_H

#include "meshwarp.h"
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
imageP readImage(char *file, int type); 

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
* saveImage: 
* 
* Save image/mesh I into file. 
*/ 
int saveImage(imageP I, char *file, int type);

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
* allocImage: 
* 
* Allocate space for a multi-channel image of width w and height h. 
* If type = BW  : image has 1 channel of type unsigned char. 
* If type = MESH: image has 2 channels of type float. 
* Return image structure pointer. 
*/ 
imageP allocImage(int w, int h, int type); 

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
* freeImage: 
* 
* Free image/mesh memory. 
*/ 
void freeImage(imageP I); 


#endif