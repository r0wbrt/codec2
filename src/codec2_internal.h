/*---------------------------------------------------------------------------*\
                                                                             
  FILE........: codec2_internal.h
  AUTHOR......: David Rowe                                                          
  DATE CREATED: April 16 2012
                                                                             
  Header file for Codec2 internal states, exposed via this header
  file to assist in testing.
                                                                             
\*---------------------------------------------------------------------------*/

/*
  Copyright (C) 2012 David Rowe

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


#ifndef __CODEC2_INTERNAL__
#define __CODEC2_INTERNAL__
#include "defines.h"

struct CODEC2 {
    int           mode;
    kiss_fft_cfg  fft_fwd_cfg;             /* forward FFT config                        */
    scalar         w[M];	                   /* time domain hamming window                */
    COMP          W[FFT_ENC];	           /* DFT of w[]                                */
    scalar         Pn[2*N];	           /* trapezoidal synthesis window              */
    scalar         Sn[M];                   /* input speech                              */
    scalar         hpf_states[2];           /* high pass filter states                   */
    void         *nlp;                     /* pitch predictor states                    */
    int           gray;                    /* non-zero for gray encoding                */

    kiss_fft_cfg  fft_inv_cfg;             /* inverse FFT config                        */
    scalar         Sn_[2*N];	           /* synthesised output speech                 */
    scalar         ex_phase;                /* excitation model phase track              */
    scalar         bg_est;                  /* background noise estimate for post filter */
    scalar         prev_Wo_enc;             /* previous frame's pitch estimate           */
    MODEL         prev_model_dec;          /* previous frame's model parameters         */
    scalar         prev_lsps_dec[LPC_ORD];  /* previous frame's LSPs                     */
    scalar         prev_e_dec;              /* previous frame's LPC energy               */
    
    scalar           lpc_pf;                  /* LPC post filter on                        */
    int           bass_boost;              /* LPC post filter bass boost                */
    scalar         beta;                    /* LPC post filter parameters                */
    scalar         gamma;

    scalar         xq_enc[2];               /* joint pitch and energy VQ states          */
    scalar         xq_dec[2];

    int           smoothing;               /* enable smoothing for channels with errors */
};

#endif
