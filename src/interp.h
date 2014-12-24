/*---------------------------------------------------------------------------*\

  FILE........: interp.h
  AUTHOR......: David Rowe
  DATE CREATED: 9/10/09

  Interpolation of 20ms frames to 10ms frames.

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

#ifndef __INTERP__
#define __INTERP__

#include "kiss_fft.h"

void interpolate(MODEL *interp, MODEL *prev, MODEL *next);
void interpolate_lsp(kiss_fft_cfg  fft_dec_cfg,
		     MODEL *interp, MODEL *prev, MODEL *next, 
		     scalar *prev_lsps, scalar  prev_e,
		     scalar *next_lsps, scalar  next_e,
		     scalar *ak_interp, scalar *lsps_interp);
void interp_Wo(MODEL *interp, MODEL *prev, MODEL *next);
void interp_Wo2(MODEL *interp, MODEL *prev, MODEL *next, scalar weight);
scalar interp_energy(scalar prev, scalar next);
scalar interp_energy2(scalar prev, scalar next, scalar weight);
void interpolate_lsp_ver2(scalar interp[], scalar prev[],  scalar next[], scalar weight);

#endif
