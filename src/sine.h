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
/*
Copyright (c) 2014, Robert C. Taylor (Synkarae)
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
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
