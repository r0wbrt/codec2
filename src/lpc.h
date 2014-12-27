/*---------------------------------------------------------------------------*\
                                                                           
  FILE........: lpc.h                                                            
  AUTHOR......: David Rowe                                                      
  DATE CREATED: 24/8/09                                                   
                                                                          
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
#include "defines.h"

#ifndef __LPC__
#define __LPC__


#define LPC_MAX_ORDER 20

void pre_emp(scalar Sn_pre[], scalar Sn[], scalar *mem, int Nsam);
void de_emp(scalar Sn_se[], scalar Sn[], scalar *mem, int Nsam);
void hanning_window(scalar Sn[],	scalar Wn[], int Nsam);
void autocorrelate(scalar Sn[], scalar Rn[], int Nsam, int order);
void levinson_durbin(scalar R[],	scalar lpcs[], int order);
void inverse_filter(scalar Sn[], scalar a[], int Nsam, scalar res[], int order);
void synthesis_filter(scalar res[], scalar a[], int Nsam,	int order, scalar Sn_[]);
void find_aks(scalar Sn[], scalar a[], int Nsam, int order, scalar *E);
void weight(scalar ak[],	scalar gamma, int order,	scalar akw[]);

#endif
