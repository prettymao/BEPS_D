/*************************************************************************
  Program:     	readsnow.c 
  -------
  Description:
  -----------
 	 Read snow pack data in a text file.
***************************************************************************
  CCRS (EMS/Applications Division)
  Written by:   J. Liu
  Last update:	Nov. 1999
*****************************************************************************/
#include"beps.h"
#include"ncar.h"
#include<string.h>
#define OUTSIDE_NCAR 250

void readsnow(line,xx,rcode)
	int long line; 
	struct xvalue xx[]; 
	int short *rcode;
{		
	FILE *infile;
    long int pix;		
  	long int ptr;
  	long int size, nitems;  
    unsigned char* snowimg;
	snowimg=(unsigned char *)malloc(npixels*sizeof(unsigned char));

	strcpy(snowpackf, "c:\\bepsinput\\1996\\csnow96.img");	
/*	Open snow pack binary file */
	if ((infile= fopen(snowpackf,"rb")) ==NULL) {
          printf("\n Unable to open file<%s>,  exitting program ...\n\n",
							snowpackf);
          *rcode = ERROR;
	  return;
     }

/*	Calculate starting point of data */
	ptr=line*NPIX_CAN + pix_offset;

/*	Seek to start of data */
	fseek(infile,ptr,0);

/*	Read data */
  	fread(&snowimg[0],size=1,nitems=npixels,infile);

/*	Convert snow pack to meter */
	for (pix=0;pix<npixels;pix++){
		xx[pix].x1= snowimg[pix]*5.0/1000.0;	// in meter
	}

/*	Close snowpack file */
	fclose(infile);

	strcpy(snowpackf, "c:\\bepsinput\\1996\\csw96.img");
/*	Open soil water binary file */
	if ((infile= fopen(snowpackf,"rb")) ==NULL) {
          printf("\n Unable to open file<%s>,  exitting program ...\n\n",
							snowpackf);
          *rcode = ERROR;
	  return;
     }

/*	Calculate starting point of data */
	ptr=line*NPIX_CAN + pix_offset;

/*	Seek to start of data */
	fseek(infile,ptr,0);

/*	Read data */
  	fread(&snowimg[0],size=1,nitems=npixels,infile);

/*	Convert soil moisture to meter */
	for (pix=0;pix<npixels;pix++){
		xx[pix].x2= snowimg[pix]*5.0/1000.0;	// in meter
	}

/*	Close snowpack file */
	fclose(infile);

	free(snowimg);
  	
	return;
  
  
/* 	Open snowpack file 
	if ((infile=fopen(snowpackf, "r")) == NULL){
          printf("\n Unable to open file <%s>,  exitting program ...\n\n",
					snowpackf);
	  *rcode = ERROR;
	  return;
    }
	 

/*
	for(i=0; i<CAN_NCAR_LAT_TOTAL; i++){
		for(j=0; j<CAN_NCAR_LON_TOTAL; j++){
			fscanf(infile, "%f", &data);     
			snow[i][j]=data/100;	//input is in cm, output is in meter
		//	printf(" %4.0f",snow[i][j]); 
		} 
		// printf("\n"); 
	}
 
/*	Bilinear interpolate to each pixel 
  	for (pix=0;pix<npixels;pix++)
   	{
		r=row_index[pix];
		c=col_index[pix];
	
		ddr=dr[pix];
		ddc=dc[pix];
		if (r== OUTSIDE_NCAR) xx[pix].x1=0;
		else
		{
		xx[pix].x1=(1-ddr)*(1-ddc)*snow[r][c]+(1-ddr)*ddc*snow[r][c-1]+ddr*ddc*snow[r-1][c-1] + ddr*(1-ddc)*snow[r-1][c];
		// printf("value of snowpack at pix %d=: %5.2f, %5.2f\n",pix,snow[r][c],xx[pix].x1); 
		xx[pix].x1= xx[pix].x1/100;	// in meter
		}
   	}
 

/*	Close snowpack file 
  	fclose(infile);
	return;
	*/
 }
