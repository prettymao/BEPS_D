/*************************************************************************
  Function:     readlai.c
  --------
  Description:
  -----------
 	Read LAI files
***************************************************************************
  CCRS (EMS/Applications Division) 
  Written by:   J. Liu
  Last update:  May 1998
*****************************************************************************/

#include"beps.h"
#include<string.h>

void readlai(line,lai,jday,rcode)
int long line;
double *lai;
int short jday;
int short *rcode;
{
	FILE *infile;
	int long pix;		
  	int long ptr;
  	int long size;
  	int long nitems;
  	unsigned char filename[255];
  	unsigned char suffix[5];
    unsigned char* laiimg;
	laiimg=(unsigned char *)malloc(npixels*sizeof(unsigned char)); 
    
  	if (jday<101) strcpy(suffix,".001");
  	if (jday>=101 && jday<111) strcpy(suffix,".001");
  	if (jday>=111 && jday<121) strcpy(suffix,".002");
  	if (jday>=121 && jday<131) strcpy(suffix,".003");
  	if (jday>=131 && jday<141) strcpy(suffix,".004");
  	if (jday>=141 && jday<152) strcpy(suffix,".005");
  	if (jday>=152 && jday<162) strcpy(suffix,".006");
  	if (jday>=162 && jday<172) strcpy(suffix,".007");
  	if (jday>=172 && jday<182) strcpy(suffix,".008");
  	if (jday>=182 && jday<192) strcpy(suffix,".009");
  	if (jday>=192 && jday<202) strcpy(suffix,".010");	
  	if (jday>=202 && jday<213) strcpy(suffix,".011");
  	if (jday>=213 && jday<223) strcpy(suffix,".012");
  	if (jday>=223 && jday<233) strcpy(suffix,".013");
  	if (jday>=233 && jday<244) strcpy(suffix,".014");
  	if (jday>=244 && jday<254) strcpy(suffix,".015");
	if (jday>=254 && jday<264) strcpy(suffix,".016");
  	if (jday>=264 && jday<274) strcpy(suffix,".017");
 	if (jday>=274 && jday<284) strcpy(suffix,".018");    
  	if (jday>=284 && jday<294) strcpy(suffix,".019"); 
 	if (jday>=294) strcpy(suffix,".020");  	

  	sprintf(filename, "%s%s",leafareaf,suffix);
	/*printf("Filename in use is: %s\n",filename); */

	/*      Open LAI file */
        if ((infile=fopen(filename, "rb")) == NULL) {
          printf("\n Unable to open file <%s>,  exitting program ...\n\n",
                                                filename);
          *rcode = ERROR;
          return;
        }
  
	ptr=line*NPIX_CAN + pix_offset;

	/* number of bytes from the beginning of the infile to the specific 	
	location, pix starts from 0, & lin starts from 1 */
  	fseek(infile,ptr,0);
  	fread(&laiimg[0],size=1,nitems=npixels,infile);
  	for (pix=0;pix<npixels;pix++)
	{
	    lai[pix]=(laiimg[pix]-1)/10.0;
	    if (lai[pix]==0) lai[pix]=0.01;
        }
	fclose(infile);
    free(laiimg);
}
