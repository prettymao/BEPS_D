/*************************************************************************
  Program:     inter.c
  --------
  Description:
  ----------- 
	Compute rain, snow to ground and to air.
  Details:
  -------
	input:
	b[4]: snow & rain interception coefficient (=0.00025 m/lai/day)
	b[7]: water absorption 
	b[8]: snow absorption 
	z[3]: precipitation
	z[8]: daily total short wave radiation (kj/m2/d)
	z[10]: lai
	z[14]: daytime average temperature (C deg)		
	output:
	g[1]:ground rain 
	g[2]:ground snow
	g[4]:evaporation of precipitation from canopy (m of h2o/ha/day)	
	g[5]:sublimation of precipitation from canopy (m of h2o/ha/day)		
	g[6]:ground rain from canopy
	g[7]:ground snow from canopy
	g[11]:intercepted precip. 
***************************************************************************
  CCRS (EMS/Applications Division)
  Written by:   J. Liu      
  Last update:	December 2000
*****************************************************************************/
	
#include "beps.h" 
  
void inter(b,g,x,z) 
    double  b[], g[], x[], z[];
{
    double lh_evp=2.5E+6; 
	double lh_sub=2.8E+6;
 
	/* fresh snow */
	b[8]=0.05; 

/* intercepted precip */
	g[11]= min(z[3],z[10]* b[4]);

/* rain vs snow */
 
    if (z[14]  > 0.0 ) {    
        g[1] = z[3] - g[11];
		g[2] = 0.0; 
    } 
    else {
	    g[1]=0.0; 
        g[2] = z[3] - g[11];		
     }

/* water loss from canopy */
	if (z[14]  > 0.0 ) {
        g[4] = min(g[11], z[8]*b[7]/lh_evp);
		g[5] =0.0;
    } 
    else {
		g[4] =0.0;
        g[5] = min(g[11], z[8]*b[8]/lh_sub);	
     }

/* canopy water to ground */
	if (z[14]  > 0.0 ) {      
		g[6] = max(0.0, g[11]-g[4]);
		g[7] =0.0;
    } 
    else { 
		g[6]=0.0;
		g[7] = max(0.0, g[11]-g[5]);
     }

    return;
}
 
 
