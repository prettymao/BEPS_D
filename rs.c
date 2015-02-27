/*************************************************************************
  Program:     rs.c
  --------
  Description:
  ----------- 
	Output stomatal resistance/conductance.
***************************************************************************
  CCRS (EMS/Applications Division)
  Written by:   J. Liu   
		(Based on BIOME-BGC)   
  Last update:	May 1998
*****************************************************************************/
	
#include "beps.h" 
 
void rs(b,g,x,z) 
    double b[], g[], x[], z[];
    {  

/* declaration for temporary variables/functions */
	double ppfd;
 	double tavg;
	double tmin;
	double topt, tmax;
	double vpd,vpd_open,vpd_close;
	double psi,psi_open,psi_close;
	double m_tavg, m_tmin, m_psi, m_co2, m_ppfd, m_vpd; 
	double m_most;
	double lai;

	double ppfd_coef;	
	double cs,cc,cbl;
	double i;	/* index for 0:sunlit leaves and 1:shaded leaves */
	double lai_under;
	int short lc_p;

	/* assign variables that are used more than once */
  	tavg =      z[14];
	tmin =      z[5];
	vpd =       0.1*z[16];		/* in kPa */
	ppfd_coef=  0.01;
	topt=20;
	tmax=40;
	psi =       1;
	lai =       z[10];

	psi_open =  -1;			/* in -MPa */
	psi_close = -8;
	vpd_open =  7.5;		/* in mb */
	vpd_close = 20.0;
	lc_p=(int short)z[23];

	switch(lc_p)
	{
	case 1: case 2: case 3: case 4: case 5: /* conifer */
		lai_under=1.175*exp(-0.991*z[10]);
       	break;	
	case 6:  				/* deciduous forest */
		lai_under=1.5;
       	break;
	case 7: case 8: case 9: case 10: /* mixe */
		lai_under=0.5*(1.5+1.175*exp(-0.991*z[10]));
		break;	
	default:
		lai_under=0.0;
  	}													
   
 for(i=0;i<=1; i++)
   {
        if (i==0) ppfd =0.5*4.55*z[36];
        else
		ppfd=0.5*4.55*z[37];
    
	m_ppfd = ppfd * ppfd_coef / (1.0 + (ppfd * ppfd_coef));

	/* use the tempcurve function to generate the daily average air 
	temperature conductance multiplier */

	if(tavg<0 || tavg>40)
		m_tavg=0.0;
	else
	{
		if(tavg<topt)
			m_tavg=log(tavg+1)/log(topt+1);
		else
			m_tavg=cos(3.1415926*(tavg-topt)/(2*(tmax-topt)));
	}


	/* soil-leaf water potential multiplier */
	
	/* leaf water potential pre-dawn  */ 
 
    psi = -b[27]/(x[2]/(1.1*z[22]));
		
		
	if (z[23]>=1 && z[23]<=5) psi_close=-4;
		else psi_close=-8;
	m_psi = (psi_close - psi) / (psi_close - psi_open);

//	m_psi=m_psi*1.0;

	/* if soil is very dry, m_psi=0 */
	if(x[2]==z[22]*0.1) m_psi=0.001; 

	m_psi=max(0, m_psi);
	m_psi=min(1, m_psi);

	/* CO2 multiplier */
	m_co2 = 1.0;

	/* freezing night minimum temperature multiplier */
	if (tmin > 0.0)        /* no effect */
		m_tmin = 1.0;
	else
	if (tmin < -8.0)       /* full tmin effect */
		m_tmin = 0.0;
	else                   /* partial reduction (0.0 to -8.0 C) */
		m_tmin = 1.0 + (0.125 * tmin);
	
	/* vapor pressure deficit multiplier, vpd in Pa */

	/* Deciduous lc=6 , mixed lc=7,8,9,10*/
	if (z[23]==6) m_vpd=1/(1+0.429*vpd);
	else if ((z[23]==7 || z[23]==8 ) || (z[23]==9 || z[23]==10))
		m_vpd=0.5*(1/(1+0.429*vpd)+ 1/(1+0.076*vpd));
	else
		m_vpd=1/(1+0.076*vpd); 	
        

	/* apply all multipliers to the maximum stomatal conductance */
	m_most= m_tavg * m_psi * m_co2 * m_tmin * m_vpd;

	//	m_most=1;
	cs =b[11]*m_ppfd*m_most;
	
	/* leaf boundary-layer conductance */
	cbl=0.08;

	/* leaf cuticular conductance */
	cc=0.00005;

	/* final stomatal conductance for sunlit and shaded leaves  */

    if (i==0) g[20] = cbl*(cc+cs)/(cbl+cc+cs); 
	else
	  g[21] = cbl*(cc+cs)/(cbl+cc+cs); 

    }
	/* canopy conductance */
         g[22]=g[20]*z[32]+g[21]*z[33];

	/* canopy conductance for big leaf model */
         
		ppfd=0.5*4.55*z[20];
    
		m_ppfd = ppfd * ppfd_coef / (1.0 + (ppfd * ppfd_coef));
		
		g[22]=z[10]*b[11]*m_ppfd*m_most;

	/* understory conductance */
		ppfd=0.5*4.55*(z[9]-z[20]);
    
		m_ppfd = ppfd * ppfd_coef / (1.0 + (ppfd * ppfd_coef));
		g[19]=lai_under*b[39]*b[11]*m_ppfd*m_most;
		
    return;
} 
