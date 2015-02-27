/*************************************************************************
  Program:     	ra.c 
  -------
  Description:
  -----------
 	Output air resistance/conductance.
***************************************************************************
  CCRS (EMS/Applications Division)
  Written by:   J. Liu
  Last update:	May 1998
*****************************************************************************/

#include"beps.h"

void ra(b,g,x,z)
  double b[],g[],x[],z[];
{		
	/* input */
	double u=5;	/* wind speed at height z */
	double zz=15;	/* height (m) */
	double h=10;	/* tree height (m) */
	double radg=500; 	/* total radiation W/m2 */
	double T=20;	/* air temperature (C) */
	double lw=0.3;	/* leaf charcteristic width =0.3 for BS*/
	double sigma=5;	/* shelter factor =5 for BS */
	double lai=2;   /* leaf area index */
	double omega=0.5; /* cluming index */

	/* constants */
	double k;	/* von Karman's constant */
	double beta;	/* Bowen's ratio	*/
	double cp;	/* specific heat  of air (J/kg/K) */
	double rho;	/* density of air (kg/m3)*/
	double gg;	/* gravitational acceleration (m/s2) */	
	double n;
	double nu_lower; /* viscosity (cm2/s) */

	/* intermediate output */
	double d;	/* displacement height (m) */
	double z0;	/* roughness length (m) */
	double ustar;	/* friction velocity (m/s) */
	double QH;	/* heat flux */
	double L;
	double Le;
		
	double uh;	/* wind speed at height h */
	double ud;	/* wind speed at height d */
	double gamma;
	double Re;	/* Reynold's number */
	double Nu;	/* Nusselt number */
	double alfac;
	double alfaw;

	/* final output */
	double ram;
	double rac;
	double ra;

	k=0.4;
	beta=0.5;
	cp=1010;
	rho=1.225;	/* at 15 C */
	gg=9.8;
	n=5.0;
	nu_lower=0.15;
	alfac=0.15;	/* for CO2 */
	alfaw=0.25;	/* for H2O */

	/********* Surface layer aerodynamic resisistance ****************/

	d=0.7*h;
	z0=0.1*h;

	ustar=u*k/log((zz-d)/z0);

	QH=beta*radg;

	L= - (rho*cp*T*pow(ustar,3))/(k*gg*QH);

	/* printf("ustar= %.2f ",ustar);
	printf("QH= %.2f ",QH);
	printf("L= %.2f\n",L); */

	ram= 1/(k*ustar)*(log((zz-d)/z0)+(n*(zz-d)/L));
	/********* Leaf boundary layer resistance ******************/

	/* Wind speed at tree top */
	uh=1.1*ustar*k;

	Le=lai*omega; 

	gamma=(0.167+0.179*uh)*pow(Le, 1/3); 

	/* Wind speed at d, taking as the mean wind speed inside a stand */
	ud=uh*exp(-0.3*gamma);

	/* Reynold's number */
	Re=(ud*lw/sigma)/nu_lower;

	/* Nusselt number */
	Nu=0.62*pow(Re,0.47);

	/*printf("ud= %.2f ",ud);
	printf("Re= %.2f ",Re);
	printf("Nu= %.2f\n",Nu); */

	/* leaf boundary resistance */
	rac=lw/(alfac*Nu);

	/********* Total air resistance ******************/

	ra=ram+rac/lai;

	/* Total air conductance */
	g[40]=1/ra;  

	/*printf("ram= %f ",ram);
	printf("rac= %f ",rac);

	printf("ra= %f\n",ra); */
	return;

 }

