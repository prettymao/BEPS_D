/*************************************************************************
  Program:     model.c
  --------
  Description:
  ----------- 
	Simulate carbon and water cycles for a single pixel on a day.
***************************************************************************
  CCRS (EMS/Applications Division)
  Written by:   J. Liu      
  Last update:	December 2000
*****************************************************************************/
   
#include "beps.h"
   
void model(jday,pix,day_start,lat_p,lai_p,awc_p,lc_p,x,b,sdat,xx) 
    int long pix;
    int short jday;
    int short day_start;

    double lai_p;
    double lat_p;
    double awc_p;
    int short lc_p;
    double x[];
    double b[];
    struct climatedata sdat[];
    struct xvalue xx[]; 
{
    FILE *output;

    double *g,*gptr;
    int    i;
    
    g= (double *)malloc(SIZEG*sizeof(double));	/* intermediate variables  */
   	
/* blank all intermediate values; all =100 */ 
    gptr = g;
    for (i=0; i<SIZEG; i++) 
   	   *(gptr++) = 0.0;
      
    if(jday==day_start)
    {
        readxx(pix,x,xx);		/* read snowpack and others */
	    setx(lc_p,awc_p,x, b);	/* set x[2]-x[10] */ 		
    }

    /* read & compute forest-bgc daily climate data */ 
    else
    {
		readxx(pix,x,xx);
    }
	
	zcomp(jday,pix,lat_p,lc_p,awc_p,lai_p,b,x,z,sdat); 

	/* daily water and carbon dynamics */ 
 
	doflux(b,g,x,z); 

  	writexx(pix,x,xx); 

/*************** Output site data *********************/	

    if(output_site=='y' || output_site=='Y')
    {
	if (jday==day_start)
	{
   	  if((output=fopen("out.dat", "w")) == NULL)
   	  {
		printf("\n Error in open output file");
		exit(0);
   	  }
	fprintf(output,"JDay, Eva_soil,Eva_total, Trans, ET, OFF, SW, SNP,Precip,RAD,Tmax,Tmin,VPD,LAI,SCsun,SCshade,NPP,Res, Tos_rain, Tos_Mel, Tos_tree, E_tree, Sub_tree, Sub_soil, T_under\n");
	fprintf(output,",     mm, mm,  mm,     mm, mm,  mm, mm,   mm,KJ/m2/day,degC,degC,mbar, ,mm/s, mm/s,gc/m2/d,gc/m2/d, mm,  mm, mm, mm,  mm, mm, mm \n");
	fprintf(output,"JDay, Eva_soil,Eva_total, Trans, ET, OFF, SW, SNP,Precip,RAD,Tmax,Tmin,VPD,LAI,SCsun,SCshade,NPP,Res, Tos_rain, Tos_Mel, Tos_tree, E_tree, Sub_tree, Sub_soil, T_under, nr_sun, nr_shade, nr_under, nr_ground\n");
	fprintf(output,",     mm, mm,  mm,     mm, mm,  mm, mm,   mm,KJ/m2/day,degC,degC,mbar, ,mm/s, mm/s,gc/m2/d,gc/m2/d, mm,  mm, mm, mm,  mm, mm, mm, W/m2,W/m2,W/m2,W/m2 \n");

	}
	else
	{
	  if((output=fopen("out.dat", "a")) == NULL)
   	  {
		printf("\n Error in open output file");
		exit(0);
   	  }
	}

	//printf("\n Printing");

	fprintf(output,"%d,",jday);
	
	fprintf(output," %.2f,",g[12]*1000);		/* evap from soil */
	fprintf(output," %.2f,",(g[4]+g[5]+g[12]+g[13])*1000);	/* total evapo */	
	fprintf(output," %.2f,",(g[17]+g[19])*1000);		/* trans  */
	fprintf(output," %.2f,",(g[4]+g[5]+g[12]+g[13] + g[17]+g[19])*1000); /* et  */
	fprintf(output," %.1f,",g[33]*1000);	/* outflow */

	fprintf(output," %.1f,",xx[pix].x2*1000);	/* soil water */
	fprintf(output," %.1f,",xx[pix].x1*1000);	/* snowpack */

	fprintf(output," %.1f,",z[3]*1000);			/* precip */

	fprintf(output," %.0f,",z[8]);				/* radiation */
	fprintf(output," %.1f,",z[4]);				/* Tax */
	fprintf(output," %.1f,",z[5]);				/* Tmin */
	fprintf(output," %.1f,",z[16]);			/* water pressure deficit */
	fprintf(output," %.1f,",lai_p);				/* LAI */
	
	fprintf(output," %.2f,",g[20]*1000);		/* sc for Leafsun in mm/s  */
	fprintf(output," %.2f,",g[21]*1000);		/* sc for Leafsha in mm/s  */
   
	fprintf(output," %.1f,",g[26]*1000);	/* npp */
	fprintf(output," %.1f,",(g[25]+g[30]+g[31])*1000); /* respiration */   	
	
	fprintf(output," %.1f,",g[1]*1000);	/* ground rain */
	fprintf(output," %.1f,",g[3]*1000);		/* melt	*/
	fprintf(output," %.1f,",g[6]*1000);		/* gound rain from tree */
	fprintf(output," %.1f,",g[4]*1000);		/* evp from tree */
	fprintf(output," %.1f,",g[5]*1000);		/* sublimation from tree */
	fprintf(output," %.1f,",g[13]*1000);	/* sublimation from soil */
	fprintf(output," %.1f,",g[19]*1000);	/* understory transpiration */
	fprintf(output," %2.2f,",z[38]);	/* mean net radiation for sunlit leaves*/
	fprintf(output," %2.2f,",z[39]);	/* mean net radiation for shaded leaves*/
	fprintf(output," %2.2f,",z[40]);	/* net radiation for understory */
	fprintf(output," %2.2f,",z[41]);	/* net radiation for ground */
	
	
   /* fprintf(output," %f",z[18]); 	 daylength */	
			
	fprintf(output, "\n");
	fclose(output);	
    }	
/***********************************************************************/
	free(g);
   	return;		
} 


void setx(lc_p,awc_p,x,b) 
    int short lc_p;
    double awc_p;  
    double x[], b[];
    {  
//	x[2]=awc_p*1.1;

/* Set x[3] to x[7], x[11]-x[16] to zero */ 

    x[3]=0.0; 
    x[4]=0.0; 
    x[5]=0.0; 
    x[6]=0.0; 
    x[7]=0.0; 
  
    x[11]=0.0; 
    x[12]=0.0;
	x[13]=0.0;
	x[14]=0.0;
	x[15]=0.0;
	x[16]=0.0;
	x[17]=0.0;
	x[18]=0.0;

	 switch(lc_p)
    {	
	case 1: case 2: case 3: case 4: case 5: /* coni	*/
		if (x[9]==0) x[9]=10.1;				/* stem biomass Canada average in Inv.*/
   		x[10]=0.2317*x[9];					/* root biomass in t/h b */
		break;
	case 6:  /* deci */
		if (x[9]==0) 	x[9]=8.8;			/* stem biomass Canada average in Inv. */
   		x[10]=exp(0.359)*pow(x[9],0.639);	/* root biomass in t/h */
		break;
	case 7: case 8: case 9: case 10:		/* mixe */
    	if (x[9]==0) x[9]=8.1;				/* stem biomass Canada average in Inv. */
   		x[10]=0.5*(0.2317*x[9]+exp(0.359)*pow(x[9],0.639));	/* root biomass in t/h */
		break;
  	default: 
		x[9]=1.0;							
   		x[10]=0.2;									
    }
} 



