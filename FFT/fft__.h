/************************************************************************
	fft.h

    FFT Audio Analysis
    Copyright (C) 2011 Simon Inns

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

	Email: simon.inns@gmail.com

************************************************************************/
#include "stm8s.h"
#ifndef _FFT_H
#define _FFT_H

#define N_SAMPLE 64

#if N_SAMPLE   == 128
#define N 6
#elif N_SAMPLE == 64
#define N 5
#elif N_SAMPLE == 32
#define N 4
#else
#error N_SAMPLE must be 32,64,128
#endif


typedef struct _tag_complex_t {
	s16 real;
	s16 image;
} complex_t;

extern const u16 ham[];
extern const u8 position[];

// Function prototypes*/
void fix_fft(s16 *inp,complex_t *compl );
void fft_out(complex_t *compl, s16 *out );
//void FIX_ab(complex_t *a, short *out);// = sqrt( A * A + B * B);
s32 FIX_MPY(s16 a,s16 b);
s32 FIX(s16 a,u16 b);
#endif 
