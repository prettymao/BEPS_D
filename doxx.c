/*************************************************************************
  Program:     doxx.c
  --------
  Description:
  -----------
 	Zero xx, read xx, write to xx.
***************************************************************************
  CCRS (EMS/Applications Division)
  Written by:   J. Liu       
  Last update:	May 1998
*****************************************************************************/
#include"beps.h"

void zeroxx(pix,x,xx)
  int long pix;
  double x[];
  struct xvalue xx[];
{
	xx[pix].x1=0;
	xx[pix].x2=0;

	xx[pix].x3=0;
	xx[pix].x4=0;

	xx[pix].x5=0;
	xx[pix].x6=0;

	xx[pix].x7=0;
	xx[pix].x8=0;

	xx[pix].x9=0;
	xx[pix].x10=0;
	xx[pix].x11=0;
	xx[pix].x12=0;
	xx[pix].x13=0;
	xx[pix].x14=0;
	xx[pix].x15=0;
	xx[pix].x16=0;
	xx[pix].x17=0;
	xx[pix].x18=0;
}

void readxx(pix,x,xx)
  int long pix;
  double x[];
  struct xvalue xx[];
{
	x[1]=xx[pix].x1;
	x[2]=xx[pix].x2;
	x[3]=xx[pix].x3;
	x[4]=xx[pix].x4;
	x[5]=xx[pix].x5;
	x[6]=xx[pix].x6;
	x[7]=xx[pix].x7;
	x[8]=xx[pix].x8;
	x[9]=xx[pix].x9;
	x[10]=xx[pix].x10;
	x[11]=xx[pix].x11;
	x[12]=xx[pix].x12;
	x[13]=xx[pix].x13;
	x[14]=xx[pix].x14;
	x[15]=xx[pix].x15;
	x[16]=xx[pix].x16;
	x[17]=xx[pix].x17;
	x[18]=xx[pix].x18;
}

void writexx(pix,x,xx)
  int long pix;
  double x[];
  struct xvalue xx[];
{
	xx[pix].x1=x[1];
	xx[pix].x2=x[2];

	xx[pix].x3=x[3];
	xx[pix].x4=x[4];

	xx[pix].x5=x[5];
	xx[pix].x6=x[6];

	xx[pix].x7=x[7];
	xx[pix].x8=x[8];

	xx[pix].x9=x[9];
	xx[pix].x10=x[10];
	xx[pix].x11=x[11];
	xx[pix].x12=x[12];
	xx[pix].x13=x[13];
	xx[pix].x14=x[14];
	xx[pix].x15=x[15];
	xx[pix].x16=x[16];
	xx[pix].x17=x[17];
	xx[pix].x18=x[18];
}
