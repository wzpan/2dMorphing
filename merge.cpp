/******************************************************************/ 
/*Merging three channel's BW format images into one RGB color image                       */ 
/*File name: merge.c                                                                                                    */ 
/*Author:    Yurong Sun, Joseph Pan                                                                                               */ 
/*Description:                                                                                                              */ 
/*            Read greyscale BW format image file                                                            */ 
/*            Create color SGI format image file                                                                */
/******************************************************************/
#include <iostream>

using namespace std;

#define IXSIZE  400
#define IYSIZE  400
#define IZSIZE  3

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
	char iname[80]; 
	
	int i, x, y,z; 
	unsigned short ixsi, iysi, izsi;//, ixsi1, iysi1; 

	unsigned char inbuf[IXSIZE][IYSIZE][IZSIZE]; 

	// make sure the user invokes this program properly 
	if(argc != 5) { 
		fprintf(stderr, "Usage: merge red.bw green.bw blue.bw out.rgb\n"); 
		exit(1); 
	}

	/* Read the input BW files*/ 

	// red channel
	if ((inf = fopen(argv[1], "rb"))==NULL) 
	{ 
		fprintf(stderr, "readImage: Can't open file %s\n", argv[1]); 
		exit(1); 
	} 
	else 
	{ 
		//ixsi1 = getshort(inf); 
		ixsi = getshort(inf); // width
		//iysi1 = getshort(inf); // height
		iysi = getshort(inf);

		for (y=0; y < iysi; y++) 
		{
			for (x=0; x < ixsi; x++) 
			{
				fread(&inbuf[x][y][0], 1, 1, inf); 
			}
		}
	} 
	fclose(inf);

	// green channel
	if ((inf = fopen(argv[2], "rb"))==NULL) 
	{ 
		fprintf(stderr, "readImage: Can't open file %s\n", argv[1]); 
		exit(1); 
	} 
	else 
	{ 
		//ixsi1 = getshort(inf); 
		ixsi = getshort(inf); 
		//iysi1 = getshort(inf); 
		iysi = getshort(inf);

		for (y=0; y < iysi; y++) 
		{
			for (x=0; x < ixsi; x++) 
			{
				fread(&inbuf[x][y][1], 1, 1, inf); 
			}
		}
	} 
	fclose(inf);

	// blue channel
	if ((inf=fopen(argv[3], "rb"))==NULL) 
	{ 
		fprintf(stderr, "readImage: Can't open file %s\n", argv[1]); 
		exit(1); 
	} 
	else 
	{ 
		//ixsi1 = getshort(inf); 
		ixsi = getshort(inf); 
		//iysi1 = getshort(inf); 
		iysi = getshort(inf);

		for (y=0; y < iysi; y++) 
		{
			for (x=0; x < ixsi; x++) 
			{
				fread(&inbuf[x][y][2], 1, 1, inf); 
			}
		}
	} 
	fclose(inf);

	/*Begin to write output files */ 
	of = fopen(argv[4],"wb"); 
	if(!of) { 
		fprintf(stderr,"sgiimage: can't open output file\n"); 
		exit(1); 
	} 
	putshort(of,474);       /* MAGIC                       */ 
	putbyte(of,0);          /* STORAGE is VERBATIM         */ 
	putbyte(of,1);          /* BPC is 1                    */ 
	putshort(of,2);         /* DIMENSION is 2              */ 
	putshort(of,ixsi);		/* XSIZE                       */ 
	putshort(of,iysi);		/* YSIZE                       */ 
	putshort(of,3);         /* ZSIZE                       */               
	putlong(of,0);          /* PIXMIN is 0                 */ 
	putlong(of,255);        /* PIXMAX is 255               */ 
	for(i=0; i<4; i++)      /* DUMMY 4 bytes       */ 
		putbyte(of,0); 
	strcpy(iname,"No Name"); 
	fwrite(iname,80,1,of);  /* IMAGENAME           */ 
	putlong(of,0);          /* COLORMAP is 0       */ 
	for(i=0; i<404; i++)    /* DUMMY 404 bytes     */ 
		putbyte(of,0); 

	for (z=0; z < 3; z++) 
	{
		for(y=0; y<iysi; y++) 
		{
			for(x=0; x<ixsi; x++) 
			{
				fwrite(&inbuf[x][y][z],1,1,of); 
			}
		}
	}

	fclose(of); 
}