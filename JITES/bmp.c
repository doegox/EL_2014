#include "stdio.h"
#include "stdlib.h"
#include "math.h"

typedef struct  __attribute__((__packed__)){
	 unsigned char type1;                 /* Magic identifier            */
	 unsigned char type2;                 /* Magic identifier            */
	 unsigned int size;                   /* File size in bytes          */
	 int reserved1;
	 unsigned int offset;              /* Offset to image data, bytes */
} HEADER;

int bmp(char *BmpName,int X,int Y,int Inverse) /*Argv = Image name, Pos X, Pos Y */
{
	int i,j,Ord,Blanc,Noir,invers;
	int goline;
	unsigned char Buff[26][10],Dummy[5],Bu[3],k;
	HEADER header;
	FILE *fptr;
	invers = 0;

	/* Open file */
	if ((fptr = fopen(BmpName,"rb")) == NULL)
		return(-1);
	/* Read and check the header */
	fread(&header,sizeof(HEADER),1,fptr);
	/* Seek to the start of the inverse data */
	fseek(fptr,54L,0);
	fread(Bu,2,1,fptr);
	if(Bu[0] == 0xFF)
		invers = 1;
	/* Seek to the start of the image data */
	fseek(fptr,header.offset,SEEK_SET);
	/* Read and return the image */
	for (i=0;i<26;i++)
	{
		fread(Buff[25 - i],10,1,fptr);
		fread(Dummy,2,1,fptr); /* le 11 / &2�me byte */
	}

	fclose(fptr);
	if(invers == 1)
	{
		if(Inverse == 1)
			Inverse = 0;
		else
			Inverse = 1;
	}
	if(Inverse == 1)
	{
		Blanc = 0;
		Noir = 15;
	}
	else
	{
		Blanc = 15;
		Noir = 0;
	}

	for (i=0,j=0;i < 26;i++)
		for(Ord = 0,j = 0;j < 10;j++)
			for(k = 128,goline = 7;(goline >= 0) && (Ord < 78);k=k/2,goline--,Ord++)
			{
				if((Buff[i][j] & k) > 0)
					putpixel(X+Ord,Y+i,Blanc);
				else
					putpixel(X+Ord,Y+i,Noir);
			}

}
/*************************************************************************/

void Inverse_Candid(int xx1,int xx2, int yy1, int yy2, int col1, int col2)
{
	int x,y;

	for(x = xx1;x < xx2;x++)
		for(y = yy1;y < yy2;y++)
			if(getpixel(x,y) == col1)
				putpixel(x,y,col2);

}
