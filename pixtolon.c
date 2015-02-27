/*****************************************************************************
  Program:     pixtolon.c
  --------
  Description:
  -----------
  This calculates NAD83 long lat coordinates for the CENTER of a pixel.

  Details:
  --------
  Program accepts pixel,line value and adds 1/2 pixel to this value to
  obtain center of pixel.  Long, lat coords are then calculated for 
  center of the pixel.
	
  Warnings:
  --------
  This program assumes that PCI axis are parallel to map projection 
  (i.e. LCC)  axis.  This means that only a shift is required to convert
  pci coordinates into  LCC coordinates.

***************************************************************************
  CCRS (EMS/Applications Division)
  Written by:   J. Liu
  Modified by:	D. Fraser
  Last update:  May 11,  1997
*****************************************************************************/

#include<stdio.h>
#include<math.h>
#include "lcc_nad83.h"
#define pi 3.141592654

void pixtolon(pix,lin,flon,flat)
int long pix,lin;
double  *flat, *flon;	/* output lat-lon */
{  
  	double t,rho,thita;	/* middle values */
	double lat, lon;	/* intermediate lat-lon */
	double temp0,temp1,lattemp;
	double cp_pix, cp_lin;	/* center of pixel, pci coords */
  	double x,y;		/* LCC coordinates of pixel */

/*	Calculate center pixel */
	cp_lin = lin+0.5;
	cp_pix = pix+0.5;
	    
/*	Convert pci (pixel,line) coords to LCC Map projection coords */
	x=cp_pix*pixel_size + xshift;
 	y=(4800-cp_lin)*pixel_size + yshift;

/*	Calculate Geographic Coordinate */
	rho=sqrt(pow(x,2.0)+pow((rho0-y),2.0));
  	thita=atan(x/(rho0-y));
  	t=pow((rho/(a*f)),1/n);
  	lat=90-2*atan(t)*rad2deg;
  
 	redo:
  	      lattemp=lat;
  	      temp0=(1-e*sin(lat*deg2rad))/(1+e*sin(lat*deg2rad));
  	      temp1=pow(temp0, e/2.0);
  	      lat=90-2*atan(t*temp1)*rad2deg;
  
  	      if((lat-lattemp)>=0.0001)
	        goto redo;
 
  	lon=(thita/n+lamda0)*rad2deg;
  		
	*flon = lon;
	*flat = lat;
}
