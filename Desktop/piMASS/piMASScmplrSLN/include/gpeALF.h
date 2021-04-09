#ifndef GPEALF_H_INCLUDED
#define GPEALF_H_INCLUDED
//~ MIT License
//
//~ Copyright (c) 2019 AliveMOon
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
#include "piMASS.h"


#define gpmbNUM( c ) ( c >= '0' && c <= '9' )
#define gpmbASCII( c, a ) (c < 0x80 ? a[c] : false)
#define gpmbABC( c, a ) (c < 0x80 ? a[c] : true)	// UTF8 kompatible azaz 0x80 felett betünek vesz mindent
//#define gpmbUTF8( c, a ) (c < 0x80 ? a[c] : true)

#define gpdUP ('a'-'A')
#define gpdUP_CASE(a){ if( a >= 'a' && a <= 'z' ){ a -= gpdUP; }}
#define gpmUP(a)( a = ( a >= 'a' && a <= 'z' ) ? a-gpdUP : a )
#define gpdALF 								('Z'-'@')
#define gpdA(a)								( a -'@' )
#define gpdAB(a,b)							(U8)( gpdA(a)*gpdALF + gpdA(b) )
#define gpdABC(a,b,c)						(U8)( gpdAB(a,b)*gpdALF + gpdA(c) )
#define gpdABCD(a,b,c,d)					(U8)( gpdABC(a,b,c)*gpdALF + gpdA(d) )
#define gpdABCDE(a,b,c,d,e)					(U8)( gpdABCD(a,b,c,d)*gpdALF + gpdA(e) )
#define gpdABCDEF(a,b,c,d,e,f)				(U8)( gpdABCDE(a,b,c,d,e)*gpdALF + gpdA(f) )
#define gpdABCDEFG(a,b,c,d,e,f,g)			(U8)( gpdABCDEF(a,b,c,d,e,f)*gpdALF + gpdA(g) )
#define gpdABCDEFGH(a,b,c,d,e,f,g,h)		(U8)( gpdABCDEFG(a,b,c,d,e,f,g)*gpdALF + gpdA(h) )
#define gpdABCDEFGHI(a,b,c,d,e,f,g,h,i)		(U8)( gpdABCDEFGH(a,b,c,d,e,f,g,h)*gpdALF + gpdA(i) )
#define gpdABCDEFGHIJ(a,b,c,d,e,f,g,h,i,j)	(U8)( gpdABCDEFGHI(a,b,c,d,e,f,g,h,i)*gpdALF + gpdA(j) )

#define gpdABCDEFGHIJK(a,b,c,d,e,f,g,h,i,j,k)		(U8)( gpdABCDEFGHIJ(a,b,c,d,e,f,g,h,i,j)*gpdALF + gpdA(k) )
#define gpdABCDEFGHIJKL(a,b,c,d,e,f,g,h,i,j,k,l)	(U8)( gpdABCDEFGHIJK(a,b,c,d,e,f,g,h,i,j,k)*gpdALF + gpdA(l) )
#define gpdABCDEFGHIJKLM(a,b,c,d,e,f,g,h,i,j,k,l,m)	(U8)( gpdABCDEFGHIJKL(a,b,c,d,e,f,g,h,i,j,k,l)*gpdALF + gpdA(m) )

typedef enum gpeALFu4:U4 {
	gpeALFu4_null = 0,
	gpeALFu4_A,
	gpeALFu4_B,
	gpeALFu4_C,

	gpeALFu4_D,
	gpeALFu4_E,
	gpeALFu4_F,
	gpeALFu4_G,

	gpeALFu4_H,
	gpeALFu4_I,
	gpeALFu4_J,
	gpeALFu4_K,

	gpeALFu4_L,
	gpeALFu4_M,
	gpeALFu4_N,
	gpeALFu4_O,

	gpeALFu4_P,
	gpeALFu4_Q,
	gpeALFu4_R,
	gpeALFu4_S,

	gpeALFu4_T,
	gpeALFu4_U,
	gpeALFu4_V,
	gpeALFu4_W,

	gpeALFu4_X,
	gpeALFu4_Y,
	gpeALFu4_Z,
	//- 2 -----------------
	gpeALFu4_AA = gpdAB('A', 'A'),
	gpeALFu4_AN = gpdAB('A', 'N'),
	gpeALFu4_AT = gpdAB('A', 'T'),
	gpeALFu4_CH = gpdAB('C', 'H'),
	gpeALFu4_CL = gpdAB('C', 'L'),
	gpeALFu4_CU = gpdAB('C', 'U'),
	gpeALFu4_CW = gpdAB('C', 'W'),
	gpeALFu4_DS = gpdAB('D', 'S'),
	gpeALFu4_FI = gpdAB('F', 'I'),
	gpeALFu4_GX = gpdAB('G', 'X'),
	gpeALFu4_ID = gpdAB('I', 'D'),
	gpeALFu4_IF = gpdAB('I', 'F'),
	gpeALFu4_IN = gpdAB('I', 'N'),

	// key ( A, B, ..., Z )
	gpeALFu4_KA = gpdAB('K', 'A'),			// KEYBOARD 'a'
	gpeALFu4_KO = gpdAB('K', 'O'),
	gpeALFu4_KZ = gpdAB('K', 'Z'),
	gpeALFu4_LG = gpdAB('L', 'G'),

	gpeALFu4_MA = gpdAB('M', 'A'),
	gpeALFu4_MN = gpdAB('M', 'N'),
	gpeALFu4_MS = gpdAB('M', 'S'),
	gpeALFu4_MX = gpdAB('M', 'X'),
	gpeALFu4_MY = gpdAB('M', 'Y'),
	gpeALFu4_NO = gpdAB('N', 'O'),
	gpeALFu4_ON = gpdAB('O', 'N'),
	gpeALFu4_OR = gpdAB('O', 'R'),
	gpeALFu4_PI = gpdAB('P', 'I'),
	gpeALFu4_RE = gpdAB('R', 'E'),
	gpeALFu4_SL = gpdAB('S', 'L'),
	gpeALFu4_SR = gpdAB('S', 'R'),
	gpeALFu4_UP = gpdAB('U', 'P'),
	gpeALFu4_WB = gpdAB('W', 'B'),
	gpeALFu4_WH = gpdAB('W', 'H'),
	gpeALFu4_WX = gpdAB('W', 'X'),
	gpeALFu4_WY = gpdAB('W', 'Y'),
	gpeALFu4_WW = gpdAB('W', 'W'),
	gpeALFu4_ZM = gpdAB('Z', 'M'),
	gpeALFu4_2 = gpdAB('Z', 'Z'),
	//- 3 -----------------
	gpeALFu4_AAA = gpdABC('A', 'A', 'A'),
	gpeALFu4_ABS = gpdABC('A', 'B', 'S'),
	gpeALFu4_ACE = gpdABC('A', 'C', 'E'), // access
	gpeALFu4_ACT = gpdABC('A', 'C', 'T'),
	gpeALFu4_ADD = gpdABC('A', 'D', 'D'),
	gpeALFu4_ADR = gpdABC('A', 'D', 'R'),
	gpeALFu4_AIN = gpdABC('A', 'I', 'N'),
	gpeALFu4_AND = gpdABC('A', 'N', 'D'),
	gpeALFu4_ARJ = gpdABC('A', 'R', 'J'),
	gpeALFu4_ARR = gpdABC('A', 'R', 'R'),
	gpeALFu4_ASM = gpdABC('A', 'S', 'M'),
	gpeALFu4_ATA = gpdABC('A', 'T', 'A'),
	gpeALFu4_BIN = gpdABC('B', 'I', 'N'),
	gpeALFu4_BLK = gpdABC('B', 'L', 'K'),
	gpeALFu4_BOB = gpdABC('B', 'O', 'B'),
	gpeALFu4_BOX = gpdABC('B', 'O', 'X'),
	gpeALFu4_CAM = gpdABC('C', 'A', 'M'),
	gpeALFu4_CLR = gpdABC('C', 'L', 'R'),
	gpeALFu4_CMX = gpdABC('C', 'M', 'X'),
	gpeALFu4_CMY = gpdABC('C', 'M', 'Y'),
	gpeALFu4_CNL = gpdABC('C', 'N', 'L'),
	gpeALFu4_COM = gpdABC('C', 'O', 'M'),
	gpeALFu4_COS = gpdABC('C', 'O', 'S'),
	gpeALFu4_CPP = gpdABC('C', 'P', 'P'),
	gpeALFu4_CPU = gpdABC('C', 'P', 'U'),
	gpeALFu4_CPY = gpdABC('C', 'P', 'Y'),
	gpeALFu4_CUT = gpdABC('C', 'U', 'T'),
	gpeALFu4_DEC = gpdABC('D', 'E', 'C'),
	gpeALFu4_DEF = gpdABC('D', 'E', 'F'),
	gpeALFu4_DEL = gpdABC('D', 'E', 'L'),
	gpeALFu4_DIR = gpdABC('D', 'I', 'R'),
	gpeALFu4_DIV = gpdABC('D', 'I', 'V'),
	gpeALFu4_DOT = gpdABC('D', 'O', 'T'),
	gpeALFu4_DST = gpdABC('D', 'S', 'T'),
	gpeALFu4_DZR = gpdABC('D', 'Z', 'R'),
	gpeALFu4_END = gpdABC('E', 'N', 'D'),
	gpeALFu4_ERU = gpdABC('E', 'R', 'U'),
	gpeALFu4_EYE = gpdABC('E', 'Y', 'E'),
	gpeALFu4_EXP = gpdABC('E', 'X', 'P'),

	// function key ( f1, f2, ..., f12)
	gpeALFu4_FKA = gpdABC('F', 'K', 'A'),
	gpeALFu4_FKL = gpdABC('F', 'K', 'L'),

	gpeALFu4_FMX = gpdABC('F', 'M', 'X'),
	gpeALFu4_FMY = gpdABC('F', 'M', 'Y'),
	gpeALFu4_FOR = gpdABC('F', 'O', 'R'),
	gpeALFu4_FPS = gpdABC('F', 'P', 'S'),
	gpeALFu4_GET = gpdABC('G', 'E', 'T'),
	gpeALFu4_GIM = gpdABC('G', 'I', 'M'),
	gpeALFu4_GIO = gpdABC('G', 'I', 'O'),
	gpeALFu4_GIS = gpdABC('G', 'I', 'S'),
	gpeALFu4_GIT = gpdABC('G', 'I', 'T'),
	gpeALFu4_GIV = gpdABC('G', 'I', 'V'),
	gpeALFu4_GPU = gpdABC('G', 'P', 'U'),
	gpeALFu4_GXT = gpdABC('G', 'X', 'T'),
	gpeALFu4_HUD = gpdABC('H', 'U', 'D'),
	gpeALFu4_INC = gpdABC('I', 'N', 'C'),
	gpeALFu4_INP = gpdABC('I', 'N', 'P'),
	gpeALFu4_INT = gpdABC('I', 'N', 'T'),
	gpeALFu4_INV = gpdABC('I', 'N', 'V'),
	gpeALFu4_JPG = gpdABC('J', 'P', 'G'),
	gpeALFu4_JOY = gpdABC('J', 'O', 'Y'),
	gpeALFu4_KEY = gpdABC('K', 'E', 'Y'),
	gpeALFu4_LOG = gpdABC('L', 'O', 'G'),
	gpeALFu4_LWO = gpdABC('L', 'W', 'O'),
	gpeALFu4_LWS = gpdABC('L', 'W', 'S'),
	gpeALFu4_MAN = gpdABC('M', 'A', 'N'),
	gpeALFu4_MAX = gpdABC('M', 'A', 'X'),
	gpeALFu4_MEM = gpdABC('M', 'E', 'M'),
	gpeALFu4_MIN = gpdABC('M', 'I', 'N'),
	gpeALFu4_MLB = gpdABC('M', 'L', 'B'),
	gpeALFu4_MMX = gpdABC('M', 'M', 'X'),
	gpeALFu4_MMY = gpdABC('M', 'M', 'Y'),
	gpeALFu4_MOM = gpdABC('M', 'O', 'M'),
	gpeALFu4_MOV = gpdABC('M', 'O', 'V'),
	gpeALFu4_MPG = gpdABC('M', 'R', 'B'),
	gpeALFu4_MRB = gpdABC('M', 'R', 'B'),

	gpeALFu4_MSG = gpdABC('M', 'S', 'G'),

	gpeALFu4_MUL = gpdABC('M', 'U', 'L'),
	gpeALFu4_NEW = gpdABC('N', 'E', 'W'),
	gpeALFu4_OFF = gpdABC('O', 'F', 'F'),
	gpeALFu4_ORM = gpdABC('O', 'R', 'M'),
	gpeALFu4_OUT = gpdABC('O', 'U', 'T'),
	gpeALFu4_PIC = gpdABC('P', 'I', 'C'),
	gpeALFu4_PIX = gpdABC('P', 'I', 'X'),
	gpeALFu4_PMX = gpdABC('P', 'M', 'X'),
	gpeALFu4_PMY = gpdABC('P', 'M', 'Y'),
	gpeALFu4_PNG = gpdABC('P', 'N', 'G'),
	gpeALFu4_PNT = gpdABC('P', 'N', 'T'),
	gpeALFu4_PRG = gpdABC('P', 'R', 'G'),
	gpeALFu4_REF = gpdABC('R', 'E', 'F'),
	gpeALFu4_REG = gpdABC('R', 'E', 'G'),
	gpeALFu4_REM = gpdABC('R', 'E', 'M'),
	gpeALFu4_REN = gpdABC('R', 'E', 'N'),
	gpeALFu4_RET = gpdABC('R', 'E', 'T'),
	gpeALFu4_RUN = gpdABC('R', 'U', 'N'),
	gpeALFu4_SEC = gpdABC('S', 'E', 'C'),
	gpeALFu4_SIN = gpdABC('S', 'I', 'N'),
	gpeALFu4_SIT = gpdABC('S', 'I', 'T'),
	gpeALFu4_SLM = gpdABC('S', 'L', 'M'),
	gpeALFu4_SRC = gpdABC('S', 'R', 'C'),
	gpeALFu4_SRM = gpdABC('S', 'R', 'M'),
	gpeALFu4_STR = gpdABC('S', 'T', 'R'),
	gpeALFu4_STK = gpdABC('S', 'T', 'K'),
	gpeALFu4_SUB = gpdABC('S', 'U', 'B'),
	gpeALFu4_SUM = gpdABC('S', 'U', 'M'),
	gpeALFu4_SYS = gpdABC('S', 'Y', 'S'),

	gpeALFu4_TAG = gpdABC('T', 'A', 'G'),
	gpeALFu4_TYF = gpdABC('T', 'Y', 'F'),
	gpeALFu4_TYI = gpdABC('T', 'Y', 'I'),
	gpeALFu4_TYU = gpdABC('T', 'Y', 'U'),

	gpeALFu4_VAL = gpdABC('V', 'A', 'L'),
	gpeALFu4_VAR = gpdABC('V', 'A', 'R'),
	gpeALFu4_VEC = gpdABC('V', 'E', 'C'),
	gpeALFu4_VOX = gpdABC('V', 'O', 'X'),
	gpeALFu4_WIN = gpdABC('W', 'I', 'N'),
	gpeALFu4_WMX = gpdABC('W', 'M', 'X'),
	gpeALFu4_WMY = gpdABC('W', 'M', 'Y'),
	gpeALFu4_YPR = gpdABC('Y', 'P', 'R'),
	gpeALFu4_XOR = gpdABC('X', 'O', 'R'),
	gpeALFu4_ZIP = gpdABC('Z', 'I', 'P'),
	gpeALFu4_3 = gpdABC('Z', 'Z', 'Z'),
	//- 4 -----------------
	gpeALFu4_ABMS = gpdABCD('A', 'B', 'M', 'S'),
	gpeALFu4_ACOS = gpdABCD('A', 'C', 'O', 'S'),
	gpeALFu4_ADDM = gpdABCD('A', 'D', 'D', 'M'),
	gpeALFu4_ANDM = gpdABCD('A', 'N', 'D', 'M'),
	gpeALFu4_ASIN = gpdABCD('A', 'S', 'I', 'N'),
	gpeALFu4_BELG = gpdABCD('B', 'E', 'L', 'G'),
	gpeALFu4_BGLG = gpdABCD('B', 'G', 'L', 'G'),
	gpeALFu4_BONE = gpdABCD('B', 'O', 'N', 'E'),
	gpeALFu4_BUMM = gpdABCD('B', 'U', 'M', 'M'),
	gpeALFu4_BUMP = gpdABCD('B', 'U', 'M', 'P'),
	gpeALFu4_CNLC = gpdABCD('C', 'N', 'L', 'C'),

	gpeALFu4_COME = gpdABCD('C', 'O', 'M', 'E'),
	gpeALFu4_COMS = gpdABCD('C', 'O', 'M', 'S'),

	gpeALFu4_COPY = gpdABCD('C', 'O', 'P', 'Y'),
	gpeALFu4_CRSA = gpdABCD('C', 'R', 'S', 'A'),
	gpeALFu4_CRSL = gpdABCD('C', 'R', 'S', 'L'),
	gpeALFu4_CRSN = gpdABCD('C', 'R', 'S', 'N'),
	gpeALFu4_CRSR = gpdABCD('C', 'R', 'S', 'R'),
	gpeALFu4_CRSX = gpdABCD('C', 'R', 'S', 'X'),
	gpeALFu4_CRSY = gpdABCD('C', 'R', 'S', 'Y'),
	gpeALFu4_CTRL = gpdABCD('C', 'T', 'R', 'L'),
	gpeALFu4_DEEP = gpdABCD('D', 'E', 'E', 'P'),
	gpeALFu4_DIME = gpdABCD('D', 'I', 'M', 'E'),
	gpeALFu4_DIMS = gpdABCD('D', 'I', 'M', 'S'),
	gpeALFu4_DIVM = gpdABCD('D', 'I', 'V', 'M'),
	gpeALFu4_EDGE = gpdABCD('E', 'D', 'G', 'E'),
	gpeALFu4_ELSE = gpdABCD('E', 'L', 'S', 'E'),
	gpeALFu4_ENDD = gpdABCD('E', 'N', 'D', 'D'),
	gpeALFu4_EQLG = gpdABCD('E', 'Q', 'L', 'G'),
	gpeALFu4_EXIT = gpdABCD('E', 'X', 'I', 'T'),
	gpeALFu4_EXPL = gpdABCD('E', 'X', 'P', 'L'),
	gpeALFu4_EXPM = gpdABCD('E', 'X', 'P', 'M'),
	gpeALFu4_FERI = gpdABCD('F', 'E', 'R', 'I'),
	gpeALFu4_FILE = gpdABCD('F', 'I', 'L', 'E'),
	gpeALFu4_FIND = gpdABCD('F', 'I', 'N', 'D'),
	gpeALFu4_FSEC = gpdABCD('F', 'S', 'E', 'C'),
	gpeALFu4_FUNC = gpdABCD('F', 'U', 'N', 'C'),
	gpeALFu4_GOLD = gpdABCD('G', 'O', 'L', 'D'),


	gpeALFu4_GPUC = gpdABCD('G', 'P', 'U', 'C'),	// gpeALFu4_GPU + VOXEL VERTEX

	gpeALFu4_HAIR = gpdABCD('H', 'A', 'I', 'R'),
	gpeALFu4_HAND = gpdABCD('H', 'A', 'N', 'D'),
	gpeALFu4_HEAD = gpdABCD('H', 'E', 'A', 'D'),
	gpeALFu4_HELP = gpdABCD('H', 'E', 'L', 'P'),
	gpeALFu4_HOST = gpdABCD('H', 'O', 'S', 'T'),
	gpeALFu4_HTML = gpdABCD('H', 'T', 'M', 'L'),
	gpeALFu4_HTTP = gpdABCD('H', 'T', 'T', 'P'),

	gpeALFu4_IDLE = gpdABCD('I', 'D', 'L', 'E'),
	gpeALFu4_INIT = gpdABCD('I', 'N', 'I', 'T'),

	gpeALFu4_ITEM = gpdABCD('I', 'T', 'E', 'M'),
	gpeALFu4_JUMP = gpdABCD('J', 'U', 'M', 'P'),

	gpeALFu4_KALT = gpdABCD('K', 'A', 'L', 'T'),
	gpeALFu4_KICK = gpdABCD('K', 'I', 'C', 'K'),

	gpeALFu4_LINE = gpdABCD('L', 'I', 'N', 'E'),
	gpeALFu4_LEFT = gpdABCD('L', 'E', 'F', 'T'),
	gpeALFu4_LELG = gpdABCD('L', 'E', 'L', 'G'),
	gpeALFu4_LOOP = gpdABCD('L', 'O', 'O', 'P'),
	gpeALFu4_LTLG = gpdABCD('L', 'T', 'L', 'G'),
	gpeALFu4_MAIL = gpdABCD('M', 'A', 'I', 'L'),
	gpeALFu4_MAIN = gpdABCD('M','A',  'I', 'N'),
	gpeALFu4_MINI = gpdABCD('M', 'I', 'N', 'I'),

	gpeALFu4_MONO = gpdABCD('M', 'O', 'N', 'O'),
	gpeALFu4_MOON = gpdABCD('M', 'O', 'O', 'N'),

	gpeALFu4_MLBX = gpdABCD('M', 'L', 'B', 'X'),		// egér X poziciója mielött le lett nyomva a BAL gomb
	gpeALFu4_MLBY = gpdABCD('M', 'L', 'B', 'Y'),		// egér Y poziciója mielött le lett nyomva a BAL gomb

	gpeALFu4_MRBX = gpdABCD('M', 'R', 'B', 'X'),		// egér X poziciója mielött le lett nyomva a JOBB gomb
	gpeALFu4_MRBY = gpdABCD('M', 'R', 'B', 'Y'),		// egér Y poziciója mielött le lett nyomva a JOBB gomb

	gpeALFu4_MULM = gpdABCD('M', 'U', 'L', 'M'),

	gpeALFu4_NAME = gpdABCD('N', 'A', 'M', 'E'),
	gpeALFu4_MSEC = gpdABCD('M', 'S', 'E', 'C'),
	gpeALFu4_NEAR = gpdABCD('N', 'E', 'A', 'R'),
	gpeALFu4_NUSE = gpdABCD('N', 'U', 'S', 'E'),
	gpeALFu4_OPER = gpdABCD('O', 'P', 'E', 'R'),
	gpeALFu4_ORLG = gpdABCD('O', 'R', 'L', 'G'),
	gpeALFu4_OTOS = gpdABCD('O', 'T', 'O', 'S'),
	gpeALFu4_OVER = gpdABCD('O', 'V', 'E', 'R'),

	gpeALFu4_PACK = gpdABCD('P', 'A', 'C', 'K'),
	gpeALFu4_PARA = gpdABCD('P', 'A', 'R', 'A'),
	gpeALFu4_PASS = gpdABCD('P', 'A', 'S', 'S'),

	gpeALFu4_PIXN = gpdABCD('P', 'I', 'X', 'N'),
	gpeALFu4_POLY = gpdABCD('P', 'O', 'L', 'Y'),
	gpeALFu4_PORT = gpdABCD('P', 'O', 'R', 'T'),
	gpeALFu4_PREV = gpdABCD('P', 'R', 'E', 'V'),

	gpeALFu4_QUAD = gpdABCD('Q', 'U', 'A', 'D'),

	//gpeALFu4_RESA =	gpdABCD( 'R','E','S','A' ),
	gpeALFu4_REMM = gpdABCD('R', 'E', 'M', 'M'),
	gpeALFu4_ROBI = gpdABCD('R', 'O', 'B', 'I'),
	gpeALFu4_ROCK = gpdABCD('R', 'O', 'C', 'K'),
	gpeALFu4_SAVE = gpdABCD('S', 'A', 'V', 'E'),
	gpeALFu4_STAR = gpdABCD('S', 'T', 'A', 'R'),
	gpeALFu4_STEP = gpdABCD('S', 'T', 'E', 'P'),
	gpeALFu4_STON = gpdABCD('S', 'T', 'O', 'N'),
	gpeALFu4_STOW = gpdABCD('S', 'T', 'O', 'W'),
	gpeALFu4_SUBM = gpdABCD('S', 'U', 'B', 'M'),
	gpeALFu4_SQRT = gpdABCD('S', 'Q', 'R', 'T'),

	gpeALFu4_TRUE = gpdABCD('T', 'R', 'U', 'E'),
	gpeALFu4_TURN = gpdABCD('T', 'U', 'R', 'N'),

	gpeALFu4_TYPE = gpdABCD('T', 'Y', 'P', 'E'),

	gpeALFu4_USER = gpdABCD('U', 'S', 'E', 'R'),

	gpeALFu4_WALK = gpdABCD('W', 'A', 'L', 'K'),
	gpeALFu4_WHAM = gpdABCD('W', 'H', 'A', 'M'),

	gpeALFu4_XORM = gpdABCD('X', 'O', 'R', 'M'),

	gpeALFu4_4 = gpdABCD('Z', 'Z', 'Z', 'Z'),
	//- 5 -----------------
	gpeALFu4_ANDLG = gpdABCDE('A', 'N', 'D', 'L', 'G'),
	gpeALFu4_ARRAY = gpdABCDE('A', 'R', 'R', 'A', 'Y'),
	gpeALFu4_BEGIN = gpdABCDE('B', 'E', 'G', 'I', 'N'),
	gpeALFu4_BLOCK = gpdABCDE('B', 'L', 'O', 'C', 'K'),
	gpeALFu4_BOBER = gpdABCDE('B', 'O', 'B', 'E', 'R'),
	gpeALFu4_BRAKE = gpdABCDE('B', 'R', 'A', 'K', 'E'),
	gpeALFu4_BRAKS = gpdABCDE('B', 'R', 'A', 'K', 'S'),
	gpeALFu4_BREAK = gpdABCDE('B', 'R', 'E', 'A', 'K'),
	gpeALFu4_BUBLE = gpdABCDE('B', 'U', 'B', 'L', 'E'),
	gpeALFu4_CACHE = gpdABCDE('C', 'A', 'C', 'H', 'E'),
	gpeALFu4_CLASS = gpdABCDE('C', 'L', 'A', 'S', 'S'),
	gpeALFu4_COLOR = gpdABCDE('C', 'O', 'L', 'O', 'R'),
	gpeALFu4_CONST = gpdABCDE('C', 'O', 'N', 'S', 'T'),
	gpeALFu4_COUNT = gpdABCDE('C', 'O', 'U', 'N', 'T'),
	gpeALFu4_CREAT = gpdABCDE('C', 'R', 'E', 'A', 'T'),
	gpeALFu4_CROSS = gpdABCDE('C', 'R', 'O', 'S', 'S'),
	gpeALFu4_CYCLE = gpdABCDE('C', 'Y', 'C', 'L', 'E'),
	gpeALFu4_DEBUG = gpdABCDE('D', 'E', 'B', 'U', 'G'),
	gpeALFu4_DEBUS = gpdABCDE('D', 'E', 'B', 'U', 'S'),
	gpeALFu4_ENTER = gpdABCDE('E', 'N', 'T', 'E', 'R'),
	gpeALFu4_ENTRY = gpdABCDE('E', 'N', 'T', 'R', 'Y'),
	gpeALFu4_ERECT = gpdABCDE('E', 'R', 'E', 'C', 'T'),
	gpeALFu4_GETMX = gpdABCDE('G', 'E', 'T', 'M', 'X'),
	gpeALFu4_GLOBA = gpdABCDE('G', 'L', 'O', 'B', 'A'),
	gpeALFu4_HISTI = gpdABCDE('H', 'I', 'S', 'T', 'I'),
	gpeALFu4_HUMAN = gpdABCDE('H', 'U', 'M', 'A', 'N'),
	gpeALFu4_INDEX = gpdABCDE('I', 'N', 'D', 'E', 'X'),
	gpeALFu4_KCTRL = gpdABCDE('K', 'C', 'T', 'R', 'L'),
	gpeALFu4_LABEL = gpdABCDE('L', 'A', 'B', 'E', 'L'),
	gpeALFu4_LATHE = gpdABCDE('L', 'A', 'T', 'H', 'E'),
	gpeALFu4_LOCAL = gpdABCDE('L', 'O', 'C', 'A', 'L'),
	gpeALFu4_NEGLG = gpdABCDE('N', 'E', 'G', 'L', 'G'),
	gpeALFu4_NEQLG = gpdABCDE('N', 'E', 'Q', 'L', 'G'),
	gpeALFu4_NGATE = gpdABCDE('N', 'G', 'A', 'T', 'E'),
	gpeALFu4_NGCON = gpdABCDE('N', 'G', 'C', 'O', 'N'),
	gpeALFu4_NGDIE = gpdABCDE('N', 'G', 'D', 'I', 'E'),

	gpeALFu4_ORBIT = gpdABCDE('O', 'R', 'B', 'I', 'T'),
	gpeALFu4_PASTE = gpdABCDE('P', 'A', 'S', 'T', 'E'),
	gpeALFu4_PICQC = gpdABCDE('P', 'I', 'C', 'Q', 'C'),
	gpeALFu4_PRINT = gpdABCDE('P', 'R', 'I', 'N', 'T'),
	gpeALFu4_REPIC = gpdABCDE('R', 'E', 'P', 'I', 'C'),

	gpeALFu4_RESET = gpdABCDE('R', 'E', 'S', 'E', 'T'),

	gpeALFu4_RIGHT = gpdABCDE('R', 'I', 'G', 'H', 'T'),
	gpeALFu4_ROOTM = gpdABCDE('R', 'O', 'O', 'T', 'M'),
	gpeALFu4_RULES = gpdABCDE('R', 'U', 'L', 'E', 'S'),

	gpeALFu4_SETUP = gpdABCDE('S', 'E', 'T', 'U', 'P'),
	gpeALFu4_SHARE = gpdABCDE('S', 'H', 'A', 'R', 'E'),
	gpeALFu4_SHOES = gpdABCDE('S', 'H', 'O', 'E', 'S'),
	gpeALFu4_SLEFT = gpdABCDE('S', 'L', 'E', 'F', 'T'),
	gpeALFu4_STORE = gpdABCDE('S', 'T', 'O', 'R', 'E'),

	gpeALFu4_STACK = gpdABCDE('S', 'T', 'A', 'C', 'K'),
	gpeALFu4_STONE = gpdABCDE('S', 'T', 'O', 'N', 'E'),
	gpeALFu4_TABWH = gpdABCDE('T', 'A', 'B', 'W', 'H'),
	gpeALFu4_TABXY = gpdABCDE('T', 'A', 'B', 'X', 'Y'),
	gpeALFu4_THISA = gpdABCDE('T', 'H', 'I', 'S', 'A'),
	gpeALFu4_THISN = gpdABCDE('T', 'H', 'I', 'S', 'N'),
	gpeALFu4_TOKEN = gpdABCDE('T', 'O', 'K', 'E', 'N'),
	gpeALFu4_TOUCH = gpdABCDE('T', 'O', 'U', 'C', 'H'),
	gpeALFu4_TRACK = gpdABCDE('T', 'R', 'A', 'C', 'K'),
	gpeALFu4_VARIA = gpdABCDE('V', 'A', 'R', 'I', 'A'),

	gpeALFu4_UNDEF = gpdABCDE('U', 'N', 'D', 'E', 'F'),

	gpeALFu4_UNSEL = gpdABCDE('U', 'N', 'S', 'E', 'L'),

	gpeALFu4_WHILE = gpdABCDE('W', 'H', 'I', 'L', 'E'),

	gpeALFu4_WMLBX = gpdABCDE('W', 'M', 'L', 'B', 'X'),		// egér X poziciója mielött le lett nyomva a BAL gomb
	gpeALFu4_WMLBY = gpdABCDE('W', 'M', 'L', 'B', 'Y'),		// egér Y poziciója mielött le lett nyomva a BAL gomb

	gpeALFu4_WMRBX = gpdABCDE('W', 'M', 'R', 'B', 'X'),		// egér X poziciója mielött le lett nyomva a JOBB gomb
	gpeALFu4_WMRBY = gpdABCDE('W', 'M', 'R', 'B', 'Y'),		// egér Y poziciója mielött le lett nyomva a JOBB gomb
	gpeALFu4_WRITE = gpdABCDE('W', 'R', 'I', 'T', 'E'),

	gpeALFu4_5 = gpdABCDE('Z', 'Z', 'Z', 'Z', 'Z'),

	//- 6 -----------------
	gpeALFu4_AAAAAA = gpdABCDEF('A', 'A', 'A', 'A', 'A', 'A'),
	gpeALFu4_ACCEPT = gpdABCDEF('A', 'C', 'C', 'E', 'P', 'T'),
	gpeALFu4_ABOARD = gpdABCDEF('A', 'B', 'O', 'A', 'R', 'D'),
	gpeALFu4_BUBLER = gpdABCDEF('B', 'U', 'B', 'L', 'E', 'R'),
	gpeALFu4_CONSTR = gpdABCDEF('C', 'O', 'N', 'S', 'T', 'R'),
	gpeALFu4_ELAPSE = gpdABCDEF('E', 'L', 'A', 'P', 'S', 'E'),
	gpeALFu4_FERIKE = gpdABCDEF('F', 'E', 'R', 'I', 'K', 'E'),
	gpeALFu4_FLINCH = gpdABCDEF('F', 'L', 'I', 'N', 'C', 'H'),
	gpeALFu4_FORBID = gpdABCDEF('F', 'O', 'R', 'B', 'I', 'D'),
	gpeALFu4_GALAXY = gpdABCDEF('G', 'A', 'L', 'A', 'X', 'Y'),
	gpeALFu4_JACKET = gpdABCDEF('J', 'A', 'C', 'K', 'E', 'T'),
	gpeALFu4_KSHIFT = gpdABCDEF('K', 'S', 'H', 'I', 'F', 'T'),
	gpeALFu4_LISTEN = gpdABCDEF('L', 'I', 'S', 'T', 'E', 'N'),

	gpeALFu4_NEWROW = gpdABCDEF('N', 'E', 'W', 'R', 'O', 'W'),

	gpeALFu4_MISSED = gpdABCDEF('M', 'I', 'S', 'S', 'E', 'D'),
	gpeALFu4_MODBUS = gpdABCDEF('M', 'O', 'D', 'B', 'U', 'S'),
	gpeALFu4_NBUILD = gpdABCDEF('N', 'B', 'U', 'I', 'L', 'D'),
	gpeALFu4_PICCPY = gpdABCDEF('P', 'I', 'C', 'C', 'P', 'Y'),
	gpeALFu4_PLANET = gpdABCDEF('P', 'L', 'A', 'N', 'E', 'T'),
	gpeALFu4_POLYER = gpdABCDEF('P', 'O', 'L', 'Y', 'E', 'R'),
	gpeALFu4_PRAGMA = gpdABCDEF('P', 'R', 'A', 'G', 'M', 'A'),
	gpeALFu4_REGGIO = gpdABCDEF('R', 'E', 'G', 'G', 'I', 'O'),
	gpeALFu4_RENAME = gpdABCDEF('R', 'E', 'N', 'A', 'M', 'E'),
	gpeALFu4_RETURN = gpdABCDEF('R', 'E', 'T', 'U', 'R', 'N'),
	//gpeALFu4_SHADER =	gpdABCDEF( 'S','H','A','D','E','R' ),

	gpeALFu4_SIZEOF = gpdABCDEF('S', 'I', 'Z', 'E', 'O', 'F'),
	gpeALFu4_SHADOW = gpdABCDEF('S', 'H', 'A', 'D', 'O', 'W'),
	gpeALFu4_SPRITE = gpdABCDEF('S', 'P', 'R', 'I', 'T', 'E'),
	gpeALFu4_SRIGHT = gpdABCDEF('S', 'R', 'I', 'G', 'H', 'T'),
	gpeALFu4_STEREO = gpdABCDEF('S', 'T', 'E', 'R', 'E', 'O'),
	gpeALFu4_STOREE = gpdABCDEF('S', 'T', 'O', 'R', 'E', 'E'),
	gpeALFu4_STOWUP = gpdABCDEF('S', 'T', 'O', 'W', 'U', 'P'),
	gpeALFu4_STRROW = gpdABCDEF('S', 'T', 'R', 'R', 'O', 'W'),
	gpeALFu4_SWITCH = gpdABCDEF('S', 'W', 'I', 'T', 'C', 'H'),
	gpeALFu4_TARGET = gpdABCDEF('T', 'A', 'R', 'G', 'E', 'T'),
	gpeALFu4_TELNET = gpdABCDEF('T', 'E', 'L', 'N', 'E', 'T'),

	gpeALFu4_6 		= gpdABCDEF('Z', 'Z', 'Z', 'Z', 'Z', 'Z'),

	//- 7 -----------------


	gpeALFu4_MWLQKWU = (U4)gpdABCDEFG('M', 'W', 'L', 'Q', 'K', 'W', 'U'),
	gpeALFu4_U4      = gpeALFu4_MWLQKWU,
} gpeALFu4;
typedef enum gpeALF:I8
{
	gpeALF_null = 0,
	gpeALF_A,
	gpeALF_B,
	gpeALF_C,

	gpeALF_D,
	gpeALF_E,
	gpeALF_F,
	gpeALF_G,

	gpeALF_H,
	gpeALF_I,
	gpeALF_J,
	gpeALF_K,

	gpeALF_L,
	gpeALF_M,
	gpeALF_N,
	gpeALF_O,

	gpeALF_P,
	gpeALF_Q,
	gpeALF_R,
	gpeALF_S,

	gpeALF_T,
	gpeALF_U,
	gpeALF_V,
	gpeALF_W,

	gpeALF_X,
	gpeALF_Y,
	gpeALF_Z,
	//- 2 -----------------
	gpeALF_AA, gpeALF_AB, gpeALF_AC,
	gpeALF_AD, gpeALF_AE, gpeALF_AF, gpeALF_AG,
	gpeALF_AH, gpeALF_AI, gpeALF_AJ, gpeALF_AK,
	gpeALF_AL, gpeALF_AM, gpeALF_AN, gpeALF_AO,
	gpeALF_AP, gpeALF_AQ, gpeALF_AR, gpeALF_AS,
	gpeALF_AT, 									// SIM7600 - AT
	gpeALF_AU, gpeALF_AV, gpeALF_AW,
	gpeALF_AX, gpeALF_AY, gpeALF_AZ,

	gpeALF_BA, gpeALF_BB, gpeALF_BC,
	gpeALF_BD, gpeALF_BE, gpeALF_BF, gpeALF_BG,
	gpeALF_BH, gpeALF_BI, gpeALF_BJ, gpeALF_BK,
	gpeALF_BL, gpeALF_BM, gpeALF_BN, gpeALF_BO,
	gpeALF_BP, gpeALF_BQ, gpeALF_BR, gpeALF_BS,
	gpeALF_BT, gpeALF_BU, gpeALF_BV, gpeALF_BW,
	gpeALF_BX, gpeALF_BY, gpeALF_BZ,

	gpeALF_CA, gpeALF_CB, gpeALF_CC,
	gpeALF_CD, gpeALF_CE, gpeALF_CF, gpeALF_CG,
	gpeALF_CH, gpeALF_CI, gpeALF_CJ, gpeALF_CK,
	gpeALF_CL, gpeALF_CM, gpeALF_CN, gpeALF_CO,
	gpeALF_CP, gpeALF_CQ, gpeALF_CR, gpeALF_CS,
	gpeALF_CT, gpeALF_CU, gpeALF_CV, gpeALF_CW,
	gpeALF_CX, gpeALF_CY, gpeALF_CZ,

	gpeALF_DA, gpeALF_DB, gpeALF_DC,
	gpeALF_DD, gpeALF_DE, gpeALF_DF, gpeALF_DG,
	gpeALF_DH, gpeALF_DI, gpeALF_DJ, gpeALF_DK,
	gpeALF_DL, gpeALF_DM, gpeALF_DN, gpeALF_DO,
	gpeALF_DP, gpeALF_DQ, gpeALF_DR, gpeALF_DS,
	gpeALF_DT, gpeALF_DU, gpeALF_DV, gpeALF_DW,
	gpeALF_DX, gpeALF_DY, gpeALF_DZ,

	gpeALF_EA, gpeALF_EB, gpeALF_EC,
	gpeALF_ED, gpeALF_EE, gpeALF_EF, gpeALF_EG,
	gpeALF_EH, gpeALF_EI, gpeALF_EJ, gpeALF_EK,
	gpeALF_EL, gpeALF_EM, gpeALF_EN, gpeALF_EO,
	gpeALF_EP, gpeALF_EQ, gpeALF_ER, gpeALF_ES,
	gpeALF_ET, gpeALF_EU, gpeALF_EV, gpeALF_EW,
	gpeALF_EX, gpeALF_EY, gpeALF_EZ,

	gpeALF_FA, gpeALF_FB, gpeALF_FC,
	gpeALF_FD, gpeALF_FE, gpeALF_FF, gpeALF_FG,
	gpeALF_FH, gpeALF_FI, gpeALF_FJ, gpeALF_FK,
	gpeALF_FL, gpeALF_FM, gpeALF_FN, gpeALF_FO,
	gpeALF_FP, gpeALF_FQ, gpeALF_FR, gpeALF_FS,
	gpeALF_FT, gpeALF_FU, gpeALF_FV, gpeALF_FW,
	gpeALF_FX, gpeALF_FY, gpeALF_FZ,

	gpeALF_GA, gpeALF_GB, gpeALF_GC,
	gpeALF_GD, gpeALF_GE, gpeALF_GF, gpeALF_GG,
	gpeALF_GH, gpeALF_GI, gpeALF_GJ, gpeALF_GK,
	gpeALF_GL, gpeALF_GM, gpeALF_GN, gpeALF_GO,
	gpeALF_GP, gpeALF_GQ, gpeALF_GR, gpeALF_GS,
	gpeALF_GT, gpeALF_GU, gpeALF_GV, gpeALF_GW,
	gpeALF_GX, gpeALF_GY, gpeALF_GZ,

	gpeALF_HA, gpeALF_HB, gpeALF_HC,
	gpeALF_HD, gpeALF_HE, gpeALF_HF, gpeALF_HG,
	gpeALF_HH, gpeALF_HI, gpeALF_HJ, gpeALF_HK,
	gpeALF_HL, gpeALF_HM, gpeALF_HN, gpeALF_HO,
	gpeALF_HP, gpeALF_HQ, gpeALF_HR, gpeALF_HS,
	gpeALF_HT, gpeALF_HU, gpeALF_HV, gpeALF_HW,
	gpeALF_HX, gpeALF_HY, gpeALF_HZ,


	gpeALF_IA, gpeALF_iA = gpeALF_IA,
	gpeALF_IB, gpeALF_IC,
	gpeALF_ID, gpeALF_IE, gpeALF_if, gpeALF_IG,
	gpeALF_IH, gpeALF_II, gpeALF_IJ, gpeALF_IK,
	gpeALF_IL, gpeALF_IM,
	gpeALF_IN, gpeALF_iN = gpeALF_IN,
	gpeALF_IO,
	gpeALF_IP, gpeALF_IQ, gpeALF_IR, gpeALF_IS,
	gpeALF_IT, gpeALF_IU, gpeALF_IV, gpeALF_IW,
	gpeALF_IX, gpeALF_IY, gpeALF_IZ,


	gpeALF_JA, gpeALF_JB, gpeALF_JC,
	gpeALF_JD, gpeALF_JE, gpeALF_JF, gpeALF_JG,
	gpeALF_JH, gpeALF_JI, gpeALF_JJ, gpeALF_JK,
	gpeALF_JL, gpeALF_JM, gpeALF_JN, gpeALF_JO,
	gpeALF_JP, gpeALF_JQ, gpeALF_JR, gpeALF_JS,
	gpeALF_JT, gpeALF_JU, gpeALF_JV, gpeALF_JW,
	gpeALF_JX, gpeALF_JY, gpeALF_JZ,


	// key ( A, B, ..., Z )
	gpeALF_KA, gpeALF_KB, gpeALF_KC,
	gpeALF_KD, gpeALF_KE, gpeALF_KF, gpeALF_KG,
	gpeALF_KH, gpeALF_KI, gpeALF_KJ, gpeALF_KK,
	gpeALF_KL, gpeALF_KM, gpeALF_KN, gpeALF_KO,
	gpeALF_KP, gpeALF_KQ, gpeALF_KR, gpeALF_KS,
	gpeALF_KT, gpeALF_KU, gpeALF_KV, gpeALF_KW,
	gpeALF_KX, gpeALF_KY, gpeALF_KZ,


	gpeALF_LA, gpeALF_LB, gpeALF_LC,
	gpeALF_LD, gpeALF_LE, gpeALF_LF, gpeALF_LG,
	gpeALF_LH, gpeALF_LI, gpeALF_LJ, gpeALF_LK,
	gpeALF_LL, gpeALF_LM, gpeALF_LN, gpeALF_LO,
	gpeALF_LP, gpeALF_LQ, gpeALF_LR, gpeALF_LS,
	gpeALF_LT, gpeALF_LU, gpeALF_LV, gpeALF_LW,
	gpeALF_LX, gpeALF_LY, gpeALF_LZ,


	gpeALF_MA, gpeALF_MB, gpeALF_MC,
	gpeALF_MD, gpeALF_ME, gpeALF_MF, gpeALF_MG,
	gpeALF_MH, gpeALF_MI, gpeALF_MJ, gpeALF_MK,
	gpeALF_ML, gpeALF_MM, gpeALF_MN, gpeALF_MO,
	gpeALF_MP, gpeALF_MQ, gpeALF_MR, gpeALF_MS,
	gpeALF_MT, gpeALF_MU, gpeALF_MV, gpeALF_MW,
	gpeALF_MX, gpeALF_MY, gpeALF_MZ,


	gpeALF_NA, gpeALF_NB, gpeALF_NC,
	gpeALF_ND, gpeALF_NE, gpeALF_NF, gpeALF_NG,
	gpeALF_NH, gpeALF_NI, gpeALF_NJ, gpeALF_NK,
	gpeALF_NL, gpeALF_NM, gpeALF_NN,
	gpeALF_NO,
	gpeALF_NP, gpeALF_NQ, gpeALF_NR, gpeALF_NS,
	gpeALF_NT, gpeALF_NU, gpeALF_NV, gpeALF_NW,
	gpeALF_NX, gpeALF_NY, gpeALF_NZ,


	gpeALF_OA, gpeALF_OB, gpeALF_OC,
	gpeALF_OD, gpeALF_OE, gpeALF_OF, gpeALF_OG,
	gpeALF_OH, gpeALF_OI, gpeALF_OJ,
	gpeALF_OK,										// SIM7600 - OK
	gpeALF_OL, gpeALF_OM, gpeALF_ON, gpeALF_OO,
	gpeALF_OP, gpeALF_OQ,
	gpeALF_OR, gpeALF_or = gpeALF_OR,
	gpeALF_OS,
	gpeALF_OT, gpeALF_OU, gpeALF_OV, gpeALF_OW,
	gpeALF_OX, gpeALF_OY, gpeALF_OZ,


	gpeALF_PA, gpeALF_PB, gpeALF_PC,
	gpeALF_PD, gpeALF_PE, gpeALF_PF, gpeALF_PG,
	gpeALF_PH, gpeALF_PI, gpeALF_PJ, gpeALF_PK,
	gpeALF_PL, gpeALF_PM, gpeALF_PN, gpeALF_PO,
	gpeALF_PP, gpeALF_PQ, gpeALF_PR, gpeALF_PS,
	gpeALF_PT, gpeALF_PU, gpeALF_PV, gpeALF_PW,
	gpeALF_PX, gpeALF_PY, gpeALF_PZ,


	gpeALF_QA, gpeALF_QB, gpeALF_QC,
	gpeALF_QD, gpeALF_QE, gpeALF_QF, gpeALF_QG,
	gpeALF_QH, gpeALF_QI, gpeALF_QJ, gpeALF_QK,
	gpeALF_QL, gpeALF_QM, gpeALF_QN, gpeALF_QO,
	gpeALF_QP, gpeALF_QQ, gpeALF_QR, gpeALF_QS,
	gpeALF_QT, gpeALF_QU, gpeALF_QV, gpeALF_QW,
	gpeALF_QX, gpeALF_QY, gpeALF_QZ,


	gpeALF_RA, gpeALF_RB, gpeALF_RC,
	gpeALF_RD, gpeALF_RE, gpeALF_RF, gpeALF_RG,
	gpeALF_RH, gpeALF_RI, gpeALF_RJ, gpeALF_RK,
	gpeALF_RL, gpeALF_RM, gpeALF_RN, gpeALF_RO,
	gpeALF_RP, gpeALF_RQ, gpeALF_RR, gpeALF_RS,
	gpeALF_RT, gpeALF_RU, gpeALF_RV, gpeALF_RW,
	gpeALF_RX, gpeALF_RY, gpeALF_RZ,


	gpeALF_SA, gpeALF_SB, gpeALF_SC,
	gpeALF_SD, gpeALF_SE, gpeALF_SF, gpeALF_SG,
	gpeALF_SH, gpeALF_SI, gpeALF_SJ, gpeALF_SK,
	gpeALF_SL, gpeALF_sl = gpeALF_SL,
	gpeALF_SM, gpeALF_SN, gpeALF_SO,
	gpeALF_SP, gpeALF_SQ,
	gpeALF_SR, gpeALF_sr = gpeALF_SR,
	gpeALF_SS,
	gpeALF_ST, gpeALF_SU, gpeALF_SV, gpeALF_SW,
	gpeALF_SX, gpeALF_SY, gpeALF_SZ,


	gpeALF_TA, gpeALF_TB, gpeALF_TC,
	gpeALF_TD, gpeALF_TE, gpeALF_TF, gpeALF_TG,
	gpeALF_TH, gpeALF_TI, gpeALF_TJ, gpeALF_TK,
	gpeALF_TL, gpeALF_TM, gpeALF_TN, gpeALF_TO,
	gpeALF_TP, gpeALF_TQ, gpeALF_TR, gpeALF_TS,
	gpeALF_TT, gpeALF_TU, gpeALF_TV, gpeALF_TW,
	gpeALF_TX, gpeALF_TY, gpeALF_TZ,


	gpeALF_UA, gpeALF_UB, gpeALF_UC,
	gpeALF_UD, gpeALF_UE, gpeALF_UF, gpeALF_UG,
	gpeALF_UH, gpeALF_UI, gpeALF_UJ, gpeALF_UK,
	gpeALF_UL, gpeALF_UM, gpeALF_UN, gpeALF_UO,
	gpeALF_UP, gpeALF_UQ, gpeALF_UR, gpeALF_US,
	gpeALF_UT, gpeALF_UU, gpeALF_UV, gpeALF_UW,
	gpeALF_UX, gpeALF_UY, gpeALF_UZ,


	gpeALF_VA, gpeALF_VB, gpeALF_VC,
	gpeALF_VD, gpeALF_VE, gpeALF_VF, gpeALF_VG,
	gpeALF_VH, gpeALF_VI, gpeALF_VJ, gpeALF_VK,
	gpeALF_VL, gpeALF_VM, gpeALF_VN, gpeALF_VO,
	gpeALF_VP, gpeALF_VQ, gpeALF_VR, gpeALF_VS,
	gpeALF_VT, gpeALF_VU, gpeALF_VV, gpeALF_VW,
	gpeALF_VX, gpeALF_VY, gpeALF_VZ,


	gpeALF_WA, gpeALF_WB, gpeALF_WC,
	gpeALF_WD, gpeALF_WE, gpeALF_WF, gpeALF_WG,
	gpeALF_WH, gpeALF_WI, gpeALF_WJ, gpeALF_WK,
	gpeALF_WL, gpeALF_WM, gpeALF_WN, gpeALF_WO,
	gpeALF_WP, gpeALF_WQ, gpeALF_WR, gpeALF_WS,
	gpeALF_WT, gpeALF_WU, gpeALF_WV, gpeALF_WW,
	gpeALF_WX, gpeALF_WY, gpeALF_WZ,


	gpeALF_XA, gpeALF_XB, gpeALF_XC,
	gpeALF_XD, gpeALF_XE, gpeALF_XF, gpeALF_XG,
	gpeALF_XH, gpeALF_XI, gpeALF_XJ, gpeALF_XK,
	gpeALF_XL, gpeALF_XM, gpeALF_XN, gpeALF_XO,
	gpeALF_XP, gpeALF_XQ, gpeALF_XR, gpeALF_XS,
	gpeALF_XT, gpeALF_XU, gpeALF_XV, gpeALF_XW,
	gpeALF_XX, gpeALF_XY, gpeALF_XZ,

	gpeALF_YA, gpeALF_YB, gpeALF_YC,
	gpeALF_YD, gpeALF_YE, gpeALF_YF, gpeALF_YG,
	gpeALF_YH, gpeALF_YI, gpeALF_YJ, gpeALF_YK,
	gpeALF_YL, gpeALF_YM, gpeALF_YN, gpeALF_YO,
	gpeALF_YP, gpeALF_YQ, gpeALF_YR, gpeALF_YS,
	gpeALF_YT, gpeALF_YU, gpeALF_YV, gpeALF_YW,
	gpeALF_YX, gpeALF_YY, gpeALF_YZ,

	gpeALF_ZA, gpeALF_ZB, gpeALF_ZC,
	gpeALF_ZD, gpeALF_ZE, gpeALF_ZF, gpeALF_ZG,
	gpeALF_ZH, gpeALF_ZI, gpeALF_ZJ, gpeALF_ZK,
	gpeALF_ZL, gpeALF_ZM, gpeALF_ZN, gpeALF_ZO,
	gpeALF_ZP, gpeALF_ZQ, gpeALF_ZR, gpeALF_ZS,
	gpeALF_ZT, gpeALF_ZU, gpeALF_ZV, gpeALF_ZW,
	gpeALF_ZX, gpeALF_ZY, gpeALF_ZZ,

	gpeALF_2 = gpeALF_ZZ,
	//- 3 -----------------
	gpeALF_AAA = gpdABC('A', 'A', 'A'),
	gpeALF_ABC = gpdABC('A', 'B', 'C'),
	gpeALF_ABS = gpdABC('A', 'B', 'S'),
	gpeALF_ACE = gpdABC('A', 'C', 'E'), // access
	gpeALF_ACT = gpdABC('A', 'C', 'T'),
	gpeALF_add = gpdABC('A', 'D', 'D'),
	gpeALF_ADR = gpdABC('A', 'D', 'R'),
	gpeALF_AIN = gpdABC('A', 'I', 'N'),
	gpeALF_and = gpdABC('A', 'N', 'D'),
	gpeALF_ARJ = gpdABC('A', 'R', 'J'),
	gpeALF_ARR = gpdABC('A', 'R', 'R'),
	gpeALF_ASM = gpdABC('A', 'S', 'M'),
	gpeALF_ATA = gpdABC('A', 'T', 'A'),		// SIM7600 - Call answer
	gpeALF_ATD = gpdABC('A', 'T', 'D'),		// SIM7600 - Call diallup
	gpeALF_ATH = gpdABC('A', 'T', 'H'),		// SIM7600 - Disconnect existing call
	gpeALF_BIN = gpdABC('B', 'I', 'N'),
	gpeALF_BLK = gpdABC('B', 'L', 'K'),
	gpeALF_BOB = gpdABC('B', 'O', 'B'),
	gpeALF_BOX = gpdABC('B', 'O', 'X'),
	gpeALF_CAM = gpdABC('C', 'A', 'M'),
	gpeALF_CLR = gpdABC('C', 'L', 'R'),

	gpeALF_CMX = gpdABC('C', 'M', 'X'),
	gpeALF_CMY = gpdABC('C', 'M', 'Y'),
	gpeALF_CMZ = gpdABC('C', 'M', 'Z'),
	/// CNL 0 -----------------------------------------------
	gpeALF_CNL = gpdABC('C', 'N', 'L'),
	gpeALF_com = gpdABC('C', 'O', 'M'),
	gpeALF_COS = gpdABC('C', 'O', 'S'),
	gpeALF_CPP = gpdABC('C', 'P', 'P'),
	gpeALF_CPU = gpdABC('C', 'P', 'U'),
	gpeALF_CPY = gpdABC('C', 'P', 'Y'),

	gpeALF_CSQ = gpdABC('C', 'S', 'Q'),		// SIM7600 - Siqnal Quality

	gpeALF_CUT = gpdABC('C', 'U', 'T'),
	gpeALF_DBG = gpdABC('D', 'B', 'G'),
	gpeALF_DEB = gpdABC('D', 'E', 'B'),
	gpeALF_dec = gpdABC('D', 'E', 'C'),
	gpeALF_DEF = gpdABC('D', 'E', 'F'),
	gpeALF_DEL = gpdABC('D', 'E', 'L'),
	gpeALF_DIR = gpdABC('D', 'I', 'R'),
	gpeALF_div = gpdABC('D', 'I', 'V'),
	gpeALF_dot = gpdABC('D', 'O', 'T'),
	gpeALF_DST = gpdABC('D', 'S', 'T'),
	gpeALF_DZR = gpdABC('D', 'Z', 'R'),
	gpeALF_end = gpdABC('E', 'N', 'D'),		// SIM7600 -
	gpeALF_ERU = gpdABC('E', 'R', 'U'),
	gpeALF_EYE = gpdABC('E', 'Y', 'E'),
	gpeALF_exp = gpdABC('E', 'X', 'P'),

	// function key ( f1, f2, ..., f12)
	gpeALF_FKA = gpdABC('F', 'K', 'A'),
	gpeALF_FKL = gpdABC('F', 'K', 'L'),

	gpeALF_FMX = gpdABC('F', 'M', 'X'),
	gpeALF_FMY = gpdABC('F', 'M', 'Y'),

	gpeALF_FND = gpdABC('F', 'N', 'D'),

	gpeALF_FOR = gpdABC('F', 'O', 'R'),
	gpeALF_FPS = gpdABC('F', 'P', 'S'),
	gpeALF_GET = gpdABC('G', 'E', 'T'),
	gpeALF_GIM = gpdABC('G', 'I', 'M'),
	gpeALF_GIO = gpdABC('G', 'I', 'O'),
	gpeALF_GIS = gpdABC('G', 'I', 'S'),
	gpeALF_GIT = gpdABC('G', 'I', 'T'),
	gpeALF_GIV = gpdABC('G', 'I', 'V'),
	gpeALF_GPU = gpdABC('G', 'P', 'U'),
	gpeALF_GSM = gpdABC('G', 'S', 'M'),
	gpeALF_GXT = gpdABC('G', 'X', 'T'),
	gpeALF_HUD = gpdABC('H', 'U', 'D'),
	gpeALF_HUP = gpdABC('H', 'U', 'P'),
	gpeALF_inc = gpdABC('I', 'N', 'C'),
	gpeALF_INP = gpdABC('I', 'N', 'P'),
	gpeALF_INT = gpdABC('I', 'N', 'T'),
	gpeALF_inv = gpdABC('I', 'N', 'V'),		/// inv
	gpeALF_JPG = gpdABC('J', 'P', 'G'),
	gpeALF_JOY = gpdABC('J', 'O', 'Y'),
	gpeALF_KEY = gpdABC('K', 'E', 'Y'),
	gpeALF_LOG = gpdABC('L', 'O', 'G'),
	gpeALF_LWO = gpdABC('L', 'W', 'O'),
	gpeALF_LWS = gpdABC('L', 'W', 'S'),
	gpeALF_MAN = gpdABC('M', 'A', 'N'),
	gpeALF_MAX = gpdABC('M', 'A', 'X'),
	gpeALF_MEM = gpdABC('M', 'E', 'M'),
	gpeALF_MIN = gpdABC('M', 'I', 'N'),
	gpeALF_MLB = gpdABC('M', 'L', 'B'),
	gpeALF_MMB = gpdABC('M', 'M', 'B'),
	gpeALF_MMX = gpdABC('M', 'M', 'X'),
	gpeALF_MMY = gpdABC('M', 'M', 'Y'),
	gpeALF_MOM = gpdABC('M', 'O', 'M'),
	gpeALF_mov = gpdABC('M', 'O', 'V'),
	gpeALF_MPG = gpdABC('M', 'R', 'B'),
	gpeALF_MRB = gpdABC('M', 'R', 'B'),

	gpeALF_MRK = gpdABC('M', 'R', 'K'),				// '\"'
	gpeALF_MSG = gpdABC('M', 'S', 'G'),

	gpeALF_mul = gpdABC('M', 'U', 'L'),
	gpeALF_NEW = gpdABC('N', 'E', 'W'),
	gpeALF_NON = gpdABC('N', 'O', 'N'),
	gpeALF_OFF = gpdABC('O', 'F', 'F'),
	gpeALF_orM = gpdABC('O', 'R', 'M'),
	gpeALF_out = gpdABC('O', 'U', 'T'),
	/// PIC 0 -----------------------------------------------
	gpeALF_PIC = gpdABC('P', 'I', 'C'),
	/// PIX dx PixelShader gl FragmentShader
	gpeALF_PIN = gpdABC('P', 'I', 'N'),		// SIM7600 - AT+CRESET

	gpeALF_PIX = gpdABC('P', 'I', 'X'),
	gpeALF_PMX = gpdABC('P', 'M', 'X'),
	gpeALF_PMY = gpdABC('P', 'M', 'Y'),
	gpeALF_PNG = gpdABC('P', 'N', 'G'),
	gpeALF_PNT = gpdABC('P', 'N', 'T'),

	gpeALF_POS = gpdABC('P', 'O', 'S'),

	gpeALF_PRG = gpdABC('P', 'R', 'G'),

	gpeALF_RDY = gpdABC('R', 'D', 'Y'),		// SIM7600 - AT+CRESET

	gpeALF_REF = gpdABC('R', 'E', 'F'),
	gpeALF_REG = gpdABC('R', 'E', 'G'),
	gpeALF_rem = gpdABC('R', 'E', 'M'),
	gpeALF_REN = gpdABC('R', 'E', 'N'),
	gpeALF_RET = gpdABC('R', 'E', 'T'),
	gpeALF_RUN = gpdABC('R', 'U', 'N'),

	gpeALF_SDH = gpdABC('S', 'D', 'H'),
	gpeALF_SDW = gpdABC('S', 'D', 'W'),
	gpeALF_SDX = gpdABC('S', 'D', 'X'),
	gpeALF_SDY = gpdABC('S', 'D', 'Y'),

	gpeALF_SEC = gpdABC('S', 'E', 'C'),
	gpeALF_SIM = gpdABC('S', 'I', 'M'),		// SIM7600 - AT+CRESET
	gpeALF_SIN = gpdABC('S', 'I', 'N'),
	gpeALF_SIT = gpdABC('S', 'I', 'T'),
	gpeALF_slM = gpdABC('S', 'L', 'M'),
	gpeALF_SMS = gpdABC('S', 'M', 'S'),
	gpeALF_SRC = gpdABC('S', 'R', 'C'),
	gpeALF_srM = gpdABC('S', 'R', 'M'),

	gpeALF_SSH = gpdABC('S', 'S', 'H'),
	gpeALF_SSW = gpdABC('S', 'S', 'W'),
	gpeALF_SSX = gpdABC('S', 'S', 'X'),
	gpeALF_SSY = gpdABC('S', 'S', 'Y'),


	gpeALF_str = gpdABC('S', 'T', 'R'),
	gpeALF_stk = gpdABC('S', 'T', 'K'),
	gpeALF_sub = gpdABC('S', 'U', 'B'),
	gpeALF_SUM = gpdABC('S', 'U', 'M'),
	gpeALF_SYS = gpdABC('S', 'Y', 'S'),

	gpeALF_TAG = gpdABC('T', 'A', 'G'),
	gpeALF_TRG = gpdABC('T', 'R', 'G'),
	gpeALF_TYF = gpdABC('T', 'Y', 'F'),
	gpeALF_TYI = gpdABC('T', 'Y', 'I'),
	gpeALF_TYU = gpdABC('T', 'Y', 'U'),

	gpeALF_VAL = gpdABC('V', 'A', 'L'),
	gpeALF_VAR = gpdABC('V', 'A', 'R'),
	gpeALF_VEC = gpdABC('V', 'E', 'C'),
	gpeALF_VOX = gpdABC('V', 'O', 'X'),
	/// VTX dx gl VertexShader
	gpeALF_VTX = gpdABC('V', 'T', 'X'),
	gpeALF_WIN = gpdABC('W', 'I', 'N'),
	gpeALF_WMX = gpdABC('W', 'M', 'X'),
	gpeALF_WMY = gpdABC('W', 'M', 'Y'),
	gpeALF_YPR = gpdABC('Y', 'P', 'R'),
	gpeALF_xor = gpdABC('X', 'O', 'R'),

	gpeALF_XXA = gpdABC('X', 'X', 'A'),
	gpeALF_XXB, gpeALF_XXC, gpeALF_XXD,
	gpeALF_XXE, gpeALF_XXF, gpeALF_XXG, gpeALF_XXH,
	gpeALF_XXI, gpeALF_XXJ, gpeALF_XXK, gpeALF_XXL,
	gpeALF_XXM, gpeALF_XXN, gpeALF_XXO, gpeALF_XXP,
	gpeALF_XXQ, gpeALF_XXR, gpeALF_XXS, gpeALF_XXT,
	gpeALF_XXU, gpeALF_XXV, gpeALF_XXW,
	gpeALF_XXX, gpeALF_XXY, gpeALF_XXZ,

	gpeALF_XYA, gpeALF_XYB, gpeALF_XYC, gpeALF_XYD,
	gpeALF_XYE, gpeALF_XYF, gpeALF_XYG, gpeALF_XYH,
	gpeALF_XYI, gpeALF_XYJ, gpeALF_XYK, gpeALF_XYL,
	gpeALF_XYM, gpeALF_XYN, gpeALF_XYO, gpeALF_XYP,
	gpeALF_XYQ, gpeALF_XYR, gpeALF_XYS, gpeALF_XYT,
	gpeALF_XYU, gpeALF_XYV, gpeALF_XYW,
	gpeALF_XYX, gpeALF_XYY, gpeALF_XYZ,

	gpeALF_XZA, gpeALF_XZB, gpeALF_XZC, gpeALF_XZD,
	gpeALF_XZE, gpeALF_XZF, gpeALF_XZG, gpeALF_XZH,
	gpeALF_XZI, gpeALF_XZJ, gpeALF_XZK, gpeALF_XZL,
	gpeALF_XZM, gpeALF_XZN, gpeALF_XZO, gpeALF_XZP,
	gpeALF_XZQ, gpeALF_XZR, gpeALF_XZS, gpeALF_XZT,
	gpeALF_XZU, gpeALF_XZV, gpeALF_XZW,
	gpeALF_XZX, gpeALF_XZY, gpeALF_XZZ,

	//gpeALF_XYZ = gpdABC('X', 'Y', 'Z'),
	gpeALF_ZIP = gpdABC('Z', 'I', 'P'),
	gpeALF_ZZZ = gpdABC('Z', 'Z', 'Z'),
	gpeALF_3 = gpdABC('Z', 'Z', 'Z'),
	//- 4 -----------------
	gpeALF_AAAA = gpdABCD('A', 'A', 'A', 'A'),
	gpeALF_ABMS = gpdABCD('A', 'B', 'M', 'S'),
	gpeALF_ACOS = gpdABCD('A', 'C', 'O', 'S'),
	gpeALF_addM = gpdABCD('A', 'D', 'D', 'M'),
	gpeALF_andM = gpdABCD('A', 'N', 'D', 'M'),
	gpeALF_ASIN = gpdABCD('A', 'S', 'I', 'N'),
	gpeALF_AXIS = gpdABCD('A', 'X', 'I', 'S'),
	gpeALF_BILL = gpdABCD('B', 'I', 'L', 'L'),
	gpeALF_beLG = gpdABCD('B', 'E', 'L', 'G'),
	gpeALF_bgLG = gpdABCD('B', 'G', 'L', 'G'),
	gpeALF_BONE = gpdABCD('B', 'O', 'N', 'E'),
	gpeALF_BUMM = gpdABCD('B', 'U', 'M', 'M'),
	gpeALF_BUMP = gpdABCD('B', 'U', 'M', 'P'),
	gpeALF_BUSY = gpdABCD('B', 'U', 'S', 'Y'),		// SIM7600 - BUSY

	gpeALF_CALL = gpdABCD('C', 'A', 'L', 'L'),		// SIM7600 -

	gpeALF_CGMI = gpdABCD('C', 'G', 'M', 'I'),		// SIM7600 - Module Manufacture
	gpeALF_CGMM = gpdABCD('C', 'G', 'M', 'M'),		// SIM7600 - Module MODEL
	gpeALF_CGMR = gpdABCD('C', 'G', 'M', 'R'),		// SIM7600 - Request revision identification
	gpeALF_CGPS = gpdABCD('C', 'G', 'P', 'S'),		// SIM7600 - GPS modul
	gpeALF_CGSN = gpdABCD('C', 'G', 'S', 'N'),		// SIM7600 - Serial Num
	gpeALF_CHUP = gpdABCD('C', 'H', 'U', 'P'),		// SIM7600 - Hang up call
	gpeALF_CLCC = gpdABCD('C', 'L', 'C', 'C'),		// SIM7600 - List current calls
	gpeALF_CLIP = gpdABCD('C', 'L', 'I', 'P'),		// SIM7600 - Calling line identification presentation
	gpeALF_CLVL = gpdABCD('C', 'L', 'V', 'L'),		// SIM7600 - Call VOLUME

	/// CNLA 1 -----------------------------------------------
	gpeALF_CNLA = gpdABCD('C', 'N', 'L', 'A'),
	gpeALF_CNLB, gpeALF_CNLC, gpeALF_CNLD,
	gpeALF_CNLE, gpeALF_CNLF, gpeALF_CNLG, 	gpeALF_CNLH,
	gpeALF_CNLI, gpeALF_CNLJ, gpeALF_CNLK, 	gpeALF_CNLL,
	gpeALF_CNLM, gpeALF_CNLN, gpeALF_CNLO, 	gpeALF_CNLP,
	gpeALF_CNLQ, gpeALF_CNLR, gpeALF_CNLS, 	gpeALF_CNLT,
	gpeALF_CNLU, gpeALF_CNLV, 				gpeALF_CNLW,
	gpeALF_CNLX, gpeALF_CNLY, gpeALF_CNLZ,

	gpeALF_CNMI = gpdABCD('C', 'N', 'M', 'I'),		// SIM7600 - New msg indikator
	gpeALF_CNMP = gpdABCD('C', 'N', 'M', 'P'),		// SIM7600 - Preferred mode selection
	gpeALF_CMGD = gpdABCD('C', 'M', 'G', 'D'),		// SIM7600 - SMS Delete
	gpeALF_CMGF = gpdABCD('C', 'M', 'G', 'F'),		// SIM7600 - SMS Format
	gpeALF_CMGL = gpdABCD('C', 'M', 'G', 'L'),		// SIM7600 - SMS LIST msg pref storage
	gpeALF_CMGR = gpdABCD('C', 'M', 'G', 'R'),		// SIM7600 - SMS Read
	gpeALF_CMTI = gpdABCD('C', 'M', 'T', 'I'),		// SIM7600 - SMS ?? Note string ??

	gpeALF_comE = gpdABCD('C', 'O', 'M', 'E'),
	gpeALF_comS = gpdABCD('C', 'O', 'M', 'S'),

	gpeALF_COPY = gpdABCD('C', 'O', 'P', 'Y'),
	gpeALF_COPS = gpdABCD('C', 'O', 'P', 'S'),		/// SIM7600 - PROVIDER?

	gpeALF_CPIN = gpdABCD('C', 'P', 'I', 'N'),		// SIM7600 - AT+CRESET
	gpeALF_CPMS = gpdABCD('C', 'P', 'M', 'S'),		// SIM7600 -
	gpeALF_CPSI = gpdABCD('C', 'P', 'S', 'I'),		// SIM7600 - Inquiring UE system information
	gpeALF_CREG = gpdABCD('C', 'R', 'E', 'G'),		// SIM7600 - Network registration





	gpeALF_CRLF = gpdABCD('C', 'R', 'L', 'F'),		// I8x2 aAT[].alf  enter

	gpeALF_CRSA = gpdABCD('C', 'R', 'S', 'A'),
	gpeALF_CRSL = gpdABCD('C', 'R', 'S', 'L'),
	gpeALF_CRSN = gpdABCD('C', 'R', 'S', 'N'),
	gpeALF_CRSR = gpdABCD('C', 'R', 'S', 'R'),
	gpeALF_CRSX = gpdABCD('C', 'R', 'S', 'X'),
	gpeALF_CRSY = gpdABCD('C', 'R', 'S', 'Y'),

	gpeALF_CSCS = gpdABCD('C', 'S', 'C', 'S'),		// SIM7600 - TE charset

	gpeALF_CTRL = gpdABCD('C', 'T', 'R', 'L'),
	gpeALF_DEEP = gpdABCD('D', 'E', 'E', 'P'),
	gpeALF_dimE = gpdABCD('D', 'I', 'M', 'E'),
	gpeALF_dimS = gpdABCD('D', 'I', 'M', 'S'),
	gpeALF_divM = gpdABCD('D', 'I', 'V', 'M'),
	gpeALF_DRAW = gpdABCD('D', 'R', 'A', 'W'),

	gpeALF_DONE = gpdABCD('D', 'O', 'N', 'E'),
	gpeALF_DROP = gpdABCD('D', 'R', 'O', 'P'),	// DRCos

	gpeALF_EDGE = gpdABCD('E', 'D', 'G', 'E'),
	gpeALF_else = gpdABCD('E', 'L', 'S', 'E'),
	gpeALF_ENDD = gpdABCD('E', 'N', 'D', 'D'),
	gpeALF_eqLG = gpdABCD('E', 'Q', 'L', 'G'),
	gpeALF_EXIT = gpdABCD('E', 'X', 'I', 'T'),
	gpeALF_EXPL = gpdABCD('E', 'X', 'P', 'L'),
	gpeALF_expM = gpdABCD('E', 'X', 'P', 'M'),
	gpeALF_FERI = gpdABCD('F', 'E', 'R', 'I'),
	gpeALF_FILE = gpdABCD('F', 'I', 'L', 'E'),
	gpeALF_FIND = gpdABCD('F', 'I', 'N', 'D'),
	gpeALF_FSEC = gpdABCD('F', 'S', 'E', 'C'),
	gpeALF_FUNC = gpdABCD('F', 'U', 'N', 'C'),
	gpeALF_FULL = gpdABCD('F', 'U', 'L', 'L'),		// SIM7600 -
	gpeALF_GINP = gpdABCD('G', 'I', 'N', 'P'),
	gpeALF_GOLD = gpdABCD('G', 'O', 'L', 'D'),
	gpeALF_GOUT = gpdABCD('G', 'O', 'U', 'T'),

	gpeALF_GPIO = gpdABCD('G', 'P', 'I', 'O'),

	gpeALF_GPUC = gpdABCD('G', 'P', 'U', 'C'),	// gpeALF_GPU + VOXEL VERTEX

	gpeALF_GRIP = gpdABCD('G', 'R', 'I', 'P'),

	gpeALF_HAIR = gpdABCD('H', 'A', 'I', 'R'),
	gpeALF_HAND = gpdABCD('H', 'A', 'N', 'D'),
	gpeALF_HEAD = gpdABCD('H', 'E', 'A', 'D'),
	gpeALF_HELP = gpdABCD('H', 'E', 'L', 'P'),
	gpeALF_HELO = gpdABCD('H', 'E', 'L', 'O'),
	gpeALF_HOST = gpdABCD('H', 'O', 'S', 'T'),
	gpeALF_HTML = gpdABCD('H', 'T', 'M', 'L'),
	gpeALF_HTTP = gpdABCD('H', 'T', 'T', 'P'),

	gpeALF_IDLE = gpdABCD('I', 'D', 'L', 'E'),
	gpeALF_INFO = gpdABCD('I', 'N', 'F', 'O'),
	gpeALF_INIT = gpdABCD('I', 'N', 'I', 'T'),

	gpeALF_ITEM = gpdABCD('I', 'T', 'E', 'M'),
	gpeALF_JOHN = gpdABCD('J', 'O', 'H', 'N'),
	gpeALF_JOIN = gpdABCD('J', 'O', 'I', 'N'),
	gpeALF_JUMP = gpdABCD('J', 'U', 'M', 'P'),

	gpeALF_KALT = gpdABCD('K', 'A', 'L', 'T'),
	gpeALF_KICK = gpdABCD('K', 'I', 'C', 'K'),

	gpeALF_LIST = gpdABCD('L', 'I', 'S', 'T'),
	gpeALF_LINE = gpdABCD('L', 'I', 'N', 'E'),
	gpeALF_LINK = gpdABCD('L', 'I', 'N', 'K'),
	gpeALF_LEFT = gpdABCD('L', 'E', 'F', 'T'),
	gpeALF_leLG = gpdABCD('L', 'E', 'L', 'G'),
	gpeALF_LOOP = gpdABCD('L', 'O', 'O', 'P'),
	gpeALF_ltLG = gpdABCD('L', 'T', 'L', 'G'),
	gpeALF_mail = gpdABCD('M', 'A', 'I', 'L'),
	gpeALF_MAIN = gpdABCD('M', 'A', 'I', 'N'),
	gpeALF_MINI = gpdABCD('M', 'I', 'N', 'I'),

	gpeALF_MONO = gpdABCD('M', 'O', 'N', 'O'),
	gpeALF_MOON = gpdABCD('M', 'O', 'O', 'N'),

	gpeALF_MLBX = gpdABCD('M', 'L', 'B', 'X'),		// egér X poziciója mielött le lett nyomva a BAL gomb
	gpeALF_MLBY = gpdABCD('M', 'L', 'B', 'Y'),		// egér Y poziciója mielött le lett nyomva a BAL gomb

	gpeALF_MRBX = gpdABCD('M', 'R', 'B', 'X'),		// egér X poziciója mielött le lett nyomva a JOBB gomb
	gpeALF_MRBY = gpdABCD('M', 'R', 'B', 'Y'),		// egér Y poziciója mielött le lett nyomva a JOBB gomb

	gpeALF_mulM = gpdABCD('M', 'U', 'L', 'M'),

	gpeALF_NAME = gpdABCD('N', 'A', 'M', 'E'),
	gpeALF_MSEC = gpdABCD('M', 'S', 'E', 'C'),
	gpeALF_NBLD = gpdABCD('N', 'B', 'L', 'D'),
	gpeALF_NEAR = gpdABCD('N', 'E', 'A', 'R'),
	gpeALF_NONS = gpdABCD('N', 'O', 'N', 'S'),
	gpeALF_NUSE = gpdABCD('N', 'U', 'S', 'E'),
	gpeALF_OPER = gpdABCD('O', 'P', 'E', 'R'),
	gpeALF_orLG = gpdABCD('O', 'R', 'L', 'G'),
	gpeALF_OTOS = gpdABCD('O', 'T', 'O', 'S'),
	gpeALF_OVER = gpdABCD('O', 'V', 'E', 'R'),

	gpeALF_PACK = gpdABCD('P', 'A', 'C', 'K'),
	gpeALF_PARA = gpdABCD('P', 'A', 'R', 'A'),
	gpeALF_PASS = gpdABCD('P', 'A', 'S', 'S'),


	/// PICA 1 -----------------------------------------------
	gpeALF_PICA = gpdABCD('P', 'I', 'C', 'A'),
	gpeALF_PICB, gpeALF_PICC, gpeALF_PICD,
	gpeALF_PICE, gpeALF_PICF, gpeALF_PICG, 	gpeALF_PICH,
	gpeALF_PICI, gpeALF_PICJ, gpeALF_PICK, 	gpeALF_PICL,
	gpeALF_PICM, gpeALF_PICN, gpeALF_PICO, 	gpeALF_PICP,
	gpeALF_PICQ, gpeALF_PICR, gpeALF_PICS, 	gpeALF_PICT,
	gpeALF_PICU, gpeALF_PICV, 				gpeALF_PICW,
	gpeALF_PICX, gpeALF_PICY, gpeALF_PICZ,




	gpeALF_PIXN = gpdABCD('P', 'I', 'X', 'N'),
	gpeALF_PLUS = gpdABCD('P', 'L', 'U', 'S'),
	gpeALF_POLY = gpdABCD('P', 'O', 'L', 'Y'),
	gpeALF_PORT = gpdABCD('P', 'O', 'R', 'T'),
	gpeALF_PREV = gpdABCD('P', 'R', 'E', 'V'),

	gpeALF_QUAD = gpdABCD('Q', 'U', 'A', 'D'),

	//gpeALF_RESA =	gpdABCD( 'R','E','S','A' ),
	gpeALF_remM = gpdABCD('R', 'E', 'M', 'M'),
	gpeALF_REND = gpdABCD('R', 'E', 'N', 'D'),

	gpeALF_RING = gpdABCD('R', 'I', 'N', 'G'),		// SIM7600 - RING
	gpeALF_RINP = gpdABCD('R', 'I', 'N', 'P'),

	gpeALF_ROBI = gpdABCD('R', 'O', 'B', 'I'),
	gpeALF_ROCK = gpdABCD('R', 'O', 'C', 'K'),

	gpeALF_ROUT = gpdABCD('R', 'O', 'U', 'T'),

	gpeALF_SAVE = gpdABCD('S', 'A', 'V', 'E'),
	gpeALF_SHLD = gpdABCD('S', 'H', 'L', 'D'),
	gpeALF_SLMP = gpdABCD('S', 'L', 'M', 'P'),
	gpeALF_SOCK = gpdABCD('S', 'O', 'C', 'K'),
	gpeALF_STAR = gpdABCD('S', 'T', 'A', 'R'),

	gpeALF_STAT = gpdABCD('S', 'T', 'A', 'T'),	// DRCos

	gpeALF_STEP = gpdABCD('S', 'T', 'E', 'P'),
	gpeALF_STON = gpdABCD('S', 'T', 'O', 'N'),

	gpeALF_STOP = gpdABCD('S', 'T', 'O', 'P'),	// DRCos

	gpeALF_STOW = gpdABCD('S', 'T', 'O', 'W'),
	gpeALF_subM = gpdABCD('S', 'U', 'B', 'M'),
	gpeALF_SQRT = gpdABCD('S', 'Q', 'R', 'T'),
	gpeALF_SYNC = gpdABCD('S', 'Y', 'N', 'C'),

	gpeALF_TOOL = gpdABCD('T', 'O', 'O', 'L'),

	gpeALF_TRGH = gpdABCD('T', 'R', 'G', 'H'),
	gpeALF_TRGW = gpdABCD('T', 'R', 'G', 'W'),

	gpeALF_TRUE = gpdABCD('T', 'R', 'U', 'E'),
	gpeALF_TURN = gpdABCD('T', 'U', 'R', 'N'),

	gpeALF_TYPE = gpdABCD('T', 'Y', 'P', 'E'),

	gpeALF_USER = gpdABCD('U', 'S', 'E', 'R'),

	gpeALF_WAIT = gpdABCD('W', 'A', 'I', 'T'),
	gpeALF_WALK = gpdABCD('W', 'A', 'L', 'K'),
	gpeALF_WHAM = gpdABCD('W', 'H', 'A', 'M'),

	gpeALF_xorM = gpdABCD('X', 'O', 'R', 'M'),

	gpeALF_ZZZZ = gpdABCD('Z', 'Z', 'Z', 'Z'),
	gpeALF_4 = gpdABCD('Z', 'Z', 'Z', 'Z'),
	//- 5 -----------------
	gpeALF_AAAAA = gpdABCDE('A', 'A', 'A', 'A', 'A'),
	gpeALF_AGAIN = gpdABCDE('A', 'G', 'A', 'I', 'N'),
	gpeALF_andLG = gpdABCDE('A', 'N', 'D', 'L', 'G'),
	gpeALF_ARRAY = gpdABCDE('A', 'R', 'R', 'A', 'Y'),
	gpeALF_begin = gpdABCDE('B', 'E', 'G', 'I', 'N'),		// SIM7600 -pl. VOICE CALL: BEGIN
	gpeALF_BLOCK = gpdABCDE('B', 'L', 'O', 'C', 'K'),
	gpeALF_BOBER = gpdABCDE('B', 'O', 'B', 'E', 'R'),
	gpeALF_brakE = gpdABCDE('B', 'R', 'A', 'K', 'E'),
	gpeALF_brakS = gpdABCDE('B', 'R', 'A', 'K', 'S'),
	gpeALF_BREAK = gpdABCDE('B', 'R', 'E', 'A', 'K'),
	gpeALF_BUBLE = gpdABCDE('B', 'U', 'B', 'L', 'E'),
	gpeALF_CACHE = gpdABCDE('C', 'A', 'C', 'H', 'E'),
	gpeALF_CALIB = gpdABCDE('C', 'A', 'L', 'I', 'B'),
	gpeALF_CLASS = gpdABCDE('C', 'L', 'A', 'S', 'S'),

	gpeALF_CMGRD = gpdABCDE('C', 'M', 'G', 'R', 'D'),

	/// CNL -----------------------------------------------
	gpeALF_CNLiT = gpdABCDE('C', 'N', 'L', 'I', 'T'),
	/// i 1
	gpeALF_CNLiX = gpdABCDE('C', 'N', 'L', 'I', 'X'),
	gpeALF_CNLiY = gpdABCDE('C', 'N', 'L', 'I', 'Y'),
	gpeALF_CNLiZ = gpdABCDE('C', 'N', 'L', 'I', 'Z'),
	gpeALF_CNLiW = gpdABCDE('C', 'N', 'L', 'I', 'W'),

	gpeALF_CNLoT = gpdABCDE('C', 'N', 'L', 'O', 'T'),
	/// o 0
	gpeALF_CNLoX = gpdABCDE('C', 'N', 'L', 'O', 'X'),
	gpeALF_CNLoY = gpdABCDE('C', 'N', 'L', 'O', 'Y'),
	gpeALF_CNLoZ = gpdABCDE('C', 'N', 'L', 'O', 'Z'),
	gpeALF_CNLoW = gpdABCDE('C', 'N', 'L', 'O', 'W'),

	gpeALF_CNLvT = gpdABCDE('C', 'N', 'L', 'V', 'T'),
	/// v 5
	gpeALF_CNLvX = gpdABCDE('C', 'N', 'L', 'V', 'X'),
	gpeALF_CNLvY = gpdABCDE('C', 'N', 'L', 'V', 'Y'),
	gpeALF_CNLvZ = gpdABCDE('C', 'N', 'L', 'V', 'Z'),
	gpeALF_CNLvW = gpdABCDE('C', 'N', 'L', 'V', 'W'),

	gpeALF_CNLxT = gpdABCDE('C', 'N', 'L', 'X', 'T'),
	/// x 10
	gpeALF_CNLxX = gpdABCDE('C', 'N', 'L', 'X', 'X'),
	gpeALF_CNLxY = gpdABCDE('C', 'N', 'L', 'X', 'Y'),
	gpeALF_CNLxZ = gpdABCDE('C', 'N', 'L', 'X', 'Z'),
	gpeALF_CNLxW = gpdABCDE('C', 'N', 'L', 'X', 'W'),



	gpeALF_COLOR = gpdABCDE('C', 'O', 'L', 'O', 'R'),
	gpeALF_CONST = gpdABCDE('C', 'O', 'N', 'S', 'T'),
	gpeALF_COUNT = gpdABCDE('C', 'O', 'U', 'N', 'T'),
	gpeALF_CREAT = gpdABCDE('C', 'R', 'E', 'A', 'T'),
	gpeALF_CROSS = gpdABCDE('C', 'R', 'O', 'S', 'S'),
	gpeALF_CSDVC = gpdABCDE('C', 'S', 'D', 'V', 'C'),		// SIM7600 - call handset on

	gpeALF_CYCLE = gpdABCDE('C', 'Y', 'C', 'L', 'E'),
	gpeALF_DEBUG = gpdABCDE('D', 'E', 'B', 'U', 'G'),
	gpeALF_DEBUS = gpdABCDE('D', 'E', 'B', 'U', 'S'),
	gpeALF_ENTER = gpdABCDE('E', 'N', 'T', 'E', 'R'),

	gpeALF_entry = gpdABCDE('E', 'N', 'T', 'R', 'Y'),

	gpeALF_ERECT = gpdABCDE('E', 'R', 'E', 'C', 'T'),
	gpeALF_ERROR = gpdABCDE('E', 'R', 'R', 'O', 'R'),		// SIM7600 - AT+CRESET
	gpeALF_HELLO = gpdABCDE('H', 'E', 'L', 'L', 'O'),
	gpeALF_GETMX = gpdABCDE('G', 'E', 'T', 'M', 'X'),
	gpeALF_GLOBA = gpdABCDE('G', 'L', 'O', 'B', 'A'),
	gpeALF_HISTI = gpdABCDE('H', 'I', 'S', 'T', 'I'),
	gpeALF_HUMAN = gpdABCDE('H', 'U', 'M', 'A', 'N'),
	gpeALF_INDEX = gpdABCDE('I', 'N', 'D', 'E', 'X'),

	gpeALF_INFOX = gpdABCDE('I', 'N', 'F', 'O', 'X'),

	gpeALF_KCTRL = gpdABCDE('K', 'C', 'T', 'R', 'L'),
	gpeALF_LABEL = gpdABCDE('L', 'A', 'B', 'E', 'L'),
	gpeALF_LATHE = gpdABCDE('L', 'A', 'T', 'H', 'E'),
	gpeALF_LOCAL = gpdABCDE('L', 'O', 'C', 'A', 'L'),
	gpeALF_MSKAB = gpdABCDE('M', 'S', 'K', 'A', 'B'),
	gpeALF_NEGLG = gpdABCDE('N', 'E', 'G', 'L', 'G'),
	gpeALF_neqLG = gpdABCDE('N', 'E', 'Q', 'L', 'G'),
	gpeALF_NGATE = gpdABCDE('N', 'G', 'A', 'T', 'E'),
	gpeALF_NGCON = gpdABCDE('N', 'G', 'C', 'O', 'N'),
	gpeALF_NGDIE = gpdABCDE('N', 'G', 'D', 'I', 'E'),
    gpeALF_NONSE = gpdABCDE('N', 'O', 'N', 'S', 'E'),

	gpeALF_notLG = gpdABCDE('N', 'O', 'T', 'L', 'G'),

	gpeALF_ORBIT = gpdABCDE('O', 'R', 'B', 'I', 'T'),
	gpeALF_PAINT = gpdABCDE('P', 'A', 'I', 'N', 'T'),
	gpeALF_PASTE = gpdABCDE('P', 'A', 'S', 'T', 'E'),
	gpeALF_PICQC = gpdABCDE('P', 'I', 'C', 'Q', 'C'),
	gpeALF_PRINT = gpdABCDE('P', 'R', 'I', 'N', 'T'),


	/// PICii 2 -----------------------------------------------
	gpeALF_PICii = gpdABCDE('P', 'I', 'C', 'I', 'I'),	// 2
	gpeALF_PICiv = gpdABCDE('P', 'I', 'C', 'I', 'V'),	// 4
	gpeALF_PICvi = gpdABCDE('P', 'I', 'C', 'V', 'I'),	// 6
	gpeALF_PICix = gpdABCDE('P', 'I', 'C', 'I', 'X'),	// 9
	gpeALF_PICxi = gpdABCDE('P', 'I', 'C', 'X', 'I'),	// 11

	gpeALF_READY = gpdABCDE('R', 'E', 'A', 'D', 'Y'),
	gpeALF_REPIC = gpdABCDE('R', 'E', 'P', 'I', 'C'),


	gpeALF_RESET = gpdABCDE('R', 'E', 'S', 'E', 'T'),

	gpeALF_RIGHT = gpdABCDE('R', 'I', 'G', 'H', 'T'),
	gpeALF_rootM = gpdABCDE('R', 'O', 'O', 'T', 'M'),
	gpeALF_RULES = gpdABCDE('R', 'U', 'L', 'E', 'S'),

	gpeALF_SCENE = gpdABCDE('S', 'C', 'E', 'N', 'E'),

	gpeALF_SETUP = gpdABCDE('S', 'E', 'T', 'U', 'P'),
	gpeALF_SHARE = gpdABCDE('S', 'H', 'A', 'R', 'E'),
	gpeALF_SHOES = gpdABCDE('S', 'H', 'O', 'E', 'S'),
	gpeALF_SIMEI = gpdABCDE('S', 'I', 'M', 'E', 'I'),
	gpeALF_SLEFT = gpdABCDE('S', 'L', 'E', 'F', 'T'),
	gpeALF_SLMPo = gpdABCDE('S', 'L', 'M', 'P', 'o'),

	gpeALF_SNAIL = gpdABCDE('S', 'N', 'A', 'I', 'L'),

	gpeALF_SPEED = gpdABCDE('S', 'P', 'E', 'E', 'D'),

	gpeALF_STORE = gpdABCDE('S', 'T', 'O', 'R', 'E'),
	gpeALF_STACK = gpdABCDE('S', 'T', 'A', 'C', 'K'),
	gpeALF_STONE = gpdABCDE('S', 'T', 'O', 'N', 'E'),
	gpeALF_TABWH = gpdABCDE('T', 'A', 'B', 'W', 'H'),
	gpeALF_TABXY = gpdABCDE('T', 'A', 'B', 'X', 'Y'),
	gpeALF_THISA = gpdABCDE('T', 'H', 'I', 'S', 'A'),
	gpeALF_THISN = gpdABCDE('T', 'H', 'I', 'S', 'N'),
	gpeALF_TOKEN = gpdABCDE('T', 'O', 'K', 'E', 'N'),
	gpeALF_TOOLA = gpdABCDE('T', 'O', 'O', 'L', 'A'),
	gpeALF_TOOLB, gpeALF_TOOLC, gpeALF_TOOLD, gpeALF_TOOLE,
	gpeALF_TOOLF, gpeALF_TOOLG, gpeALF_TOOLH, gpeALF_TOOLI,
	gpeALF_TOOLJ, gpeALF_TOOLK, gpeALF_TOOLL, gpeALF_TOOLM,
	gpeALF_TOOLN, gpeALF_TOOLO, gpeALF_TOOLP, gpeALF_TOOLQ,
	gpeALF_TOOLR, gpeALF_TOOLS, gpeALF_TOOLT,
	gpeALF_TOOLU, gpeALF_TOOLV, gpeALF_TOOLW,
	gpeALF_TOOLX, gpeALF_TOOLY, gpeALF_TOOLZ,

	gpeALF_TOUCH = gpdABCDE('T', 'O', 'U', 'C', 'H'),
	gpeALF_TRACK = gpdABCDE('T', 'R', 'A', 'C', 'K'),
	gpeALF_TRGCP = gpdABCDE('T', 'R', 'G', 'C', 'P'),

	gpeALF_UNDEF = gpdABCDE('U', 'N', 'D', 'E', 'F'),

	gpeALF_UNSEL = gpdABCDE('U', 'N', 'S', 'E', 'L'),

	gpeALF_VARIA = gpdABCDE('V', 'A', 'R', 'I', 'A'),
	gpeALF_VOICE = gpdABCDE('V', 'O', 'I', 'C', 'E'),		// SIM7600 -

	gpeALF_WHILE = gpdABCDE('W', 'H', 'I', 'L', 'E'),

	gpeALF_WCDMA = gpdABCDE('W', 'C', 'D', 'M', 'A'),		// SIM7600 - CPSI SysMOD

	gpeALF_WMLBX = gpdABCDE('W', 'M', 'L', 'B', 'X'),		// egér X poziciója mielött le lett nyomva a BAL gomb
	gpeALF_WMLBY = gpdABCDE('W', 'M', 'L', 'B', 'Y'),		// egér Y poziciója mielött le lett nyomva a BAL gomb

	gpeALF_WMRBX = gpdABCDE('W', 'M', 'R', 'B', 'X'),		// egér X poziciója mielött le lett nyomva a JOBB gomb
	gpeALF_WMRBY = gpdABCDE('W', 'M', 'R', 'B', 'Y'),		// egér Y poziciója mielött le lett nyomva a JOBB gomb
	gpeALF_WRITE = gpdABCDE('W', 'R', 'I', 'T', 'E'),

	gpeALF_5 = gpdABCDE('Z', 'Z', 'Z', 'Z', 'Z'),

	//- 6 -----------------
	gpeALF_AAAAAA = gpdABCDEF('A', 'A', 'A', 'A', 'A', 'A'),
	gpeALF_ACCEPT = gpdABCDEF('A', 'C', 'C', 'E', 'P', 'T'),
	gpeALF_ABOARD = gpdABCDEF('A', 'B', 'O', 'A', 'R', 'D'),
	gpeALF_BUBLER = gpdABCDEF('B', 'U', 'B', 'L', 'E', 'R'),
	gpeALF_CONSTR = gpdABCDEF('C', 'O', 'N', 'S', 'T', 'R'),


	/// CNL -----------------------------------------------
	gpeALF_CNLiiT = gpdABCDEF('C', 'N', 'L', 'I', 'I', 'T'),
	/// ii 2
	gpeALF_CNLiiX = gpdABCDEF('C', 'N', 'L', 'I', 'I', 'X'),
	gpeALF_CNLiiY = gpdABCDEF('C', 'N', 'L', 'I', 'I', 'Y'),
	gpeALF_CNLiiZ = gpdABCDEF('C', 'N', 'L', 'I', 'I', 'Z'),
	gpeALF_CNLiiW = gpdABCDEF('C', 'N', 'L', 'I', 'I', 'W'),

	gpeALF_CNLivT = gpdABCDEF('C', 'N', 'L', 'I', 'V', 'T'),
	/// iv 4
	gpeALF_CNLivX = gpdABCDEF('C', 'N', 'L', 'I', 'V', 'X'),
	gpeALF_CNLivY = gpdABCDEF('C', 'N', 'L', 'I', 'V', 'Y'),
	gpeALF_CNLivZ = gpdABCDEF('C', 'N', 'L', 'I', 'V', 'Z'),
	gpeALF_CNLivW = gpdABCDEF('C', 'N', 'L', 'I', 'V', 'W'),

	gpeALF_CNLviT = gpdABCDEF('C', 'N', 'L', 'V', 'I', 'T'),
	/// vi 6
	gpeALF_CNLviX = gpdABCDEF('C', 'N', 'L', 'V', 'I', 'X'),
	gpeALF_CNLviY = gpdABCDEF('C', 'N', 'L', 'V', 'I', 'Y'),
	gpeALF_CNLviZ = gpdABCDEF('C', 'N', 'L', 'V', 'I', 'Z'),
	gpeALF_CNLviW = gpdABCDEF('C', 'N', 'L', 'V', 'I', 'W'),

	gpeALF_CNLixT = gpdABCDEF('C', 'N', 'L', 'I', 'X', 'T'),
	/// ix 9
	gpeALF_CNLixX = gpdABCDEF('C', 'N', 'L', 'I', 'X', 'X'),
	gpeALF_CNLixY = gpdABCDEF('C', 'N', 'L', 'I', 'X', 'Y'),
	gpeALF_CNLixZ = gpdABCDEF('C', 'N', 'L', 'I', 'X', 'Z'),
	gpeALF_CNLixW = gpdABCDEF('C', 'N', 'L', 'I', 'X', 'W'),

	gpeALF_CNLxiT = gpdABCDEF('C', 'N', 'L', 'X', 'I', 'T'),
	/// xi 11
	gpeALF_CNLxiX = gpdABCDEF('C', 'N', 'L', 'X', 'I', 'X'),
	gpeALF_CNLxiY = gpdABCDEF('C', 'N', 'L', 'X', 'I', 'Y'),
	gpeALF_CNLxiZ = gpdABCDEF('C', 'N', 'L', 'X', 'I', 'Z'),
	gpeALF_CNLxiW = gpdABCDEF('C', 'N', 'L', 'X', 'I', 'W'),

	gpeALF_CRESET = gpdABCDEF('C', 'R', 'E', 'S', 'E', 'T'),	// SIM7600 - AT


	gpeALF_ELAPSE = gpdABCDEF('E', 'L', 'A', 'P', 'S', 'E'),
	gpeALF_FERIKE = gpdABCDEF('F', 'E', 'R', 'I', 'K', 'E'),
	gpeALF_FLINCH = gpdABCDEF('F', 'L', 'I', 'N', 'C', 'H'),
	gpeALF_FORBID = gpdABCDEF('F', 'O', 'R', 'B', 'I', 'D'),
	gpeALF_FORMAT = gpdABCDEF('F', 'O', 'R', 'M', 'A', 'T'),
	gpeALF_GALAXY = gpdABCDEF('G', 'A', 'L', 'A', 'X', 'Y'),
	gpeALF_JACKET = gpdABCDEF('J', 'A', 'C', 'K', 'E', 'T'),
	gpeALF_KSHIFT = gpdABCDEF('K', 'S', 'H', 'I', 'F', 'T'),
	gpeALF_KOSZON = gpdABCDEF('K', 'O', 'S', 'Z', 'O', 'N'),
	gpeALF_LISTEN = gpdABCDEF('L', 'I', 'S', 'T', 'E', 'N'),

	gpeALF_newrow = gpdABCDEF('N', 'E', 'W', 'R', 'O', 'W'),
    gpeALF_NONSEN = gpdABCDEF('N', 'O', 'N', 'S', 'E', 'N'),

	gpeALF_MISSED = gpdABCDEF('M', 'I', 'S', 'S', 'E', 'D'),	// SIM7600 -
	gpeALF_MODBUS = gpdABCDEF('M', 'O', 'D', 'B', 'U', 'S'),
	gpeALF_NBUILD = gpdABCDEF('N', 'B', 'U', 'I', 'L', 'D'),
	gpeALF_ONLINE = gpdABCDEF('O', 'N', 'L', 'I', 'N', 'E'),	// SIM7600 - CPSI OpMod
	gpeALF_PICCPY = gpdABCDEF('P', 'I', 'C', 'C', 'P', 'Y'),
	gpeALF_PLANET = gpdABCDEF('P', 'L', 'A', 'N', 'E', 'T'),
	gpeALF_POLYER = gpdABCDEF('P', 'O', 'L', 'Y', 'E', 'R'),
	gpeALF_PRAGMA = gpdABCDEF('P', 'R', 'A', 'G', 'M', 'A'),

	/// PICiii 3 -----------------------------------------------
	gpeALF_PICiii = gpdABCDEF('P', 'I', 'C', 'I', 'I', 'I'), // 3
	gpeALF_PICvii = gpdABCDEF('P', 'I', 'C', 'V', 'I', 'I'), // 7
	gpeALF_PICxii = gpdABCDEF('P', 'I', 'C', 'X', 'I', 'I'), // 12

	gpeALF_REGGIO = gpdABCDEF('R', 'E', 'G', 'G', 'I', 'O'),
	gpeALF_RENAME = gpdABCDEF('R', 'E', 'N', 'A', 'M', 'E'),
	gpeALF_RETURN = gpdABCDEF('R', 'E', 'T', 'U', 'R', 'N'),
	//gpeALF_SHADER =	gpdABCDEF( 'S','H','A','D','E','R' ),

	gpeALF_SIZEOF = gpdABCDEF('S', 'I', 'Z', 'E', 'O', 'F'),
	gpeALF_SHADOW = gpdABCDEF('S', 'H', 'A', 'D', 'O', 'W'),
	gpeALF_SHIELD = gpdABCDEF('S', 'H', 'I', 'E', 'L', 'D'),
	gpeALF_SPRITE = gpdABCDEF('S', 'P', 'R', 'I', 'T', 'E'),
	gpeALF_SRIGHT = gpdABCDEF('S', 'R', 'I', 'G', 'H', 'T'),
	gpeALF_STEREO = gpdABCDEF('S', 'T', 'E', 'R', 'E', 'O'),
	gpeALF_STOREE = gpdABCDEF('S', 'T', 'O', 'R', 'E', 'E'),
	gpeALF_STOWUP = gpdABCDEF('S', 'T', 'O', 'W', 'U', 'P'),
	gpeALF_STRROW = gpdABCDEF('S', 'T', 'R', 'R', 'O', 'W'),
	gpeALF_SWITCH = gpdABCDEF('S', 'W', 'I', 'T', 'C', 'H'),
	gpeALF_TARGET = gpdABCDEF('T', 'A', 'R', 'G', 'E', 'T'),
	gpeALF_TELNET = gpdABCDEF('T', 'E', 'L', 'N', 'E', 'T'),
	gpeALF_TRGCPA = gpdABCDEF('T', 'R', 'G', 'C', 'P', 'A'),
	gpeALF_TRGCPB, gpeALF_TRGCPC, gpeALF_TRGCPD, gpeALF_TRGCPE,
	gpeALF_TRGCPF, gpeALF_TRGCPG, gpeALF_TRGCPH, gpeALF_TRGCPI,
	gpeALF_TRGCPJ, gpeALF_TRGCPK, gpeALF_TRGCPL, gpeALF_TRGCPM,
	gpeALF_TRGCPN, gpeALF_TRGCPO, gpeALF_TRGCPP, gpeALF_TRGCPQ,
	gpeALF_TRGCPR, gpeALF_TRGCPS, gpeALF_TRGCPT,
	gpeALF_TRGCPU, gpeALF_TRGCPV, gpeALF_TRGCPW,
	gpeALF_TRGCPX, gpeALF_TRGCPY, gpeALF_TRGCPZ,


	gpeALF_6 = gpdABCDEF('Z', 'Z', 'Z', 'Z', 'Z', 'Z'),

	//- 7 -----------------

	gpeALF_AAAAAAA = gpdABCDEFG('A', 'A', 'A', 'A', 'A', 'A', 'A'),
	gpeALF_ACCOUNT = gpdABCDEFG('A', 'C', 'C', 'O', 'U', 'N', 'T'),
	gpeALF_CAMLIST = gpdABCDEFG('C', 'A', 'M', 'L', 'I', 'S', 'T'),

	/// CNL -----------------------------------------------
	gpeALF_CNLiiiT = gpdABCDEFG('C', 'N', 'L', 'I', 'I', 'I', 'T'),
	/// iii 3
	gpeALF_CNLiiiX = gpdABCDEFG('C', 'N', 'L', 'I', 'I', 'I', 'X'),
	gpeALF_CNLiiiY = gpdABCDEFG('C', 'N', 'L', 'I', 'I', 'I', 'Y'),
	gpeALF_CNLiiiZ = gpdABCDEFG('C', 'N', 'L', 'I', 'I', 'I', 'Z'),
	gpeALF_CNLiiiW = gpdABCDEFG('C', 'N', 'L', 'I', 'I', 'I', 'W'),

	gpeALF_CNLviiT = gpdABCDEFG('C', 'N', 'L', 'V', 'I', 'I', 'T'),
	/// vii 7
	gpeALF_CNLviiX = gpdABCDEFG('C', 'N', 'L', 'V', 'I', 'I', 'X'),
	gpeALF_CNLviiY = gpdABCDEFG('C', 'N', 'L', 'V', 'I', 'I', 'Y'),
	gpeALF_CNLviiZ = gpdABCDEFG('C', 'N', 'L', 'V', 'I', 'I', 'Z'),
	gpeALF_CNLviiW = gpdABCDEFG('C', 'N', 'L', 'V', 'I', 'I', 'W'),

	gpeALF_CNLxiiT = gpdABCDEFG('C', 'N', 'L', 'X', 'I', 'I', 'T'),
	/// xii 12
	gpeALF_CNLxiiX = gpdABCDEFG('C', 'N', 'L', 'X', 'I', 'I', 'X'),
	gpeALF_CNLxiiY = gpdABCDEFG('C', 'N', 'L', 'X', 'I', 'I', 'Y'),
	gpeALF_CNLxiiZ = gpdABCDEFG('C', 'N', 'L', 'X', 'I', 'I', 'Z'),
	gpeALF_CNLxiiW = gpdABCDEFG('C', 'N', 'L', 'X', 'I', 'I', 'W'),



	gpeALF_CONNECT = gpdABCDEFG('C', 'O', 'N', 'N', 'E', 'C', 'T'),
	gpeALF_COMMAND = gpdABCDEFG('C', 'O', 'M', 'M', 'A', 'N', 'D'),


	gpeALF_FAVICON = gpdABCDEFG('F', 'A', 'V', 'I', 'C', 'O', 'N'),

	gpeALF_GIOFILE = gpdABCDEFG('G', 'I', 'O', 'F', 'I', 'L', 'E'),
	gpeALF_GRIPPER = gpdABCDEFG('G', 'R', 'I', 'P', 'P', 'E', 'R'),
	gpeALF_NOBUILD = gpdABCDEFG('N', 'O', 'B', 'U', 'I', 'L', 'D'),
	gpeALF_NONSENS = gpdABCDEFG('N', 'O', 'N', 'S', 'E', 'N', 'S'),
	gpeALF_NRENDER = gpdABCDEFG('N', 'R', 'E', 'N', 'D', 'E', 'R'),
	// 0xffffFFFF                A    B    C    D    E    F    G
	gpeALF_MISSING = gpdABCDEFG('M', 'I', 'S', 'S', 'I', 'N', 'G'),		// SIM7600 -
	gpeALF_MWLQKWU = gpdABCDEFG('M', 'W', 'L', 'Q', 'K', 'W', 'U'),
	gpeALF_U4      = gpeALF_MWLQKWU,


	gpeALF_PICCPYE = gpdABCDEFG('P', 'I', 'C', 'C', 'P', 'Y', 'E'),
	/// PICviii -----------------------------------------------
	gpeALF_PICviii = gpdABCDEFG('P', 'I', 'C', 'v', 'i', 'i', 'i'), // 8

	gpeALF_PICLIST = gpdABCDEFG('P', 'I', 'C', 'L', 'I', 'S', 'T'),


	gpeALF_REFERER = gpdABCDEFG('R', 'E', 'F', 'E', 'R', 'E', 'R'),
	gpeALF_RESTART = gpdABCDEFG('R', 'E', 'S', 'T', 'A', 'R', 'T'),
	//gpeALF_INCLUDE =	gpdABCDEFG( 'I','N','C','L','U','D','E' ),
	gpeALF_STEREOO = gpdABCDEFG('S', 'T', 'E', 'R', 'E', 'O', 'O'),
	gpeALF_TROUSET = gpdABCDEFG('T', 'R', 'O', 'U', 'S', 'E', 'T'),
	gpeALF_UPGRADE = gpdABCDEFG('U', 'P', 'G', 'R', 'A', 'D', 'E'),
	gpeALF_VEHICLE = gpdABCDEFG('V', 'E', 'H', 'I', 'C', 'L', 'E'),
	gpeALF_WINMOVE = gpdABCDEFG('W', 'I', 'N', 'M', 'O', 'V', 'E'),
	gpeALF_7 = gpdABCDEFG('Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z'),

	//- 8 -----------------
	gpeALF_AAAAAAAA = gpdABCDEFGH('A', 'A', 'A', 'A', 'A', 'A', 'A', 'A'),
	gpeALF_BONELIST = gpdABCDEFGH('B', 'O', 'N', 'E', 'L', 'I', 'S', 'T'),
	gpeALF_CGPSAUTO = gpdABCDEFGH('C', 'G', 'P', 'S', 'A', 'U', 'T', 'O'),
	gpeALF_CGPSCOLD = gpdABCDEFGH('C', 'G', 'P', 'S', 'C', 'O', 'L', 'D'),
	gpeALF_CGPSINFO = gpdABCDEFGH('C', 'G', 'P', 'S', 'I', 'N', 'F', 'O'),
	gpeALF_CONTINUE = gpdABCDEFGH('C', 'O', 'N', 'T', 'I', 'N', 'U', 'E'),
	gpeALF_GATELIST = gpdABCDEFGH('G', 'A', 'T', 'E', 'L', 'I', 'S', 'T'),
	gpeALF_GIOLAYER = gpdABCDEFGH('G', 'I', 'O', 'L', 'A', 'Y', 'E', 'R'),

	gpeALF_GPICBGRA = gpdABCDEFGH('G', 'P', 'I', 'C', 'B', 'G', 'R', 'A'),


	gpeALF_GXBULLET = gpdABCDEFGH('G', 'X', 'B', 'U', 'L', 'L', 'E', 'T'),

	gpeALF_GXBUYFTO = gpdABCDEFGH('G', 'X', 'B', 'U', 'Y', 'F', 'T', 'O'),
	gpeALF_GXBUYGPX = gpdABCDEFGH('G', 'X', 'B', 'U', 'Y', 'G', 'P', 'X'),
	gpeALF_GXBUYGRN = gpdABCDEFGH('G', 'X', 'B', 'U', 'Y', 'G', 'R', 'N'),
	gpeALF_GXBUYPNG = gpdABCDEFGH('G', 'X', 'B', 'U', 'Y', 'P', 'N', 'G'),
	gpeALF_GXBUYPNR = gpdABCDEFGH('G', 'X', 'B', 'U', 'Y', 'P', 'N', 'R'),
	gpeALF_GXBUYTIM = gpdABCDEFGH('G', 'X', 'B', 'U', 'Y', 'T', 'I', 'M'),
	gpeALF_GXBUYVOX = gpdABCDEFGH('G', 'X', 'B', 'U', 'Y', 'V', 'O', 'X'),

	gpeALF_GXDDDDST = gpdABCDEFGH('G', 'X', 'D', 'D', 'D', 'D', 'S', 'T'),
	gpeALF_GXDDDGET = gpdABCDEFGH('G', 'X', 'D', 'D', 'D', 'G', 'E', 'T'),
	gpeALF_GXDDDGPX = gpdABCDEFGH('G', 'X', 'D', 'D', 'D', 'G', 'P', 'X'),

	gpeALF_GXDDDLST = gpdABCDEFGH('G', 'X', 'D', 'D', 'D', 'L', 'S', 'T'),
	gpeALF_GXDDDRST = gpdABCDEFGH('G', 'X', 'D', 'D', 'D', 'R', 'S', 'T'),

	gpeALF_GXDDDPNG = gpdABCDEFGH('G', 'X', 'D', 'D', 'D', 'P', 'N', 'G'),	// gx_id, size_3d, div_3d, sizeof(a_dddpng), (gpcI4)near_xyz
	gpeALF_GXDDDRDY = gpdABCDEFGH('G', 'X', 'D', 'D', 'D', 'R', 'D', 'Y'),

	// GIV
	gpeALF_GXGIVDST = gpdABCDEFGH('G', 'X', 'G', 'I', 'V', 'D', 'S', 'T'),
	gpeALF_GXGIVGET = gpdABCDEFGH('G', 'X', 'G', 'I', 'V', 'G', 'E', 'T'),
	gpeALF_GXGIVGPX = gpdABCDEFGH('G', 'X', 'G', 'I', 'V', 'G', 'P', 'X'),
	gpeALF_GXGIVLST = gpdABCDEFGH('G', 'X', 'G', 'I', 'V', 'L', 'S', 'T'),
	gpeALF_GXGIVPUB = gpdABCDEFGH('G', 'X', 'G', 'I', 'V', 'P', 'U', 'B'),
	gpeALF_GXGIVRDY = gpdABCDEFGH('G', 'X', 'G', 'I', 'V', 'R', 'D', 'Y'),
	gpeALF_GXGIVRST = gpdABCDEFGH('G', 'X', 'G', 'I', 'V', 'R', 'S', 'T'),


	gpeALF_GXIABOVR = gpdABCDEFGH('G', 'X', 'I', 'A', 'B', 'O', 'V', 'R'),
	gpeALF_GXIABTIM = gpdABCDEFGH('G', 'X', 'I', 'A', 'B', 'T', 'I', 'M'),
	gpeALF_GXIABTUR = gpdABCDEFGH('G', 'X', 'I', 'A', 'B', 'T', 'U', 'R'),
	gpeALF_GXIACTID = gpdABCDEFGH('G', 'X', 'I', 'A', 'C', 'T', 'I', 'D'), // akció azonosító
	gpeALF_GXIACTIM = gpdABCDEFGH('G', 'X', 'I', 'A', 'C', 'T', 'I', 'M'), // action (animáció) idő kezdete
	gpeALF_GXIMOMID = gpdABCDEFGH('G', 'X', 'I', 'M', 'O', 'M', 'I', 'D'),
	gpeALF_GXISIZER = gpdABCDEFGH('G', 'X', 'I', 'S', 'I', 'Z', 'E', 'R'),
	gpeALF_GXITAVEC = gpdABCDEFGH('G', 'X', 'I', 'T', 'A', 'V', 'E', 'C'),
	gpeALF_GXITBORN = gpdABCDEFGH('G', 'X', 'I', 'T', 'B', 'O', 'R', 'N'),
	gpeALF_GXITBVEC = gpdABCDEFGH('G', 'X', 'I', 'T', 'B', 'V', 'E', 'C'),
	gpeALF_GXITEMID = gpdABCDEFGH('G', 'X', 'I', 'T', 'E', 'M', 'I', 'D'), // elem egyedi azonosító
	gpeALF_GXITMPOS = gpdABCDEFGH('G', 'X', 'I', 'T', 'M', 'P', 'O', 'S'),
	gpeALF_GXITMYPR = gpdABCDEFGH('G', 'X', 'I', 'T', 'M', 'Y', 'P', 'R'), // xy YAW PITCH ROLL float
	gpeALF_GXITNAME = gpdABCDEFGH('G', 'X', 'I', 'T', 'N', 'A', 'M', 'E'),
	gpeALF_GXITPACK = gpdABCDEFGH('G', 'X', 'I', 'T', 'P', 'A', 'C', 'K'), // elem block

	gpeALF_GXITPLAY = gpdABCDEFGH('G', 'X', 'I', 'T', 'P', 'L', 'A', 'Y'),

	gpeALF_GXITSTAR = gpdABCDEFGH('G', 'X', 'I', 'T', 'S', 'T', 'A', 'R'),
	gpeALF_GXITTYPE = gpdABCDEFGH('G', 'X', 'I', 'T', 'T', 'Y', 'P', 'E'),
	gpeALF_GXITYVEC = gpdABCDEFGH('G', 'X', 'I', 'T', 'Y', 'V', 'E', 'C'), // YZ Y VEC folat
	gpeALF_GXITZVEC = gpdABCDEFGH('G', 'X', 'I', 'T', 'Z', 'V', 'E', 'C'), // YZ Z VEC float
	gpeALF_GXITYZTM = gpdABCDEFGH('G', 'X', 'I', 'T', 'Y', 'Z', 'T', 'M'), // YZ tengelyek bázis ideje
	gpeALF_GXMAINTM = gpdABCDEFGH('G', 'X', 'M', 'A', 'I', 'N', 'T', 'M'), // server idő
	gpeALF_IDXCOUNT = gpdABCDEFGH('I', 'D', 'X', 'C', 'O', 'U', 'N', 'T'),
	gpeALF_IDXSTART = gpdABCDEFGH('I', 'D', 'X', 'S', 'T', 'A', 'R', 'T'),
	gpeALF_INTERNET = gpdABCDEFGH('I', 'N', 'T', 'E', 'R', 'N', 'E', 'T'),
	gpeALF_NLAYALOC = gpdABCDEFGH('N', 'L', 'A', 'Y', 'A', 'L', 'O', 'C'),
	gpeALF_NONSENSE = gpdABCDEFGH('N', 'O', 'N', 'S', 'E', 'N', 'S', 'E'),
	gpeALF_NSURFPIC = gpdABCDEFGH('N', 'S', 'U', 'R', 'F', 'P', 'I', 'C'),
	gpeALF_PARTSURF = gpdABCDEFGH('P', 'A', 'R', 'T', 'S', 'U', 'R', 'F'),
	gpeALF_PICSFILE = gpdABCDEFGH('P', 'I', 'C', 'S', 'F', 'I', 'L', 'E'),
	gpeALF_PLAYERID = gpdABCDEFGH('P', 'L', 'A', 'Y', 'E', 'R', 'I', 'D'),
	gpeALF_PLAYNAME = gpdABCDEFGH('P', 'L', 'A', 'Y', 'N', 'A', 'M', 'E'),
	gpeALF_PLAYPASW = gpdABCDEFGH('P', 'L', 'A', 'Y', 'P', 'A', 'S', 'W'),
	gpeALF_ROWORBIT = gpdABCDEFGH('R', 'O', 'W', 'O', 'R', 'B', 'I', 'T'),
	gpeALF_RULEPACK = gpdABCDEFGH('R', 'U', 'L', 'E', 'P', 'A', 'C', 'K'),
	gpeALF_SHUTDOWN = gpdABCDEFGH('S', 'H', 'U', 'T', 'D', 'O', 'W', 'N'),
	gpeALF_STEREOOO = gpdABCDEFGH('S', 'T', 'E', 'R', 'E', 'O', 'O', 'O'),
	gpeALF_TRIANGLE = gpdABCDEFGH('T', 'R', 'I', 'A', 'N', 'G', 'L', 'E'),
	gpeALF_VRTXBASE = gpdABCDEFGH('V', 'R', 'T', 'X', 'B', 'A', 'S', 'E'),
	gpeALF_VXNCOUNT = gpdABCDEFGH('V', 'X', 'N', 'C', 'O', 'U', 'N', 'T'),
	gpeALF_VXPTCUFB = gpdABCDEFGH('V', 'X', 'P', 'T', 'C', 'U', 'F', 'B'),

	gpeALF_8 = gpdABCDEFGH('Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z'),

	//- 8 -----------------

	gpeALF_STEREOOOO = gpdABCDEFGHI('S', 'T', 'E', 'R', 'E', 'O', 'O', 'O', 'O'),
	gpeALF_9 = gpdABCDEFGHI('Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z'),

	//- 9 -----------------

	//- 10 -----------------
	gpeALF_CONNECTION 	= gpdABCDEFGHIJ('C','O','N','N','E','C','T','I','O','N'),
	gpeALF_10 = gpdABCDEFGHIJ('Z','Z','Z','Z','Z','Z','Z','Z','Z','Z'),

	//- 11 -----------------
	gpeALF_11 = gpdABCDEFGHIJK('Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z'),

	//- 12 -----------------
	gpeALF_12 = gpdABCDEFGHIJKL('Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z'),
	//- 13 -----------------
	gpeALF_13 = gpdABCDEFGHIJKLM('Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z'),

	gpeALF_resEND = gpeALF_10,

	gpeALF_CRPXNLSKVLJFHG = 0x7fffffffffffffff,
} GPT_ALFA;

int alfLEN( gpeALF alf );
U1*		gpf_aALF_init(void);
//U8 		gpfALF2STR( U1* p_out, I8 d0 );
inline U8 gpfALF2STR( void* p_out, I8 d0 )
{

	if( !p_out )
		return 0;
	if( !d0 )
	{
		*(U1*)p_out = 0;
		return 0;
	}
	char	lx_s_buff[0x40],
            *p_buff = lx_s_buff + 0x3f,
            *p_end = p_buff;

	*p_end = 0;
	bool b_minus = false;
	if( d0 < 0 )
	{
		b_minus = true;
		d0 *= -1;
	}

	I8 d1;
	while( d0 )
	{
		d1 = d0;
		d0 = (d0-1)/gpdALF;
		p_buff--;
		*p_buff = (d1-d0*gpdALF)+'\`';
	}

	if( b_minus )
	{
		p_buff--;
		*p_buff = '-';
	}
	U2 n = p_end-p_buff;
	memcpy( p_out, p_buff, n+1 );
	((U1*)p_out)[n] = 0;
	return n;
}
inline gpeALF gpfSTR2ALF( const U1* p_str, const U1* p_end, U1** pp_str = NULL );




#endif // GPEALF_H_INCLUDED
