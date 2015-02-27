/*************************************************************************
  Program:     readconf.c
  --------
  Description:
  -----------
  Reads control file for NPP calculations.  The control file
  defines the area in which NPP will be calulated and the data
  files used in the calculations.

***************************************************************************
  CCRS (EMS/Applications Division)
  Written by:	D. Fraser/J. Liu  
  Last update: June, 1998 
*****************************************************************************/
#include <stdio.h>
#include "beps.h"

void readconf(conf,rcode)
char *conf;		/* control file name */
int *rcode;		/* Return code */
{

	char field_descr[255];	/* Field description in control file */
	FILE *fcon;		/* Control file pointer */

/*      If can not open control parameter file */
        if ((fcon = fopen(conf,"r")) == NULL) {
          printf("Can not open parameter file <%s>, exitting program ...\n",
                                                                conf);
          *rcode = ERROR;
	  return;
        }

/*	Read control file parameters */
    field_descr[0]='\0';
    fscanf(fcon,"%s %d", field_descr, &jday_start);
    field_descr[0]='\0';
    fscanf(fcon,"%s %d", field_descr, &jday_end);
    field_descr[0]='\0';
	fscanf(fcon,"%s %d", field_descr, &cycle_display);
    field_descr[0]='\0';
	fscanf(fcon,"%s %f", field_descr, &factor);
    field_descr[0]='\0';
	fscanf(fcon,"%s %d", field_descr, &lin_offset);
    field_descr[0]='\0';
	fscanf(fcon,"%s %d", field_descr, &nlines);
    field_descr[0]='\0';
	fscanf(fcon,"%s %d", field_descr, &pix_offset);
    field_descr[0]='\0';
	fscanf(fcon,"%s %d", field_descr, &npixels);
	field_descr[0]=awcf[0]='\0';
	fscanf(fcon,"%s %s", field_descr, awcf);
	field_descr[0]=landcoverf[0]='\0';
	fscanf(fcon,"%s %s", field_descr, landcoverf);
	field_descr[0]=leafareaf[0]='\0';
	fscanf(fcon,"%s %s", field_descr, leafareaf);
	field_descr[0]=snowpackf[0]='\0';
	fscanf(fcon,"%s %s", field_descr, snowpackf);
	field_descr[0]=ncarbinf[0]='\0';
	fscanf(fcon,"%s %s", field_descr, ncarbinf);
	field_descr[0]=rowf[0]='\0';
	fscanf(fcon,"%s %s", field_descr, rowf);
	field_descr[0]=colf[0]='\0';
	fscanf(fcon,"%s %s", field_descr, colf);
	field_descr[0]=deltarowf[0]='\0';
	fscanf(fcon,"%s %s", field_descr, deltarowf);
	field_descr[0]=deltacolf[0]='\0';
	fscanf(fcon,"%s %s", field_descr, deltacolf);
	field_descr[0]=rowminf[0]='\0';
	fscanf(fcon,"%s %s", field_descr, rowminf);
	field_descr[0]='\0'; 
	fscanf(fcon,"%s %c", field_descr, &output_NPP);	
	field_descr[0]='\0'; 
	fscanf(fcon,"%s %c", field_descr, &output_res);
	field_descr[0]='\0';
	fscanf(fcon,"%s %c", field_descr, &output_eva);	
	field_descr[0]='\0';
	fscanf(fcon,"%s %c", field_descr, &output_tra);
	field_descr[0]='\0';
	fscanf(fcon,"%s %c", field_descr, &output_site);

	
/*	Output control parameters */
printf("\n Control Parameters\n\n");
printf(" Julian day_start [1,365]: %d\n",jday_start);
printf(" Julian day_end [1,365]: %d\n",jday_end);
printf(" Cycle display [1,365]: %d\n",cycle_display);
printf(" NPP multiplication factor [0.1,10]: %f\n",factor);
printf(" Line offset (PCI coordintate system) [0,4799]: %d\n",lin_offset);
printf(" Number of Lines (PCI coordintate system) [1,4800]: %d\n",nlines);
printf(" Pixel offset (PCI coordintate system) [0,5699]: %d\n",pix_offset);
printf(" Number of pixels (PCI coordintate system) [1,5700]: %d\n",npixels);
printf(" Available soil water holding capacity:  %s\n",awcf);
printf(" Land cover data file:  %s\n",landcoverf);
printf(" LAI data file:  %s\n",leafareaf);
printf(" Snowpack data file:  %s\n",snowpackf);
printf(" NCAR binary data file:  %s\n",ncarbinf);
printf(" Row file:  %s\n",rowf);
printf(" Column file:  %s\n",colf);
printf(" Delta row file:  %s\n",deltarowf);
printf(" Delta column file:  %s\n",deltacolf);
printf(" Row minimum file:  %s\n",rowminf);
printf(" OutputNPP?  %c\n",output_NPP);
printf(" Output autorespiration?  %c\n",output_res);
printf(" Output evapiration?  %c\n",output_eva);
printf(" Output transpiration?  %c\n",output_tra); 
printf(" Output site file?  %c\n",output_site); 

fclose(fcon);

}



