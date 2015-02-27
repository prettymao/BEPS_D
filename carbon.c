/*************************************************************************
  Program:     carbon.c
  --------
  Description:
  ----------- 
	Estimate photosythesis and plant respiration.
  Details:
  -------	
 	Use Farquhar's model for photosynthesis calculation
	Input:
	b[25]: Q10 for leaf
	b[28]: Q10 for stem	
	b[29]: Q10 for root	

	b[19]: leaf maintenance res. coe 
	b[20]: stem maintenance res. coe  	
	b[21]: coarse root maintenance res. coe
	b[24]: fine root maintenance res. coe 
	x[8]: leafC
	x[9]: stemC
	x[10]: rootC
	z[4]:Tmax
	z[5]:Tmin
	z[10]: LAI	
	z[14]: daylight average temperature
	z[18]: daylength
	Output
	g[25]: leaf Rm(forest) or total Rm (other land cover) in kg C/m2/day
***************************************************************************
  CCRS (EMS/Applications Division)
  Written by:   J. Liu      
  Last update:	December 2000
*****************************************************************************/
	
#include "beps.h" 
#include<math.h>

void carbon(b,g,x,z) 
  double b[], g[], x[], z[];
{
  double exponent;
  double sapw_max=0.1;	
  double rf25, pr;		/* for Rm with Bonan (1995) */
  int lc_p;
  double ratio_froot=1;
  double tb=15;			/* base temperature */

/* RESPIRATION	*/

/* leaf day time maintenance respiration*/
/* a equation below can overwrite this one */
	
    exponent=(z[14]-tb)/10.0;
    g[23] = x[8]*b[19]*pow(b[25],exponent)*z[18]/86400;
    g[23] = max( g[23], 0.0);

/* gross psn */ 

   farq_psn(z,g);

/* daily gross psn */

/* convert umol C/m2/s --> kg C/m2/period */
    g[24] = (g[28]*z[32]+g[29]*z[33])*z[18]*12/1000000000;
  
/*	Rm calculation 
	Bonan (1995)
	z[10]: LAI; z[4]: Tmax; z[5]:Tmin
*/
	lc_p=(int)z[23];
    switch(lc_p)
	{
	case 1: case 2: case 3: case 4: case 5: /* conifer */
    case 6:  	/* deciduous forest */
	case 7: case 8: case 9: case 10: /* mixe */
		{
		/* leaf carbon = LAI/SLA */ 
		x[8]= z[10]/b[1];

		/* leaf night time maintenance respiration */
		exponent=(z[15]-tb)/10.0;
		g[25] =x[8]*b[19]*pow(b[25],exponent)*(86400.0-z[18])/86400;
		g[25] = max( g[25], 0.0);

		/* stem maintenance respiration */
		exponent=(z[15]-tb)/10.0;
		g[30] = (0.05*x[9]*0.1)/(0.05*x[9]+0.1)*b[20]*pow(b[28],exponent);
		g[30] = max( g[30], 0.0);
 
		/* root maintenance respiration */
		exponent=(z[15]-tb)/10.0;
		ratio_froot=exp(1.007)*pow(x[10],-(0.841));
		ratio_froot=min(0.9, ratio_froot);
				
		g[31]=0.05*x[10]*(1-ratio_froot)*b[21]*pow(b[29],exponent);		/*coarse root */
		g[31]=g[31]+0.05*x[10]*ratio_froot*b[24]*pow(b[29],exponent);	/* coarse + fine root */
		g[31] = max  (g[31], 0.0);  
//		printf("ratio_froot%f\n",ratio_froot);

		}
		break;
	case 23: case 24: case 25: case 26: case 27: case 28:	/* agricultural land */
		rf25=0.50;    
		pr=0;
		/* g[25] is total rm in kg C/m2/day*/
		g[30]=0.0;
		g[31]=0.0;
		g[25] = 1.0368*(2*z[10])*rf25*(1+pr)*pow(2, 0.1*(0.5*(z[4]+z[5])-25.0))/1000;
   		break;
	case 16:	/* pasture */
		rf25=0.5;    
		pr=0; 
		/* g[25] is total rm in kg C/m2/day*/
		g[30]=0.0;
		g[31]=0.0;
		g[25] = 1.0368*(2*z[10])*rf25*(1+pr)*pow(2, 0.1*(0.5*(z[4]+z[5])-25.0))/1000;
		break;
	default:	/* other land cover types */
		rf25=0.25;    
		pr=0; 
		/* g[25] is total rm in kg C/m2/day*/
		g[30]=0.0;
		g[31]=0.0;
		g[25] = 1.0368*(2*z[10])*rf25*(1+pr)*pow(2, 0.1*(0.5*(z[4]+z[5])-25.0))/1000;
	}
	

/* 24HR. NET C/m2  */

   	g[26] = 0.75*g[24]-g[25]-g[30]-g[31];

 /*   printf("\n GPP g[24]=%f ",g[24]*1000); 
    printf("\n Leaf Res g[25]=%f g C/m2/d ",g[25]*1000);
    printf("\n Sten Res g[30]=%f ",g[30]*1000);
    printf("\n Root Res g[31]=%f ",g[31]*1000);
    printf("\n NPP g[26]=%f ",g[26]*1000);
	
    printf("\n Leaf bio =%f kg C/m2",x[8]);
    printf("\n Sten bio =%f kg C/m2",x[9]);
    printf("\n Root bio =%f kg C/m2",x[10]);
 */   

	return;

    }

int farq_psn(z,g)
	double z[], g[];
{
	double pa; 		/* (Pa) atmospheric pressure */
//	double co2;		/* (ppm) atmospheric [CO2] */
	double t;		/* (deg C) air temperature */
	double irad;	/* (umol/m2/s) PAR photon flux density */
	double gg;      /* (m/s) conductance to CO2 */
	double Rd;		/* (umol/m2/s) dark respiration rate  */
//	double lnc;		/* (kg Nleaf/m2) leaf N concentration, area units */
//	double flnr;	/* (kg NRub/kg Nleaf) fraction of leaf N in Rubisco */
	
	double tk;     /* (K) absolute temperature */
	double O2;     /* (Pa) atmospheric partial pressure O2 */ 
	double Ca;     /* (Pa) atmospheric partial pressure CO2 */
	double gamma;  /* (Pa) co2 compensation point, no dark respiration */
	double Kc;     /* (Pa) MM constant for carboxylase reaction */
	double Ko;     /* (Pa) MM constant for oxygenase reaction */
	double act;    /* (umol/kgRubisco/s) Rubisco activity */
	double Vmax;   /* (umol/m2/s) maximum carboxylation velocity */
	double Jmax;   /* (umol/m2/s) maximum rate of electron transport */
	double J;      /* (umol/m2/s) maximum rate of Rubisco regeneration */
	double Av;     /* (umol/m2/s) Rubisco limited assimilation rate */
	double Aj;     /* (umol/m2/s) RuBP regeneration limited assim rate */
	double A;      /* (umol/m2/s) net assimilation rate */
	double Vmax25;
	double Nratio;
	int short lc_p;
	
	double kk,aa,bb,cc,dd,ee, term1,term2,term3;  

	static double fnr = 7.16;   	/* kg Rub/kg NRub */
	static double Kc25 = 30.0; 		/* (Pa) MM const carboxylase, 25 deg C */ 
	static double q10Kc = 2.1;    	/* (DIM) Q_10 for kc */
	static double Ko25 = 30000.0;   /* (Pa) MM const oxygenase, 25 deg C */
	static double q10Ko = 1.2;   	/* (DIM) Q_10 for ko */
	static double act25 = 3.6;    	/* (umol/mgRubisco/min) Rubisco activity */
	static double q10act = 2.4;  	/* (DIM) Q_10 for Rubisco activity */			
        int i;		/* index for sunlit/shaded leaves, 0: sun, 1 shade */			
  	for (i=0;i<=1;i++)
        {

	/* convert w/m2 to umol/m2/s */
	if (i==0) irad=2.04*z[34];
	else 
          irad=2.04*z[35]; 

	t = z[14];
	tk = t + 273.15;
	
	if (i==0) gg=g[20];
  	else
	  gg=g[21];

	/* convert conductance from m/s --> umol/m2/s/Pa */
	gg =gg/1.6 * 1000000 / (8.3143 * tk); 
	
	/* calculate the atomsheric pressure */
	pa=100000;

	/* convert atmospheric CO2 from ppm --> Pa */
	Ca = 360 * pa / 1e6;
	
	/* calculate atmospheric O2 in Pa, assumes 21% O2 by volume */
	O2 = 0.21 * pa;

	/* correct kinetic constants for temperature, and do unit conversions */
	Ko = Ko25 * pow(q10Ko, (t-25.0)/10.0);
	Kc = Kc25 * pow(q10Kc, (t-25.0)/10.0);
	act = pow(q10act, (t-25.0)/10.0);

	/* calculate gamma (Pa), assumes Vomax/Vcmax = 0.21 */
	gamma = 4.02*pow(1.75, (t-25)/10);
	 
	/**************** Modification according to Bonan ************* */

	lc_p=(int)z[23];
    switch(lc_p)
	{
	case 1: case 2: case 3: case 4: case 5: /* conifer */
    case 6:  	/* deciduous forest */
	case 7: case 8: case 9: case 10: /* mixe */
	case 23: case 24: case 25: case 26: case 27: case 28:	/* agricultural land */
	case 16:	/* pasture */
		Vmax25=33.0;
		Nratio=0.5;
		break;
	default: /* other land cover */
		Vmax25=17.0;
		Nratio=0.5;
	}
	
	Vmax=Nratio*Vmax25*act;
		
	/**************** End of the modification ********************/

	/* calculate Jmax = f(Vmax) */

	Jmax = 29.1 + 1.64*Vmax;
	
	/* calculate J = f(Jmax, I) */
	J = Jmax * irad / (irad + 2.1*Jmax);
	
	/* Rd */
  	Rd=0.015*Vmax;
	g[23]=Rd*z[18]*12.01;
/***************** With daily integration *****************************/ 
	kk=Kc*(1+O2/Ko);
  
/* Av */   
	Av=0;

    	aa = (kk+Ca)*(kk+Ca);
    	bb = 2*(2*gamma+kk-Ca)*Vmax+2*(Ca+kk)*Rd;
    	cc = (Vmax-Rd)*(Vmax-Rd);

     	if (aa>0 && cc>0)
        {
	dd = sqrt(aa*gg*gg+bb*gg+cc);
	ee = (bb*bb-4*aa*cc)/(8*aa*sqrt(aa));

	term1=sqrt(aa)*gg*gg/2+sqrt(cc)*gg;
	term2=((2*aa*gg+bb)*dd-bb*sqrt(cc))/(4*aa);
	term3=ee*(log(2*aa*gg+bb+2*sqrt(aa)*dd)-log(bb+2*sqrt(aa)*sqrt(cc)));

	if (gg!=0) Av=0.5*1.27*(term1-term2+term3)/gg;

	}

/* Aj */
 	Aj=0;

	aa = (2.33*gamma+Ca)*(2.33*gamma+Ca);
    	bb = 0.4*(4.3*gamma-Ca)*J+2*(Ca+2.3*gamma)*Rd;
    	cc = (0.22*J-Rd)*(0.22*J-Rd);

      	if (aa>0 && cc>0)
        {
	dd = sqrt(aa*gg*gg+bb*gg+cc);
	ee = (bb*bb-4*aa*cc)/(8*aa*sqrt(aa));

	term1=sqrt(aa)*gg*gg/2+sqrt(cc)*gg;
	term2=((2*aa*gg+bb)*dd-bb*sqrt(cc))/(4*aa);
	term3=ee*(log(2*aa*gg+bb+2*sqrt(aa)*dd)-log(bb+2*sqrt(aa)*sqrt(cc)));

	if (gg!=0) Aj=0.5*1.27*(term1-term2+term3)/gg;

	}

	/* calculate A as the minimum of (Av,Aj) */
	if (Av < Aj) A = Av; 
	else         A = Aj;

	/* primary output */
	
	/* printf("A=%f",A); */
	if (i==0) g[28]=A;
	else g[29]=A;
		
   }
	return 1;
 }	

