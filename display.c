/*************************************************************************
  Program:     display.c
  --------
  Description:
  ----------- 
	Write output data to an output file.
  Details:
  --------
  	Transfer a 1 byte (8 bits) asc file into a 1 byte binary file line by line.
***************************************************************************
  CCRS (EMS/Applications Division)
  Written by:   J. Liu
  Last update:	May 1998
*****************************************************************************/
	
#include"beps.h"
   
void display(outf_name,out,rcode)
  unsigned char *out;
  char outf_name[255];
  int short *rcode;
  {
   FILE *outfile;   

  int size,nelems;
 
  
  if ((outfile=fopen(outf_name, "a+b"))== NULL)
    {
     printf("\n Unable to open file <%s>,  exitting program ...\n\n",
                                                outf_name);
     *rcode = ERROR;
    }
  
  fwrite(&out[0],size=1,nelems=npixels,outfile);
  fclose(outfile);
  }

