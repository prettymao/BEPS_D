/*************************************************************************
  Function:     readlc.c 
  --------
  Description:
  -----------
  Reads landcover data for a specific line,  pixel dimensions
  Land cover data is stored in binary format.
***************************************************************************
  CCRS (EMS/Applications Division)
  Written by:   J. Liu/D. Fraser
  Last update:  May 22,  1997
*****************************************************************************/

#include"beps.h"

void readlc(line,lc,rcode)
int long line; 
int short *lc;
int long *rcode;
{		
	FILE *infile;
    long int pix;		
  	long int ptr;
  	long int size, nitems;  
    unsigned char* lcimg;
	lcimg=(unsigned char *)malloc(npixels*sizeof(unsigned char)); 
/*	Open Land cover data file */
  	if ((infile=fopen(landcoverf, "rb")) == NULL) {
          printf("\n Unable to open file <%s>,  exitting program ...\n\n",
						landcoverf);
	  *rcode = ERROR;
	  return;
    	}

/*	Calculate starting point of data */
	ptr=line*NPIX_CAN + pix_offset;

/*	Seek to start of data */
	fseek(infile,ptr,0);

/*	Read data */
  	fread(&lcimg[0],size=1,nitems=npixels,infile);

/*	Convert to shorts */
  	for (pix=0;pix<npixels;pix++)
    	  lc[pix]=lcimg[pix];

/*	Close Land cover file */
	free(lcimg);
  	fclose(infile);
 }
