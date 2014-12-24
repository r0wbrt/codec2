/*---------------------------------------------------------------------------*\
                                                                             
  FILE........: quantise.h
  AUTHOR......: David Rowe                                                          
  DATE CREATED: 31/5/92                                                       
                                                                             
  Quantisation functions for the sinusoidal coder.  
                                                                             
\*---------------------------------------------------------------------------*/

/*
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

#ifndef __QUANTISE__
#define __QUANTISE__

#include "kiss_fft.h"
#include "comp.h"
#include "defines.h"

#define WO_BITS     7
#define WO_LEVELS   (1<<WO_BITS)
#define WO_DT_BITS  3

#define E_BITS      5
#define E_LEVELS    (1<<E_BITS)
#define E_MIN_DB   -10.0
#define E_MAX_DB    40.0

#define LSP_SCALAR_INDEXES    10
#define LSPD_SCALAR_INDEXES    10
#define LSP_PRED_VQ_INDEXES    3
#define LSP_DIFF_FREQ_INDEXES  5
#define LSP_DIFF_TIME_BITS     7

#define LSPDT_ALL   0
#define LSPDT_LOW   1
#define LSPDT_HIGH  2

#define WO_E_BITS   8

#define LPCPF_GAMMA 0.5
#define LPCPF_BETA  0.2

void quantise_init();
scalar lpc_model_amplitudes(scalar Sn[], scalar w[], MODEL *model, int order,
			   int lsp,scalar ak[]);
void aks_to_M2(kiss_fft_cfg fft_fwd_cfg, scalar ak[], int order, MODEL *model, 
	       scalar E, scalar *snr, int dump, int sim_pf, 
               int pf, int bass_boost, scalar beta, scalar gamma, COMP Aw[]);

int   encode_Wo(scalar Wo, int bits);
scalar decode_Wo(int index, int bits);
int   encode_log_Wo(scalar Wo, int bits);
scalar decode_log_Wo(int index, int bits);
int   encode_Wo_dt(scalar Wo, scalar prev_Wo);
scalar decode_Wo_dt(int index, scalar prev_Wo);
void  encode_lsps_scalar(int indexes[], scalar lsp[], int order);
void  decode_lsps_scalar(scalar lsp[], int indexes[], int order);
void  encode_lspds_scalar(int indexes[], scalar lsp[], int order);
void  decode_lspds_scalar(scalar lsp[], int indexes[], int order);
void  encode_lsps_diff_freq_vq(int indexes[], scalar lsp[], int order);
void  decode_lsps_diff_freq_vq(scalar lsp_[], int indexes[], int order);
void  encode_lsps_diff_time(int indexes[], 
			    scalar lsp[], 
			    scalar lsp__prev[], 
			    int order);
void decode_lsps_diff_time(scalar lsp_[], 
			   int indexes[], 
			   scalar lsp__prev[],
			   int order);

void encode_lsps_vq(int *indexes, scalar *x, scalar *xq, int order);
void decode_lsps_vq(int *indexes, scalar *xq, int order, int stages);

long quantise(const scalar * cb, scalar vec[], scalar w[], int k, int m, scalar *se);
void lspvq_quantise(scalar lsp[], scalar lsp_[], int order); 
void lspjnd_quantise(scalar lsp[], scalar lsp_[], int order);
void lspdt_quantise(scalar lsps[], scalar lsps_[], scalar lsps__prev[], int mode);
void lspjvm_quantise(scalar lsps[], scalar lsps_[], int order);
void lspanssi_quantise(scalar lsps[], scalar lsps_[], int order, int mbest_entries);

void quantise_WoE(MODEL *model, scalar *e, scalar xq[]);
int  encode_WoE(MODEL *model, scalar e, scalar xq[]);
void decode_WoE(MODEL *model, scalar *e, scalar xq[], int n1);

int encode_energy(scalar e, int bits);
scalar decode_energy(int index, int bits);

void pack(unsigned char * bits, unsigned int *nbit, int index, unsigned int index_bits);
void pack_natural_or_gray(unsigned char * bits, unsigned int *nbit, int index, unsigned int index_bits, unsigned int gray);
int  unpack(const unsigned char * bits, unsigned int *nbit, unsigned int index_bits);
int  unpack_natural_or_gray(const unsigned char * bits, unsigned int *nbit, unsigned int index_bits, unsigned int gray);

int lsp_bits(int i);
int lspd_bits(int i);
int lspdt_bits(int i);
int lsp_pred_vq_bits(int i);

void apply_lpc_correction(MODEL *model);
scalar speech_to_uq_lsps(scalar lsp[],
			scalar ak[],
		        scalar Sn[], 
		        scalar w[],
		        int   order
			);
int check_lsp_order(scalar lsp[], int lpc_order);
void bw_expand_lsps(scalar lsp[], int order, scalar min_sep_low, scalar min_sep_high);
void bw_expand_lsps2(scalar lsp[], int order);
void locate_lsps_jnd_steps(scalar lsp[], int order);
scalar decode_amplitudes(MODEL *model, 
			scalar  ak[],
		        int    lsp_indexes[], 
		        int    energy_index,
			scalar  lsps[],
			scalar *e);

#endif
