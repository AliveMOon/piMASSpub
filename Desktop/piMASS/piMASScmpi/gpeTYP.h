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

	gpeOPid_n,
} gpeOPid_U1;
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

typedef enum gpeEAsz:U1 {
	gpeEAsz0,
	gpeEAszB,
	gpeEAszW,
	gpeEAszL,
	gpeEAszQ,
	gpeEAszX,
} gpeEAsz_U1;

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
	gpeOPid_add,	gpeOPid_mov,


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

	// gpeOPid_n
	"\n 0x%0.4x jsr\t%s",
};

/// TYP
typedef enum gpeTYP:U4
{
	/// x[7s,6f,5r,4str : 3-0 nBYTE = 1<<(x&0xf) ]
	/// yz[ dimXY ] 	, w nBYTE //= 1<<(x&0xf)
	gpeTYP_null,
	gpeTYP_STR 	= MAKE_ID( 0x10, 1, 1, 1 ),
	gpeTYP_U1 	= MAKE_ID( 0x00, 1, 1, 1 ),
	gpeTYP_I1 	= MAKE_ID( 0x80, 1, 1, 1 ),
	gpeTYP_U2 	= MAKE_ID( 0x01, 1, 1, 1 ),
	gpeTYP_I2 	= MAKE_ID( 0x81, 1, 1, 1 ),
	gpeTYP_U4 	= MAKE_ID( 0x02, 1, 1, 4 ),
	gpeTYP_I4 	= MAKE_ID( 0x82, 1, 1, 4 ),
	gpeTYP_UF  	= MAKE_ID( 0x42, 1, 1, 4 ),
	gpeTYP_F  	= MAKE_ID( 0xc2, 1, 1, 4 ),
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
