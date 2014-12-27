/*---------------------------------------------------------------------------*\

  FILE........: codec2.h
  AUTHOR......: David Rowe
  DATE CREATED: 21 August 2010

  Codec 2 fully quantised encoder and decoder functions.  If you want use 
  Codec 2, these are the functions you need to call.

\*---------------------------------------------------------------------------*/

/*
  Copyright (C) 2010 David Rowe

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
#include "defines.h"

#ifdef __cplusplus
  extern "C" {
#endif

#ifndef __CODEC2__
#define  __CODEC2__

/* set up the calling convention for DLL function import/export for
   WIN32 cross compiling */

#ifdef __CODEC2_WIN32__
#ifdef __CODEC2_BUILDING_DLL__
#define CODEC2_WIN32SUPPORT __declspec(dllexport) __stdcall
#else
#define CODEC2_WIN32SUPPORT __declspec(dllimport) __stdcall
#endif
#else
#define CODEC2_WIN32SUPPORT
#endif

#define CODEC2_MODE_3200 0
#define CODEC2_MODE_2400 1
#define CODEC2_MODE_1600 2
#define CODEC2_MODE_1400 3
#define CODEC2_MODE_1300 4
#define CODEC2_MODE_1200 5
#define CODEC2_MODE_450  6

struct CODEC2;

struct CODEC2 * CODEC2_WIN32SUPPORT codec2_create(int mode);
void CODEC2_WIN32SUPPORT codec2_destroy(struct CODEC2 *codec2_state);
void CODEC2_WIN32SUPPORT codec2_encode(struct CODEC2 *codec2_state, unsigned char * bits, short speech_in[]);
void CODEC2_WIN32SUPPORT codec2_decode(struct CODEC2 *codec2_state, short speech_out[], const unsigned char *bits);
void CODEC2_WIN32SUPPORT codec2_decode_ber(struct CODEC2 *codec2_state, short speech_out[], const unsigned char *bits, scalar ber_est);
int  CODEC2_WIN32SUPPORT codec2_samples_per_frame(struct CODEC2 *codec2_state);
int  CODEC2_WIN32SUPPORT codec2_bits_per_frame(struct CODEC2 *codec2_state);

void CODEC2_WIN32SUPPORT codec2_set_lpc_post_filter(struct CODEC2 *codec2_state, int enable, int bass_boost, scalar beta, scalar gamma);
int  CODEC2_WIN32SUPPORT codec2_get_spare_bit_index(struct CODEC2 *codec2_state);
int  CODEC2_WIN32SUPPORT codec2_rebuild_spare_bit(struct CODEC2 *codec2_state, int unpacked_bits[]);
void CODEC2_WIN32SUPPORT codec2_set_natural_or_gray(struct CODEC2 *codec2_state, int gray);

#endif

#ifdef __cplusplus
}
#endif

