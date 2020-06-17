#pragma once
#ifndef GPETYP_H
#define GPETYP_H
//~ MIT License
//
//~ Copyright (c) 2020 AliveMOon
//
//~ Permission is hereby granted, free of charge, to any person obtaining a copy
//~ of this software and associated documentation files (the "Software"), to deal
//~ in the Software without restriction, including without limitation the rights
//~ to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//~ copies of the Software, and to permit persons to whom the Software is
//~ furnished to do so, subject to the following conditions:
//
//~ The above copyright notice and this permission notice shall be included in all
//~ copies or substantial portions of the Software.
//
//~ THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//~ IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//~ FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//~ AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//~ LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//~ OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//~ SOFTWARE.
#include <UIF1248etc.h>
#define MAKE_ID( a,b,c,d )		\
(								\
	  (((U4)d)<<24)				\
	| (((U4)c)<<16)				\
	| (((U4)b)<< 8)				\
	| ((U4)a)					\
)
/// TYP
typedef enum gpeTYP:U4
{
	/// x[7s,6f,5r,4str : 3-0 nBYTE = 1<<(x&0xf) ]
	/// yz[ dimXY ] 	, w nBYTE //= 1<<(x&0xf)
	gpeTYP_null,
	gpeTYP_STR 	= MAKE_ID( 0x10, 1, 1, 1 ),
	gpeTYP_U1 	= MAKE_ID( 0x00, 1, 1, 1 ),
	gpeTYP_U4 	= MAKE_ID( 0x02, 1, 1, 4 ),
	gpeTYP_I4 	= MAKE_ID( 0x82, 1, 1, 4 ),
	gpeTYP_UF  	= MAKE_ID( 0x42, 1, 1, 4 ),
	gpeTYP_F  	= MAKE_ID( 0xc2, 1, 1, 4 ),
	gpeTYP_U8 	= MAKE_ID( 0x03, 1, 1, 8 ),
	gpeTYP_I8 	= MAKE_ID( 0x83, 1, 1, 8 ),
	gpeTYP_UD  	= MAKE_ID( 0x43, 1, 1, 8 ),	// azt jelenti, hogy pozitív
	gpeTYP_D  	= MAKE_ID( 0xc3, 1, 1, 8 ),

	gpeTYP_A	= MAKE_ID( 0x93, 1, 1, 8 ),
	gpeTYP_AN 	= MAKE_ID( 0x93, 2, 1, 16 ),


	gpeTYP_U14 	= MAKE_ID( 0x00, 4, 1, 4 ),
	gpeTYP_U44 	= MAKE_ID( 0x02, 4, 1, 16 ),
	gpeTYP_I44 	= MAKE_ID( 0x82, 4, 1, 16 ),
	gpeTYP_F4  	= MAKE_ID( 0xc3, 4, 1, 16 ),
	gpeTYP_F4x4 = MAKE_ID( 0xc3, 4, 4, 64 ),

	gpeTYP_U84 	= MAKE_ID( 0x03, 4, 1, 32 ),
	gpeTYP_I84 	= MAKE_ID( 0x83, 4, 1, 32 ),
	gpeTYP_D4  	= MAKE_ID( 0xc3, 4, 1, 32 ),

	gpeTYP_STRmsk = ~gpeTYP_STR, //MAKE_ID( 0xef, 0xff, 0xff, 0xff ),
} gpeTYPU4;

#endif // GPETYP_H
