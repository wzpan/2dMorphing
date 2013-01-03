/* ====================================================================== 
* morph.c -  Generate a metamorphosis sequence. 
* Copyright (C) 1993 by George Wolberg 
* 
* Written by: George Wolberg, 1993 
* Edited by: Joseph Pan, 2013
* ====================================================================== 
*/

#ifndef MORPH_H
#define MORPH_H

#include <cv.h>
#include <highgui.h>
using namespace cv;

/** 
* \brief Generate a morph sequence of frames between images I1 and I2. 
* Correspondence points among I1 and I2 are given in meshes M1 and M2. 
* nframes frames are generated (including I1 and I2). 
* The output is stored in files "basename_xxx.bw" where xxx are sequential 
* 3-digit frame numbers. 
*/ 
void morph(const imageP I1, const imageP I2, const imageP M1, const imageP M2, int nframes, const char *basename);

#endif 