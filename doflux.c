/*************************************************************************
  Program:     doflux.c
  --------
  Description:
  ----------- 
	Calculate and update the fluxes in carbon and water cycles.
***************************************************************************
  CCRS (EMS/Applications Division)
  Written by:   J. Liu      
  Last update:	December 2000
*****************************************************************************/
	 
#include "beps.h"  
double penmon();

void doflux(b,g,x,z)
    double b[], g[], x[], z[];
    {          

	double soilw_in=0;
	double soilw_out=0;
	b[34]=z[22]*0.1;	/* Field capacity */
	b[35]=z[22]*1.1;	/* Field capacity */
	b[36]=z[22]*2.0;   	/* Saturation */


/*  canopy interception */
    inter(b, g, x, z);    

/*  snowmelt routine  */
    melt (b, g, x, z);

/*  resistance */
	/*ra(b, g, x, z); */
//	g[40]=0.22;
/*case 23: case 24: case 25: case 26: case 27: case 28:	 agricultural land */		  
/*	case 16:	 pasture */
	if( (z[23]<=23 && z[23]<=28) || (z[23]==16)) g[40]=1.0/30.0;
	else
		g[40]=1.0/5.0;

	rs(b, g, x, z);
	
    rsoil(b, g, x, z); 
	  
	carbon(b, g, x, z); 
   
/* Penman - Monteith estimate of transpiration, overstory */

	g[16]=		penmon(z[14],z[16],z[38],g[40],g[20])*z[32];
	g[16]=g[16]+penmon(z[14],z[16],z[39],g[40],g[21])*z[33]; 
	
//	g[16]= penmon(z[14],z[16],z[20],g[40],g[22]); 
	
  
/* canopy transpiration  */
	g[17] = g[16]* z[18];

/* Penman - Monteith estimate of evaporation from soil */
	/* if snowpack>0 or snow/ice area  evp_soil=0 */
	if((z[14]<0) || (z[23]==31)) g[12]=0.0;
	else
		g[12]= z[18]*penmon(z[14],z[16],z[41],g[40],g[41]); 

/* Urban area */
	if (z[23]==29) g[12]=0.8*g[12];
	
	
/* Penman - Monteith estimate of evaporation from understory */
/* if LAI_u z[42]=0, temperature z[14]<0 or  ice z[23]=31, trans_under=0 */
	if( ((z[14]<0) || (z[23]==31)) || z[42]==0) g[19]=0.0;
	else
		g[19]= z[18]*penmon(z[14],z[16],z[40],g[40],g[19]); 
 // printf("\n Tunder=%f",g[19]*1000.0);
 
/* ********** Water update *****************/

/*  snow = snow + ground snow + snow from canopy - melt - sublimation */
    x[1] = x[1] + g[2]+ g[7] - g[3]- g[13]; 
 
/* swc[i]=swc[i-1] + ground rain + snowmelt + rain from tree - evap from soil-trans*/
 
	soilw_in = g[1]+ g[3] + g[6];
	soilw_out = g[12]+g[17]+g[19];
	x[2] = x[2] + soilw_in - soilw_out;

/* outflow=soil moisture - filed capacity -wilting point */   
    g[33]=max(0.0, (x[2]-b[35]));

/* if soil water > filed holding capacity: b[35], set water=cap */
    
    if(x[2]>b[35]) 
	x[2]=b[35];

 /* if soil water goes negative set, b[34]: wilting point   */ 
    if(x[2] < b[34]) {
    	x[2] = b[34];  
/*    	printf("Available soil water (%8.5f) is less than zero.\n",x[2]); */	
    }

   
/*  outflow = outflow + new outflow  */

    x[3] = x[3] + g[33];

/*	trans = trans + new trans */

    x[4] = x[4] + g[17] +g[19]; 
 
/*  evapo = evapo + evap_canopy + sublimatio_canopy + evap_soil + sublimation_snow */

   x[5] = x[5] + g[4]+g[5]+g[12]+g[13];  
   
/*  understory = trans_under + new trans_under */

   x[18] = x[18] + g[19];   

/********* carbon update **********/ 

    x[6] = x[6] + g[26]; 				/* acumulated NPP */

//   x[7] = x[7] + (g[25]+g[30]+g[31]); 	/* acumulated total Rm */
//	x[14] = x[14] + g[25]; 				/* acumulated leaf Rm */
//	x[15] = x[15] + g[30]; 				/* acumulated stem Rm */
//	x[16] = x[16] + g[31]; 				/* acumulated root Rm */


//    x[11]= x[11]+ g[24];				/* acumulated gpp */

/********* For this run only, uptake E **********/ 

	x[7] = x[7] +g[4]; 					/* E_can */
	x[14] = x[14] + g[5]; 				/* S_can */
	x[15] = x[15] + g[12]; 				/* E_soil */
	x[16] = x[16] + g[13]; 				/* S_soil */

    return;
}





