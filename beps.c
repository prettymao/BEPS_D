/*************************************************************************
  Program:     	beps.c (Boreal Ecosystem Productivity Simulator)
  --------
 
  Description:
  -----------
  Calculate key components in carbon and water cycles for whole Canada or 
  for a specified area in Canada. The key components include:
  NPP
  Plant respiration
  Evaporation
  Transpiration
 ***************************************************************************
  Copyright:    CCRS (EMS/Applications Division)
  ---------
  Written by:   J. Liu/D. Fraser
  Last update:	December 2000
*****************************************************************************/

#include "beps.h"
#include "ncar.h"

double **dmatrix();		/* Function which allocates mem for matrix */

int main(argc,argv)
int argc;
char *argv[];
{
	int long j=1;			/* Counters: j for output counter */
	int long pix,lin;		/* Counters */
	int long line,pixel;	/* Position of line,pixel in Canada, referenced to 0,0 NOT 1,1 */				  
	int long jday;			/* Julian day */
	int ncobs;				/* Number of climate observations */
	int nstats;				/* # of stations per line of latitude (row) */
	int short lastlc;		/* Last landcover value */

	int short rcode;		/* Error code */

	double b[SIZEB];		/* Biological parameters */
	double longi, latit;    /* Longitude, latitude of center of pixel */

	int short *lc;			/* Land cover for a line in a given study area */
	double *lai;			/* LAI for a line in a given study area */
	double *awc;   			/* Av soil water hold cap for a line in a given study area */	
	double *lon,*lat;		/* Long, lat values for a line in study area */

	double *x;				/* X variables */
	double **climate;       /* Climate data for a line (study area) */

	double *npp;			/* NPP */
	unsigned char *output;	/* For output images */

	struct xvalue *xx;		/* X variables for a line in a given study area */
	struct climatedata *sdat;	/* climate data */
	
	FILE *cfile;			/* Climate Output file */

/*      Check for input files */
        if (argc<2) {
          fprintf(stderr," Usage: %s <control_file>\n", argv[0]);
          exit(0);
        }

/* 	Read Control parameters */
	readconf(argv[1],&rcode);
	if (rcode == ERROR)
          exit(0);

/*	Allocate memory */
	lc=(int short*)malloc(npixels*sizeof(int short));
	awc=(double*)malloc(npixels*sizeof(double));	
	lai=(double*)malloc(npixels*sizeof(double));
	lon=(double*)malloc(npixels*sizeof(double));
	lat=(double*)malloc(npixels*sizeof(double));
	npp=(double*)malloc(npixels*sizeof(double));
	output=(unsigned char*)malloc(npixels*sizeof(unsigned char));
	row_index=(unsigned char *)malloc(npixels* sizeof(unsigned char));
    col_index=(unsigned char *)malloc(npixels* sizeof(unsigned char));
    dr=(float *)malloc(npixels*sizeof(float));
    dc=(float *)malloc(npixels*sizeof(float));

/*	Daily Model Parameters  (Currently there are 12) */
	x=(double*)malloc(SIZEX*sizeof(double));

/*	Memory for model parameters which change daily */
	xx=(struct xvalue*)malloc(npixels*sizeof(struct xvalue));
		
	sdat=(struct climatedata*)malloc(npixels*sizeof(struct climatedata));

	z= (double *)malloc(SIZEZ*sizeof(double));	/* z variables */

/*      Open Climate file */
        if ((cfile=fopen("cfile.out", "w")) == NULL) {
          printf("\n Unable to open file cfile.out\n");
          exit(0);
        }

/*	Set error code to no error */
	rcode = NOERROR;

	ncobs = NCLIMATE_OBS;
        climate = dmatrix(&rcode,0,npixels,0,ncobs);
        if (climate == NULL) {
          printf("Error allocating matrix\n");
          exit(0);
        }

/*	Read default biological paramenters */
	readb_init(b);


/* LLLLLLLLLL	For each line in Study Area 	LLLLLLLLL*/
	for (lin = 0; lin <nlines; lin++) {

/*	  Calculate true line in study area (referenced to 0 origin)*/
	  line = lin_offset + lin;

	  printf("\nL: %d...\n", line+1);	

/*	  Calculate longitude and latitude values */
	  for (pix=0; pix<npixels; pix++)
	  {

/*    Calculate true pixel in study area (reference to 0 origin) */
        pixel = pix_offset + pix;

/*	  Calulate long,lat value for true pixel in study area */
		pixtolon(pixel,line,&longi,&latit);
	    lon[pix]=-longi;
	    lat[pix]=latit;
	  }

/*	  Clean memory */
	  for(pix=0; pix<npixels; pix++) {
		npp[pix]=0.0;
		zeroxx(pix,x,xx);
		output[pix]= 0;
	  }

/*	  Read index file */
	  readindex(line,&rcode);
	  if (rcode==ERROR)
		  exit(0);

/* 	  Read landcover values (for each pixel in the line) */
	  readlc(line,lc,&rcode); 
	  if (rcode == ERROR) {
	    exit(0);
	  }
	  else ;
			
/*	  Calculate Available Water Capacity from soil data */
	  readawc(line,awc,&rcode); 	/* in m */

/*	  Read Snow pack data */
	  
	  readsnow(line,xx,rcode);
	  if (rcode == ERROR) {
	    exit(0);
	  }

/*	  Read biomass data */
	  readbm(line,xx,&rcode); 
	  if (rcode == ERROR) {
	    exit(0);
	  }
    
/* DDDDDDDDDD	For each day  DDDDDDDDDD */
	  
 	  for (jday=jday_start; jday<=jday_end; jday++) {  
//		   printf("\n SIMULATION IN DAY %d...\n", jday);
		            
           /*Read Leaf Area Index*/
	       readlai(line,lai,jday,&rcode); 
	       if (rcode == ERROR)
	       exit(0);

           /*Assign parameter values used in read climate routine */
	       nstats = 98;

           /*Interpolate observation data */
	       readclim(line,jday,ncobs,nstats,climate,&rcode); 
	       if (rcode == ERROR) {
	          exit(0);
		   } 
	       else ; 

           /*Assign interpolated climate values to structure */
	       for (pix=0; pix<npixels; pix++) {
	           sdat[pix].srad = climate[pix][0];
	           sdat[pix].tmin = climate[pix][1];
	           sdat[pix].tmax = climate[pix][2];
	           sdat[pix].precip = climate[pix][3];
	           sdat[pix].dewp = climate[pix][4];
		   }

           /* PPPPPPPPPP	For each PIXEL in A Line PPPPPPPPPP */
	       for (pix=0; pix<npixels; pix++) {
			   
               /*If landcover is Water */
	           if(lc[pix]==0) npp[pix]=0.00;
	           else {
					
                    /*If land cover parameters need to be updated */
		            if (lastlc != lc[pix]) {
                        /*Read parameters according to cover type */
	                    readb(b,lc[pix]);		
                        /*Set last lc update to current land cover type */
		                lastlc = lc[pix];
					}
				
				        /*Beps Model => Calculate NPP for each pixel */
                    model(jday,pix,jday_start,lat[pix],lai[pix],awc[pix],lc[pix],x,b,sdat,xx);					
	                npp[pix]=xx[pix].x6;										
		         }		   					
		   }	
           
		   /* PPPPPPPPPP	End of PIXEL cycle PPPPPPPPPP */
	     
	       if(jday % cycle_display == 0) {
			   if(output_NPP=='y' || output_NPP=='Y') {                
				   for(pix=0; pix<npixels; pix++) {
					  output[pix]=(unsigned char)(npp[pix]*factor);	
					/*	printf("npp=%6.4f\n",npp[0]);
					 	printf("gpp=%6.4f\n",xx[0].x11);					   
						printf("rm=%6.4f\n",xx[0].x7);
						printf("rm leaf=%6.4f\n",xx[0].x14);
						printf("rm stem=%6.4f\n",xx[0].x15);
						printf("rm root=%6.4f\n",xx[0].x16);
						printf("out=%6.4f\n",xx[0].x3);
					  	printf("tra=%6.4f\n",xx[0].x4); 
					  	printf("evp=%6.4f\n",xx[0].x5); 
						printf("evp_soil=%6.4f\n",xx[0].x15); */
                	   if(lc[pix]!=0 && output[pix]==0)  output[pix]=1;
			           if(lc[pix]!=0 && output[pix]>200) output[pix]=1; 
				   }
		           sprintf(outf_name,"cnpp%d.img",j);
	               display(outf_name,output,&rcode);
	               if (rcode == ERROR) exit(0);
			   }
/*
			    if(output_res=='y' || output_res=='Y') {                
				   for(pix=0; pix<npixels; pix++) {
					  output[pix]=(unsigned char)(0.8*xx[pix].x11*factor);
                	   if(lc[pix]!=0 && output[pix]==0)  output[pix]=1;
			       }
		           sprintf(outf_name,"cgpp%d.img",j);
	               display(outf_name,output,&rcode);
	               if (rcode == ERROR) exit(0);
			   }

  */

	           if(output_eva=='y' || output_eva=='Y') {
				   for(pix=0; pix<npixels; pix++) {
					   output[pix]=(unsigned char)(xx[pix].x7*factor);
                	   if(lc[pix]!=0 && output[pix]==0) output[pix]=1;
				   } 	
		           sprintf(outf_name,"cetree%d.img",j);     
	               display(outf_name,output,&rcode);
	               if (rcode == ERROR) exit(0);
			   }
	     
	           if(output_tra=='y' || output_tra=='Y') {
				   for(pix=0; pix<npixels; pix++) {
					   output[pix]=(unsigned char)(xx[pix].x15*factor);
                	   if(lc[pix]!=0 && output[pix]==0)  output[pix]=1;
			           if(lc[pix]!=0 && output[pix]>200) output[pix]=1;
				   } 
		           sprintf(outf_name,"cesoil%d.img",j);     
	               display(outf_name,output,&rcode);
	               if (rcode == ERROR) exit(0);
			   }

			if(output_eva=='y' || output_eva=='Y') {
				   for(pix=0; pix<npixels; pix++) {
					   output[pix]=(unsigned char)(xx[pix].x14*factor);
                	   if(lc[pix]!=0 && output[pix]==0)  output[pix]=1;
			           if(lc[pix]!=0 && output[pix]>200) output[pix]=1;
				   } 
		           sprintf(outf_name,"cstree%d.img",j);     
	               display(outf_name,output,&rcode);
	               if (rcode == ERROR) exit(0);
			   }

				if(output_eva=='y' || output_eva=='Y') {
				   for(pix=0; pix<npixels; pix++) {
					   output[pix]=(unsigned char)(xx[pix].x16*factor);
                	   if(lc[pix]!=0 && output[pix]==0)  output[pix]=1;
			           if(lc[pix]!=0 && output[pix]>200) output[pix]=1;
				   } 
		           sprintf(outf_name,"csgrou%d.img",j);     
	               display(outf_name,output,&rcode);
	               if (rcode == ERROR) exit(0);
			   }

	           if(output_tra=='y' || output_tra=='Y') {
				   for(pix=0; pix<npixels; pix++) {
			          if(lc[pix]!=0) output[pix]=(unsigned char)(xx[pix].x4*factor);
                	  if(lc[pix]!=0 && output[pix]==0) output[pix]=1;
			          if(lc[pix]!=0 && output[pix]>200)output[pix]=1;
				   } 
		           sprintf(outf_name,"ctra%d.img",j);      
	               display(outf_name,output,&rcode);
	               if (rcode == ERROR) exit(0);
			   }  

			   if(output_eva=='y' || output_eva=='Y') {
				   for(pix=0; pix<npixels; pix++) {
					   output[pix]=(unsigned char)(xx[pix].x5*factor);
                	   if(lc[pix]!=0 && output[pix]==0)  output[pix]=1;
			           if(lc[pix]!=0 && output[pix]>200) output[pix]=1;
				   } 
		           sprintf(outf_name,"ceva%d.img",j);     
	               display(outf_name,output,&rcode);
	               if (rcode == ERROR) exit(0);
			   }

			   if(output_tra=='y' || output_tra=='Y') {
				   for(pix=0; pix<npixels; pix++) {
			          if(lc[pix]!=0) output[pix]=(unsigned char)((xx[pix].x4+xx[pix].x5)*factor);
                	  if(lc[pix]!=0 && output[pix]==0) output[pix]=1;
			          if(lc[pix]!=0 && output[pix]>200)output[pix]=1;
				   } 
		           sprintf(outf_name,"cet%d.img",j);      
	               display(outf_name,output,&rcode);
	               if (rcode == ERROR) exit(0);
			   } 

			   if(output_tra=='y' || output_tra=='Y') {
				   for(pix=0; pix<npixels; pix++) {
			          if(lc[pix]!=0) output[pix]=(unsigned char)(xx[pix].x18*factor);
                	  if(lc[pix]!=0 && output[pix]==0) output[pix]=1;
			          if(lc[pix]!=0 && output[pix]>200)output[pix]=1;
				   } 
		           sprintf(outf_name,"ctun%d.img",j);      
	               display(outf_name,output,&rcode);
	               if (rcode == ERROR) exit(0);
			   }  
  
               j++;
      	    }	/* End of Display */

	  }
/* DDDDDDDDDD	End of daily cycle DDDDDDDDDD*/			    	  	
	  j=1;
	        
	}	
/* LLLLLLLLLL	End of lin_cycle LLLLLLLLLL*/
       
	fclose(cfile);

/*	Free Memory */
	free(climate);
    free(dc);
	free(dr);
	free(row_index);
	free(col_index);
    free(lc);
	free(awc);	
	free(lai);
	free(lon);
	free(lat);
	free(npp);
	free(output);
	free(x);
	free(xx);
	free(z);

	printf("\nFactor=%5.1f\n",factor);
	printf("\njday_start=%d\n",jday_start);
	printf("\njday_end=%d\n",jday_end);

	printf("\n ***End of BEPS***\n");

	return 1;
}	/* end of main */

