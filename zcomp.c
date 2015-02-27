/*************************************************************************
  Program:     zcomp.c
  --------
 	 Description:
	 ------------ 
	 Calculate z variables.
	 
	 Details:
	 -------

	Z 	Description
	-------------------------------------------------------------

   	2	day of year  
   	3	precipitation 				(m)  
	4 	max. Air temperature 		(deg c) 	
 	5  	min. Air temperature 		(deg c)    
   	6  	relative humidity			(%)  
   	7  	soil temperature, ave 24 hr temp   	(deg c)  
   	8  	daily total incoming solar radiation  (kj/m2/d)
   	9  	average incoming solar radiation	(W/m2)		
	10	LAI

    14 	daylight average air temperature 	(deg c)  
    15 	average night min. temperature  	(deg c)  
    16 	vapor pressure deficit        		(mb)  
    17 	absolute humidity deficit   		(mic. gm/m3)  
   	18 	running periods (daylength for daily model)  (sec)  
   	19 	daily total absorbed radiation    	(kj/m2/d)
	20	daily averaged absorbed radiation  	(W/m2)
       		
	22	available water capacity 	 	(m)
	23	land cover type

	26	leaf nitrogen concentration  fraction	

	30	Cos(Theta_noon)
	31	Cos(Theta_mean)
	32	sunlit LAI 
	33	shaded LAI
	34	daily mean radiation over sunlit leaves (W/m2)
	35	daily mean radiation over shaded leaves (W/m2)
	36	radiation over sunlit leaves at noon (W/m2)
	37	radiation over shaded leaves at noon (W/m2)
	38	net radiation for sunlit leaves (W/m2)
	39	net radiation for shaded leaves (W/m2)
	40	net radiation for understory (W/m2)
	41	net radiation for ground (W/m2)
	42  understory LAI
***************************************************************************
  CCRS (EMS/Applications Division)
  Written by:   J. Liu      
  Last update:	May 2000
*****************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"beps.h"

void zcomp(jday,pix,lat_p,lc_p,awc_p,lai_p,b,x,z,sdat) 
    int short jday; 
    int long pix;
    double lat_p;
    int short  lc_p;
    double awc_p;
    double lai_p;
    double   x[], z[], b[];
    struct climatedata sdat[];
    { 
    double   xtmax, xtmin, xrad, xppt, tave, q; 
    double   esd, es, ampl, day, xd; 

    double gfn;			/* gap function at noon */
    double omega;		/* clumping index */
    double theta_m;		/* solar zenith angle at noon */
    double alfa;
    double ssun;		/* radiation for sunlit leaves */
    double sshade;		/* radiation for shaded leaves */  

	// for shortwave radiation
	double lai_u;			// understory lai
	double n_ssun;			// net shortwave radiation for sun lit leaves
	double n_sshade;		// net shortwave radiation for sun lit leaves
	double n_sunder;		// net shortwave radiation for understory
	double n_sground;		// net shortwave radiation for gound

	// for longwave radiation
	double m_lnet_o;		// mean longwave radiation for overstory
	double m_lnet_u;		// mean longwave radiation for understory
	double lnet_g;			// net longwave radiation for ground

    xtmax = sdat[pix].tmax;
    xtmin = sdat[pix].tmin;
	/*****************************Area Model*******************************
    q	  = sdat[pix].dewp;			//note: In this program, it is specific humidity in g/kg
    xrad  = sdat[pix].srad*0.676;	//note: 0.676 is for this data set only
	******************************Site Model*********************************
	q	  = sdat[pix].dewp;			//note: In this program, it is RH
    xrad  = sdat[pix].srad;			//note: 0.676 is not needed
	**************************************************************************/
    q	  = sdat[pix].dewp;			//note: In this program, it is specific humidity in g/kg
    xrad  = sdat[pix].srad*0.676;	//note: 0.676 is for this data set only
	xppt  = sdat[pix].precip;

    z[2] = jday;
 
/* convert ppt. Mm to meters */       
    z[3] = xppt / 1000.0;
 
    z[4] = xtmax; 
    z[5] = xtmin; 
 
    tave = (z[4]+z[5])/2.0; 
 
    if (jday <= 1) 
	z[1] = b[22];

    z[1] = max((z[1]+tave), b[22]);
 
/* average temperatures, soil, air */ 
    
    z[7]  = tave;

/* if snowpack exists, soil temp defined as 0 deg */ 

    if (x[1] > 0.0) 	
	z[7] = 0.0;

/* leaf area index one sides */ 
    z[10]  =lai_p;
     
/* daylight mean temp. z[14], nighttime mean temp. z[15] */
    z[14]  =  0.212 * (z[4] - tave) + tave;
    z[15]  =  (z[14] + z[5])/2.0; 
/*****************************Area Model*******************************
//absolute humidity z[17], in g/m3 dailymean  
	z[17]= (1.2833-0.0045*tave)*q;

// vapour pressure in mbar, dayly mean
	es =0.46*q*(tave+273.16)/100;	

// vpd z[16], daylight, in mbar 
    esd  =  6.1078 * exp((17.269*z[14])/(237.3 + z[14]));
	z[16]  =  max((esd-es),0.0);	

// Relative humidity 
	z[6]= 100*es/esd;		
*******************************Site Model***********************************

/* Relative humidity 
	z[6]= q;	

/* vpd z[16], daylight, in mbar 
    esd  =  6.1078 * exp((17.269*z[14])/(237.3 + z[14]));
	es=esd*(q/100);
	z[16]  =  max((esd-es),0.0);	
****************************************************************************/
// absolute humidity z[17], in g/m3 dailymean  
	z[17]= (1.2833-0.0045*tave)*q;

// vapour pressure in mbar, dayly mean
	es =0.46*q*(tave+273.16)/100;	

// vpd z[16], daylight, in mbar 
    esd  =  6.1078 * exp((17.269*z[14])/(237.3 + z[14]));
	z[16]  =  max((esd-es),0.0);	

// Relative humidity 
	z[6]= 100*es/esd;

/* compute daylength in seconds */ 
    xd  =  (double) jday - 79.0;

    if (xd < 0.0) 
	xd = 286.0 + (double) jday;

    ampl = exp(7.42+0.045*lat_p)/3600.0; 
    day  = ampl * (sin(xd*0.01721)) + 12.0; 
    z[18]  =  day * 3600.0 * 0.85; 
//	z[18]  =  day * 3600.0; 

/* total incoming solar radiation in kj/m2/s */ 
   z[8]  = xrad ;

/* average incoming solar radiation in watt/m2 */ 

   z[9] = 1000* z[8]/z[18];

/* FPAR */ 
   theta_m=(lat_p-23.5*sin((jday-81)*2*PI/365))*PI/180;
   
   if (fabs(PI/2-theta_m)<0.01)	alfa=0.05;
 
	else alfa=(cos(theta_m)-(PI/2-theta_m)*sin(theta_m))/((PI/2-theta_m)*(1-sin(theta_m)));
 
	switch(lc_p)
    {
	case 7: case 8: case 9: case 10: /*mixe =0.6 */
		omega=0.6;
		break;
	case 6: /*deci =0.7 */
		omega=0.7;
		break;
	case 1: case 2: case 3: case 4: case 5: /*coni=0.5 */
		omega=0.5;
		break;
	case 16:	/* grass=0.9 */
		omega=0.9;
		break;
	case 23: case 24: case 25: case 26: case 27: case 28:	/* agri=0.9 */
		omega=0.9;
		break;
	default:
		omega=1.0;
    }

	if (fabs(PI/2-theta_m)<0.01)	gfn=0.1;
	else
	   gfn = exp(-0.4*omega*z[10]/cos(theta_m));

//	z[19]= z[8]*(0.95-0.94*alfa*gfn);

    z[19]= max(0.0,(0.95-0.94*alfa*gfn));
	z[19]=z[8]*z[19];

/*  daily averaged absorbed radiation in W/m2 */
    z[20]=1000*z[19]/z[18];

/* leaf nitrogen */ 
    z[21] = b[26];

/* awc */
    z[22]=awc_p;
 
/* land cover */
    z[23]=(double)lc_p;

/* cos(Thita_m)  */

  /* cos(Thita_m)  */
    z[30]=cos(theta_m);

/* cos(Thita_avg) */
    z[31]=cos(PI/8+3*theta_m/4);

/* average sunlit leaves */
	if (z[31]<0.01)
		z[32]=0;
	else
		z[32]=2*z[31]*(1-exp(-0.5*omega*lai_p/z[31]));

/* average shaded leaves */
    z[33]=lai_p-z[32];

/* daily averaged radiation for sunlit and shaded leaves */
    rad_ssl(z[9],z[31],z[10],omega,&ssun,&sshade); 
	z[34]=ssun;
    z[35]=sshade; 

/*  radiation for sunlit and shaded leaves at noon*/
    rad_ssl(z[9]*PI/2,z[30],z[10],omega,&ssun,&sshade); 
	z[36]=ssun;
    z[37]=sshade; 
	
    /*printf("z[36]=%f",z[36]); */


	// understory lai
	switch(lc_p)
	{
	case 1: case 2: case 3: case 4: case 5: /* conifer */
		lai_u=1.175*exp(-0.991*z[10]);
       	break;	
	case 6:  				/* deciduous forest */
		lai_u=1.5;
       	break;
	case 7: case 8: case 9: case 10: /* mixe */
		lai_u=0.5*(1.5+1.175*exp(-0.991*z[10]));
		break;	
	default:
		lai_u=0.0;
  	}
	
   	// net shortwave radiation calculation
	net_shortwave(z[9],z[31],z[10],omega,&ssun,&sshade,lai_u,&n_ssun,&n_sshade,&n_sunder,&n_sground);

	// net longwave radiation calculation

	// z[14]: daytime average temperature
	net_longwave(lai_p,lai_u,omega,es,(z[14]+275.0),&m_lnet_o,&m_lnet_u,&lnet_g);
	z[38]=n_ssun+m_lnet_o;
	z[39]=n_sshade+m_lnet_o;
	z[40]=n_sunder + m_lnet_u;
	z[41]=n_sground + lnet_g;
	z[42]=lai_u;
/*	printf("\nz[38]=%f",z[38]); 
	printf("z[39]=%f",z[39]); 
	printf("z[40]=%f",z[40]); */
    return;
}

int rad_ssl(sg,cos_theta,lai_p,omega,ssun,sshade)
    double sg;			/* global radiation W m-2 */
    double cos_theta;		/* solar zenith angle */
    double lai_p;		/* LAI */
    double omega;		/* clumping index */
    double *ssun;		/* radiation for sunlit leaves */
    double *sshade;		/* radiation for shaded leaves */    
{		   
    double theta_avg; 		/* Mean cos(thita) */
    double s0;			/* solar constant (=1367 W m-2) */
    double rr;			/* ratio of sdif_over to sg */
    double sdir;		/* direct radiation W m-2 */
    double sdif_over;		/* diffusive radiation over plant canopy */
    double sdif_under;		/* diffusive radiation under plant canopy */
    double c;			/* radiation from multiple scattering */

/************* calcuate sdir and sdif_over ********************/

    s0=1367;

    if (cos_theta<0.01)
	{
		sdif_over=0;
		sdir=0;
	}
	else
	{
		rr=sg/(s0*cos_theta);
		if (rr>0.8) sdif_over=0.13*sg;
		else
		sdif_over=sg*(0.943+0.732*rr-4.9*pow(rr,2)+1.796*pow(rr,3)+2.058*pow(rr,4)); 		
		sdir=sg-sdif_over;
	}
		
/************* calculate ssun and sshade ***********************/

/* radiation from multiple scattering */
	c=0.07*omega*sdir*(1.1-0.1*lai_p)*exp(-cos_theta);
	
/* sdif_under */
    theta_avg=0.537+0.025*lai_p;
    sdif_under=sdif_over*exp(-0.5*omega*lai_p/theta_avg);
	
/* radiation for shaded leaves */
    if ((lai_p<0.01) && (sdif_over-sdif_under)<0)
		*sshade=0;
	else	
		*sshade=(sdif_over-sdif_under)/lai_p+c;

/* radiation for sunlit leaves */
    if (cos_theta <0.01)
		*ssun=0;
	else
		*ssun=0.5*sdir/cos_theta +*sshade;

    return 1;
}

int net_shortwave(sg,cos_theta,lai_p,omega,ssun,sshade,lai_u,n_ssun,n_sshade,n_sunder,n_sground)
    double sg;			/* global radiation W m-2 */
    double cos_theta;		/* solar zenith angle */
    double lai_p;		/* LAI */
    double omega;		/* clumping index */
    double *ssun;		/* radiation for sunlit leaves */
    double *sshade;		/* radiation for shaded leaves */ 
	double lai_u;		/* understory lai */
	double *n_ssun;			// net shortwave radiation for sun lit leaves
	double *n_sshade;		// net shortwave radiation for sun lit leaves
	double *n_sunder;		// net shortwave radiation for understory
	double *n_sground;		// net shortwave radiation for gound

{		   
    double theta_avg; 		/* Mean cos(theta) */
    double s0;			/* solar constant (=1367 W m-2) */
    double rr;			/* ratio of sdif_over to sg */
    double sdir;		/* direct radiation W m-2 */
    double sdif_over;		/* diffusive radiation over plant canopy */
    double sdif_under;		/* diffusive radiation under plant canopy */
    double c;			/* radiation from multiple scattering */
	double c_for_net;
	double alpha_l=0.25;
	double alpha_g=0.2;
	double theta_avg_under; /* Mean cos(theta) for understory */

/************* calcuate sdir and sdif_over ********************/

    s0=1367;

    if (cos_theta<0.01)
	{
		sdif_over=0;
		sdir=0;
	}
	else
	{
		rr=sg/(s0*cos_theta);
		if (rr>0.8) sdif_over=0.13*sg;
		else
		sdif_over=sg*(0.943+0.732*rr-4.9*pow(rr,2)+1.796*pow(rr,3)+2.058*pow(rr,4)); 		
		sdir=sg-sdif_over;
	}
		
/************* calculate ssun and sshade ***********************/

/* radiation from multiple scattering */
	c=0.07*omega*sdir*(1.1-0.1*lai_p)*exp(-cos_theta);
	
/* sdif_under */
    theta_avg=0.537+0.025*lai_p;
    sdif_under=sdif_over*exp(-0.5*omega*lai_p/theta_avg);
	
/* radiation for shaded leaves */
    if ((lai_p<0.01) && (sdif_over-sdif_under)<0)
		*sshade=0.0;
	else	
		*sshade=(sdif_over-sdif_under)/lai_p+c;

/* radiation for sunlit leaves */
    if (cos_theta <0.01)
		*ssun=0.0;
	else
		*ssun=0.5*sdir/cos_theta +*sshade;
	
/* net short radiation for shaded leaves */
	c_for_net=alpha_l*omega*sdir*(1.1-0.1*lai_p)*exp(-cos_theta);;
    if ((lai_p<0.01) && (sdif_over-sdif_under)<0.0)
		*n_sshade=0;
	else	
		*n_sshade=(sdif_over-sdif_under)/lai_p+c_for_net;
		
/* net short radiation for sun lit leaves */
	*n_ssun=(1-alpha_l)**ssun + *n_sshade;

/* net short radiation for understory */
	*n_sunder=(1-alpha_l)*(sdir*exp(-0.5*omega*lai_p/cos_theta)+sdif_under);

/* net short radiation for ground */
	theta_avg_under=theta_avg=0.537+0.025*lai_u;
	*n_sground=(1-alpha_g)*(sdir*exp(-0.5*omega*(lai_p+lai_u)/cos_theta)+sdif_under*exp(-0.5*omega*lai_u/theta_avg_under));

    return 1;
}

int net_longwave(lai_o,lai_u,omega,es,ta,m_lnet_o,m_lnet_u,lnet_g)
	double lai_o;
	double lai_u;
	double omega;
	double es;
	double ta;
	double *m_lnet_o;
	double *m_lnet_u;		// mean longwave radiation
	double *lnet_g;			// longwave radiation for soil
{
	double sigma;			//Stefan-Boltzmann constant =5.67*10^(-8) W m^-2 K^-4
	double epsilon_a, epsilon_o,epsilon_u, epsilon_g;
	double to,tu,tg;		//temperature, in K
	double l_a,l_o,l_u,l_g;				// longwave radiation
	double r_ctheta_o, r_ctheta_u; //representive zenith angle
	double exponent_o, exponent_u;
	double lnet_o, lnet_u;

	sigma=5.67/100000000;

	epsilon_o=0.98;
	epsilon_u=0.98;
	epsilon_g=0.95;
	epsilon_a=1.24*pow((es/ta), (1.0/7.0));  // es in mbar, ta in K
//	epsilon_a=0.7;

	to=ta;	// in K
	tu=ta;
	tg=ta;

	// longwave radiation
	l_a=epsilon_a*sigma*pow(ta,4.0);
	l_o=epsilon_o*sigma*pow(to,4.0);
	l_u=epsilon_u*sigma*pow(tu,4.0);
	l_g=epsilon_o*sigma*pow(tg,4.0);
	
	// represntive angle
	r_ctheta_o=0.573+0.025*lai_o;
	r_ctheta_u=0.573+0.025*lai_u;

	exponent_o=exp(-0.5*lai_o*omega/r_ctheta_o);
	exponent_u=exp(-0.5*lai_u*omega/r_ctheta_u);

	//net longwave radiation
	lnet_o=(l_a + l_u* (1-exponent_u) + l_g* exponent_u - 2*l_o)*(1-exponent_o);
	lnet_u=(l_a*exponent_o + l_o* (1-exponent_o) + l_g - 2*l_u)*(1-exponent_u);
	*lnet_g=(l_a*exponent_o + l_o* (1-exponent_o))*exponent_u + l_u*(1-exponent_u) - l_g ;

	if (lai_o>0.01) *m_lnet_o =lnet_o/lai_o;
		else *m_lnet_o=0.0;
	if (lai_u !=0) *m_lnet_u = lnet_u/lai_u;
		else *m_lnet_u=0.0;
/*	printf("m_lnet_o=%f",*m_lnet_o); 
	printf("m_lnet_u=%f",*m_lnet_u); 
	printf("lnet_g=%f",*lnet_g); 
	printf("epsilon_a=%f",epsilon_a); */
		
	return 1;
}