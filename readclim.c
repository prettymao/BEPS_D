/*************************************************************************
  Program:     readclim.c
  --------
  Description:
  -----------
  Interpolates ncar (gridded) data using bilinear interpolation
  for all pixels in a line.

  Notes:
  ------
  File rowmin.dat is used to indicate those latitude lines of ncar data
  used in the interpolation process.  Rowin also indicates if the line
  is outside of the NCAR grid.  This function first checks if the line
  is outside the NCAR grid.  If the line is outside,  the function returns
  to main.  If it is inside interpolation is performed.

  Warnings
  --------
  A)  The following files MUST be generated before you use this program.
	row_index.dat
	col_index.dat
	dr.dat
	dc.dat
	binary.dat:	ncar binary file for study area

  B)  Control file information (i.e. julian_day_start, julian_day_end,
      nlines,  npixels) must be identical to that used to generate
      binary file.

***************************************************************************
  CCRS (EMS/Applications Division)
  Written by:   D. Fraser/J. Liu       
  Last update:	Oct 1999
*****************************************************************************/

#include <stdio.h>
#include <string.h>
#include "ncar.h"
#include "beps.h"

#define OUTSIDE_NCAR 250

void readclim(lin,jday,ncobs,nstats,climate,rcode)

int lin;		/* Line number */
int jday;		/* Julian Day */
int ncobs;		/* Number of climate observations */
int nstats;		/* Number of stations per line of latitude (row) */
double **climate;	/* Climate data for a line in the study area */
int short *rcode;
{

    FILE *frmin; /* Pointers to bilinear
					          interpolation files */
    FILE *fncar;	/* Pointer to ncar data file */
	
	float *ncar_data; /* NCAR data */
	float rc,r_1c,r_1c_1,rc_1;  /* 4 NCAR data points used to interpolate 
				    a climate value for a specific pixel */
	float i_value;	/* Interpolated value */
	int ncar_ncobs; /* Number of ncar data points needed to interpolate
			   a line of NPP data */
	int njdays=365;	/* Number of Julian days,  for 1993 and 1994 */
	int start_ncar; /* Start of NCAR data needed to interpolate a specific
			   line of NCAR data */
	int row_ncobs; 	/* Number of climate observations elements in one 
			   row of the ncar data file */
	int stn_ncobs;	/* Number of climate observations elements for one 
		   	   station */
	int bytes_four =4;	/* Number of bytes for a float data type */
    long int arrpos,size,nelems;
	int rmin, rmax;
	int pixel,r,c,i;
	int pos_rc,pos_r_1c,pos_r_1c_1,pos_rc_1; 
			/* Position of NCAR data points in ncar data array
			   used to interpolate a climate value for a 
			   specific pixel */ 

	unsigned char row_min[2];

/*	Account for offset between c array start (i.e. at 0) and julian
	day start (i.e. at 1) */
	jday = jday -1;

/*	Calculate number of data climate observations for one row 
	(i.e. latitude) in ncar binary file */
	row_ncobs = nstats * njdays * ncobs;

/*	Calculate number of data climate observations for one station in ncar 
	binary file */
	stn_ncobs = njdays * ncobs;


/*	Open rowmin file */
	if ((frmin=fopen(rowminf,"rb")) == NULL) {
          printf("\n Unable to open file <%s>,  exitting program ...\n\n",
							rowminf);
          *rcode = ERROR;
	  return;
        }

/*      Seek to and Read data from row minumum file */
        arrpos = lin * 2;
        fseek(frmin,arrpos,0);
        fread(&row_min[0],size=1,nelems=2,frmin);

/*	Set minimum and maximum NCAR lines needed to perform bilinear interp.
	for specified line in the study area */
	rmin = (int)row_min[MIN];
	rmax = (int)row_min[MAX];

/*	If line is outside NCAR Grid */
	if (rmax == OUTSIDE_NCAR) {

	  printf(" Default values assigned for entire line\n");

/*	  Assign defualt values to pixels */
          for (pixel=0;  pixel<npixels; pixel++) {
	    climate[pixel][0] = DEFAULT_RAD;
	    climate[pixel][1] = DEFAULT_TMIN;
	    climate[pixel][2] = DEFAULT_TMAX;
	    climate[pixel][3] = DEFAULT_PRECIP;
		climate[pixel][4]= DEFAULT_HUM;
	  }
	  fclose(frmin);
	  
/*	  Return to calling function */
	  return;
	}
	else ;


/*	Open NCAR binary file */
	if ((fncar=fopen(ncarbinf,"rb")) == NULL) {
          printf("\n Unable to open binary file<%s>,  exitting program ...\n\n",
							ncarbinf);
          *rcode = ERROR;
	  return;

        }

/*	For each pixel,  shift row index values */
/*
	for (pixel=0; pixel <npixels; pixel++)
	  row_index[pixel] = row_index[pixel] - rmin +1;
*/


/*	Calculate memory needed for ncar data */
	ncar_ncobs = row_ncobs * (rmax - (rmin -1) + 1);

/*	Allocate memory for ncar data needed in interpolation */
        ncar_data=(float *)malloc(ncar_ncobs*sizeof(float));
	    if (ncar_data == NULL) {
	    printf("Error allocating memory for ncar_data array\n");
	    exit(0);
	}

/*	Calculate starting position of ncar data needed for interpolation */
	start_ncar = row_ncobs * 4 * (rmin - 1);

/*	*** Read only those NCAR data lines needed in bilinear 
	    interpolation of line for study area		****	*/

/*	Seek to start of ncar data */
	fseek (fncar,start_ncar,0);

/*	Read NCAR data */
    fread(&ncar_data[0],size=4,nelems=ncar_ncobs,fncar);

/*	******		PERFORM	INTERPOLATION 	*/
	
/*	for each pixel */
	for (pixel=0;  pixel<npixels; pixel++) {
	  
/*	  Get row,col indices for the pixel */
	  r = row_index[pixel];
	  c = col_index[pixel];
	  
/*	  If pixel is outside of NCAR Grid */
	  if (r == OUTSIDE_NCAR) {

/*	    Set climate values to default values */
	    climate[pixel][0] = DEFAULT_RAD;
	    climate[pixel][1] = DEFAULT_TMIN;
	    climate[pixel][2] = DEFAULT_TMAX;
	    climate[pixel][3] = DEFAULT_PRECIP;
		climate[pixel][4] = DEFAULT_HUM;
	  }

/*	  Else perform interpolation for the pixel */
	  else {

/*	    Shift row index value */
            r = r - rmin +1;

/*	    Calculate position of stations in NCAR data array which
	    will be used in the interpolation */
	    pos_rc = r * row_ncobs + c*stn_ncobs;
	    pos_r_1c = (r-1) * row_ncobs + c*stn_ncobs;
	    pos_r_1c_1 = (r-1) * row_ncobs + (c-1)*stn_ncobs;
	    pos_rc_1 = r * row_ncobs + (c-1)*stn_ncobs;

/*	    Move to climate data of station for specific julian day */
	    pos_rc = pos_rc + jday*ncobs;
        pos_r_1c = pos_r_1c + jday*ncobs;
        pos_r_1c_1 = pos_r_1c_1 + jday*ncobs;
        pos_rc_1 = pos_rc_1 + jday*ncobs;

/*	    For each climate observation */
	    for (i=0; i<ncobs; i++) {
	    
/*	      Get specific observation data for jday,station couple */
	      rc = ncar_data[pos_rc + i];
	      r_1c = ncar_data[pos_r_1c + i];
	      rc_1 = ncar_data[pos_rc_1 + i];
	      r_1c_1 = ncar_data[pos_r_1c_1 + i];

/*	      Perform bilinear interpolation */
	      i_value = (1-dr[pixel])*(1-dc[pixel])*rc + 
		        (1-dr[pixel])*dc[pixel]*rc_1 + 
		        dr[pixel]*dc[pixel]*r_1c_1 + 
		        dr[pixel]*(1-dc[pixel])*r_1c;


/*	      Assign value to array */
	      climate[pixel][i] = i_value;

	    }
	  }
	}


/*      Close Input, Output Files */
        fclose(fncar);
        fclose(frmin);

/*	Free memory */
	free(ncar_data);

}






