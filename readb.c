/*************************************************************************
  Program:     readb.c
  --------
  Description:
  -----------
 	Read biological parameters based on vegetation type.
***************************************************************************
  CCRS (EMS/Applications Division)
  Written by:   J. Liu      
  Last update:	December 2000
*****************************************************************************/
#include"beps.h"

void readb(b,lc_p) 
  double *b;
  int short lc_p;
{

	switch(lc_p)
	{
	case 1: case 2: case 3: case 4: case 5: /* conifer */
		  b[1]= 10;				/* specific leaf area m2 kg-1 C */		
          b[11]=0.0016;			/* b(11) max. canopy cond., h20  m s-1         */
          b[12]=2.1;			/* b(12) lwp at stomatal closure  -MPa         */
          b[16]=6.0e-04;		/* b(16) max. mesophyll cond., CO2  m s-1      */
          b[19]=0.002/RTIMES;	/* b(19) leaf resp co.  kg C-1 d-1 kg-1        */
          b[20]=0.001/RTIMES;	/* b(20) stem resp co.   kg C-1 d-1 kg-1      */
          b[21]=0.001/RTIMES;	/* b(21) coarse root resp co.   kg C-1 d-1 kg-1       */
		  b[24]=0.003/RTIMES;	/* b(24) fine root resp co.   kg C-1 d-1 kg-1       */
		  b[28]=1.7;			/* b(28) Q10: constant for exp. resp. for stem	*/
		  b[39]=2.0;			/* b(29) Ratio of gmax of overstory to understory	*/
       	break;	
	case 6:  				/* deciduous forest */
		  b[1]= 25;			/* specific leaf area m2 kg-1 C */		
          b[11]=0.005;		/* b(11) max. canopy cond., h20  m s-1         */
          b[12]=2.1;		/* b(12) lwp at stomatal closure  -MPa         */
          b[16]=8.0e-04;	/* b(16) max. mesophyll cond., CO2  m s-1      */
          b[19]=0.006/RTIMES;  /* b(19) leaf resp co.  kg C-1 d-1 kg-1        */
          b[20]=0.001/RTIMES;  /* b(20) stem resp co.   kg C-1 d-1 kg-1       */
          b[21]=0.001/RTIMES;  /* b(21) coarse root resp co.   kg C-1 d-1 kg-1       */
		  b[24]=0.003/RTIMES;  /* b(24) fine root resp co.   kg C-1 d-1 kg-1 */
		  b[28]=1.3;			/* b(28) Q10: constant for exp. resp. for stem	*/
		  b[39]=0.5;			/* b(29) Ratio of gmax of overstory to understory	*/
       	break;
	case 7: case 8: case 9: case 10: /* mixe */
		  b[1]= 17;			/* specific leaf area m2 kg-1 C */		
          b[11]=0.003;		/* b(11) max. canopy cond., h20  m s-1         */
          b[12]=2.1;		/* b(12) lwp at stomatal closure  -MPa         */
          b[16]=7.0e-04;	/* b(16) max. mesophyll cond., CO2  m s-1      */
          b[19]=0.004/RTIMES;  /* b(19) leaf resp co.  kg C-1 d-1 kg-1        */
          b[20]=0.001/RTIMES;  /* b(20) stem resp co.   kg C-1 d-1 kg-1       */
          b[21]=0.001/RTIMES;  /* b(21) coarse root resp co.   kg C-1 d-1 kg-1       */
		  b[24]=0.003/RTIMES;  /* b(24) fine root resp co.   kg C-1 d-1 kg-1       */
		  b[28]=1.5;			/* b(28) Q10: constant for exp. resp. for stem	*/
		  b[39]=1.0;			/* b(29) Ratio of gmax of overstory to understory	*/
		break;
	case 23: case 24: case 25: case 26: case 27: case 28:	/* agricultural land */
		  b[11]=0.005;		/* b(11) max. canopy cond., h20  m s-1         */
   		break;
	case 16:	/* pasture */
		  b[11]=0.005;		/* b(11) max. canopy cond., h20  m s-1         */
		break;
	default:
		  b[1]= 25;			/* specific leaf area m2 kg-1 C */	
          b[11]=0.0016;		/* b(11) max. canopy cond., h20  m s-1         */
 
  	}													
}	
