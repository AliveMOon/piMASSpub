#ifndef GPEALF_H_INCLUDED
#define GPEALF_H_INCLUDED

#include "piMASS.h"


#define gpmbNUM( c ) ( c >= '0' && c <= '9' )
#define gpmbASCII( c, a ) (c < 0x80 ? a[c] : false)
#define gpmbABC( c, a ) (c < 0x80 ? a[c] : true)	// UTF8 kompatible azaz 0x80 felett betünek vesz mindent
//#define gpmbUTF8( c, a ) (c < 0x80 ? a[c] : true)

#define gpdUP ('a'-'A')
#define gpdUP_CASE(a){ if( a >= 'a' && a <= 'z' ){ a -= gpdUP; }}
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

typedef enum gpeALFu4: U4
{
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
typedef enum gpeALF: I8
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
	gpeALF_AA = gpdAB('A', 'A'),
	gpeALF_AN = gpdAB('A', 'N'),
	gpeALF_AT = gpdAB('A', 'T'),
	gpeALF_CH = gpdAB('C', 'H'),
	gpeALF_CL = gpdAB('C', 'L'),
	gpeALF_CU = gpdAB('C', 'U'),
	gpeALF_CW = gpdAB('C', 'W'),
	gpeALF_DS = gpdAB('D', 'S'),
	gpeALF_FI = gpdAB('F', 'I'),
	gpeALF_GX = gpdAB('G', 'X'),
	gpeALF_ID = gpdAB('I', 'D'),
	gpeALF_IF = gpdAB('I', 'F'),
	gpeALF_IH = gpdAB('I', 'H'),

	gpeALF_IA = gpdAB('I', 'A'),
	gpeALF_IN = gpdAB('I', 'N'),

	gpeALF_IW = gpdAB('I', 'W'),
	gpeALF_IX = gpdAB('I', 'X'),
	gpeALF_IY = gpdAB('I', 'Y'),

	// key ( A, B, ..., Z )
	gpeALF_KA = gpdAB('K', 'A'),			// KEYBOARD 'a'
	gpeALF_KO = gpdAB('K', 'O'),
	gpeALF_KZ = gpdAB('K', 'Z'),
	gpeALF_LG = gpdAB('L', 'G'),

	gpeALF_MA = gpdAB('M', 'A'),
	gpeALF_MN = gpdAB('M', 'N'),
	gpeALF_MS = gpdAB('M', 'S'),
	gpeALF_MX = gpdAB('M', 'X'),
	gpeALF_MY = gpdAB('M', 'Y'),

	gpeALF_NA = gpdAB('N', 'A'),
	gpeALF_NO = gpdAB('N', 'O'),

	gpeALF_OK = gpdAB('O', 'K'),

	gpeALF_ON = gpdAB('O', 'N'),
	gpeALF_OR = gpdAB('O', 'R'),
	gpeALF_PI = gpdAB('P', 'I'),
	gpeALF_RE = gpdAB('R', 'E'),
	gpeALF_SL = gpdAB('S', 'L'),
	gpeALF_SR = gpdAB('S', 'R'),
	gpeALF_UP = gpdAB('U', 'P'),
	gpeALF_WB = gpdAB('W', 'B'),
	gpeALF_WH = gpdAB('W', 'H'),
	gpeALF_WX = gpdAB('W', 'X'),
	gpeALF_WY = gpdAB('W', 'Y'),
	gpeALF_WW = gpdAB('W', 'W'),
	gpeALF_ZM = gpdAB('Z', 'M'),
	gpeALF_2 = gpdAB('Z', 'Z'),
	//- 3 -----------------
	gpeALF_AAA = gpdABC('A', 'A', 'A'),
	gpeALF_ABS = gpdABC('A', 'B', 'S'),
	gpeALF_ACE = gpdABC('A', 'C', 'E'), // access
	gpeALF_ACT = gpdABC('A', 'C', 'T'),
	gpeALF_ADD = gpdABC('A', 'D', 'D'),
	gpeALF_ADR = gpdABC('A', 'D', 'R'),
	gpeALF_AIN = gpdABC('A', 'I', 'N'),
	gpeALF_AND = gpdABC('A', 'N', 'D'),
	gpeALF_ARJ = gpdABC('A', 'R', 'J'),
	gpeALF_ARR = gpdABC('A', 'R', 'R'),
	gpeALF_ASM = gpdABC('A', 'S', 'M'),
	gpeALF_BIN = gpdABC('B', 'I', 'N'),
	gpeALF_BLK = gpdABC('B', 'L', 'K'),
	gpeALF_BOB = gpdABC('B', 'O', 'B'),
	gpeALF_BOX = gpdABC('B', 'O', 'X'),
	gpeALF_CAM = gpdABC('C', 'A', 'M'),
	gpeALF_CLR = gpdABC('C', 'L', 'R'),
	gpeALF_CMX = gpdABC('C', 'M', 'X'),
	gpeALF_CMY = gpdABC('C', 'M', 'Y'),
	gpeALF_CNL = gpdABC('C', 'N', 'L'),
	gpeALF_COM = gpdABC('C', 'O', 'M'),
	gpeALF_COS = gpdABC('C', 'O', 'S'),
	gpeALF_CPP = gpdABC('C', 'P', 'P'),
	gpeALF_CPU = gpdABC('C', 'P', 'U'),
	gpeALF_CPY = gpdABC('C', 'P', 'Y'),
	gpeALF_CUT = gpdABC('C', 'U', 'T'),
	gpeALF_DEC = gpdABC('D', 'E', 'C'),
	gpeALF_DEF = gpdABC('D', 'E', 'F'),
	gpeALF_DEL = gpdABC('D', 'E', 'L'),
	gpeALF_DIR = gpdABC('D', 'I', 'R'),
	gpeALF_DIV = gpdABC('D', 'I', 'V'),
	gpeALF_DOT = gpdABC('D', 'O', 'T'),
	gpeALF_DST = gpdABC('D', 'S', 'T'),
	gpeALF_DZR = gpdABC('D', 'Z', 'R'),
	gpeALF_END = gpdABC('E', 'N', 'D'),
	gpeALF_ERU = gpdABC('E', 'R', 'U'),
	gpeALF_EYE = gpdABC('E', 'Y', 'E'),
	gpeALF_EXP = gpdABC('E', 'X', 'P'),

	// function key ( f1, f2, ..., f12)
	gpeALF_FKA = gpdABC('F', 'K', 'A'),
	gpeALF_FKL = gpdABC('F', 'K', 'L'),

	gpeALF_FMX = gpdABC('F', 'M', 'X'),
	gpeALF_FMY = gpdABC('F', 'M', 'Y'),
	gpeALF_FOR = gpdABC('F', 'O', 'R'),
	gpeALF_FPS = gpdABC('F', 'P', 'S'),
	gpeALF_GET = gpdABC('G', 'E', 'T'),
	gpeALF_GIM = gpdABC('G', 'I', 'M'),
	gpeALF_GIO = gpdABC('G', 'I', 'O'),
	gpeALF_GIS = gpdABC('G', 'I', 'S'),
	gpeALF_GIT = gpdABC('G', 'I', 'T'),
	gpeALF_GIV = gpdABC('G', 'I', 'V'),
	gpeALF_GPU = gpdABC('G', 'P', 'U'),
	gpeALF_GXT = gpdABC('G', 'X', 'T'),
	gpeALF_HUD = gpdABC('H', 'U', 'D'),
	gpeALF_INC = gpdABC('I', 'N', 'C'),
	gpeALF_INP = gpdABC('I', 'N', 'P'),
	gpeALF_INT = gpdABC('I', 'N', 'T'),
	gpeALF_INV = gpdABC('I', 'N', 'V'),
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
	gpeALF_MOV = gpdABC('M', 'O', 'V'),
	gpeALF_MPG = gpdABC('M', 'R', 'B'),
	gpeALF_MRB = gpdABC('M', 'R', 'B'),
	gpeALF_MUL = gpdABC('M', 'U', 'L'),
	gpeALF_NEW = gpdABC('N', 'E', 'W'),
	gpeALF_NON = gpdABC('N', 'O', 'N'),
	gpeALF_OFF = gpdABC('O', 'F', 'F'),
	gpeALF_ORM = gpdABC('O', 'R', 'M'),
	gpeALF_OUT = gpdABC('O', 'U', 'T'),
	gpeALF_PIC = gpdABC('P', 'I', 'C'),
	gpeALF_PIX = gpdABC('P', 'I', 'X'),
	gpeALF_PMX = gpdABC('P', 'M', 'X'),
	gpeALF_PMY = gpdABC('P', 'M', 'Y'),
	gpeALF_PNG = gpdABC('P', 'N', 'G'),
	gpeALF_PNT = gpdABC('P', 'N', 'T'),

	gpeALF_POS = gpdABC('P', 'O', 'S'),

	gpeALF_PRG = gpdABC('P', 'R', 'G'),
	gpeALF_REF = gpdABC('R', 'E', 'F'),
	gpeALF_REG = gpdABC('R', 'E', 'G'),
	gpeALF_REM = gpdABC('R', 'E', 'M'),
	gpeALF_REN = gpdABC('R', 'E', 'N'),
	gpeALF_RET = gpdABC('R', 'E', 'T'),
	gpeALF_RUN = gpdABC('R', 'U', 'N'),

	gpeALF_SDH = gpdABC('S', 'D', 'H'),
	gpeALF_SDW = gpdABC('S', 'D', 'W'),
	gpeALF_SDX = gpdABC('S', 'D', 'X'),
	gpeALF_SDY = gpdABC('S', 'D', 'Y'),

	gpeALF_SEC = gpdABC('S', 'E', 'C'),
	gpeALF_SIN = gpdABC('S', 'I', 'N'),
	gpeALF_SIT = gpdABC('S', 'I', 'T'),
	gpeALF_SLM = gpdABC('S', 'L', 'M'),
	gpeALF_SRC = gpdABC('S', 'R', 'C'),
	gpeALF_SRM = gpdABC('S', 'R', 'M'),

	gpeALF_SSH = gpdABC('S', 'S', 'H'),
	gpeALF_SSW = gpdABC('S', 'S', 'W'),
	gpeALF_SSX = gpdABC('S', 'S', 'X'),
	gpeALF_SSY = gpdABC('S', 'S', 'Y'),


	gpeALF_STR = gpdABC('S', 'T', 'R'),
	gpeALF_STK = gpdABC('S', 'T', 'K'),
	gpeALF_SUB = gpdABC('S', 'U', 'B'),
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
	gpeALF_WIN = gpdABC('W', 'I', 'N'),
	gpeALF_WMX = gpdABC('W', 'M', 'X'),
	gpeALF_WMY = gpdABC('W', 'M', 'Y'),
	gpeALF_YPR = gpdABC('Y', 'P', 'R'),
	gpeALF_XOR = gpdABC('X', 'O', 'R'),
	gpeALF_ZIP = gpdABC('Z', 'I', 'P'),
	gpeALF_3 = gpdABC('Z', 'Z', 'Z'),
	//- 4 -----------------
	gpeALF_AAAA = gpdABCD('A', 'A', 'A', 'A'),
	gpeALF_ABMS = gpdABCD('A', 'B', 'M', 'S'),
	gpeALF_ACOS = gpdABCD('A', 'C', 'O', 'S'),
	gpeALF_ADDM = gpdABCD('A', 'D', 'D', 'M'),
	gpeALF_ANDM = gpdABCD('A', 'N', 'D', 'M'),
	gpeALF_ASIN = gpdABCD('A', 'S', 'I', 'N'),
	gpeALF_BELG = gpdABCD('B', 'E', 'L', 'G'),
	gpeALF_BGLG = gpdABCD('B', 'G', 'L', 'G'),
	gpeALF_BONE = gpdABCD('B', 'O', 'N', 'E'),
	gpeALF_BUMM = gpdABCD('B', 'U', 'M', 'M'),
	gpeALF_BUMP = gpdABCD('B', 'U', 'M', 'P'),
	gpeALF_CNLC = gpdABCD('C', 'N', 'L', 'C'),

	gpeALF_COME = gpdABCD('C', 'O', 'M', 'E'),
	gpeALF_COMS = gpdABCD('C', 'O', 'M', 'S'),

	gpeALF_COPY = gpdABCD('C', 'O', 'P', 'Y'),
	gpeALF_CRSA = gpdABCD('C', 'R', 'S', 'A'),
	gpeALF_CRSL = gpdABCD('C', 'R', 'S', 'L'),
	gpeALF_CRSN = gpdABCD('C', 'R', 'S', 'N'),
	gpeALF_CRSR = gpdABCD('C', 'R', 'S', 'R'),
	gpeALF_CRSX = gpdABCD('C', 'R', 'S', 'X'),
	gpeALF_CRSY = gpdABCD('C', 'R', 'S', 'Y'),
	gpeALF_CTRL = gpdABCD('C', 'T', 'R', 'L'),
	gpeALF_DEEP = gpdABCD('D', 'E', 'E', 'P'),
	gpeALF_DIME = gpdABCD('D', 'I', 'M', 'E'),
	gpeALF_DIMS = gpdABCD('D', 'I', 'M', 'S'),
	gpeALF_DIVM = gpdABCD('D', 'I', 'V', 'M'),
	gpeALF_DRAW = gpdABCD('D', 'R', 'A', 'W'),
	gpeALF_EDGE = gpdABCD('E', 'D', 'G', 'E'),
	gpeALF_ELSE = gpdABCD('E', 'L', 'S', 'E'),
	gpeALF_ENDD = gpdABCD('E', 'N', 'D', 'D'),
	gpeALF_EQLG = gpdABCD('E', 'Q', 'L', 'G'),
	gpeALF_EXIT = gpdABCD('E', 'X', 'I', 'T'),
	gpeALF_EXPL = gpdABCD('E', 'X', 'P', 'L'),
	gpeALF_EXPM = gpdABCD('E', 'X', 'P', 'M'),
	gpeALF_FERI = gpdABCD('F', 'E', 'R', 'I'),
	gpeALF_FILE = gpdABCD('F', 'I', 'L', 'E'),
	gpeALF_FIND = gpdABCD('F', 'I', 'N', 'D'),
	gpeALF_FSEC = gpdABCD('F', 'S', 'E', 'C'),
	gpeALF_FUNC = gpdABCD('F', 'U', 'N', 'C'),
	gpeALF_GOLD = gpdABCD('G', 'O', 'L', 'D'),

	gpeALF_GPIO = gpdABCD('G', 'P', 'I', 'O'),

	gpeALF_GPUC = gpdABCD('G', 'P', 'U', 'C'),	// gpeALF_GPU + VOXEL VERTEX

	gpeALF_HAIR = gpdABCD('H', 'A', 'I', 'R'),
	gpeALF_HAND = gpdABCD('H', 'A', 'N', 'D'),
	gpeALF_HEAD = gpdABCD('H', 'E', 'A', 'D'),
	gpeALF_HELP = gpdABCD('H', 'E', 'L', 'P'),
	gpeALF_HELO = gpdABCD('H', 'E', 'L', 'O'),
	gpeALF_HOST = gpdABCD('H', 'O', 'S', 'T'),
	gpeALF_HTML = gpdABCD('H', 'T', 'M', 'L'),
	gpeALF_HTTP = gpdABCD('H', 'T', 'T', 'P'),

	gpeALF_IDLE = gpdABCD('I', 'D', 'L', 'E'),
	gpeALF_INIT = gpdABCD('I', 'N', 'I', 'T'),

	gpeALF_ITEM = gpdABCD('I', 'T', 'E', 'M'),
	gpeALF_JOIN = gpdABCD('J', 'O', 'I', 'N'),
	gpeALF_JUMP = gpdABCD('J', 'U', 'M', 'P'),

	gpeALF_KALT = gpdABCD('K', 'A', 'L', 'T'),
	gpeALF_KICK = gpdABCD('K', 'I', 'C', 'K'),

	gpeALF_LIST = gpdABCD('L', 'I', 'S', 'T'),
	gpeALF_LINE = gpdABCD('L', 'I', 'N', 'E'),
	gpeALF_LEFT = gpdABCD('L', 'E', 'F', 'T'),
	gpeALF_LELG = gpdABCD('L', 'E', 'L', 'G'),
	gpeALF_LOOP = gpdABCD('L', 'O', 'O', 'P'),
	gpeALF_LTLG = gpdABCD('L', 'T', 'L', 'G'),
	gpeALF_MAIL = gpdABCD('M', 'A', 'I', 'L'),
	gpeALF_MAIN = gpdABCD('M','A',  'I', 'N'),
	gpeALF_MINI = gpdABCD('M', 'I', 'N', 'I'),

	gpeALF_MONO = gpdABCD('M', 'O', 'N', 'O'),
	gpeALF_MOON = gpdABCD('M', 'O', 'O', 'N'),

	gpeALF_MLBX = gpdABCD('M', 'L', 'B', 'X'),		// egér X poziciója mielött le lett nyomva a BAL gomb
	gpeALF_MLBY = gpdABCD('M', 'L', 'B', 'Y'),		// egér Y poziciója mielött le lett nyomva a BAL gomb

	gpeALF_MRBX = gpdABCD('M', 'R', 'B', 'X'),		// egér X poziciója mielött le lett nyomva a JOBB gomb
	gpeALF_MRBY = gpdABCD('M', 'R', 'B', 'Y'),		// egér Y poziciója mielött le lett nyomva a JOBB gomb

	gpeALF_MULM = gpdABCD('M', 'U', 'L', 'M'),

	gpeALF_NAME = gpdABCD('N', 'A', 'M', 'E'),
	gpeALF_MSEC = gpdABCD('M', 'S', 'E', 'C'),
	gpeALF_NBLD = gpdABCD('N', 'B', 'L', 'D'),
	gpeALF_NEAR = gpdABCD('N', 'E', 'A', 'R'),
	gpeALF_NONS = gpdABCD('N', 'O', 'N', 'S'),
	gpeALF_NUSE = gpdABCD('N', 'U', 'S', 'E'),
	gpeALF_OPER = gpdABCD('O', 'P', 'E', 'R'),
	gpeALF_ORLG = gpdABCD('O', 'R', 'L', 'G'),
	gpeALF_OTOS = gpdABCD('O', 'T', 'O', 'S'),
	gpeALF_OVER = gpdABCD('O', 'V', 'E', 'R'),

	gpeALF_PACK = gpdABCD('P', 'A', 'C', 'K'),
	gpeALF_PARA = gpdABCD('P', 'A', 'R', 'A'),
	gpeALF_PASS = gpdABCD('P', 'A', 'S', 'S'),

	gpeALF_PIXN = gpdABCD('P', 'I', 'X', 'N'),
	gpeALF_POLY = gpdABCD('P', 'O', 'L', 'Y'),
	gpeALF_PORT = gpdABCD('P', 'O', 'R', 'T'),
	gpeALF_PREV = gpdABCD('P', 'R', 'E', 'V'),

	gpeALF_QUAD = gpdABCD('Q', 'U', 'A', 'D'),

	//gpeALF_RESA =	gpdABCD( 'R','E','S','A' ),
	gpeALF_REMM = gpdABCD('R', 'E', 'M', 'M'),

	gpeALF_RINP = gpdABCD('R', 'I', 'N', 'P'),

	gpeALF_ROBI = gpdABCD('R', 'O', 'B', 'I'),
	gpeALF_ROCK = gpdABCD('R', 'O', 'C', 'K'),

	gpeALF_ROUT = gpdABCD('R', 'O', 'U', 'T'),

	gpeALF_SAVE = gpdABCD('S', 'A', 'V', 'E'),
	gpeALF_SLMP = gpdABCD('S', 'L', 'M', 'P'),
	gpeALF_SOCK = gpdABCD('S', 'O', 'C', 'K'),
	gpeALF_STAR = gpdABCD('S', 'T', 'A', 'R'),
	gpeALF_STEP = gpdABCD('S', 'T', 'E', 'P'),
	gpeALF_STON = gpdABCD('S', 'T', 'O', 'N'),
	gpeALF_STOW = gpdABCD('S', 'T', 'O', 'W'),
	gpeALF_SUBM = gpdABCD('S', 'U', 'B', 'M'),
	gpeALF_SQRT = gpdABCD('S', 'Q', 'R', 'T'),
	gpeALF_SYNC = gpdABCD('S', 'Y', 'N', 'C'),

	gpeALF_TOOL = gpdABCD('T', 'O', 'O', 'L'),

	gpeALF_TRGH = gpdABCD('T', 'R', 'G', 'H'),
	gpeALF_TRGW = gpdABCD('T', 'R', 'G', 'W'),

	gpeALF_TRUE = gpdABCD('T', 'R', 'U', 'E'),
	gpeALF_TURN = gpdABCD('T', 'U', 'R', 'N'),

	gpeALF_TYPE = gpdABCD('T', 'Y', 'P', 'E'),

	gpeALF_USER = gpdABCD('U', 'S', 'E', 'R'),

	gpeALF_WALK = gpdABCD('W', 'A', 'L', 'K'),
	gpeALF_WHAM = gpdABCD('W', 'H', 'A', 'M'),

	gpeALF_XORM = gpdABCD('X', 'O', 'R', 'M'),

	gpeALF_4 = gpdABCD('Z', 'Z', 'Z', 'Z'),
	//- 5 -----------------
	gpeALF_AAAAA = gpdABCDE('A', 'A', 'A', 'A', 'A'),
	gpeALF_ANDLG = gpdABCDE('A', 'N', 'D', 'L', 'G'),
	gpeALF_ARRAY = gpdABCDE('A', 'R', 'R', 'A', 'Y'),
	gpeALF_BEGIN = gpdABCDE('B', 'E', 'G', 'I', 'N'),
	gpeALF_BLOCK = gpdABCDE('B', 'L', 'O', 'C', 'K'),
	gpeALF_BOBER = gpdABCDE('B', 'O', 'B', 'E', 'R'),
	gpeALF_BRAKE = gpdABCDE('B', 'R', 'A', 'K', 'E'),
	gpeALF_BRAKS = gpdABCDE('B', 'R', 'A', 'K', 'S'),
	gpeALF_BREAK = gpdABCDE('B', 'R', 'E', 'A', 'K'),
	gpeALF_BUBLE = gpdABCDE('B', 'U', 'B', 'L', 'E'),
	gpeALF_CACHE = gpdABCDE('C', 'A', 'C', 'H', 'E'),
	gpeALF_CLASS = gpdABCDE('C', 'L', 'A', 'S', 'S'),
	gpeALF_COLOR = gpdABCDE('C', 'O', 'L', 'O', 'R'),
	gpeALF_CONST = gpdABCDE('C', 'O', 'N', 'S', 'T'),
	gpeALF_COUNT = gpdABCDE('C', 'O', 'U', 'N', 'T'),
	gpeALF_CREAT = gpdABCDE('C', 'R', 'E', 'A', 'T'),
	gpeALF_CROSS = gpdABCDE('C', 'R', 'O', 'S', 'S'),
	gpeALF_CYCLE = gpdABCDE('C', 'Y', 'C', 'L', 'E'),
	gpeALF_DEBUG = gpdABCDE('D', 'E', 'B', 'U', 'G'),
	gpeALF_DEBUS = gpdABCDE('D', 'E', 'B', 'U', 'S'),
	gpeALF_ENTER = gpdABCDE('E', 'N', 'T', 'E', 'R'),
	gpeALF_ENTRY = gpdABCDE('E', 'N', 'T', 'R', 'Y'),
	gpeALF_ERECT = gpdABCDE('E', 'R', 'E', 'C', 'T'),
	gpeALF_HELLO = gpdABCDE('H', 'E', 'L', 'L', 'O'),
	gpeALF_GETMX = gpdABCDE('G', 'E', 'T', 'M', 'X'),
	gpeALF_GLOBA = gpdABCDE('G', 'L', 'O', 'B', 'A'),
	gpeALF_HISTI = gpdABCDE('H', 'I', 'S', 'T', 'I'),
	gpeALF_HUMAN = gpdABCDE('H', 'U', 'M', 'A', 'N'),
	gpeALF_INDEX = gpdABCDE('I', 'N', 'D', 'E', 'X'),
	gpeALF_KCTRL = gpdABCDE('K', 'C', 'T', 'R', 'L'),
	gpeALF_LABEL = gpdABCDE('L', 'A', 'B', 'E', 'L'),
	gpeALF_LATHE = gpdABCDE('L', 'A', 'T', 'H', 'E'),
	gpeALF_LOCAL = gpdABCDE('L', 'O', 'C', 'A', 'L'),
	gpeALF_NEGLG = gpdABCDE('N', 'E', 'G', 'L', 'G'),
	gpeALF_NEQLG = gpdABCDE('N', 'E', 'Q', 'L', 'G'),
	gpeALF_NGATE = gpdABCDE('N', 'G', 'A', 'T', 'E'),
	gpeALF_NGCON = gpdABCDE('N', 'G', 'C', 'O', 'N'),
	gpeALF_NGDIE = gpdABCDE('N', 'G', 'D', 'I', 'E'),
    gpeALF_NONSE = gpdABCDE('N', 'O', 'N', 'S', 'E'),

	gpeALF_ORBIT = gpdABCDE('O', 'R', 'B', 'I', 'T'),
	gpeALF_PAINT = gpdABCDE('P', 'A', 'I', 'N', 'T'),
	gpeALF_PASTE = gpdABCDE('P', 'A', 'S', 'T', 'E'),
	gpeALF_PICQC = gpdABCDE('P', 'I', 'C', 'Q', 'C'),
	gpeALF_PRINT = gpdABCDE('P', 'R', 'I', 'N', 'T'),
	gpeALF_REPIC = gpdABCDE('R', 'E', 'P', 'I', 'C'),

	gpeALF_RESET = gpdABCDE('R', 'E', 'S', 'E', 'T'),

	gpeALF_RIGHT = gpdABCDE('R', 'I', 'G', 'H', 'T'),
	gpeALF_ROOTM = gpdABCDE('R', 'O', 'O', 'T', 'M'),
	gpeALF_RULES = gpdABCDE('R', 'U', 'L', 'E', 'S'),

	gpeALF_SETUP = gpdABCDE('S', 'E', 'T', 'U', 'P'),
	gpeALF_SHARE = gpdABCDE('S', 'H', 'A', 'R', 'E'),
	gpeALF_SHOES = gpdABCDE('S', 'H', 'O', 'E', 'S'),
	gpeALF_SLEFT = gpdABCDE('S', 'L', 'E', 'F', 'T'),

	gpeALF_SPEED = gpdABCDE('S', 'P', 'E', 'E', 'D'),

	gpeALF_STORE = gpdABCDE('S', 'T', 'O', 'R', 'E'),
	gpeALF_STACK = gpdABCDE('S', 'T', 'A', 'C', 'K'),
	gpeALF_STONE = gpdABCDE('S', 'T', 'O', 'N', 'E'),
	gpeALF_TABWH = gpdABCDE('T', 'A', 'B', 'W', 'H'),
	gpeALF_TABXY = gpdABCDE('T', 'A', 'B', 'X', 'Y'),
	gpeALF_THISA = gpdABCDE('T', 'H', 'I', 'S', 'A'),
	gpeALF_THISN = gpdABCDE('T', 'H', 'I', 'S', 'N'),
	gpeALF_TOKEN = gpdABCDE('T', 'O', 'K', 'E', 'N'),
	gpeALF_TOUCH = gpdABCDE('T', 'O', 'U', 'C', 'H'),
	gpeALF_TRACK = gpdABCDE('T', 'R', 'A', 'C', 'K'),
	gpeALF_VARIA = gpdABCDE('V', 'A', 'R', 'I', 'A'),

	gpeALF_UNDEF = gpdABCDE('U', 'N', 'D', 'E', 'F'),

	gpeALF_UNSEL = gpdABCDE('U', 'N', 'S', 'E', 'L'),

	gpeALF_WHILE = gpdABCDE('W', 'H', 'I', 'L', 'E'),

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
	gpeALF_ELAPSE = gpdABCDEF('E', 'L', 'A', 'P', 'S', 'E'),
	gpeALF_FERIKE = gpdABCDEF('F', 'E', 'R', 'I', 'K', 'E'),
	gpeALF_FLINCH = gpdABCDEF('F', 'L', 'I', 'N', 'C', 'H'),
	gpeALF_FORBID = gpdABCDEF('F', 'O', 'R', 'B', 'I', 'D'),
	gpeALF_FORMAT = gpdABCDEF('F', 'O', 'R', 'M', 'A', 'T'),
	gpeALF_GALAXY = gpdABCDEF('G', 'A', 'L', 'A', 'X', 'Y'),
	gpeALF_JACKET = gpdABCDEF('J', 'A', 'C', 'K', 'E', 'T'),
	gpeALF_KSHIFT = gpdABCDEF('K', 'S', 'H', 'I', 'F', 'T'),
	gpeALF_LISTEN = gpdABCDEF('L', 'I', 'S', 'T', 'E', 'N'),

	gpeALF_NEWROW = gpdABCDEF('N', 'E', 'W', 'R', 'O', 'W'),
    gpeALF_NONSEN = gpdABCDEF('N', 'O', 'N', 'S', 'E', 'N'),

	gpeALF_MODBUS = gpdABCDEF('M', 'O', 'D', 'B', 'U', 'S'),
	gpeALF_NBUILD = gpdABCDEF('N', 'B', 'U', 'I', 'L', 'D'),
	gpeALF_PICCPY = gpdABCDEF('P', 'I', 'C', 'C', 'P', 'Y'),
	gpeALF_PLANET = gpdABCDEF('P', 'L', 'A', 'N', 'E', 'T'),
	gpeALF_POLYER = gpdABCDEF('P', 'O', 'L', 'Y', 'E', 'R'),
	gpeALF_PRAGMA = gpdABCDEF('P', 'R', 'A', 'G', 'M', 'A'),
	gpeALF_REGGIO = gpdABCDEF('R', 'E', 'G', 'G', 'I', 'O'),
	gpeALF_RENAME = gpdABCDEF('R', 'E', 'N', 'A', 'M', 'E'),
	gpeALF_RETURN = gpdABCDEF('R', 'E', 'T', 'U', 'R', 'N'),
	//gpeALF_SHADER =	gpdABCDEF( 'S','H','A','D','E','R' ),

	gpeALF_SIZEOF = gpdABCDEF('S', 'I', 'Z', 'E', 'O', 'F'),
	gpeALF_SHADOW = gpdABCDEF('S', 'H', 'A', 'D', 'O', 'W'),
	gpeALF_SPRITE = gpdABCDEF('S', 'P', 'R', 'I', 'T', 'E'),
	gpeALF_SRIGHT = gpdABCDEF('S', 'R', 'I', 'G', 'H', 'T'),
	gpeALF_STEREO = gpdABCDEF('S', 'T', 'E', 'R', 'E', 'O'),
	gpeALF_STOREE = gpdABCDEF('S', 'T', 'O', 'R', 'E', 'E'),
	gpeALF_STOWUP = gpdABCDEF('S', 'T', 'O', 'W', 'U', 'P'),
	gpeALF_STRROW = gpdABCDEF('S', 'T', 'R', 'R', 'O', 'W'),
	gpeALF_SWITCH = gpdABCDEF('S', 'W', 'I', 'T', 'C', 'H'),
	gpeALF_TARGET = gpdABCDEF('T', 'A', 'R', 'G', 'E', 'T'),
	gpeALF_TELNET = gpdABCDEF('T', 'E', 'L', 'N', 'E', 'T'),

	gpeALF_6 = gpdABCDEF('Z', 'Z', 'Z', 'Z', 'Z', 'Z'),

	//- 7 -----------------

	gpeALF_AAAAAAA = gpdABCDEFG('A', 'A', 'A', 'A', 'A', 'A', 'A'),
	gpeALF_ACCOUNT = gpdABCDEFG('A', 'C', 'C', 'O', 'U', 'N', 'T'),
	gpeALF_CAMLIST = gpdABCDEFG('C', 'A', 'M', 'L', 'I', 'S', 'T'),
	gpeALF_CONNECT = gpdABCDEFG('C', 'O', 'N', 'N', 'E', 'C', 'T'),
	gpeALF_COMMAND = gpdABCDEFG('C', 'O', 'M', 'M', 'A', 'N', 'D'),


	gpeALF_FAVICON = gpdABCDEFG('F', 'A', 'V', 'I', 'C', 'O', 'N'),

	gpeALF_GIOFILE = gpdABCDEFG('G', 'I', 'O', 'F', 'I', 'L', 'E'),
	gpeALF_GRIPPER = gpdABCDEFG('G', 'R', 'I', 'P', 'P', 'E', 'R'),
	gpeALF_NOBUILD = gpdABCDEFG('N', 'O', 'B', 'U', 'I', 'L', 'D'),
	gpeALF_NONSENS = gpdABCDEFG('N', 'O', 'N', 'S', 'E', 'N', 'S'),
	gpeALF_NRENDER = gpdABCDEFG('N', 'R', 'E', 'N', 'D', 'E', 'R'),
	// 0xffffFFFF                A    B    C    D    E    F    G
	gpeALF_MWLQKWU = gpdABCDEFG('M', 'W', 'L', 'Q', 'K', 'W', 'U'),
	gpeALF_U4      = gpeALF_MWLQKWU,

	gpeALF_PICCPYE = gpdABCDEFG('P', 'I', 'C', 'C', 'P', 'Y', 'E'),
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

	gpeALF_CONNECTION 	= gpdABCDEFGHIJ('C','O','N','N','E','C','T','I','O','N'),


	gpeALF_10 = gpdABCDEFGHIJ('Z','Z','Z','Z','Z','Z','Z','Z','Z','Z'),
	gpeALF_resEND = gpeALF_10,
	//- 10 -----------------

	gpeALF_CRPXNLSKVLJFHG = 0x7fffffffffffffff,
} GPT_ALFA;

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
