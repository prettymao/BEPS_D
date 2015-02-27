/*************************************************************************
  Program:     melt.c
  --------
  Description:
  ----------- 
	Compute snowmelt.
  Details:
  -------
	input:
	b[6]: snow melt temp. coefficient (m/C/day)(=0.001) 
	b[8]: snow aborptivity 
	x[1]: snowpack (m)
	z[8]: shortwave radiation at forest floor (kj/m2/d)
	z[14]: daytime average temperature (C deg)		
	output:
	g[3]: total melt (m)
	g[8]: radiation melt (m)
	g[9]: temperature melt (m)
	g[13]: sublimation from the snow at ground (m)
***************************************************************************
  CCRS (EMS/Applications Division)
  Written by:   J. Liu     
  Last update:	December 2000
*****************************************************************************/
#include "beps.h" 

void melt (b, g, x, z) 
    double b[], g[], x[], z[];
    {
	double lh_fus=3.5E+5;
	double lh_evp=2.5E+6; 
	double lh_sub=2.8E+6;

/*	b[8]: snow absorption */
	b[8]=0.3/3.0;
//	b[8]=0.3;


    if (z[14] > 0.0) { 
	    g[8] = z[8]*b[8]/lh_fus;	/* radiation  melt */ 
		g[9] = b[6] * z[14];		/* temperature melt */ 
		g[3] = min (x[1], (g[8]+g[9]));
		g[13] =0.0;
	}
    else { 
		g[8]=0.0;
		g[9]=0.0;
		g[3]=0.0;
		g[13] = min(x[1], ((z[8]-z[19])*b[8]/lh_sub));
	}
//	printf("g[8]=%6.4f \n", g[8]*1000);
    return;
  } 
 
