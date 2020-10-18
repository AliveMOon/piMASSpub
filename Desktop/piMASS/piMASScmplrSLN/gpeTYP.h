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


typedef enum gpeOPid:U1{
	gpeOPid_nop,

	gpeOPid_notLG,	gpeOPid_LG,		gpeOPid_inv,

	gpeOPid_stk,	gpeOPid_newrow,

	gpeOPid_comS, 	gpeOPid_comE,	gpeOPid_com,


	gpeOPid_and, 	gpeOPid_andLG, 	gpeOPid_andM,
	gpeOPid_mul,	gpeOPid_exp, 	gpeOPid_mulM,
									gpeOPid_expM,
	gpeOPid_div, 	gpeOPid_divM, 	gpeOPid_rootM,
	gpeOPid_rem, 	gpeOPid_remM,
	gpeOPid_xor,	gpeOPid_xorM,


	gpeOPid_mov, 	gpeOPid_eqLG,  	gpeOPid_neqLG,
	gpeOPid_or, 	gpeOPid_orLG,	gpeOPid_orM,
	gpeOPid_add, 	gpeOPid_inc,	gpeOPid_addM,	/// -------------- GOOD
	gpeOPid_sub, 	gpeOPid_dec,	gpeOPid_subM,


	gpeOPid_leLG,	gpeOPid_ltLG, 	gpeOPid_sl, 	gpeOPid_slM,
	gpeOPid_beLG,	gpeOPid_bgLG, 	gpeOPid_sr, 	gpeOPid_srM,

	gpeOPid_dot, 	gpeOPid_entry, 	gpeOPid_out,
	gpeOPid_brakS,	gpeOPid_brakE,
	gpeOPid_dimS, 	gpeOPid_dimE,
	gpeOPid_begin, 	gpeOPid_end,
	gpeOPid_if,		gpeOPid_else,
	gpeOPid_mail,	gpeOPid_str,

	gpeOPid_jsr,

	gpeOPid_SWAP,	gpeOPid_EXTB,	gpeOPid_EXT,	gpeOPid_EXTL,
} gpeOPid_U1;

static const char* gpasCsz[] = {

	".b",	//	0	1		00:00	byte
	".w",	//	1	2		00:01	word
	".l",	//	2	4		00:10	long
	".q",	//	3	8		00:11	quad
	/// olyan nincsen hogy float és nincs előjel
	/// azaz ha nincs bepipálva az előjel bit akkor mást jelent
	".4",	//  4   1x4		01:00	RGBA	pixel
	".p",	//  5   1->0	01:01	// gpeCsz_ptr pointer
								//	12345678901234
	".a",	//	6	8		01:10	ABCDEFGHIJKLMN
	".c",	//  7	16		01:11   ABCDEF 0x00000000 // 2D koordináta?
	/// előjeles
	".B",	//	8	1		10:00	signed byte
	".W",	//	9	2		10:01	signed word
	".L",	//	a	4		10:10	signed long
	".Q",	//	b	8		10:11	signed quad
	/// lebegőpontos
	".f",	//	c	4		11:00	float
	".d",	//  d	8		11:01	double
	".K",	//  e	16		11:10	KID
	".0",	//	f	0		11:11	OFF
};
typedef enum gpeCsz:U1{
	gpeCsz_b,	//	0	1		00:00	byte
	gpeCsz_w,	//	1	2		00:01	word
	gpeCsz_l,	//	2	4		00:10	long
	gpeCsz_q,	//	3	8		00:11	quad
	/// olyan nincsen hogy float és nincs előjel (lglbb is itt)
	/// azaz ha nincs bepipálva az előjel bit akkor mást jelent
	gpeCsz_4,	//  4   1x4		01:00	RGBA	pixel

	/// gpeCsz_ptr pointer
	gpeCsz_ptr,	//  5   1->0	01:01	string
										//	12345678901234
	gpeCsz_a,	//	6	8		01:10	ABCDEFGHIJKLMN
	gpeCsz_c,	//  7	16		01:11   ABCDEF 0x00000000 // 2D koordináta?
	/// előjeles
	gpeCsz_B,	//	8	1		10:00	signed byte
	gpeCsz_W,	//	9	2		10:01	signed word
	gpeCsz_L,	//	a	4		10:10	signed long
	gpeCsz_Q,	//	b	8		10:11	signed quad
	/// lebegőpontos
	gpeCsz_f,	//	c	4		11:00	float
	gpeCsz_d,	//  d	8		11:01	double
	gpeCsz_K,	//  e	16		11:10	KID
	gpeCsz_OFF,	//	f	16		11:11	ZRO
} gpeCsz_U1;
static const U4 gpaCsz[] = {

	1,	//".b",		//	0	00:00	byte
	2,	//".w",		//	1	00:01	word
	4,	//".l",		//	2	00:10	long
	8,	//".q",		//	3	00:11	quad
	/// olyan nincsen hogy float és nincs előjel
	/// azaz ha nincs bepipálva az előjel bit akkor mást jelent
	4,	//".4",		//  4   01:00	RGBA	pixel
	4+sizeof(U1*),	//".u",	//  5   01:01	string
										//	12345678901234
	8,	//".a",		//	6	01:10	ABCDEFGHIJKLMN
	16,	//".c",		//  7	01:11   ABCDEF 0x00000000 // 2D koordináta?
	/// előjeles
	1,	//".B",		//	8	10:00	signed byte
	2,	//".W",		//	9	10:01	signed word
	4,	//".L",		//	a	10:10	signed long
	8,	//".Q",		//	b	10:11	signed quad
	/// lebegőpontos
	4,	//".f",		//	c	11:00	float
	8,	//".d",		//  d	11:01	double
	4,	//".K",		//  e	11:10	KID
	0,	//".0",		//	f	11:11	OFF
};
static const U8 gpaCszMX[] = {

	0xff,				//".b",	//	0	00:00	byte
	0xffFF,				//".w",	//	1	00:01	word
	0xffffFFFF,			//".l",	//	2	00:10	long
	0xffffFFFFffffFFFF,	//".q",	//	3	00:11	quad
	/// olyan nincsen hogy float és nincs előjel
	/// azaz ha nincs bepipálva az előjel bit akkor mást jelent
	0xffffFFFF,			//".4",	//  4   01:00	RGBA	pixel
	0xff,				//".u",	//  5   01:01	string
										//	12345678901234
	0x7fffFFFFffffFFFF,	//".a",	//	6	01:10	ABCDEFGHIJKLMN
	0x7fffFFFFffffFFFF,	//".c",	//  7	01:11   ABCDEF 0x00000000 // 2D koordináta?
	/// előjeles
	0x7f,				//".B",	//	8	10:00	signed byte
	0x7fff,				//".W",	//	9	10:01	signed word
	0x7fffFFFF,			//".L",	//	a	10:10	signed long
	0x7fffFFFFffffFFFF,	//".Q",	//	b	10:11	signed quad
	/// lebegőpontos
	0x7fffFFFF,			//".f",	//	c	11:00	float
	0x7fffFFFFffffFFFF,	//".d",	//  d	11:01	double
	0x7fffFFFF,			//".K",	//  f	11:11	KID
	0,					//".0",	//	e	11:10	OFF
};


#define gpmFMUL( a, b, mx ) 					\
		switch(b) 								\
		{										\
			case gpeOPid_and:					\
				*((a*)p_dst) &= *((a*)p_src);	\
				break;							\
			case gpeOPid_rem:					\
				*(a*)p_dst = ((*(a*)p_src)!=(a)0) ? (*(a*)p_dst)%(*(a*)p_src) : (a)0;	\
				break;							\
			case gpeOPid_div:					\
				*(a*)p_dst = ((*(a*)p_src)!=(a)0) ? (*(a*)p_dst)/(*(a*)p_src) : (a)mx;	\
				break;							\
			default:							\
				*((a*)p_dst) *= *((a*)p_src);	\
				break;							\
		} 										\

#define gpmMUL( a, b, mx ) 						\
		switch(b) 								\
		{										\
			case gpeOPid_xor:					\
				*((a*)p_dst) ^= *((a*)p_src);	\
				break;							\
			case gpeOPid_and:					\
				*((a*)p_dst) &= *((a*)p_src);	\
				break;							\
			case gpeOPid_rem:					\
				*(a*)p_dst = ((*(a*)p_src)!=(a)0) ? (*(a*)p_dst)%(*(a*)p_src) : (a)0;	\
				break;							\
			case gpeOPid_div:					\
				*(a*)p_dst = ((*(a*)p_src)!=(a)0) ? (*(a*)p_dst)/(*(a*)p_src) : (a)mx;	\
				break;							\
			default:							\
				*((a*)p_dst) *= *((a*)p_src);	\
				break;							\
		} 										\

#define gpmADD( a, b ) 							\
		switch(b) 								\
		{										\
			case gpeOPid_or:					\
				*((a*)p_dst) |= *((a*)p_src);	\
				break;							\
			case gpeOPid_sub:					\
				*((a*)p_dst) -= *((a*)p_src);	\
				break;							\
			default:							\
				*((a*)p_dst) += *((a*)p_src);	\
				break;							\
		} 										\


typedef enum gpeEA:U1 {
	gpeEA_OFF,
	gpeEA_Dn,
	gpeEA_An,

	gpeEA_IAnI,
	gpeEA_IAnIp,
	gpeEA_sIAnI,

	gpeEA_d16IAnI,
	gpeEA_d16IAnDnI,

	gpeEA_d16IPcI,
	gpeEA_d16IPcDnI,

	gpeEA_W,
	gpeEA_L,

	gpeEA_num,
} gpeEA_U1;


static gpeOPid gpaOPgrp[] = {
	gpeOPid_nop,

	gpeOPid_mul,	gpeOPid_mul,	gpeOPid_mul,

	gpeOPid_stk,	gpeOPid_stk,

	gpeOPid_comS, 	gpeOPid_comE,	gpeOPid_com,


	gpeOPid_mul, 	gpeOPid_mul, 	gpeOPid_mov,
	gpeOPid_mul,	gpeOPid_mul, 	gpeOPid_mov,
									gpeOPid_mov,
	gpeOPid_mul, 	gpeOPid_mov, 	gpeOPid_mov,
	gpeOPid_mul, 	gpeOPid_mov,
	gpeOPid_mul,	gpeOPid_mov,


	gpeOPid_mov, 	gpeOPid_sub,  	gpeOPid_sub,
	gpeOPid_add, 	gpeOPid_add,	gpeOPid_mov,
	gpeOPid_add, 	gpeOPid_add,	gpeOPid_mov,	/// -------------- GOOD
	gpeOPid_add, 	gpeOPid_add,	gpeOPid_mov,


	gpeOPid_sub,	gpeOPid_sub, 	gpeOPid_mul, 	gpeOPid_mov,
	gpeOPid_sub,	gpeOPid_sub, 	gpeOPid_mul, 	gpeOPid_mov,

	gpeOPid_entry, 	gpeOPid_entry, 	gpeOPid_out,
	gpeOPid_entry,	gpeOPid_out,
	gpeOPid_entry, 	gpeOPid_out,
	gpeOPid_entry, 	gpeOPid_out,
	gpeOPid_entry,	gpeOPid_entry,
	gpeOPid_mail,	gpeOPid_str,
};
static gpeOPid gpaaOPid[][0x20] = {
	//gpeOPid_nop,
	{gpeOPid_nop,},
	//gpeOPid_notLG,
	{gpeOPid_nop,},
	//gpeOPid_LG,
	{gpeOPid_nop,},
	//gpeOPid_inv,
	{gpeOPid_nop,},
	//gpeOPid_stk,
	{	gpeOPid_mov,	gpeOPid_andM,	gpeOPid_mulM,	gpeOPid_expM,
		gpeOPid_divM,	gpeOPid_rootM,	gpeOPid_remM,	gpeOPid_xorM,
		gpeOPid_addM,	gpeOPid_subM,	gpeOPid_slM,	gpeOPid_srM,

		gpeOPid_brakS,	gpeOPid_brakE,
		gpeOPid_begin,	gpeOPid_stk,	gpeOPid_newrow, },
	//gpeOPid_newrow,
	{},
	//gpeOPid_comS,
	{},
	//gpeOPid_comE,
	{},
	//gpeOPid_com,
	{},

	//gpeOPid_and,
	{},
	//gpeOPid_andLG,
	{},
	//gpeOPid_andM,
	{},
	//gpeOPid_mul,
	{	gpeOPid_mov,	gpeOPid_andM,	gpeOPid_mulM,	gpeOPid_expM,
		gpeOPid_divM,	gpeOPid_rootM,	gpeOPid_remM,	gpeOPid_xorM,
		gpeOPid_addM,	gpeOPid_subM,	gpeOPid_slM,	gpeOPid_srM,

		gpeOPid_brakS,	gpeOPid_brakE,
		gpeOPid_begin,	gpeOPid_stk,	gpeOPid_newrow,

		gpeOPid_and,	gpeOPid_mul,	gpeOPid_exp,	gpeOPid_div,
		gpeOPid_rem,	gpeOPid_sl,		gpeOPid_sr,
	},
	//gpeOPid_exp,
	{},
	//gpeOPid_mulM,
	{},
	//gpeOPid_expM,
	{},
	//gpeOPid_div,
	{},
	//gpeOPid_divM,
	{},
	//gpeOPid_rootM,
	{},
	//gpeOPid_rem,
	{},
	//gpeOPid_remM,
	{},
	//gpeOPid_xor,
	{},
	//gpeOPid_xorM,
	{},
	//gpeOPid_mov,
	{},
	//gpeOPid_eqLG,
	{},
	//gpeOPid_neqLG,
	{},
	//gpeOPid_or,
	{},
	//gpeOPid_orLG,
	{},
	//gpeOPid_orM,
	{},

	//gpeOPid_add,
	{	gpeOPid_mov,	gpeOPid_andM,	gpeOPid_mulM,	gpeOPid_expM,
		gpeOPid_divM,	gpeOPid_rootM,	gpeOPid_remM,	gpeOPid_xorM,
		gpeOPid_addM,	gpeOPid_subM,	gpeOPid_slM,	gpeOPid_srM,

		gpeOPid_brakS,	gpeOPid_brakE,
		gpeOPid_begin,	gpeOPid_stk,	gpeOPid_newrow,

		gpeOPid_add,	gpeOPid_inc,	gpeOPid_sub,	gpeOPid_dec,
		gpeOPid_or,		gpeOPid_xor,
	},
	//gpeOPid_inc,
	{},
	//gpeOPid_addM,	/// -------------- GOOD
	{},

	//gpeOPid_sub,
	{	gpeOPid_mov,	gpeOPid_andM,	gpeOPid_mulM,	gpeOPid_expM,
		gpeOPid_divM,	gpeOPid_rootM,	gpeOPid_remM,	gpeOPid_xorM,
		gpeOPid_addM,	gpeOPid_subM,	gpeOPid_slM,	gpeOPid_srM,

		gpeOPid_brakS,	gpeOPid_brakE,
		gpeOPid_begin,	gpeOPid_stk,	gpeOPid_newrow,

		gpeOPid_add,	gpeOPid_inc,	gpeOPid_sub,	gpeOPid_dec,
		gpeOPid_or,		gpeOPid_xor,
	},
	//gpeOPid_dec,
	{},
	//gpeOPid_subM,
	{},

	//gpeOPid_leLG,
	{},
	//gpeOPid_ltLG,
	{},
	//gpeOPid_sl,
	{},
	//gpeOPid_slM,
	{},
	//gpeOPid_beLG,
	{},
	//gpeOPid_bgLG,
	{},
	//gpeOPid_sr,
	{},
	//gpeOPid_srM,
	{},

	//gpeOPid_dot,
	{},
	//gpeOPid_entry,
	{},
	//gpeOPid_out,
	{},

	//gpeOPid_brakS,
	{},
	//gpeOPid_brakE,
	{gpeOPid_brakS,	gpeOPid_brakE,	gpeOPid_nop,},

	//gpeOPid_dimS,
	{},
	//gpeOPid_dimE,
	{gpeOPid_dimS,gpeOPid_nop,},

	//gpeOPid_begin,
	{},
	//gpeOPid_end,
	{gpeOPid_begin,	gpeOPid_end, gpeOPid_nop,},

	//gpeOPid_if,
	{},
	//gpeOPid_else,
	{gpeOPid_if,gpeOPid_nop,},

	//gpeOPid_mail,
	{},
	//gpeOPid_str,
	{gpeOPid_str,gpeOPid_nop,},
};

static const char* gpas68k[] = {
	"\n 0x%0.4x nop ;-------------------------",

	"\n 0x%0.4x notLG%s\t%s,%s",		"\n 0x%0.4x LG%s\t%s,%s",	"\n 0x%0.4x inv%s\t%s,%s",

	"\n 0x%0.4x stk%s\t%s,%s",			"\n 0x%0.4x nrow%s\t%s,%s",

	"\n 0x%0.4x comS%s\t%s,%s", 		"\n 0x%0.4x comE%s\t%s,%s",	"\n 0x%0.4x com%s\t%s,%s",


	"\n 0x%0.4x AND%s\t%s,%s\t\t; &", 	"\n 0x%0.4x ANDlg%s\t%s,%s\t\t; &&",	"\n 0x%0.4x ANDm%s\t%s,%s\t\t; &=",
	"\n 0x%0.4x MUL%s\t%s,%s\t\t; *", 	"\n 0x%0.4x EXP%s\t%s,%s\t\t; **",	"\n 0x%0.4x MULm%s\t%s,%s\t\t; *=",
										"\n 0x%0.4x EXPm%s\t%s,%s\t\t; **=",
	"\n 0x%0.4x DIV%s\t%s,%s\t%s%s\t\t; /", 	"\n 0x%0.4x DIVm%s\t%s,%s\t\t; /=",	"\n 0x%0.4x ROOTm%s\t%s,%s\t\t; //=",
	"\n 0x%0.4x REM%%s\t%s,%s\t%s%s\t\t; %", 	"\n 0x%0.4x REMm%s\t%s,%s\t\t; %=",
	"\n 0x%0.4x XOR%s\t%s,%s\t%s%s\t\t; ^",		"\n 0x%0.4x XORm%s\t%s,%s\t\t; ^=",

	// gpeOPid_mov,
	"\n 0x%0.4x move%s\t%s,%s\t%s%s",

	"\n 0x%0.4x eqLG%s\t%s,%s",  		"\n 0x%0.4x neqLG%s\t%s,%s",
	"\n 0x%0.4x or%s\t%s,%s", 			"\n 0x%0.4x orLG%s\t%s,%s",	"\n 0x%0.4x orM%s\t%s,%s",

	"\n 0x%0.4x ADD%s\t%s,%s\t%s%s\t\t; + ",
	"\n 0x%0.4x INC%s\t%s,%s\t\t; ++",
	"\n 0x%0.4x ADDm%s\t%s,%s\t\t; += ",	/// -------------- GOOD

	"\n 0x%0.4x SUB%s\t%s,%s\t%s%s\t\t; -",
	"\n 0x%0.4x DEC%s\t%s,%s\t\t; --",
	"\n 0x%0.4x SUBm%s\t%s,%s\t\t; -=",


	"\n 0x%0.4x leLG%s\t%s,%s",	"\n 0x%0.4x ltLG%s\t%s,%s", 	"\n 0x%0.4x sl%s\t%s,%s",
	"\n 0x%0.4x slM%s\t%s,%s",
	"\n 0x%0.4x beLG%s\t%s,%s",	"\n 0x%0.4x bgLG%s\t%s,%s", 	"\n 0x%0.4x sr%s\t%s,%s",
	"\n 0x%0.4x srM%s\t%s,%s",

	// gpeOPid_dot,
	"\n 0x%0.4x jsr entryOBJ2A0",

	//gpeOPid_entry,
	"\n 0x%0.4x entry%s\t%s,%s",
	//gpeOPid_out,
	"\n 0x%0.4x out%s\t%s,%s",

	//gpeOPid_brakS,
	"\n 0x%0.4x jsr%s\t%s,%s",

	//gpeOPid_brakE,

	"\n 0x%0.4x brakE%s\t%s,%s",

	//gpeOPid_dimS,
	"\n 0x%0.4x dimS%s\t%s,%s",
	// 	gpeOPid_dimE,
	"\n 0x%0.4x dimE%s\t%s,%s",

	// gpeOPid_begin,
	"\n 0x%0.4x begin%s\t%s,%s",
	// gpeOPid_end,
	"\n 0x%0.4x end%s\t%s,%s",
	// gpeOPid_if,
	"\n 0x%0.4x if%s\t%s,%s",
	// gpeOPid_else,
	"\n 0x%0.4x else%s\t%s,%s",
	// gpeOPid_mail,
	"\n 0x%0.4x mail%s\t%s,%s",
	//gpeOPid_str,
	"\n 0x%0.4x str%s\t%s,%s",

	// gpeOPid_jsr
	"\n 0x%0.4x jsr\t%s",
};

/// x[7s,6f,5r,4str : 3-0 nBYTE = 1<<(x&0xf) ]
typedef enum gpeOPtyp:U1{
	gpeOPtyp_U1,	// 0x00 unsigned
	gpeOPtyp_U2,	// 0x01
	gpeOPtyp_U4,	// 0x02
	gpeOPtyp_U8,	// 0x03

	gpeOPtyp_sU1 = 0x10, 	// strU1
	gpeOPtyp_sU2, 		 	// strU2
	gpeOPtyp_sU4, 		 	// strU4
	gpeOPtyp_sU8, 			// strU8

	gpeOPtyp_I1 = 0x80,	// signed I1
	gpeOPtyp_I2,
	gpeOPtyp_I4,
	gpeOPtyp_I8,

	gpeOPtyp_sA1 = 0x90,	// signed ALF
	gpeOPtyp_sA2,
	gpeOPtyp_sA4,
	gpeOPtyp_sA8,

	gpeOPtyp_HH = 0xC0,	// float
	gpeOPtyp_H,
	gpeOPtyp_F,
	gpeOPtyp_D,

	gpeOPtyp_sHH = 0xD0,	// signed I1
	gpeOPtyp_sH,
	gpeOPtyp_sF,
	gpeOPtyp_sD,

} gpeOPtyp;
/// TYP
typedef enum gpeTYP:U4 {
	/// x[7s,6f,5r,4str : 3-0 nBYTE = 1<<(x&0xf) ]
	/// yz[ dimXY ] 	, w nBYTE //= 1<<(x&0xf)
	gpeTYP_null,
	gpeTYP_STR 	= MAKE_ID( 0x10, 1, 1, 1 ),	/// x[0001:0000]
	gpeTYP_U1 	= MAKE_ID( 0x00, 1, 1, 1 ),	/// x[0000:0000]
	gpeTYP_I1 	= MAKE_ID( 0x80, 1, 1, 1 ), /// x[1000:0000]
	gpeTYP_U2 	= MAKE_ID( 0x01, 1, 1, 1 ), /// x[0000:0001]
	gpeTYP_I2 	= MAKE_ID( 0x81, 1, 1, 1 ), /// x[1000:0001]
	gpeTYP_U4 	= MAKE_ID( 0x02, 1, 1, 4 ),	/// x[0000:0010]
	gpeTYP_I4 	= MAKE_ID( 0x82, 1, 1, 4 ), /// x[1000:0010]

	gpeTYP_UF  	= MAKE_ID( 0x42, 1, 1, 4 ), /// x[0100:0010]

	gpeTYP_F  	= MAKE_ID( 0xc2, 1, 1, 4 ), /// x[1100:0010]
	gpeTYP_U8 	= MAKE_ID( 0x03, 1, 1, 8 ),
	gpeTYP_I8 	= MAKE_ID( 0x83, 1, 1, 8 ),
	gpeTYP_UD  	= MAKE_ID( 0x43, 1, 1, 8 ),	// azt jelenti, hogy pozitív
	gpeTYP_D  	= MAKE_ID( 0xc3, 1, 1, 8 ),

	gpeTYP_sA8	= MAKE_ID( 0x93, 1, 1, 8 ),
	gpeTYP_sA8N = MAKE_ID( 0x93, 2, 1, 16 ),


	gpeTYP_U14 	= MAKE_ID( 0x00, 4, 1, 4 ),
	gpeTYP_U44 	= MAKE_ID( 0x02, 4, 1, 16 ),
	gpeTYP_I44 	= MAKE_ID( 0x82, 4, 1, 16 ),
	gpeTYP_F4  	= MAKE_ID( 0xc3, 4, 1, 16 ),
	gpeTYP_F4x4 = MAKE_ID( 0xc3, 4, 4, 64 ),

	gpeTYP_U84 	= MAKE_ID( 0x03, 4, 1, 32 ),
	gpeTYP_I84 	= MAKE_ID( 0x83, 4, 1, 32 ),
	gpeTYP_D4  	= MAKE_ID( 0xc3, 4, 1, 32 ),

	gpeTYP_OP 	= MAKE_ID( 0xff, 0, 0, 0 ),

	gpeTYP_STRmsk = ~gpeTYP_STR, //MAKE_ID( 0xef, 0xff, 0xff, 0xff ),
} gpeTYPU4;

#endif // GPETYP_H
