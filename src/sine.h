/*---------------------------------------------------------------------------*\
                                                                             
  FILE........: sine.h
  AUTHOR......: David Rowe                                                          
  DATE CREATED: 1/11/94
                                                                             
  Header file for sinusoidal analysis and synthesis functions.
                                                                             
\*---------------------------------------------------------------------------*/

/*
  Copyright (C) 2009 David Rowe

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

#ifndef __SINE__
#define __SINE__

#include "defines.h"
#include "comp.h"
#include "kiss_fft.h"

void make_analysis_window(kiss_fft_cfg fft_fwd_cfg, scalar w[], COMP W[]);
scalar hpf(scalar x, scalar states[]);
void dft_speech(kiss_fft_cfg fft_fwd_cfg, COMP Sw[], scalar Sn[], scalar w[]);
void two_stage_pitch_refinement(MODEL *model, COMP Sw[]);
void estimate_amplitudes(MODEL *model, COMP Sw[], COMP W[], int est_phase);
scalar est_voicing_mbe(MODEL *model, COMP Sw[], COMP W[], COMP Sw_[],COMP Ew[]);
void make_synthesis_window(scalar Pn[]);
void synthesise(kiss_fft_cfg fft_inv_cfg, scalar Sn_[], MODEL *model, scalar Pn[], int shift);

#define CODEC2_RAND_MAX 32767
int codec2_rand(void);

#endif
