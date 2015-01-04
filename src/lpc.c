/*---------------------------------------------------------------------------*\
                                                                           
  FILE........: lpc.c                                                              
  AUTHOR......: David Rowe                                                      
  DATE CREATED: 30 Sep 1990 (!)                                                 
                                                                          
  Linear Prediction functions written in C.                                
                                                                          
\*---------------------------------------------------------------------------*/

/*
  Copyright (C) 2009-2012 David Rowe

  All rights reserved.

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License version 2.1, as
  published by the Free Software Foundation.  This program is
  distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
  License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with this program; if not, see <http://www.gnu.org/licenses/>.
*/

/*
Copyright (c) 2014, Robert C. Taylor (Synkarae)
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
*/

#define LPC_MAX_N 512		/* maximum no. of samples in frame */
#define PI 3.141592654		/* mathematical constant */

#define ALPHA 1.0
#define BETA  0.94

#include <assert.h>
#include <math.h>
#include "defines.h"
#include "lpc.h"

/*---------------------------------------------------------------------------*\
                                                                         
  pre_emp()                                                        
                                                                         
  Pre-emphasise (high pass filter with zero close to 0 Hz) a frame of
  speech samples.  Helps reduce dynamic range of LPC spectrum, giving
  greater weight and hensea better match to low energy formants.  

  Should be balanced by de-emphasis of the output speech.
                                                                           
\*---------------------------------------------------------------------------*/

void pre_emp(
  scalar  Sn_pre[], /* output frame of speech samples                     */
  scalar  Sn[],	   /* input frame of speech samples                      */
  scalar *mem,      /* Sn[-1]single sample memory                         */
  int   Nsam	   /* number of speech samples to use                    */
)
{
    int   i;

	scalar ALPHA_ = fl_to_numb(ALPHA);
    for(i=0; i<Nsam; i++) {
	Sn_pre[i] = s_sub(Sn[i] , s_mul(ALPHA_ , mem[0]));
	mem[0] = Sn[i];
    }

}


/*---------------------------------------------------------------------------*\
                                                                         
  de_emp()                                                        
                                                                         
  De-emphasis filter (low pass filter with polse close to 0 Hz).
                                                                           
\*---------------------------------------------------------------------------*/

void de_emp(
  scalar  Sn_de[],  /* output frame of speech samples                     */
  scalar  Sn[],	   /* input frame of speech samples                      */
  scalar *mem,      /* Sn[-1]single sample memory                         */
  int    Nsam	   /* number of speech samples to use                    */
)
{
    int   i;
    scalar BETA_ = fl_to_numb(BETA);
    for(i=0; i<Nsam; i++) {
	Sn_de[i] = s_add(Sn[i] , s_mul(BETA_, mem[0]));
	mem[0] = Sn_de[i];
    }

}


/*---------------------------------------------------------------------------*\
                                                                         
  hanning_window()                                                        
                                                                         
  Hanning windows a frame of speech samples.                              
                                                                           
\*---------------------------------------------------------------------------*/

void hanning_window(
  scalar Sn[],	/* input frame of speech samples */
  scalar Wn[],	/* output frame of windowed samples */
  int Nsam	/* number of samples */
)
{
  int i;	/* loop variable */
  scalar HALF = fl_to_numb(0.5);
  scalar PI2_ = fl_to_numb(2*PI);
  scalar Nsam_ = int_to_numb(Nsam-1);

  for(i=0; i<Nsam; i++){

	scalar sc_cos = s_mul(HALF,s_cos(s_mul( PI2_, s_div(fl_to_numb((float)i),Nsam_))));
	scalar sc_a  = s_sub(HALF, sc_cos);
    Wn[i] = s_mul(Sn[i], sc_a);
	}
}

/*---------------------------------------------------------------------------*\
                                                                           
  autocorrelate()                                                          
                                                                          
  Finds the first P autocorrelation values of an array of windowed speech 
  samples Sn[].                                                            
                                                                          
\*---------------------------------------------------------------------------*/

void autocorrelate(
  scalar Sn[],	/* frame of Nsam windowed speech samples */
  scalar Rn[],	/* array of P+1 autocorrelation coefficients */
  int Nsam,	/* number of windowed samples to use */
  int order	/* order of LPC analysis */
)
{
  int i,j;	/* loop variables */

  scalar Zero = fl_to_numb(0.0);
  for(j=0; j<order+1; j++) {
    Rn[j] = Zero;
    for(i=0; i<Nsam-j; i++)
      Rn[j] = s_add(Rn[j] , s_mul(Sn[i],Sn[i+j]));
  }
}

/*---------------------------------------------------------------------------*\
                                                                            
  levinson_durbin()                                                        
                                                                           
  Given P+1 autocorrelation coefficients, finds P Linear Prediction Coeff. 
  (LPCs) where P is the order of the LPC all-pole model. The Levinson-Durbin
  algorithm is used, and is described in:                                   
                                                                           
    J. Makhoul                                                               
    "Linear prediction, a tutorial review"                                   
    Proceedings of the IEEE                                                
    Vol-63, No. 4, April 1975                                               
                                                                             
\*---------------------------------------------------------------------------*/

void levinson_durbin(
  scalar R[],		/* order+1 autocorrelation coeff */
  scalar lpcs[],		/* order+1 LPC's */
  int order		/* order of the LPC analysis */
)
{
  scalar a[order+1][order+1];
  scalar sum, e, k;
  int i,j;				/* loop variables */

  scalar ZERO = fl_to_numb(0.0);

  e = R[0];				/* Equation 38a, Makhoul */

  for(i=1; i<=order; i++) {
    sum = ZERO;
    for(j=1; j<=i-1; j++)
      sum = s_add(sum, s_mul(a[i-1][j],R[i-j])); 
    k = s_mul(-1.0, s_div(s_add(R[i], sum),e));		/* Equation 38b, Makhoul */
    if (s_abs(k) > fl_to_numb(1.0))
      k = ZERO;

    a[i][i] = k;

    for(j=1; j<=i-1; j++)
      a[i][j] = s_add(a[i-1][j] , s_mul(k,a[i-1][i-j]));	/* Equation 38c, Makhoul */

    e = s_mul(e, s_sub(fl_to_numb(1.0),s_mul(k,k)));				/* Equation 38d, Makhoul */
  }

  for(i=1; i<=order; i++)
    lpcs[i] = a[order][i];
  lpcs[0] = fl_to_numb(1.0);  
}

/*---------------------------------------------------------------------------*\
                                                                             
  inverse_filter()                                                          
                                                                           
  Inverse Filter, A(z).  Produces an array of residual samples from an array
  of input samples and linear prediction coefficients.                      
                                                                             
  The filter memory is stored in the first order samples of the input array.
                                                                             
\*---------------------------------------------------------------------------*/

void inverse_filter(
  scalar Sn[],	/* Nsam input samples */
  scalar a[],	/* LPCs for this frame of samples */
  int Nsam,	/* number of samples */
  scalar res[],	/* Nsam residual samples */
  int order	/* order of LPC */
)
{
  int i,j;	/* loop variables */

  for(i=0; i<Nsam; i++) {
    res[i] = fl_to_numb(0.0);
    for(j=0; j<=order; j++)
      res[i] = s_add(res[i] , s_mul(Sn[i-j],a[j]));
  }    
}

/*---------------------------------------------------------------------------*\
                                                                            
 synthesis_filter()                                                        
                                                                           
 C version of the Speech Synthesis Filter, 1/A(z).  Given an array of   
 residual or excitation samples, and the the LP filter coefficients, this
 function will produce an array of speech samples.  This filter structure is
 IIR.                                                                       
                                                                           
 The synthesis filter has memory as well, this is treated in the same way 
 as the memory for the inverse filter (see inverse_filter() notes above). 
 The difference is that the memory for the synthesis filter is stored in  
 the output array, wheras the memory of the inverse filter is stored in the
 input array.                                                              
                                                                           
 Note: the calling function must update the filter memory.                
                                                                             
\*---------------------------------------------------------------------------*/

void synthesis_filter(
  scalar res[],	/* Nsam input residual (excitation) samples */
  scalar a[],	/* LPCs for this frame of speech samples */
  int Nsam,	/* number of speech samples */
  int order,	/* LPC order */
  scalar Sn_[]	/* Nsam output synthesised speech samples */
)
{
  int i,j;	/* loop variables */

  /* Filter Nsam samples */

  for(i=0; i<Nsam; i++) {
    Sn_[i] = s_mul(res[i],a[0]);
    for(j=1; j<=order; j++)
      Sn_[i] = s_sub(Sn_[i],  s_mul(Sn_[i-j],a[j]));
  }
}

/*---------------------------------------------------------------------------*\
                                                                            
  find_aks()                                                                 
                                                                            
  This function takes a frame of samples, and determines the linear           
  prediction coefficients for that frame of samples.                         
                                                                            
\*---------------------------------------------------------------------------*/

void find_aks(
  scalar Sn[],	/* Nsam samples with order sample memory */
  scalar a[],	/* order+1 LPCs with first coeff 1.0 */
  int Nsam,	/* number of input speech samples */
  int order,	/* order of the LPC analysis */
  scalar *E	/* residual energy */
)
{
  scalar Wn[LPC_MAX_N];	/* windowed frame of Nsam speech samples */
  scalar R[order+1];	/* order+1 autocorrelation values of Sn[] */
  int i;

  assert(Nsam < LPC_MAX_N);

  hanning_window(Sn,Wn,Nsam);
  autocorrelate(Wn,R,Nsam,order);
  levinson_durbin(R,a,order);

  *E = fl_to_numb(0.0);
  for(i=0; i<=order; i++)
    *E = s_add(*E , s_mul(a[i],R[i]));
  if (*E < fl_to_numb(0.0)) {
	#ifdef MATH_Q16_16   
	 *E = 1;
	#else
	 *E = powf(2, -16);//fl_to_numb(1E-12); For debuging purposes. 
	#endif
  }
}

/*---------------------------------------------------------------------------*\
                                                                            
  weight()                                                                  
                                                                          
  Weights a vector of LPCs.						   
                                                                          
\*---------------------------------------------------------------------------*/

void weight(
  scalar ak[],	/* vector of order+1 LPCs */
  scalar gamma,	/* weighting factor */
  int order,	/* num LPCs (excluding leading 1.0) */
  scalar akw[]	/* weighted vector of order+1 LPCs */
)
{
  int i;
  
  for(i=1; i<=order; i++)
    akw[i] = s_mul(ak[i],s_powf(gamma,fl_to_numb((float)i)));
}
    
