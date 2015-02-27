/*************************************************************************
  Program:     	readawc.c 
  -------
  Description:
  -----------
 	 Read AWC data in bionary file (format:8u).
***************************************************************************
  CCRS (EMS/Applications Division)
  Written by:   J. Liu
  Last update:	May 1998
*****************************************************************************/
#include"beps.h"

void readawc(line,awc,rcode)
  int long line; 
  double *awc;
  int long *rcode;
  {
  FILE *infile;	
  long int pix;		
  long int ptr;
  long int size, nitems;
  unsigned char* awcimg;
  awcimg=(unsigned char *)malloc(npixels*sizeof(unsigned char)); 

	/* 	Open AWC file */
  	if ((infile=fopen(awcf, "rb")) == NULL)
    	{
          printf("\n Unable to open file <%s>,  exitting program ...\n\n",
						awcf);
	  *rcode = ERROR;
	  return;
    	}
 
	/*	Calculate starting point of data */
	ptr=line*NPIX_CAN + pix_offset;

	/*	Seek to start of data */
	fseek(infile,ptr,0);

	/*	Read data */
  	fread(&awcimg[0],size=1,nitems=npixels,infile);

	/*	Convert to awc */
  	for (pix=0;pix<npixels;pix++)
   	{

	switch(awcimg[pix])
	{ 
	  case 1:
 	  	awc[pix]=0.05;
		break;
  	  case 2:
 	  	awc[pix]=0.1;
		break;
	  case 3:
	  	awc[pix]=0.15;
		break;
	  case 4:
 	  	awc[pix]=0.2;
		break;
	  case 5:
	  	awc[pix]=0.25;
		break;
	  default:
		awc[pix]=0.15;
		break;

	}
	/* printf("value of awc at pix %d=: %d, %5.2f\n",pix,awcimg[pix],awc[pix]); */
   	}
    free(awcimg);

	/*	Close AWC file */
  	fclose(infile);
 }
