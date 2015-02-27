/********************************************************************
**  Function Name: dmatrix
**  Description:   Allocates memory for double precision matrix
**
**  Details:       If the start position of the matrix is zero,
**                 a matrix of size (nrh,nch) is allocated, otherwise it
**                 allocates a dble matrix with range [nrl..nrh][ncl..nch]
**
**  References:
**                 Press, W.H., B.P. Flannery, S.A. Teukolsky, and
**                 W.T. Vetterling, "Numerical Recipes in C - The Art of
**                 of Scientific Computing," Cambridge University Press,
**                 1988.
*********************************************************************
**  Copyright:     CCRS
**  Written by:    David Fraser
******************************************************************/

#include <stdio.h>
#include <stdlib.h>

double **dmatrix(err, nrl, nrh, ncl, nch)

int *err;
int nrl, nrh, ncl, nch;
{
        int i;
        double **m;

        *err = 0;	/*NOERR*/

/*      Allocate pointers to rows */
        if (nrl ==0)
          m = (double **) malloc((unsigned) (nrh)*sizeof(double));

/*      Else allocate a dble matrix with range [nrl..nrh][ncl..nch] */
        else
          m = (double **) malloc((unsigned) (nrh-nrl+1)*sizeof(double));

        if (!m) {
	  printf("Error occured in allocating the matrix\n");
          return (double **)0;
        }

        m -= nrl;

/*      Allocate rows and set pointers to them */
        if (ncl==0)
          for (i=nrl; i<nrh; i++)
          {
            m[i] = (double *) malloc((unsigned) (nch)*sizeof(double));
            if (!m[i]) {
	      printf("Error occured in allocating the matrix\n");
              return (double **)0;
            }
            m[i] -= ncl;
          }
        else
          for (i=nrl; i<=nrh; i++)
          {
            m[i] = (double *) malloc((unsigned) (nch-ncl+1)*sizeof(double));
            if (!m[i]) {
	      printf("Error occured in allocating the matrix\n");
              return (double **)0;
            }
            m[i] -= ncl;
          }

        return m;
}
