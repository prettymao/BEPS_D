/*************************************************************************
  Program:     rsoil.c
  --------
  Description:
  ----------- 
	Output soil conductance/resistance to water, based on BIOME-BGC.
***************************************************************************
  CCRS (EMS/Applications Division)
  Written by:   J. Liu      
  Last update:	May 1998
*****************************************************************************/	
#include "beps.h" 
 
void rsoil(b,g,x,z) 
    double b[], g[], x[], z[];
{ 
	double rsw;		/* relative soil moisture */
    
    rsw=100*x[2]/z[22];
   
    g[41]=0.002; 		/* default value */
    
	 if(rsw<20.0) 				g[41]=0.0000001;
    if(rsw>20.0 && rsw<=30.0) 	g[41]=0.00001;
    if(rsw>30.0 && rsw<=40.0) 	g[41]=0.00006;
    if(rsw>40.0 && rsw<=50.0) 	g[41]=0.000125; 
    if(rsw>50.0 && rsw<=60.0)	g[41]=0.00025;
    if(rsw>60.0 && rsw<=70.0) 	g[41]=0.0005;
    if(rsw>70.0 && rsw<=80.0)	g[41]=0.001;
    if(rsw>80.0) 				g[41]=0.002;
    g[41]=min(0.002,g[41]);
	
    return;
}
