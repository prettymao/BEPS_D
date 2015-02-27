/*************************************************************************
  Program:     readbm.c
  --------
  Description:
  -----------
  Read biomass from a file in a given line.

 ***************************************************************************
  CCRS (EMS/Applications Division)
  Written by:  J. Liu       
  Last update: 2000
*****************************************************************************/
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include "beps.h"

void readbm(line,xx,rcode)
int long line;		/* Line number */
struct xvalue xx[]; 
int short *rcode;
{
	FILE *fbm; /* Pointers biomass file */
	char filename_bm[80] = "c:\\bepsinput\\common\\bm_ccrs00.img";
	int short *bm;
	int ptr, size, nitems;
	int bytes=2;		/* Number of bytes for a unsigned int data type */
	int long pix;

	bm=(int short*)malloc(npixels*sizeof(int short));
	
/*	Open biomass file */
	if ((fbm= fopen(filename_bm,"rb")) ==NULL) {
          printf("\n Unable to open file<%s>,  exitting program ...\n\n",
							filename_bm);
          *rcode = ERROR;
	  return;
     }

/*	*** Read a line of data from the biomass file ***/

/*      Set pointer to start of line data */
       
        ptr=bytes*(line*NPIX_CAN + pix_offset);

/*      Seek to and Read data from delta row and col files */
        
		fseek(fbm,ptr,0);     
        fread(&bm[0],size=bytes,nitems=npixels,fbm);

		for (pix=0;pix<npixels;pix++){
/*		Biomass in ton/ha */
			xx[pix].x9=(double)bm[pix]*0.1;
			//printf("biomass=%f\n",xx[pix].x9);

		}
/*	    Close input file and memory */
        fclose(fbm);
		free(bm);

		return;
}






