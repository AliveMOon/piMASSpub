#ifndef GPCSRC_H
#define GPCSRC_H
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
#include "gpcgt.h"
#include <gpcOPCD.h>

//#include "gpeTYP.h"

class gpcRES;
class gpcCRS;
class gpcWIN;
class gpCORE;
//#include "gpcres.h"

//#include "gpccrs.h"



#define gpdSRC_COLw 4
#define gpdSRC_ROWw 1
#define gpdPRGsep " \t\r\n\a .,:;!? =<> -+*/%^ &~|@#$ \\ \" \' ()[]{} "
#define gpdDBG 1
//extern U1 gpaALFsub[]; //0x100];
//extern U1 gpsSTRpub[]; //[0x10000];



enum gpeMASSsw:U8 {
	gpeMASSzero,
	gpeMASSsub = gpeMASSzero,
	gpeMASSret,
	gpeMASSentr,
	gpeMASSunsel,
	gpeMASSinp,
	gpeMASSpass,
	gpeMASSoff,
	gpeMASSmain,
	gpeMASSzn,
	gpeMASSloop,
	gpeMASSdebug,

	gpeMASSalert,
	gpeMASSprg,
	gpeMASSgpu,
	gpeMASScrsL,
	gpeMASScrsR,


	gpeMASSsubMSK = 1<<gpeMASSsub,
	gpeMASSretMSK = 1<<gpeMASSret,
	gpeMASSentrMSK = 1<<gpeMASSentr,
	gpeMASSunselMSK = 1<<gpeMASSunsel,
	gpeMASSinpMSK = 1<<gpeMASSinp,
	gpeMASSpassMSK = 1<<gpeMASSpass,
	gpeMASSoffMSK = 1<<gpeMASSoff,
	gpeMASSmainMSK = 1<<gpeMASSmain,
	gpeMASSznMSK = 1<<gpeMASSzn,
	gpeMASSloopMSK = 1<<gpeMASSloop,
	gpeMASSdebugMSK = 1<<gpeMASSdebug,

	gpeMASSclrMSK = (1<<gpeMASSalert)-1,
	gpeMASSalertMSK,
	gpeMASSprgMSK = 1<<gpeMASSprg,
	gpeMASSgpuMSK = 1<<gpeMASSgpu,
	gpeMASScrslMSK = 1<<gpeMASScrsL,
	gpeMASScrsrMSK = 1<<gpeMASScrsR,


};

static const gpcOPCD gpaOPCi[] = {
	// this,	pS,			a, m, i, nDT,				d,		wip, typ;
	{ gpaOPCi,	"false", 	0, 0, 0, sizeof(U1), 		0.0, gpeALF_null },
	{ gpaOPCi,	"true", 	0, 0, 0, sizeof(U1), 		0.0, gpeALF_TRUE },

	{ gpaOPCi,	"U1", 		0, 0, 0, sizeof(U1), 		0.0, gpeALF_CLASS, gpeALF_DEF },
	{ gpaOPCi,	"U2", 		0, 0, 0, sizeof(U2), 		0.0, gpeALF_CLASS, gpeALF_DEF },
	{ gpaOPCi,	"U4", 		0, 0, 0, sizeof(U4), 		0.0, gpeALF_CLASS, gpeALF_DEF },
	{ gpaOPCi,	"U8", 		0, 0, 0, sizeof(U8), 		0.0, gpeALF_CLASS, gpeALF_DEF },
	{ gpaOPCi,	"I1", 		0, 0, 0, sizeof(I1), 		0.0, gpeALF_CLASS, gpeALF_DEF },
	{ gpaOPCi,	"I2", 		0, 0, 0, sizeof(I2), 		0.0, gpeALF_CLASS, gpeALF_DEF },
	{ gpaOPCi,	"I4", 		0, 0, 0, sizeof(I4), 		0.0, gpeALF_CLASS, gpeALF_DEF },
	{ gpaOPCi,	"I8", 		0, 0, 0, sizeof(I8), 		0.0, gpeALF_CLASS, gpeALF_DEF },
	{ gpaOPCi,	"F4", 		0, 0, 0, sizeof(float),		0.0, gpeALF_CLASS, gpeALF_DEF },
	{ gpaOPCi,	"F8", 		0, 0, 0, sizeof(double),	0.0, gpeALF_CLASS, gpeALF_DEF },

	{ gpaOPCi,	"sizeof(", 	0, 0, 0, 0,					0.0, gpeALF_FUNC, gpeALF_SIZEOF },
	{ gpaOPCi,	"if(", 		0, 0, 0, 0,					0.0, gpeALF_FUNC, gpeALF_if },
	{ gpaOPCi,	"for(", 	0, 0, 0, 0,					0.0, gpeALF_CYCLE, gpeALF_FOR },
	{ gpaOPCi,	"while(", 	0, 0, 0, 0,					0.0, gpeALF_CYCLE, gpeALF_WHILE },
	{ gpaOPCi,	"switch(",	0, 0, 0, 0,					0.0, gpeALF_FUNC, gpeALF_SWITCH },

	{ gpaOPCi,	"break",	0, 0, 0, 0,					0.0, gpeALF_SYS, gpeALF_BREAK },
	{ gpaOPCi,	"continue",	0, 0, 0, 0,					0.0, gpeALF_SYS, gpeALF_CONTINUE },
	{ gpaOPCi,	"return",	0, 0, 0, 0,					0.0, gpeALF_SYS, gpeALF_RETURN },
	{ gpaOPCi,	"discard",	0, 0, 0, 0,					0.0, gpeALF_SYS, gpeALF_BREAK },

	{ gpaOPCi,	"class",	0, 0, 0, 0,					0.0, gpeALF_OPER, gpeALF_CLASS },
	{ gpaOPCi,	"pub",		0, 0, 0, 0,					0.0, gpeALF_CLASS, gpeALF_CTRL },
	{ gpaOPCi,	"prot",		0, 0, 0, 0,					0.0, gpeALF_CLASS, gpeALF_CTRL },

	{ gpaOPCi,	"new",		0, 0, 0, 0,					0.0, gpeALF_OPER, gpeALF_NEW },
	{ gpaOPCi,	"del",		0, 0, 0, 0,					0.0, gpeALF_OPER, gpeALF_DEL },

	{ gpaOPCi,	"main",		0, 0, 0, 0,					0.0, gpeALF_PRG, gpeALF_MAIN },



};
static const char* gpasOP[] = {
	"0 nop",

	"! notLG",	"!! LG",	"~ inv",

	", stk",	"; newrow",

	"/* comS", 	"*/ comE",	"// com",


	"& and", 	"&& andLG",	"&= andM",
	"* mul", 	"** exp",	"*= mulM",
							"**= expM",
	"/ div", 	"/= divM",	"//= rootM",
	"% rem", 	"%= remM",
	"^ xor",	"^= xorM",


	"= mov", 	"== eqLG",  "!= neqLG",
	"| or", 	"|| orLG",	"|= orM",
	"+ add", 	"++ inc",	"+= addM",	/// -------------- GOOD
	"- sub", 	"-- dec",	"-= subM",


	"<= leLG",	"< ltLG", 	"<< sl", 	"<<= slM",
	">= beLG",	"> bgLG", 	">> sr", 	">>= srM",

	". dot", 	"-> entry", ":: out",
	"( brakS",	") brakE",
	"[ dimS", 	"] dimE",
	"{ begin", 	"} end",
	"? if",		": else",
	"@ mail",	"\" str",
};


inline U4 gpfUTF8( const U1* pS, U1** ppS ) {
	U4 utf8 = *pS;
	if( !(utf8&0x80) )
	{
		if( !ppS )
			return utf8;

		*ppS = ((U1*)pS) + (!!utf8);
		return utf8;
	}

	if( (utf8&0xc0) == 0x80 )
	{
		// azaz hibás 0x40-nek igaznak kéne lenie
		while( (utf8&0xc0) == 0x80 )
		{
			pS++;
			utf8 = *pS;
			if( !utf8 )
			{
				if( !ppS )
					return 0;

				*ppS = (U1*)pS;
				return 0;
			}
		}

		if( !ppS )
			return '.';

		*ppS = (U1*)pS;
		return '.';
	}

	U4	u0 = utf8<<1, n = 0;
	U4	static const aADD[] = { 0, 		0x80,	0x800,	0x10000, 0x200000 };
	U4	static const aMSK[] = { 0x7F, 	0x7FF,	0xFFFF, 0x1FFFFF };

	while( u0&0x80 )
	{
		n++;
		u0 <<= 1;
		utf8 <<= 6;
		utf8 |= pS[n]&0x3f;
	}
	utf8 += aADD[n];
	utf8 &= aMSK[n];
	if( !ppS )
		return utf8;
	pS += n+1;
	*ppS = (U1*)pS;
	return utf8;
}

inline U8 gpfVAN( const U1* pU, const U1* pVAN, U8& nLEN, bool bDBG = false ) {
	// return nBYTE
	// nLEN = nCODE
	nLEN = 0;
	if( pU ? !*pU : true )
		return 0;

	U1 *pS = (U1*)pU;
	if( !pVAN )
	{
		// gpmSTRLEN!!
		while( *pS )
		{
			if( (*pS&0xc0) != 0x80 )
				nLEN++;	// csak a 0x80 asokat nem számoljuk bele mert azok tötike karakterek
			pS++;
		}
		return pS-pU;
	}
	U8 nVAN = gpfVAN( pVAN, NULL, nLEN ); // utf8len
	bool abVAN[0x80];
	gpmZ( abVAN );
	*abVAN = true;
	gpcLZY* pTREE = NULL;
	U4 u4, nT = 0;

	if( nVAN == nLEN )
	{
		//van-ban nincsen UTF8 akkor turbó
		abVAN[pVAN[0]] = true;
		for( U8 v = 1; v < nLEN; v++ )
		{
			abVAN[pVAN[v]] = true;
		}

		while( *pS )
		{
			if( *pS >= 0x80 )
				break;
			if( abVAN[*pS] )
				return nLEN = pS-pU;

			pS++;
		}

		nLEN = pS-pU;
		if( !*pS )
			return nLEN;

		if( bDBG )
			nLEN = pS-pU;

	} else {
		U1	*pV = (U1*)pVAN,
			*pE = pV+nVAN;
		while( pV < pE )
		{
			u4 = gpfUTF8( pV, &pV );
			if( u4 < 0x80 )
			{
				abVAN[u4] = true;
				continue;
			}

			pTREE = pTREE->tree_add( u4, nT );
		}

		nLEN = 0;
	}
	U1* pSS;
	while( *pS )
	{
		u4 = gpfUTF8( pS, &pSS );
		if( u4 < 0x80 )
		{
			if( abVAN[u4] )
				break;
			pS++;
			nLEN++;
			continue;
		}

		if( pTREE->tree_fnd(u4, nT) < nT )
			break;

		pS = pSS;
		nLEN++;
	}

	return pS-pU;
}

inline U4x2 lenMILL( U4x2 pos, U4x4&crn, U1* pUi, U1* pUie ) {
	for( ; pUi < pUie; pUi++ )
	{
		if( *pUi&0x80 )
		{
			if( !(*pUi&0x40) )
				continue;
			pos.x++;
		}
		else switch( *pUi )
		{
			case '\r':
				pos.x = 0;
				break;
			case '\n':
				pos.x = 0;
				pos.y++;
				break;
			case '\a':
				pos.x = 0;
				pos.y++;
				break;
			case '\t':
				pos.x = ((pos.x/4)+1)*4;
				break;
			default:
				pos.x++;
				break;
		}
		crn.a4x2[0].mx( pos );
		crn.z++;
	}
	return pos;
}
inline U8 gpfUTFlen( U1* pU, U1* pUe, U4& col, U4& row, U1* pVAN = NULL ) {
	U8 nLEN = 0, nBYTE, nUTF8 = 0;
	if( !pVAN )
		pVAN = (U1*)"\t\r\n\a";
	col = row = 0;
	U1	*pS,
		aCHR[] = " ";
	U4 xx = 0, n;
    while( *pU ? pU < pUe : false )
    {
		nBYTE = gpfVAN( pU, pVAN, nLEN );
		nUTF8 += nLEN;
		xx += nLEN;
		if( col < xx )
			col = xx;

		pS = pU+nBYTE;
		aCHR[0] = *pS;
		n = gpmNINCS( pS, aCHR );
		switch( *pS )
		{
			case '\r':
				if( pS[1] != '\n' )
				{
					xx = 0;
					break;
				}
				n++;
				xx = 0;
				row++;
				break;
			case '\n':
				xx = 0;
				row += n;
				break;
			case '\a':
				row += n;
				xx = 0;
				break;
			case '\t':
				xx = (xx/4 + n)*4;
				if( col >= xx )
					break;

				col = xx;
				break;
			case ' ':
				xx++;
				break;
		}
		nUTF8 += n;
		pU = pS+n;

    }
	col++;
	row++;
	return nUTF8;
}
inline U8 gpfVANn( U1* pS, const U1* pVAN ) {
	if( !pVAN )
		return 0;
	U2 nV = strlen( (char*)pVAN );
	if( !nV )
		return 0;

	U4 n = 0, nVAN[0x80];
	gpmZ( nVAN );

	while( *pS )
	{
		if( *pS < 0x80 )
		{
			nVAN[*pS]++;
			pS++;
			continue;
		}

		(*nVAN)++;
		pS++;
	}

	for( U8 i = 0; i < nV; i++  )
	{
		if( pVAN[i] < 0x80 )
			n += nVAN[pVAN[i]];
	}

	return n;
}
U8 inline gpfABC( U1* p_str, U1* pE, U8& nLEN );
inline U1* gpfUTF8left( U1* pB, U1* pS, U1 n, U1 b ) {
	if( pB >= pS )
		return pB;

	if( b )
	{
		while( pS > pB )
		{
			if( pS[-1] == n || pS[-1] == b  )
				return pS;
			pS--;
		}

		return pB;
	}

	while( pS > pB )
	{
		if( pS[-1] == n )
			return pS;
		pS--;
	}

	return pB;
}
inline U1* gpfUTF8stpX( U1* pB, U1* pE, U4 x, U4 nT = 4 ) {
	if(!x)
		return pB;

	U4 xUP = 0;
	while( pB < pE )
	{
		if( xUP >= x )
			return pB;
		if( (pB[0]&0x80) )
		{
			xUP++;
			pB += 2;
			continue;
		}

		if( pB[0] == '\t' )
			xUP = ((xUP/nT)+1)*nT;
		else
			xUP++;

		pB++;
	}
	return pB;
}
inline U4 gpfUTF8rig( U1* pB, U1* pE, U4 nT = 4 ) {
	U4 r = 0;
	while( pB < pE )
	{
		if( (pB[0]&0x80) )
		{
			r++;
			pB += 2;
			continue;
		}

		if( pB[0] == '\t' )
			r = ((r/nT)+1)*nT;
		else
			r++;

		pB++;
	}
	return r;
}
U8 inline gpfSTR2U8( void* pV, void* ppV = NULL ) {
	if( !pV )
		return 0;
	char *p_str = (char*)pV;

	U8 u8 = strtol( p_str, &p_str, 10 );
	if( !u8 )
	{
		switch( *(U1*)p_str )
		{
			case 'x':
			case 'X':
				p_str++;
				u8 = strtoll( p_str, &p_str, 16 );
				break;
			case 'b':
			case 'B':
				p_str++;
				u8 = strtol( p_str, &p_str, 2 );
				break;
			case 'd':
			case 'D':
				p_str++;
				u8 = strtol( p_str, &p_str, 10 );
				break;
		}
	}
	if( ppV )
		*(char**)ppV = p_str;
	return u8;
}
I8 inline gpfSTR2I8( void* pV, void* ppV = NULL ) {
	if( !pV )
		return 0;

	char *p_str = (char*)pV;
	U8 nLEN;
    p_str += gpmVAN(p_str, "+-0123456789xXbBdD", nLEN );
	I8 i8 = strtol( p_str, &p_str, 10 );
	if( !i8 )
	{
		switch( *(U1*)p_str )
		{
			case 'x':
			case 'X':
				p_str++;
				i8 = strtol( p_str, &p_str, 16 );
				break;
			case 'b':
			case 'B':
				p_str++;
				i8 = strtol( p_str, &p_str, 2 );
				break;
			case 'd':
			case 'D':
				p_str++;
				i8 = strtol( p_str, &p_str, 10 );
				break;
		}
	}
	if( ppV )
		*(char**)ppV = p_str;
	return i8;
}

class gpcLAY {
public:
	gpeALF	id;		// 0
	gpeNET4	typ;	// 8
	void	*pVOID;	// 12
	gpcLAY() { gpmCLR; }

	void* pRE( U4 nZ, U4 nN, U4 oz, U4 oy )
	{
		if( !this )
			return NULL;

		void* pKILL = pVOID;
		nZ *= *(U1*)&typ;
		oz *= *(U1*)&typ;

		pVOID = (void*)(new U1[nZ*nN]);
		gpmZnOF( (U1*)pVOID, nZ*nN );

		for( U1* pS = (U1*)pKILL, *pD = (U1*)pVOID, *pSe = pS+oz*oy ; pS < pSe; pS += oz, pD += nZ  )
		{
			gpmMcpyOF( pD, pS, oz );
		}

		gpmDELary( pKILL );
		return pVOID;
	}

};
class gpcMAP {
public:
	U4x4	mapZN44;	// xy load zw alloc
	U4		*pMAP, *pCOL, *pROW, nLAY;
	gpcLAY	*pLAY;

	gpcMAP(void)
	{
		gpmCLR;
	};
	~gpcMAP()
	{
		gpmDELary(pMAP);
	}
	U4* MAPalloc( const U4x2& spcZN, U4x4& outZN, U4 iRST = 4 ) {
		// mCR -
		if(!this)
		{
			outZN = 0;
			return NULL;
		}

		if( mapZN44.x <= spcZN.x )
			mapZN44.x = spcZN.x+1;
		if( mapZN44.y <= spcZN.y )
			mapZN44.y = spcZN.y+1;

		outZN = mapZN44;
		if(	(mapZN44.z > mapZN44.x) && (mapZN44.w > mapZN44.y) )
		{
			if( iRST < 0x4 )
			{
				U4 i = mapZN44.a4x2[1].sum()*iRST,
					*pC = pCOL+i,
					*pR = pROW+i;

				*pC = gpdSRC_COLw;
				*pR = 1;

				gpfMset( (pC+1), mapZN44.z-1, pC, sizeof(*pC) );
				gpfMset( (pR+1), mapZN44.w-1, pR, sizeof(*pR) );

				//gpfMset( pCOL+mapZN44.a4x2[1].sum(), 3, pCOL, mapZN44.a4x2[1].sum() );
			}
			return pMAP;
		}

		U4	*pK = pMAP,
			*pKC = pK ? pK+outZN.a4x2[1].area() : NULL, //AREAzw(),
			*pKR = pK ? pKC+outZN.z : NULL;

		mapZN44.z = max( mapZN44.z, gpmPAD( mapZN44.x, 0x10 ) );
		mapZN44.w = max( mapZN44.w, gpmPAD( mapZN44.y, 0x10 ) );

		U4 	nARE = mapZN44.a4x2[1].area(),
			nALL = nARE + mapZN44.a4x2[1].sum()*4;

		pMAP = new U4[nALL];
		pROW = (
					pCOL = pMAP+nARE
				) + mapZN44.z;

		*pCOL = gpdSRC_COLw;
		*pROW = gpdSRC_ROWw;
		*pMAP = 0;

		//gpmZnOF( pMAP, nARE );
		gpfMset( (pMAP+1), nARE-1, pMAP, sizeof(*pMAP) );
		gpfMset( (pCOL+1), mapZN44.z-1, pCOL, sizeof(*pCOL) );
		gpfMset( (pROW+1), mapZN44.w-1, pROW, sizeof(*pROW) );

		gpfMset( pCOL+mapZN44.a4x2[1].sum(), 3, pCOL, mapZN44.a4x2[1].sum() );

		if( pK )
		{
			if( iRST > 3 )
			for( U4 i = 0, d = mapZN44.a4x2[1].sum(),s = outZN.a4x2[1].sum(); i < 4; i++ )
			{
				gpmMcpyOF( (pCOL + i*d), (pKC + i*s), outZN.z );
				gpmMcpyOF( (pROW + i*d), (pKR + i*s), outZN.w );

			}

			for( U4* pS = pK, *pD = pMAP; pS < pKC; pS += outZN.z, pD += mapZN44.z  )
			{
				gpmMcpyOF( pD, pS, outZN.z );
			}
			gpmDELary(pK);
		}
		for( U4 i = 0; i < nLAY; i++ )
		{
			pLAY[i].pRE( mapZN44.z, mapZN44.w, outZN.z, outZN.y );
		}
		outZN = mapZN44;
		return pMAP;
	}
};

#define iMNindt au4x4[0]
#define iMNi au4x4[0].x
#define iMNn au4x4[0].y
#define iMNdct au4x4[0].z
#define iMNtyp au4x4[0].w

#define rMNpos ai4x4[1].a4x2[0]
#define rMNclr au4x4[1].z
//#define rMNlnk au4x4[1].w
static char gpsNoWord[] = {
							"\\ \t\a\r\n*&/%+-|~^?!=$.,:;\'\"{}[]()"
						};
static char gpsOPERA[] = {
							"\\*&/%+-|~^?!=$.,:;{}[]()"
						};

#define gpmSCP aSCOOP[iSCP]

class gpcSCOOP {
	gpcLZY 		lzyMiN;		I8x4 rMN;	U4 nMiN;
public:
	gpcLZYdct	lzyDCT;					U4 nDCT;
	gpcLZY		lzyLiNK; 	U4x4 rLNK;	U4 nLiNK;
	gpcLZY		lzyCNST;				U4 nCNST;
	gpcLZY		lzyASM;		I4x4 rINS;	U4 nINS;

	U1	*p_str;
	gpcSCOOP(){ gpmCLR; };
	void rst( U1* pUTF )
	{
		lzyMiN.lzyRST();

		lzyDCT.rst();
		lzyLiNK.lzyRST();
		lzyCNST.lzyRST();
		lzyASM.lzyRST();

		nDCT = nLiNK = nCNST = nMiN = nINS = 0;
		p_str = pUTF;	// ezt a gpcSRC::srcBRK adja
	}
	//I8x4* pMN();

	U4 nMN() {
		/*if(nMiN)
			return nMiN;*/

		return nMiN = lzyMiN.nLD(sizeof(rMN));
	}
	I8x4* pMN() {
		if( nMiN )
			return (I8x4*)lzyMiN.p_alloc;

		return nMN() ? (I8x4*)lzyMiN.p_alloc : NULL;
	}


	U4 nLNK() {
		if( nLiNK )
			return nLiNK;

		return nLiNK = lzyLiNK.nLD(sizeof(rLNK));
	}
	U4x4* pLNK() {
		if( nLiNK )
			return (U4x4*)lzyLiNK.p_alloc;

		return nLNK() ? (U4x4*)lzyLiNK.p_alloc : NULL;
	}

	U4 nASM() {
		/*if( nINS )
			return nINS;*/

		return 	nINS = lzyASM.nLD(sizeof(rINS));
	}

	U4 DCTadd( U4x2 pos, U1* pUi, U8 nU, U4 color, U4 typ = 0xff ) {
		if( !this )
			return 0;

		if( !nU )
			return nMN();

 		U8 nS = nU, nI;
 		U4 iDCT = lzyDCT.dctMILLfndnS( pUi, nS, nDCT );
 		if( iDCT < nDCT )
		if( (nI=lzyDCT.nSTRix(iDCT)) != nS )
			iDCT = nDCT;

		if( iDCT >= nDCT )
		{
			// nem volt a SZóTáRBAN
			iDCT = nDCT; // a végére kerül ha hozzá adjuk
			lzyDCT.dctMILLadd( pUi, nU );
			nDCT++;
		}
		// typ U4x4.w:
		/// x[7s,6f,5r,4str : 3-0 nBYTE = 1<<(x&0xf) ]
		/// yz[ dimXY ] 	, w nBYTE //= 1<<(x&0xf)
		rLNK = U4x4(nMN());

		rMN.iMNindt = U4x4( pUi-p_str, nU, iDCT, typ );
		rMN.rMNpos = pos;
		rMN.rMNclr = color;
		lzyMiN.lzyADD( &rMN, sizeof(rMN), nU = -1, -1 );

		lzyLiNK.lzyADD( &rLNK, sizeof(rLNK), nU = -1, -1 );
		return nMN();
	}
	U4 STRadd( U4x2 pos, U1* pUi, U8 nU, U4 color ) {
		if( !nU )
			return 0;

		if( !nU )
			return nMN();

		// typ U4x4.w:
		/// x[7s,6f,5r,4str : 3-0 nBYTE = 1<<(x&0xf) ]
		/// yz[ dimXY ] 	, w nBYTE //= 1<<(x&0xf)
		//U1x4 typ(0x10,1,1,0);
		rMN.iMNindt = U4x4( pUi-p_str, nU, -1, gpeTYP_STR ); //typ.typ().u4 );
		rMN.rMNpos = pos;
		rMN.rMNclr = color;
		lzyMiN.lzyADD( &rMN, sizeof(rMN), nU = -1, -1 );
		return nMN();
	}
	U4 NOTEadd( U4x2 pos, U1* pUi, U8 nU, U4 color ) {
		if( !nU )
			return 0;

		if( !nU )
			return nMN();

		// typ U4x4.w:
		/// x[7s,6f,5r,4str : 3-0 nBYTE = 1<<(x&0xf) ]
		/// yz[ dimXY ] 	, w nBYTE //= 1<<(x&0xf)
		//U1x4 typ(0x10,1,1,0);
		rMN.iMNindt = U4x4( pUi-p_str, nU, -1, gpeTYP_STR ); //typ.typ().u4 );
		rMN.rMNpos = pos;
		rMN.rMNclr = color;
		lzyMiN.lzyADD( &rMN, sizeof(rMN), nU = -1, -1 );
		return nMN();
	}
};

class gpCLASS {
public:
	gpcLZY lzyOBJ;
	gpCLASS(){ gpmCLR; };
};

class gpOBJ {
public:
	I4x2 d2D;
	I4	oID, cID, sOF, iPC;
	gpOBJ(){ gpmCLR; };
};

class gpROW{
public:
	I4	iBLK,	oID,
		iMN,	cID,
		iPC,	sOF;

	gpeOPid preOP, pstOP;
	gpROW(){};

};

class gpBLOCK {
	I4 nR, iI;
public:
	gpcLZY	lzyROW;
	gpeOPid opID;

	I4 iIN() { return iI; }
	I4 setIN( I4 i )
	{
		if( iI >= i )
			return iI;

		if( i > nR )
			i = nR;

		iI = i;
		return iI;
	}

	I4 nROW() {
		if( !this )
			return 0;
		if( nR )
			return nR;
		return nR = lzyROW.nLD(sizeof(gpROW));
	}
	I4 iLAST() { return nROW()-1; }
	gpROW* pLAST() { return pROW(iLAST()); }
	gpROW* pROW( I4 r, bool bADD = false ) {
		if( r < 0 )
			return NULL;

		if( nROW() <= r )
		{
			if( bADD )
				return pROWadd( r );
			return NULL;
		}

		return ((gpROW*)gpmLZYvali( gpROW, &lzyROW )) + r;
	}

	gpROW* pROWadd( I4 iR ) {
		gpROW* pR = (gpROW*)lzyROW.Ux( iR, sizeof(*pR) );
		if( !pR )
			return NULL;
		nR = 0;
		return pR;
	}
	gpBLOCK(){ gpmCLR; };
};
class gpcLZYblk {
	I4		nB;
public:
	gpcLZY	lzyBLK;

	gpcLZYblk(){ gpmCLR; }
	~gpcLZYblk()
	{
		gpBLOCK** ppB0 = gpmLZYvali( gpBLOCK*, &lzyBLK );
		if( ppB0 )
		for( I4 nBLK = lzyBLK.nLD(sizeof(*ppB0)), b = 0; b < nBLK; b++ )
			gpmDEL( ppB0[b] );
	}
	I4 nBLK() {
		if( !this )
			return 0;
		if( nB )
			return nB;
		return nB = lzyBLK.nLD(sizeof(gpBLOCK*));
	}
	gpBLOCK* pBLOCK(){
		gpBLOCK** ppB = (gpBLOCK**)lzyBLK.Ux( nBLK(), sizeof(*ppB) );
		if( !(*ppB) )
		{
			return (*ppB) = new gpBLOCK;
		}
		return (*ppB);
	}
};

class gpMEM {
public:
	gpcLZY	lzyMEM,
			lzyOBJ,
			lzyCLASS;
	I4		iSTK,nDAT,nINST;
	gpMEM( I4 i = 0x2000 ){ gpmCLR; iSTK = nDAT = i; }
	U1* iPC( U4 iPC, U4 n )
	{
		if( !this )
			return NULL;
		return lzyMEM.Ux( iPC, n, true, 1 );
	}
};

class gpcSRC {
public:
    U1  	*pA, *pB;			// pA - alloc *pB - tartalom
    U8		nLD,
			nA,			// ha nA == 0 nm mi foglaltuk
			bSW;		// pB = pA+iB()
    U4x4	spcZN, dim;
    U4		IX,		retIX, nALFtg, strtD,
			endD,	picID, bobID,
			msBLD, msBLTdly;
    SOCKET	iGT;

    gpeALF	*pALFtg;
    gpcRES	*pEXE0,
			*apOUT[4];
	gpcLZY	*pMINI, *pDBG,
			*pABI;

	gpcMAP	*pMAP;

	gpcSCOOP 	aSCOOP[3];
	gpCORE		*pCORE;

	gpcLZYblk	lzyBLOCK;
	gpMEM		*pMEM;

	gpOBJ* srcOBJfnd( I4 id )
	{
		if( !pMEM )
			return NULL;
		gpOBJ* pO0 = gpmLZYvali( gpOBJ, &pMEM->lzyOBJ );
		if( !pO0 )
			return NULL;
        for( U4 nO = pMEM->lzyOBJ.nLD(sizeof(gpOBJ)), o = 0; o < nO; o++ )
        {
			if( pO0[o].oID != id )
				continue;
			return pO0+o;
        }
        return NULL;
	}
	gpOBJ* srcOBJadd( I4 id )
	{
		if( !pMEM )
			pMEM = new gpMEM;
		U4 nO = pMEM->lzyOBJ.nLD(sizeof(gpOBJ));
		gpOBJ* pO = (gpOBJ*)pMEM->lzyOBJ.Ux( nO, sizeof(*pO) );
		pO->oID = id;
		return pO;
	}
	U1* srcMEMiPC( I4 iPC, U4 nU1 )
	{
		if( !pMEM )
			pMEM = new gpMEM;
		return pMEM->iPC( iPC, nU1 );
	}
	gpBLOCK* srcBLKstk( gpBLOCK* pBLK, gpeOPid opID )
	{
		///kEND(scp);
		gpROW	*pRL = pBLK->pLAST();
		if( pRL )
		{
			switch( gpaOPgrp[pRL->pstOP] )
			{
				case gpeOPid_stk: /// , ;
					pRL->pstOP = opID;
					return pBLK;
			}
		}

		if( pBLK )
		{

		}

		return pBLK;
	}
	gpBLOCK* srcBLKnew( I4 id )
	{
		gpBLOCK	*pBLK = lzyBLOCK.pBLOCK();
		gpROW	*pROW = pBLK->pROW(0,true);
		if( pROW )
		{
			pROW->oID = id;

			pROW->pstOP =
			pBLK->opID = gpeOPid_stk;
		}
		return pBLK;
	}
	gpBLOCK* srcBLKarySTR( gpBLOCK* pBLK, I4 id, U1* pSTR, U4 nSTR ) {
		if( id > 0 )
			id *= -1;

		gpOBJ *pO = srcOBJfnd(id);
		U1* pU1 = NULL;
		if( !pO )
		{
			pO = srcOBJadd(id);
			if( !pO )
				return NULL;

			pO->cID = gpeCsz_b;
			pO->d2D = I4x2( nSTR+1, 1 );
			pO->sOF = pO->d2D.area()*gpaCsz[pO->cID];
			pU1 = srcMEMiPC(
									pO->iPC = pMEM->nDAT,
									pO->sOF
								);
			pMEM->nDAT += pO->sOF;
			gpmMcpy( pU1, pSTR, nSTR )[nSTR] = 0;
		}

		if( !pBLK )
			return srcBLKnew( id );

		switch( pBLK->opID )
		{
			case gpeOPid_stk:
			case gpeOPid_newrow:{
					gpROW* pROW = pBLK->pROW( pBLK->nROW(), true );
					if( pROW )
					{
						pROW->oID = id;

						pROW->pstOP =
						pBLK->opID = gpeOPid_stk;
					}
					return pBLK;
				} break;
			default:{


			} break;
		}

		return pBLK;
	}
	gpBLOCK* srcBLKaryAN( gpBLOCK* pBLK, I4 id, gpeCsz* pcVAR, const I8x2& AN ) {
		gpOBJ *pO = srcOBJfnd(id);
		U1* pU1 = NULL;
		if( !pO )
		{
			pO = srcOBJadd(id);
			if( !pO )
				return NULL;

			pO->cID = pcVAR[1];
			pO->d2D = I4x2(1,1);
			pO->sOF = pO->d2D.area()*gpaCsz[pO->cID];
			pU1 = srcMEMiPC(
								pO->iPC = pMEM->nDAT,
								pO->sOF
							);
			pMEM->nDAT += pO->sOF;
			gpmMcpy( pU1, &AN, pO->sOF );
		}

		if( !pBLK )
			return srcBLKnew( id );

		switch( gpaOPgrp[pBLK->opID] )
		{
			case gpeOPid_stk: {
					gpROW* pROW = pBLK->pROW( pBLK->nROW(), true );
					if( pROW )
					{
						pROW->oID = id;
						pROW->pstOP = gpeOPid_stk;
					}
					return pBLK;
				} break;
			default:{


			} break;
		}

		return pBLK;
	}
	gpBLOCK* srcBLKaryNUM( gpBLOCK* pBLK, I4 id, gpeCsz* pcVAR, const I8x2& AN ) {
		gpOBJ *pO = srcOBJfnd(id);
		U1* pU1 = NULL;
		if( !pO )
		{
			pO = srcOBJadd(id);
			if( !pO )
				return NULL;

			pO->cID = pcVAR[1];
			pO->d2D = I4x2(1,1);
			pO->sOF = pO->d2D.area()*gpaCsz[pO->cID];
			pU1 = srcMEMiPC(
								pO->iPC = pMEM->nDAT,
								pO->sOF
							);
			pMEM->nDAT += pO->sOF;
			gpmMcpy( pU1, &AN, pO->sOF );
		}

		if( !pBLK )
			return srcBLKnew( id );

		switch( gpaOPgrp[pBLK->opID] )
		{
			case gpeOPid_stk: {
					gpROW* pROW = pBLK->pROW( pBLK->nROW(), true );
					if( pROW )
					{
						pROW->oID = id;
						pROW->pstOP = gpeOPid_stk;
					}
					return pBLK;
				} break;
			default:{


			} break;
		}

		return pBLK;
	}
	gpBLOCK* srcBLKmov( gpBLOCK* pBLK, gpeOPid opID ) {
		///kMOV(scp); //, iOPe ); // Ai--; // Ai--; //
		///kOBJ(scp); //, iOPe );
		///++SP;
		///*pSTRT = now;
		U4 nR = pBLK->nROW(), iI = pBLK->iIN();
		if( nR && !iI )
		{
			//már van benne valami cucc, de nem volt OUT
			gpROW	*pR0 = pBLK->pROW( 0 ),
					*pRL = pBLK->pLAST();
			pRL->pstOP = opID;
			if( nR == 1 )
			{
				pBLK->setIN( 1 );
				return pBLK;
			}
		}

		return pBLK;
	}
	gpBLOCK* srcBLKadd( gpBLOCK* pBLK, gpeOPid opID ) {
		///kOBJ(scp); //, iOPe );
		// a =b*c +d		// iMUL 1
		// a =b*c/d +e		// iMUL 2
		// a +=b*c/d +e		// iMUL 3
		// a +b*c +e		// iMUL 1
		// a*b +b*c			// iMUL 1
		///kMUL(scp,false);
		///LEVaddEXP()[lADD++] = now;
		///++SP;
		if( pBLK )
		{

		}

		return pBLK;
	}
	gpBLOCK* srcBLKmul( gpBLOCK* pBLK, gpeOPid opID ) {
		// a =b +c*d 	// iADD 1 // de nem adtam hozzá még a c-t
		//   -1-^
		// a =b+c +d*e  // iADD 2 // de nem adtam hozzá még a d-t
		//   -2-1-^
		// d*e +f*g		// iADD 1
		//-2-1-^
		// d*e +f -g*h	// iADD 2
		//-3-2 -1-^
		///kADD(scp); //, iOPe );
		///kOBJ(scp); //, iOPe );
		///LEVmulEXP()[lMUL++] = now;
		///++SP;
		///++stkCD;
		if( pBLK )
		{

		}

		return pBLK;
	}
	gpBLOCK* srcBLKentry( gpBLOCK* pBLK, gpeOPid opID ) {
		///switch( now )
		///{
		///	case gpeOPid_dot:
		///		break;
		///	case gpeOPid_brakS:
		///	default:
		///		LEVup(scp);
		///		break;
		///}
		///++SP;
		///++stkCD;
		if( pBLK )
		{

		}

		return pBLK;
	}
	gpBLOCK* srcBLKout( gpBLOCK* pBLK, gpeOPid opID ) {
		/// ITT MÉG FENT
		///LEVdwn(scp,now);
		/// ITT MÁR LENT
		///++SP;
		///++stkCD;
		if( pBLK )
		{

		}

		return pBLK;
	}

	U8 n_ld( U8 in ) {
		if( !this )
			return 0;

		if( nLD == in )
			return nLD;
		if( nLD < in )
		{
			nLD = in;
			return nLD;
		}
		nLD = in;
		return nLD;
	}
	U8 n_ld_add( I8 add = 0 ) {
		if( !this )
			return 0;

		if( !add )
			return nLD;
		return n_ld( nLD+add );
	}

	U4 srcUPDT( SOCKET ig = INVALID_SOCKET ) {
		U8 nLEN = 0;
		pB = pA + gpfVAN( pA, (U1*)"\a", nLEN );
		nVERr = nHD+1;
		iGT = ig;

		return nVERr;
	}
	bool qBLD( void ) {
		if( nVERr > nBLD )
			return true;	// már kérte valaki

		//nVERr = max( nHD, nBLD ); //+1;
		return false; // mi kérjük elösször
    }
    U4 rdyBLD( void ) {
		nBLD = nVERr;
		return nVERr;
    }

	U4 nCLR(void) { return gpmOFF( gpcSRC, nVERr )-gpmOFF( gpcSRC, pALFtg ); }
	U1* pSRCalloc( bool bNoMini, U1 selID ) {
		bool	bHD = false,
				bMINI = bHD ? false : ( bNoMini ? false : !!pMINI );
		if( !bMINI )
		{
			dim.w = n_ld_add();
			return pA;
		}

		if( pDBG->nLD() ? selID == gpdDBG : false )
		{
			dim.w = pDBG->nLD();
			return pDBG->p_alloc;
		}

		dim.w = bMINI ? pMINI->nLD() : n_ld_add();
		return bMINI ? pMINI->p_alloc : pA;
	}
	U1* pSRCstart( bool bNoMini, U1 selID ) {
		// bHD akkor igaz, ha szerkesztés alatt van a rublika
		// és ráadásul a \a elöt6t van a cursor
		bool	bHD = false,
				bMINI = bHD	? false : ( bNoMini ? false : !!pMINI );

		U1	*pC = bMINI ? pMINI->p_alloc : pA;
		dim.w = bMINI ? pMINI->nLD() : n_ld_add();
		if( bMINI )
		{
			if( pDBG->nLD() ? selID != gpdDBG :true )
				return pC;

			dim.w = pDBG->nLD();
			return pDBG->p_alloc;
		}

		if( bHD )
			return pC;

		pC += iPUB(); //iB()+1;
		dim.w -= (pC-pA);
		return pC;
	}

	U8 SRCmnCR( I4x2& cr, bool bNoMini, U1 selID ) {
		if( !this )
		{
			cr.null();
			return 0;
		}
		cr.y = min( cr.y, dim.y-1 );

		//if( !cr.sum() )
		//	return pC - pSRCalloc();;
		I4x4 cxy = 0;
		U1 sC[] = " ", *pROW = pSRCstart(bNoMini, selID ), *pCe, *pC;
		U4 n;
		for( pC = pROW, pCe = pC+dim.w; pC < pCe; pC++ )
		{
			if( cxy.y >= cr.y )
				break;

			switch( *pC )
			{
				case '\r':
					if( pC[1] != '\n' )
					{
						cxy.x = 0;
						continue;
					}
					pC++;
					pROW = pC+1;
					cxy.x = 0;
					cxy.y++;
					continue;
				case '\n':
					pROW = pC+1;
					cxy.x = 0;
					cxy.y++;
					continue;
				case '\a':
					pROW = pC+1;
					cxy.y++;
					cxy.x = 0;
					continue;
				case '\t':
					sC[0] = *pC;
					n = gpmNINCS( pC+1, sC );
					cxy.x = 0 + (cxy.x/4 + n)*4 + 4;
					pC += n;
					continue;
				case ' ':
					cxy.x++;
					continue;
			}

			if( *pC < ' ' )
				continue;
			if( *pC & 0x80 )
				pC++;
			cxy.x++;
		}

		cxy.x = 0;
		for( pC = pROW; pC < pCe; pC++ )
		{
			if( cxy.x >= cr.x )
			{
				break;
			}
			else if( *pC&0x80 )
			{
				pC++;
			}

			switch( *pC )
			{
				case '\n':
				case '\r':
				case '\a':
					return pC - pSRCalloc(bNoMini, selID);

				case '\t':
					sC[0] = *pC;
					n = gpmNINCS( pC+1, sC );
					cxy.x =(cxy.x/4 + n)*4 + 4;
					pC += n;
					continue;
				case ' ':
					cxy.x++;
					continue;
			}

			cxy.x++;
		}
		return pC - pSRCalloc(bNoMini, selID);
	}
	gpcSRC* SRCfrm(	U1x4* p1, const I4x4& xy, gpeCLR fr, const I4x4& fxyz ); //, I4 fz );
	I4x2 SRCmini(
					U1x4* pO, I4x2 xy,
					I4 fx,
					I4 fy,

					I4 fz, I4 zz,

					gpcCRS& crs,
					gpeCLR bg, //gpeCLR fr,
					gpeCLR ch,
					bool bNoMini, U1 selID
				);

	U4x4 CRSdim( bool bNoMini, U1 selID ) {
		if( !this )
			return U4x4( gpdSRC_COLw, gpdSRC_ROWw );

		U1* pC = pSRCstart( bNoMini, selID );
        dim.z = gpfUTFlen( pC, pC+dim.w, dim.x, dim.y ); // x oszlop y sor
		return dim;
	}
	U4x4	srcBRK( bool bNoMini, U1 selID, const char* pVAN = NULL );
	U1		srcSCN( gpcCRS& crs, bool bNoMini, U1 selID );
	I4x2	srcRDY(
						U1x4* pO,
						I4x2 xy, I4x2 fWH,
						I4 fz, I4 zz,
						gpcCRS& crs,
						bool bNoMini, U1 selID
					);
	void 	srcDBG( gpcLZYdct& dOP, U1 iSCP );
	void 	srcCMPLR( gpcLZYdct& dOP, U1 iSCP );
	void 	srcBLD( gpcMASS* pMASS ); //, gpcWIN& win );
	gpCORE* srcRUN( gpcMASS* pMASS, gpcWIN* pWIN, gpCORE* pMOM = NULL );

	gpcLZY* srcMINI( gpcLZY* pLZY, gpcMASS* pMASS, gpcWIN* pWIN, gpCORE* pMOM = NULL );

	void 	srcCMPLR_tresh( gpcLZYdct& dOP, U1 iSCP );

	//gpcRES* SRCmnMILLrunTRESH( gpcMASS* pMASS, gpcWIN* pWIN, gpcRES* pMOM = NULL );
    bool bSUB( gpcMASS& mass ) {
		if( !this )
			return false;

		hd( &mass );

		// beljebb lép
		return bSW&gpeMASSsubMSK;
    }
    bool bRET( gpcMASS& mass ) {
		if( !this )
			return false;

		hd( &mass );

		// kijebb lép
		return bSW&gpeMASSretMSK;
    }
    bool bENTR( gpcMASS& mass, U4x4& _spc, U4 x = 0 ) {
		if( !this )
			return false;


		hd( &mass );

		// új sort kezd a táblázatban
		if( bSW&gpeMASSznMSK )
		{
			_spc = spcZN;
			_spc.x++;
			return false;
		}
		if( !(bSW&gpeMASSentrMSK) )
			return false;

		spcZN.x = x;
		spcZN.y++;
		_spc = spcZN;
		_spc.x++;
		return true;

    }
    bool bUNsel( gpcMASS& mass ) {
		if( !this )
			return false;

		hd( &mass );

		// pointerrel ne lehessen kijelölni
		// pl. rajzolásnál hasznos, meg gombnál
		return bSW&gpeMASSunselMSK;
    }

	bool bIN( gpcMASS& mass ) {
		if( !this )
			return false;

		// input rublika
		hd( &mass );

		return bSW&gpeMASSinpMSK;
    }

    bool bPASS( gpcMASS& mass ) {
		if( !this )
			return false;

		hd( &mass );

		// csillagokat kell írni a betű helyett?

		return bSW&gpeMASSpassMSK;
    }
    bool bALERT( void ) {
		if( !this )
			return false;

		return bSW&gpeMASSalertMSK;
    }
    bool bMAIN( gpcMASS& mass, bool bDBG = false ) {
		if( !this )
			return false;
		hd( &mass );

		bool bM = bSW&gpeMASSmainMSK;
		if( !bM )
			return false;

		//cmpi( mass, bDBG );

		return true;
    }
    void hd( gpcMASS *pMASS, gpeALF* pTGpub = NULL );
    void cmpi( gpcMASS& mass, bool bDBG );
	void cmpi_undo00( gpcMASS& mass, bool bDBG );
    void cmpi_SKELETON( gpcMASS& mass, bool bDBG );


    //void cmpi_trash2( gpcMASS& mass, bool bDBG );
    //void cmpi_trash( gpcMASS& mass, bool bDBG );

    gpcSRC();
    virtual ~gpcSRC();
	U8 iB( void ) {
		if( !pA || !n_ld_add() )
			return 0;

		if( pB < pA )
			pB = pA;

		U8 i = pB-pA;
		if( i )
		if( i >= n_ld_add() )
		{
			if( i == n_ld_add() )
				return n_ld_add(); // ezzel jelezük, ha valaki már kereste
							// azaz nem 0

			pB = ( pA ? pA+n_ld_add() : NULL );
			return n_ld_add();
		}

		if( pB[i] == '\a' )
			return i;	// ez a fasza

		// még van esély keressük meg, hátha há
		U8 nLEN;
		i = gpfVAN( pA, (U1*)"\a", nLEN );
		if( i )
		if( i >= n_ld_add() )
		{
			if( i == n_ld_add() )
				return n_ld_add();	// i = nL -> ezzel jelezük, hogy kerestük de biza nem vót // szar

			pB = ( pA ? pA+n_ld_add() : NULL );
			return n_ld_add();
		}

		// na meguszta talált egyet
		pB = pA+i;
		return i;
	}
	U8 iPUB() {
		return iB()+1;
	}
	U1* pPUB() {
		if( this ? pA : NULL )
		{
			if( pA[n_ld_add()] != 0 )
				pA[n_ld_add()] = 0;
			return (U1*)pA+iPUB();
		}

		return NULL;
	}
	gpcSRC& reset( U1* pC, U1* pE, U1** ppSRC, U4x4& spcZN, U4 nADD = 1 );

    gpcSRC& SRCcpy( U1* pC, U1* pE );
	bool SRCcmp( U1* pS, U4 nS )
	{
        if( nS != n_ld_add() )
			return false;
		return gpmMcmp( pA, pS, n_ld_add() ) == n_ld_add();

		/*U4 nCMP = gpmMcmp( pA, pS, nL )-pA;
		return nCMP == nL;*/
	}

    gpcSRC( gpcSRC& B );
    gpcSRC& operator = ( gpcSRC& B );

protected:
    U4	nVERr, nBLD, nHD;
private:
};

// #include "gpcSCHL.h"
class gpcCLASS {

	gpcLZY	*paLZY,
			*pLST, *pFND;
	I8	nLST, nCLASS,
		idFND, ixFND;

	gpcLZY** ppCLASS( I8 ix )
	{
		if( ix >= nLST )
			return NULL;

		gpcLZY** ppC = NULL;
		if( paLZY )
			ppC = (gpcLZY**)paLZY->p_alloc;

		nCLASS = ppC ? paLZY->n_load/sizeof(pFND) : 0;

		if( nCLASS <= ix )
		{
			I8	nALL = paLZY ? paLZY->n_alloc/sizeof(pFND) : 0,
				nADD;
			if( nALL <= ix )
			{
				nALL = ix+1;
				nADD = nALL-nCLASS;

				U8 s = -1;

				paLZY = paLZY->lzyADD( NULL, nADD*sizeof(pFND), s );
				ppC = NULL;
				if( paLZY )
					ppC = (gpcLZY**)paLZY->p_alloc;
				else
					return NULL;

				if( !ppC )
					return NULL;

				gpmZnOF( ppC+nCLASS, nADD );
				nCLASS = paLZY->n_load/sizeof(pFND) ;
			}
		}
		return ppC+ix;
	}
	gpcCLASS( I8 id )
	{
		gpmCLR;
		I8 n = 0;
		pLST = pLST->tree_add( id, n );
		if( !pLST )
			return;

		nLST = n;
	}
public:
	gpcCLASS( void )
	{
		gpmCLR;
	}

	gpcLZY* pGET( I8 ix )
	{
		if( ix >= nLST )
			return NULL;

		gpcLZY** ppC = ppCLASS( ix );
		if( !ppC )
			return NULL;

		return *ppC;
	}
	gpcLZY** ppGET( I8 ix )
	{
		if( ix >= nLST )
			return NULL;

		return ppCLASS( ix );
	}

	I8 fnd( I8 id )
	{
		if( id ? !this : true )
			return nLST;

		if( nLST )
		if( idFND == id )
			return ixFND;

		return pLST->tree_fnd(id, nLST);
	}
	gpcLZY* p_fnd( I8 id, I8& ix )
	{
		ix = 0;
		if( id ? !this : true )
			return NULL;
		if( !nLST )
			return 0;

		if( idFND == id )
		{
			ix = ixFND;
			return pFND;
		}

		ix = pLST->tree_fnd(id, nLST);
		if(ix >= nLST)
			return NULL;

		gpcLZY** ppC = ppCLASS( ix );
		if( ppC ? !*ppC : true )
		{
			ix = nLST;
			return NULL;
		}

		ixFND = ix;
		idFND = id;
		return pFND = *ppC;
	}
	gpcCLASS* add( I8 id, I8& ix, I8& n )
	{
		if( !id )
		{
			ix = n = (this ? nLST: 0);
			return this;
		}
		if( !this )
		{
			gpcCLASS* pSCHL = new gpcCLASS(id); ///
			ix = 0;
			n = 1;
			return pSCHL;
		}

		ix = pLST->tree_fnd( id, nLST );
		if( ix < nLST )
		{
			n = nLST;
			return this;
		}

		pLST = pLST->tree_add(id, nLST);
		n = nLST;
		/*if( ix >= nLST ) // is és nLST - ha továbbra is egyenlő akkor nem tudta hozzá adni
			return this;

		ppCLASS( ix );
		if( nCLASS > ix )
			return this;

		U8 s = -1;
		nALLOC = nLST;
		pKIDS = pKIDS->lzyADD( NULL, sizeof(*pKIDS), s );*/
		return this;
	}
};

class gpcMASS {
	gpcCLASS	*pTG;
	gpcLZY		*pSRCc,
				*pLST;
	U4			nLST, xADD, xFND, nALLOC, nSP, nOP0, nOP1, iMAIN,
				aSPix[0x100];
	gpcSRC		*pFND,
				*apSP[0x100];
	U4x4 		aSP44[0x100];

	gpcSRC** ppSRC( void )
	{
		return (gpcSRC**)(pSRCc ? pSRCc->p_alloc : NULL);
	}
public:
	gpcMAP		mapCR;

	gpeALF		aTGwip[0x100];
	//gpcOPCD		aPRG[0x1000];
	//U1			asPRG[0x1000],
	//			*pPUB; //, nDICT;

	// CMPL ----------------------------
	gpcLZY		CMPL;
	gpcCMPL		PC;
	U4			aPC[0x100], iPC,
				aiDAT[0x100], alDAT;

	// CPLD ----------------------------



	//gpcLZY		*apDICTopcd[0x1000];
	//gpcLZYdct	*apDICTix[0x1000];
	U4	anDICTix[0x1000],
		//aLEVsp[0x100],
		rstLEV, iLEV, nLEV, topLEV;

	/// aGLcnl --------
	F4		aGLcnl[0x10];
	int		aGLpic[0x10];
	//gpcPIC* aGLpPIC[0x10];
	/// OPER --------
	gpcLZYdct	OPER;
	I8x2		aOP[0x100];
	gpeALF		mxOP;
	/// GATE --------
	gpcLZYall	GTlzyALL;
	gpcGTall	GTacpt, GTcnct;
	gpcLZY		*pJPGsnd;
	/// PIC ---------
	gpcLZYall	PIClzyALL;
	gpcPICall	PIC;
	/// CAM ---------
	gpcPICAM*	pCAM;
	/// TXT ---------
	U4 nTXT = 0;
	char* pTXT, *pT;

	gpcLZYdct* pOPER();
	U4* pM( U4x2& zn, U1 id = 4 )
	{
		zn = 0;
		if( !this )
			return NULL;
		U4x4 mpZN;
		U4* p_map = mapCR.MAPalloc( zn, mpZN, id );
		zn = mpZN.a4x2[1];
		return p_map;
	}
	U4 getXFNDzn( const U4x2& zn )
	{
		if( !this )
			return 0;

		U4x4 mpZN;
		U4 *pM = mapCR.MAPalloc( zn, mpZN );
		if( !pM )
			return 0;

		U4	i = zn*U4x2( 1, mpZN.z );
		return pM[i];
	}
	U4 getXFNDan( I4x2 an )
	{
		if( this ? !an.x : true )
			return 0;
		an.x--;
		return getXFNDzn(an);
	}
	U4 jDOitREF( gpcWIN* pWIN, U4 i, U4& ie, U4 **ppM, U4 **ppC, U4 **ppR, U4* pZ = NULL );

	U4 relLEV( void )
	{
		return iLEV-rstLEV;
	}

	gpcCMPL* piLEVmom( void )
	{
		return CMPL.pPC( aPC[iLEV-1] );
	}
	gpcCMPL* piLEViPC( void )
	{
		return CMPL.pPC( aPC[iLEV] );
	}
	U4 incLEV( void )
	{

		aPC[iLEV] = iPC;
		U4 iDT = aiDAT[iLEV];

		iLEV++;

		aiDAT[iLEV] = iDT;
		aPC[iLEV] = iPC;

		nLEV = iLEV+1;
		return iLEV;
	};

	//void reset_o( void );
	U1* msRST( U1* pPUB );
	void tag_add( gpeALF tg, U4 iKID )
	{
		I8 ix, n;
		U8 s = -1;

        pTG = pTG->add( (I8)tg, ix, n );

        gpcLZY* pLZY = pTG->pGET( ix ); 	///
		if( !pLZY )
		{
			*pTG->ppGET( ix ) = pLZY = new gpcLZY;
		}
		pLZY->lzyADD( (U4*)&iKID, sizeof(U4), s, 8 );
	}
	void tag_sub( gpeALF tg, U4 iKID )
	{
		if( !pTG )
			return;
		I8 ix;
		gpcLZY* pLZY = pTG->p_fnd( tg, ix );
		if( !pLZY )
			return;

		U4	*paLZY = (U4*)pLZY->p_alloc;
		U8 nKID = pLZY->n_load/sizeof(ix);
		for( U8 i = 0; i < nKID; i++ )
		{
			if( paLZY[i] != iKID )
				continue;
			nKID--;
			if( !nKID )
				continue;

			paLZY[i] = paLZY[nKID];
			i--;
		}
		if( !nKID )
		{
			gpmDEL( *pTG->ppGET( ix ) );
			return;
		}
		pLZY->n_load = nKID*sizeof(iKID);
	}

	/// gpcMASS:: find in main.cpp
	gpcSRC* SRCnew( gpcSRC& tmp, U1* pS, I4x2 an, I4 nGT, U4 nS = 0 );


	bool HTMLsave( U1* pPATH, U1* pFILE, U1* pNAME, bool bALT );
	bool SRCsave( U1* pPATH, U1* pFILE );
	U1* justDOit( gpcWIN* pWIN ); //U1* sKEYbuff, I4x4& mouseXY, U4* pKT, I4x4& SRCxycr, I4x4& SRCin );


	gpcMASS&	null();
	gpcMASS&	operator = ( const gpcMASS& b );
	/// -------------------



	gpcMASS(){ gpmCLR; }
	gpcMASS( const U1* pU, U8 nU );
	gpcMASS( const gpcMASS& b )
	{
		*this = b;
	}


	virtual ~gpcMASS();
	gpcSRC* get( U4 i )
	{
		if( i >= nLST )
			return NULL;
		gpcSRC** ppS = ppSRC();
		return ppS ? ppS[i] : NULL;
	}
	gpcSRC* srcFND( U4 xfnd )
	{
		if(!this)
			return NULL;

		if( nLST )
		if( xFND == xfnd )
			return pFND;

		U4 iFND = pLST->tree_fnd(xfnd, nLST);
		if(iFND >= nLST)
			return NULL;

		gpcSRC** ppS = ppSRC();
		if( ppS ? !ppS[iFND] : true )
			return pFND;

		pFND = ppS[iFND];
		xFND = xfnd;

		return pFND;
	}

	gpcSRC* SRCadd( gpcSRC* pSRC, U4 xfnd, U4& is, U4& n );



};

#endif // GPCSRC_H
