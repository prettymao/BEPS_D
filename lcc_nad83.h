/*************************************************************************
  program: 	lcc_nad83.h
  Description:  Header file for defining constants in lat/lon calculation 	
		for LCC projection
***************************************************************************
  CCRS (EMS/Applications Division)
  Wrintten by: 	J. Liu/D. Fraser
  Last update:  December 2000
*****************************************************************************/

#include <math.h>

/* NAD83 parameters */
double  a=6378137;			/* Equitorial radius,  GRS 80 */
double  e2=0.00669438;		/* eccentrity squared,  GRS 80 */
double  e=0.081819190903;	/* eccentrity squared,  GRS 80 */

/* LCC map projection parameters */
double phi1=0.855211333589;	/* 1st standard parallel in radians */
double phi2=1.343903524211;	/* 2nd standard parallel in radians */
double phi0=0.0;			/* Latitude for origin of LCC Coordinates,in radians*/			   
double lamda0=-1.658062789611;	/* Longitude for origin of LCC Coordinates,in radians*/
                               
/* Factors for converting radians2degrees etc.  */
double deg2rad=0.017453292522;
double rad2deg=57.295779505601;

/*
Note: 
1) Lamda0, phi0 refered to as reference long, lat by PCI
2) LCC Y axis lies along lamda0,  LCC X axis perpendicular to LCC Y axis
3) LCC projection has 2 standard parallels,  ie phi1,  phi2 
*/


/* Calculation Constants for LCC,NAD83 Map projection */
double m1=0.6573134033;
double m2=0.2256693320;
double t1=0.3757808604;
double t2=0.1146828078;
double t0=1.0000000002;
double n=0.9007900869;
double f=1.7621526918;
double rho0=11239251.285494;

/*  PCI to LCC conversion factors */
double pixel_size = 1000.0;
double xshift = -2600000.0;
double yshift = 5700000.0;

