/********************************************************************/  
/*Seperate SGI color image to BW image files                                                                 */ 
/*File name: seperate.c                                                                                                    */ 
/*Author:    Yurong Sun, Joseph Pan                                                                                                 */ 
/*Description:                                                                                                                  */ 
/*            Read color SGI format image file                                                                      */ 
/*            Create three grayscale BW format images relative to r, g, b channels                  */ 
/********************************************************************/ 

#include <iostream>

using namespace std;

#define IXSIZE 400
#define IYSIZE 400
#define IZSIZE 3

void putbyte(FILE *outf, unsigned char val)
{ 
	unsigned char buf[1]; 

	buf[0] = val; 
	fwrite(buf,1,1,outf); 
} 

void putshort(FILE *outf, unsigned short val) 
{ 
	unsigned char buf[2]; 

	buf[0] = (val>>8); 
	buf[1] = (val>>0); 
	fwrite(buf,2,1,outf); 
} 

static int putlong(FILE *outf, unsigned long val) 
{ 
	unsigned char buf[4]; 

	buf[0] = (val>>24); 
	buf[1] = (val>>16); 
	buf[2] = (val>>8); 
	buf[3] = (val>>0); 
	return fwrite(buf,4,1,outf); 
}

unsigned short getshort(FILE *inf) 
{ 
	unsigned char buf[2]; 
	fread(buf, 2, 1, inf); 
	return (buf[0]<<8) + (buf[1]<<0); 
}

static long getlong(FILE *inf) 
{ 
	unsigned char buf[4]; 
	fread(buf, 4, 1, inf); 
	return (buf[0]<<24) + (buf[1]<<16)+ (buf[2]<<8) + (buf[3]<<0); 
}



void main(int argc, char **argv) 
{ 
	FILE *of, *inf; 
	// char iname[80]; 
	
	unsigned short magic, dimension, ixsi, iysi, izsi;//, ixsi1, iysi1; 
	static long colormap, pixmin, pixmax;
	unsigned long *starttab;	// Start table
	unsigned long *lengthtab;  // Length table
	char storage, bpc; 

	unsigned char inbuf[IXSIZE][IYSIZE][IZSIZE]; 

	for (int z=0; z < IZSIZE; z++)	 // R, G, B
	{
		for (int y=0; y < IYSIZE; y++) // height
		{
			for (int x=0; x < IXSIZE; x++)// width 
			{
				inbuf[x][y][z]=0;
			}
		}
	}

	/* make sure the user invokes this program properly */ 
	if(argc != 5) { 
		fprintf(stderr, "Usage: seperate in.rgb outr.bw outg.bw outb.bw\n"); 
		exit(1); 
	}

	/* Read the input RGB file*/ 
	if ((inf=fopen(argv[1], "rb"))==NULL) 
	{ 
		fprintf(stderr, "readImage: Can't open file %s\n", argv[1]); 
		exit(1); 
	} 
	else 
	{ 
		magic = getshort(inf); // 2 bytes short magic
		storage = fgetc(inf); // 1 byte char storage
		//fread(&storage, 1, 1, inf); // 1 byte char storage
		bpc = fgetc(inf);  // 1 byte char bpc
		//fread(&bpc, 1, 1, inf); // 1 byte char bpc
		dimension = getshort(inf); // 2 bytes short dimension

		ixsi = getshort(inf);  // 2 bytes xsize
		iysi = getshort(inf);  // 2 bytes ysize
		izsi = getshort(inf);  // 2 bytes zsize

		pixmin = getlong(inf); // 4 bytes pixel minimum
		pixmax = getlong(inf); // 4 bytes pixel maximum

		fseek(inf, 84, SEEK_CUR);	// 4 bytes dump and 80 bytes imagename

		colormap = getlong(inf);	// 4 bytes colormap ID

		fseek(inf, 404, SEEK_CUR);	// 404 bytes dump

		for (int z=0; z < izsi; z++)	 // R, G, B
		{
			for (int y=0; y < iysi; y++) // height
			{
				for (int x=0; x < ixsi; x++)// width 
				{
					fread(&inbuf[x][y][z], 1, 1, inf); 
				}
			}
		}
	} 
	fclose(inf);

	// Begin to write output files
	// ixsi1=0; 
	// iysi1=0; 

	// output the red channel BW file
	of = fopen(argv[2],"wb"); 

	if(!of) { 
		fprintf(stderr,"sgiimage: can't open output file\n"); 
		exit(1); 
	} 
	//putshort(of, ixsi1); 
	putshort(of, ixsi);  
	//putshort(of, iysi1); 
	putshort(of, iysi);

	for (int y=0; y < iysi; y++) 
	{
		for (int x=0; x < ixsi; x++) 
		{
			fwrite(&inbuf[x][y][0], 1, 1, of);
		}
	}

	fclose(of);

	// output the green channel BW file
	of = fopen(argv[3],"wb"); 
	if(!of) { 
		fprintf(stderr,"sgiimage: can't open output file\n"); 
		exit(1); 
	} 
	//putshort(of, ixsi1); 
	putshort(of, ixsi); 
	//putshort(of, iysi1); 
	putshort(of, iysi);

	for (int y=0; y < iysi; y++) 
	{
		for (int x=0; x < ixsi; x++) 
		{
			fwrite(&inbuf[x][y][1], 1, 1, of);
		}
	}

	fclose(of);

	// output the blue channel BW file
	of = fopen(argv[4],"wb"); 
	if(!of) { 
		fprintf(stderr,"sgiimage: can't open output file\n"); 
		exit(1); 
	} 
	//putshort(of, ixsi1); 
	putshort(of, ixsi); 
	//putshort(of, iysi1); 
	putshort(of, iysi);

	for (int y=0; y < iysi; y++) 
	{
		for (int x=0; x < ixsi; x++) 
		{
			fwrite(&inbuf[x][y][2], 1, 1, of);
		}
	}

	fclose(of); 
}