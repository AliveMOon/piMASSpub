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
class gpcGL;
class gpCORE;
class gpDBlst;
class gpMEM;
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
inline U8 gpfVANnNINCS( const U1* pSTR, const U1* pVAN ) {
	U8 nLEN;
	U1* pS = (U1*)pSTR;
	pS += gpfVAN( pS, pVAN, nLEN );
	pS += gpmNINCS( pS, pVAN );
	return pS-pSTR;
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

	if(b)
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
I8 inline gpfSTR2I8( void* pV, void* ppV = NULL, const void* pSTOP = NULL, bool bHEX = false ) {
	if( !pV )
		return 0;

	char *p_str = (char*)pV;
	U8 nLEN;
	int n0 = gpmVAN(p_str, (bHEX ? "+-0123456789aAbBcCdDeEfF": "+-0123456789xXbBdD"), nLEN ), n1;
	if( pSTOP ) {
		n1 = gpmVAN(p_str, (char*)pSTOP, nLEN );
		if(n0>n1)
			n0=n1;
	}
    p_str += n0;
	I8 i8 = strtol( p_str, &p_str, bHEX ? 16 : 10 );
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

		mapZN44.z = gpmMAX( mapZN44.z, gpmPAD( mapZN44.x, 0x10 ) );
		mapZN44.w = gpmMAX( mapZN44.w, gpmPAD( mapZN44.y, 0x10 ) );

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
	U8	hsSTR;
	U4x4 dim;

	gpcSCOOP(){ gpmCLR; };

	bool bGD( U1* pUTF, U1** ppUe ) {
		U8	hs2 = 0;
		U4	i = 0;
		while( pUTF[i] )
		{
			hs2 += i + (U8)pUTF[i];
			i++;
		}
		if( ppUe )
			*ppUe = pUTF+i;
		if( hsSTR == hs2 )
			return true;

		hsSTR = hs2;
		return false;
	}

	void rst( U1* pUTF ) {
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
			lzyDCT.dctMILLaddS( pUi, nU );
			nDCT++;
		}
		// typ U4x4.w:
		/// x[7s,6f,5r,4str : 3-0 nBYTE = 1<<(x&0xf) ]
		/// yz[ dimXY ] 	, w nBYTE //= 1<<(x&0xf)
		rLNK = U4x4(nMN());

		rMN.iMNindt = U4x4( pUi-p_str, nU, iDCT, typ );
		rMN.rMNpos = pos;
		rMN.rMNclr = color;
		lzyMiN.lzyADD( &rMN, sizeof(rMN), nU = -1 );

		lzyLiNK.lzyADD( &rLNK, sizeof(rLNK), nU = -1 );
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
		lzyMiN.lzyADD( &rMN, sizeof(rMN), nU = -1 );
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
		lzyMiN.lzyADD( &rMN, sizeof(rMN), nU = -1 );
		return nMN();
	}
};

class gpCLASS {
public:
	gpcLZY lzyOBJ;
	gpCLASS(){ gpmCLR; };
};


class gpMEM;

class gpOBJ {
public:
	gpMEM*	pMEM;
	I4		dctID, iPTR;
	I8x2 	AN;
	gpeCsz 	cAN;

	gpOBJ(){ gpmCLR; };

	gpPTR* 	pPTR();
	gpPTR* 	pPTRu1();
	gpPTR* 	pPTRd2D();
	U1*		pU1();

	gpOBJ& operator = ( gpPTR* pPb ) {
		gpPTR* pPt = pPTR();
		if( pPt->mNdID == pPb->mNdID )
		{
			*pPt = *pPb;
			return *this;
		}

		return *this;
	}
	U4 sOF();
	I4 cID(); // { return pPTR() ? pPTR()->cID : gpeCsz_OFF; }
	I4x2* pd2D() {
		if( !this )
			return 0;
		return pPTRd2D()->pd2D();
	}

	bool bARY(){ return pd2D()->area() > 1; }
	bool bUTF8() {
		if( cID() != gpeCsz_b )
			return false;
		return bARY();
	}
	bool bAN() { return this? cAN == gpeCsz_c:false; }
	bool bALF() { return this? cAN == gpeCsz_a:false; }
	I8 iVAR();
	bool bVAR();
	I8 iFUN();
	bool bFUN();

};

class gpROW{
public:
	I4		mNdID,	mnID, bIDup;
	gpeOPid	preOP, pstOP;

	gpROW(){};
	gpROW* operator = ( gpOBJ& O ) {
		if( !this )
			return this;
		if( &O )
			mNdID = O.dctID;

		mnID = mNdID;
		if( mnID < 0 )
			mnID*=-1;
		return this;
	}
	gpROW* operator = ( gpOBJ* pO ) {
		return (*this = *pO);
	}

};

class gpBLK {
	I4 nR, iI;
public:
	gpMEM*	pMEM;

	I4	bIDm, bIDmR,
        bID, mnID,
		iPTR;

	gpcLZY	lzyROW;
	gpeOPid opID; //grp;

	gpeOPid opIDgrp(){ return (this ? gpaOPgrp[opID] : gpeOPid_nop); }

	gpPTR* BLKpPTR( char *pS );
	gpROW* pROWadd( I4 iR ) {
		gpROW* pR = (gpROW*)lzyROW.Ux( iR, sizeof(*pR) );
		if( !pR )
			return NULL;
        //pR->iPC = -1;
		nR = 0;
		return pR;
	}
	I4 nROW() {
		if( !this )
			return 0;
		if( nR )
			return nR;
		return nR = lzyROW.nLD(sizeof(gpROW));
	}
	I4 iLAST() { return nROW()-1; }
	gpROW* pROW( I4 iR = 0, bool bADD = false ) {
		if( !this )
			return NULL;

		if( iR < 0 )
			return NULL;

		if( nROW() <= iR )
		{
			if( !bADD )
				return NULL;
			gpROW* pRl = pLASTrow();
			if( pRl )
			if( pRl->pstOP == gpeOPid_nop )
				return pRl;
			return pROWadd( iR );
		}

		return ((gpROW*)gpmLZYvali( gpROW, &lzyROW )) + iR;
	}
	gpROW* pLASTrow() { return pROW(iLAST()); }
	gpROW* pLASTrow( char* pS, gpOBJ* pO, I4 mnID ) {
		gpROW* pRl = pROW(iLAST());
		if( !pRl )
			return NULL;
		(*pRl = pO)->mnID = mnID;
		return pRl;
	}
	gpROW* pNEWrow() { return pROW(nROW(), true); }


	gpBLK( I4 bid, I4 bIDmom, I4 bIDmomR, I4 mnid ){
		gpmCLR;
		iPTR = -1;
		bID 	= bid;
		bIDm	= bIDmom;
		bIDmR	= bIDmomR;
        mnID    = mnid;
		gpROW* pR0	= pROW( 0, true );
	};
	gpBLK* pRST( gpMEM* pMEM );

	gpBLK* iROWblk( char* pS, I4 i, gpROW** ppR );
	gpPTR* iROWptr( char* pS, I4 i, gpROW** ppR, gpOBJ** ppO, gpcSRC** ppSRC = NULL, gpOBJ** ppOin = NULL );
	gpPTR* instFUN( char* pS, I8x2& AN, gpBLK *pARG );


	// gpPTR* moveAB( gpROW* pRdst, gpPTR* pPsrc );
	// gpPTR* subAB( char* pS, gpPTR* pPa, gpROW* pRb, bool bADD );
};
class gpcLZYblk {
	I4		nB;
public:
	gpcLZY	lzyBLK;

	gpcLZYblk(){ gpmCLR; }
	~gpcLZYblk() {
		gpBLK** ppB0 = gpmLZYvali( gpBLK*, &lzyBLK );
		if( ppB0 )
		for( I4 nBLK = lzyBLK.nLD(sizeof(*ppB0)), b = 0; b < nBLK; b++ )
			gpmDEL( ppB0[b] );
	}
	I4 nBLK() {
		if( !this )
			return 0;
		if( nB )
			return nB;
		return nB = lzyBLK.nLD(sizeof(gpBLK*));
	}
	gpBLK* pSTPdwn( I4 bID ) {
		return pSTPup( bID, -1, -1, 0 );
	}
	gpBLK* pSTPup( I4 bID, I4 bIDmom, I4 bIDr, I4 mnID ){
		if( bID == -1 )	// pont -1 volt azaz nincs
			return NULL;

		if( bID < 0 )
		{
			bID = nBLK(); // bID -2 vagy kissebb akkor új blokot kérünk
			nB = 0;
		} else if( bID >= nBLK() )
			bID = nBLK();

		gpBLK** ppB = (gpBLK**)lzyBLK.Ux( bID, sizeof(*ppB) );
		if( *ppB )
			return (*ppB);

		return (*ppB) = new gpBLK( bID, bIDmom, bIDr, mnID );
	}
	gpBLK* pNEWblk( gpeOPid opID, I4 bIDmom, I4 bIDr, I4 mnID ) {
		gpBLK	*pBLK = pSTPup( -2, bIDmom, bIDr, mnID );
		if( !pBLK )
			return NULL;
        pBLK->iPTR = -1;
		pBLK->opID = opID;
		return pBLK;
	}
};
class gpINST {
public:
	gpeOPid op; gpeCsz cID;
	gpeEA mS; U1 iS, xS;
	gpeEA mD; U1 iD, xD;
	I8x2	a8x2;
	gpINST& c( gpeCsz cc ){ cID = cc; return *this; };

	gpINST& Dn( U1 i ) {
		if( !mS )
		{
			mS = gpeEA_Dn;
			iS = i%8;
			return *this;
		}
		mD = gpeEA_Dn;
		iD = i%8;
		return *this;
	};
	gpINST& An( U1 i ) {
		if( !mS )
		{
			mS = gpeEA_An;
			iS = i%8;
			return *this;
		}
		mD = gpeEA_An;
		iD = i%8;
		return *this;
	};
	gpINST& IAnI( U1 i ) {
		if( !mS )
		{
			mS = gpeEA_IAnI;
			iS = i%8;
			return *this;
		}
		mD = gpeEA_IAnI;
		iD = i%8;
		return *this;
	};
	gpINST& IAnIp( U1 i ) {
		if( !mS )
		{
			mS = gpeEA_IAnIp;
			iS = i%8;
			return *this;
		}
		mD = gpeEA_IAnIp;
		iD = i%8;
		return *this;
	};
	gpINST& sIAnI( U1 i ) {
		if( !mS )
		{
			mS = gpeEA_sIAnI;
			iS = i%8;
			return *this;
		}
		mD = gpeEA_sIAnI;
		iD = i%8;
		return *this;
	};
	gpINST& d16IAnI( I8 d16, U1 i ) {
		if( !mS )
		{
			mS = gpeEA_d16IAnI;
			iS = i%8;
			a8x2.x = d16;
			return *this;
		}
		mD = gpeEA_d16IAnI;
		iD = i%8;
		a8x2.y = d16;
		return *this;
	};
	gpINST& d16IAnI( I8 d16, U1 i, U1 x ) {
		if( !mS )
		{
			mS = gpeEA_d16IAnDnI;
			iS = i%8;
			xS = x%8;
			a8x2.x = d16;
			return *this;
		}
		mD = gpeEA_d16IAnDnI;
		iD = i%8;
		xD = x%8;
		a8x2.y = d16;
		return *this;
	};
	gpINST& EAw( I8 i ) {
		if( !mS )
		{
			mS = gpeEA_W;
			a8x2.x = i;
			return *this;
		}
		mD = gpeEA_W;
		a8x2.y = i;
		return *this;
	};
	gpINST& EAl( I8 i ) {
		if( !mS )
		{
			mS = gpeEA_L;
			a8x2.x = i;
			return *this;
		}
		mD = gpeEA_L;
		a8x2.y = i;
		return *this;
	};
	gpINST& instDBG( gpcLZY* pDBG, gpMEM* pMEM, U1* pU1 = NULL );
};
class gpGL{
public:
	I4x4		trgWH;
	I4			aPICid[0x10],
				aBOBid[0x10];
	gpcPIC		*apPIC[0x10],
				*apBOB[0x10],
				*pTRG;
	gpcLZY		lzyCNL, lzyVTX, lzyPIX;
	U4 			nCNL, mskPIC, nBLD;
	gpcPICAM	*pCAM;
	SDL_Texture	*apTX[0x10];
	char		*pVTX, *pPIX;
	char* VTX( char* pVX ) {
		if( pVX ? !*pVX : true )
			return pVTX;
		U8 nLEN, s = 0;
		pVX += gpmNINCS( pVX, "\"");
		U4 nVX = gpmVAN(pVX,"\"",nLEN);
		lzyVTX.lzyADD( pVX, nVX, s = 0 );

		return pVTX = (char*)lzyVTX.p_alloc;
	}

	char* PIX( char* pPX ) {
		if( pPX ? !*pPX : true )
			return pPIX;
		U8 nLEN, s = 0;
		pPX += gpmNINCS( pPX, "\"");
		U4 nPX = gpmVAN(pPX,"\"",nLEN);
		lzyPIX.lzyADD( pPX, nPX, s = 0 );

		return pPIX = (char*)lzyPIX.p_alloc;
	}
	gpGL(){ gpmCLR; };
	~gpGL(){
		gpmDEL(pCAM);
	};
	U4 nBUILD() {
		if( !this )
			return 0;
		mskPIC = nCNL = 0;
		nBLD++;
		return nBLD;
	}
};
class gpCTRL{
public:
	char	sNAME[0x100];
	gpCTRL(){ gpmCLR; };
	/*~gpCTRL(){

	};*/
};
/// CCR XNZVC
class gpDBlst;

class gpDBitm {
public:
	I8 mID, ID;
	gpcLZY	atLST,
			voidLST;
	gpDBitm(){};
	~gpDBitm(){};
	gpDBitm* read( gpDBlst *pIDlst );
	gpDBitm* store( gpDBlst *pIDlst, I8x2* pAT, void* pVAR );
	gpDBitm& null(){
		/// ha lesz valami törölni való
		gpmCLR;
		return *this;
	}
	size_t sOF( gpeALF b );
	U1* fndXB( gpeALF b ) {
		I8x4* pA0 = (I8x4*)atLST.Ux( 0, sizeof(*pA0) );
		I4 iA = 0;
		for( I4 nA = atLST.nLD(sizeof(*pA0)); iA < nA; iA++ ) {
			if( pA0[iA].a8x2[0].b != b )
				continue;
			return voidLST.Ux( pA0[iA].a8x2[1].x, pA0[iA].a8x2[1].y, true, 1 );
		}
		return NULL;
	}

	U1* fndAB( I8x2& ab ) {
		I8x4* pA0 = (I8x4*)atLST.Ux( 0, sizeof(*pA0) );
		I4 iA = 0;
		for( I4 nA = atLST.nLD(sizeof(*pA0)); iA < nA; iA++ ) {
			if( pA0[iA].a8x2[0] != ab )
				continue;
			return voidLST.Ux( pA0[iA].a8x2[1].x, pA0[iA].a8x2[1].y, true, 1 );
		}
		return NULL;
	}
	U1* pAB( I8x2& ab, size_t& n ) {
		U1* pU = fndAB( ab );
		n = sOF(ab.b);
		if( pU )
			return pU;


		I8x4* pA0 = NULL;
		I4 nA = atLST.nLD(sizeof(*pA0));
		pA0 = (I8x4*)atLST.Ux( nA, sizeof(*pA0) );
		if( !pA0 )
			return NULL;

		pA0->a8x2[0] = ab;
		pA0->a8x2[1].x = voidLST.nLD();
		pA0->a8x2[1].y = n;
		return voidLST.Ux( pA0->a8x2[1].x, pA0->a8x2[1].y, true, 1 );
	}
	I4x4* fndXYR( gpeALF alf ) {
		if( !this )
			return NULL;
		I8x2 ab( alf, gpeALF_XYR );
		return (I4x4*)fndAB( ab );
	}
};
class gpDBlst {
public:
	char	sPATH[0x400], *pF;
	I8		newID;
	I4		aixITMsel[2], iSW;
	gpDBitm	itmSEL;
	gpcLZY	itmLST;
	gpMEM*	pMEM;
	gpDBlst(){};
	gpDBlst( gpMEM* pM, char* pU1, U2 nU1 ) {
		gpmCLR;
		pMEM = pM;
		pF = sPATH+nU1;
		gpmMcpy( sPATH, pU1, nU1 );
	}
	~gpDBlst(){
		gpDBitm* pITM = (gpDBitm*)itmLST.p_alloc;
		if( !pITM )
			return;

		for( I4 i = 0, n = itmLST.nLD(sizeof(gpDBitm)); i < n; i++ )
			pITM[i].null();
	}
	gpDBitm* pITM( I4 ix = 0 ) {
		U4 n = itmLST.nLD(sizeof(gpDBitm));
		gpDBitm* pI = this ? (gpDBitm*)itmLST.Ux( ix, sizeof(*pI) ) : NULL;
		if( !pI )
			return NULL;
		if( ix < n )
			return pI;
		pI->mID = pI->ID = newID;
		++newID;
		pI->read( this );
		return pI;
	}
	gpDBitm* pITMid( I8 id ) {
		U4 n = itmLST.nLD(sizeof(gpDBitm));
		if( n ? (id>=newID): true)
			return NULL;
		gpDBitm* pI = this ? (gpDBitm*)itmLST.Ux( 0, sizeof(*pI) ) : NULL;
		if( !pI )
			return NULL;
		for( U4 i = 0; i < n; i++ ) {
			if( pI[i].ID == id )
				return pI+i;
		}
		return NULL;
	}
	int nITM() { return itmLST.nLD(sizeof(gpDBitm)); }
};


class gpc3Dly;
class gpc3Dlst;

class gpc3Dtri {
public:
	U4 prt;
	gpcLZY	p,l,t,
			srf;
	U4x4 aIIXN[4];

	gpc3Dtri(){};
	gpc3Dtri* null() {
		p.null();
		l.null();
		t.null();
		srf.null();
	}
	U4 nSRF() {
		if( !this )
			return 0;
		U4 n = srf.nLD(sizeof(U4x4));
		return n>1 ? n-1 : 0;
	}
	U4x4* pSRF( U4 s ) {
		if( s >= nSRF() )
			return NULL;

		return srf.pU4x4n( s );
	}
	U4x4* pSRFadd( U4 s ) {
		if( !this )
			return NULL;

		U4x4* pS = srf.pU4x4n( srf.nLD(sizeof(*pS)) );
		*pS = U4x4( s, p.nLD(sizeof(U4)), l.nLD(sizeof(U4)), t.nLD(sizeof(U4)) );
		return pS;
	}
	~gpc3Dtri(){
		null();
	};
};
class gpc3Dsrf {	///B
public:
	char sNAME[0x100],
		 sPIC[0x100], //,sALF[0x100],
		 sVMAP[0x100];
	I4 imag, imap, ix;
	I8x2 an;
	gpc3Dsrf(){};
};
class gpc3D {
	gpcLZY	lzySRF,
			rstMX;
public:
	char sPATH[gpdMAX_PATH];
	I8x4 id;

	gpcLZY	*p_lwo,
			ly3D,
			tgIX;
	U4 nRDY, nBLD;

	~gpc3D(){
		gpmDEL(p_lwo);
	}
	gpc3D( I4 i, const char* pP, gpeALF alf ) {
		gpmCLR;
		U8 nLEN;
		id.x = i;
		id.a8x2[0].b = alf;
		id.z = pP ? gpmVAN(pP," \t\r\n\"\a",nLEN) : 0;
		gpmSTRCPY( sPATH, pP );
		nBLD = nRDY+1;
	}
	gpc3D* pLWO( gpcLZY& lwo, gpcLZYdct& dctBN );
	U4 nLY();
	gpc3Dly* pLYix( U4 i );
	gpc3D* prt2ix( gpcGL* pGL );
	U4 nSRF() { return this ? lzySRF.nLD(sizeof(gpc3Dsrf)) : 0; }
	gpc3Dsrf* pSRFi( U4 i );
	gpc3Dsrf* pSRFadd( U4 i );
	U4 nMX() {
		return this ? rstMX.nF4x4() : 0;
	}
	F4x4* pMX( U4 n ) {
		if( !n )
			return rstMX.pF4x4( 0 );
		F4x4* pMX = rstMX.pF4x4n(0,n);
		return rstMX.pF4x4( 0 );
	}
};

class gpc3Dact {
public:
	float	b, e, start, mix, fade, gim, loop, long_s;
	I8		begin_ms, end_ms, start_ms, loop_ms, long_ms;
	gpc3Dact(	float _begin, float _end,
			float _start, float _mix,
			float _fade, float _gim, float _loop )
	{
		b		= _begin;
		e		= _end;
		start	= _start;
		mix		= _mix;
		fade	= _fade;
		gim		= _gim;
		loop	= _loop;

		long_ms = (I8)((long_s = e-b)*ms2sec);
		begin_ms = (I8)(b*ms2sec);
		end_ms = (I8)(e*ms2sec);
		loop_ms = (I8)(loop*ms2sec);
		start_ms = ((I8)(start-b))*ms2sec;
	}
	float sec( I8 ms ) const {
		float sec = 0.0;
		ms += start_ms; // - begin_ms;

		if( ms < long_ms )
			sec = float(ms)/float(ms2sec);
		else if( end_ms <= loop_ms )
			sec = long_s;
		else
			sec = float(ms%long_ms)/float(ms2sec);

		return sec + b;
	}
};
typedef enum gpeACT : int {
	gpeACT_DEF,
	gpeACT_IDLE,	// <BEGIN>  1.0 <END> 31.0 <START>  1.0 <MIX>  0.0  <FADE> 0.25 <GIM> 1.0 <LOOP> 1.0 <ENDTRACK>
	gpeACT_WALK,	// <BEGIN> 32.5 <END> 34.5 <START> 32.5 <MIX> 33.5  <FADE> 0.5  <GIM> 1.0 <LOOP> 33.0 <ENDTRACK>
	gpeACT_RUN,		// <BEGIN> 36.5 <END> 38.5 <START> 36.5 <MIX> 38.5  <FADE> 0.5  <GIM> 0.3 <LOOP> 37.0 <ENDTRACK>
	gpeACT_JUMP,	// <BEGIN> 40.0 <END> 42.0 <START> 40.0 <MIX> 41.5  <FADE> 0.5  <GIM> 0.3 <STOP> <ENDTRACK>
	gpeACT_SLEFT,	// <BEGIN> 43.0 <END> 45.0 <START> 43.0 <MIX> 44.5  <FADE> 0.5  <GIM> 1.0 <LOOP> 43.0 <ENDTRACK>
	gpeACT_SRIGHT,	// <BEGIN> 46.0 <END> 48.0 <START> 46.0 <MIX> 47.5  <FADE> 0.5  <GIM> 1.0 <LOOP> 46.0 <ENDTRACK>
	gpeACT_FLINCH,	// <BEGIN> 49.0 <END> 52.0 <START> 49.5 <MIX> 51.5  <FADE> 0.5  <GIM> 0.5 <LOOP> 50.0 <ENDTRACK>
	gpeACT_BOX,		// <BEGIN> 53.0 <END> 54.0 <START> 53.0 <MIX> 53.75 <FADE> 0.25 <GIM> 0.1 <LOOP> 53.0 <ENDTRACK>
	gpeACT_KICK,	// <BEGIN> 55.0 <END> 56.0 <START> 55.0 <MIX> 55.5  <FADE> 0.5  <GIM> 0.1 <LOOP> 55.0 <ENDTRACK>
	gpeACT_ABOARD,	// <BEGIN> 57.0 <END> 58.0 <START> 57.0 <MIX> 57.75 <FADE> 0.25 <GIM> 0.2 <STOP> <ENDTRACK>
	gpeACT_DEBUS,	// <BEGIN> 58.0 <END> 60.0 <START> 58.0 <MIX> 60.0  <FADE> 0.25 <GIM> 0.2 <STOP> <ENDTRACK>
	gpeACT_STOW,	// <BEGIN> 61.0 <END> 61.5 <START> 61.0 <MIX> 61.5  <FADE> 0.25 <GIM> 0.1 <STOP> <ENDTRACK>
	gpeACT_STOWUP,	// <BEGIN> 61.5 <END> 62.0 <START> 61.5 <MIX> 62.0  <FADE> 0.25 <GIM> 0.1 <STOP> <ENDTRACK>
	gpeACT_WHAM,	// <BEGIN> 63.0 <END> 64.0 <START> 63.0 <MIX> 63.75 <FADE> 0.25 <GIM> 0.1 <LOOP> 63.0 <ENDTRACK>
	gpeACT_KO,		// <BEGIN> 65.0 <END> 69.0 <START> 65.0 <MIX> 68.9  <FADE> 0.01 <GIM> 0.1 <STOP> <ENDTRACK>
	gpeACT_ERECT,	// <BEGIN> 69.0 <END> 74.0 <START> 69.0 <MIX> 73.9  <FADE> 0.5  <GIM> 0.1 <STOP> <ENDTRACK>
	gpeACT_END,
} gpeACT_int;
static const   gpc3Dact gpaACT_man[] = {
	//			begin,	end,	start,	mix,	fade,	gim,		loop
	gpc3Dact(	  0.0,	1.0,	 0.0,	 0.0,	0.25,	1.0,		0.0		),	//GPE_ACTION_DEF,
	gpc3Dact(	  1.0,	31.0,	 1.0,	 0.0,	0.25,	1.0,		1.0		),	//GPE_ACTION_IDLE,
	gpc3Dact(	 32.0,	34.0,	32.5,	33.5,	0.5,	1.0,		32.0	),	//GPE_ACTION_WALK,
	gpc3Dact(	 36.5,	38.5,	36.5,	38.5,	0.5,	0.3,		37.0	),	//GPE_ACTION_RUN,
	gpc3Dact(	 40.0,	42.0,	40.0,	41.5,	0.5,	0.3,		42.0	),	//GPE_ACTION_JUMP,
	gpc3Dact(	 43.0,	45.0,	43.0,	44.5,	0.5,	1.0,		43.0	),	//GPE_ACTION_SLEFT,
	gpc3Dact(	 46.0,	48.0,	46.0,	47.5,	0.5,	1.0,		46.0	),	//GPE_ACTION_SRIGHT,
	gpc3Dact(	 49.0,	51.0,	49.5,	51.5,	0.5,	0.5,		49.0	),	//GPE_ACTION_FLINCH,
	gpc3Dact(	 53.0,	54.0,	53.0,	53.75,	0.25,	0.1,		54.0	),	//GPE_ACTION_BOX,
	gpc3Dact(	 55.0,	56.0,	55.0,	55.5,	0.5,	0.1,		56.0	),	//GPE_ACTION_KICK,
	gpc3Dact(	 57.0,	58.0,	57.0,	57.75,	0.25,	0.2,		58.0	),	//GPE_ACTION_ABOARD,
	gpc3Dact(	 58.0,	60.0,	58.0,	60.0,	0.25,	0.2,		60.0	),	//GPE_ACTION_DEBUS,
	gpc3Dact(	 61.0,	61.5,	61.0,	61.5,	0.25,	0.1,		61.5	),	//GPE_ACTION_STOW,
	gpc3Dact(	 61.5,	62.0,	61.5,	62.0,	0.25,	0.1,		62.0	),	//GPE_ACTION_STOWUP,
	gpc3Dact(	 63.0,	64.0,	63.0,	63.75,	0.25,	0.1,		64.0	),	//GPE_ACTION_WHAM,
	gpc3Dact(	 65.0,	69.0,	65.0,	68.9,	0.01,	0.1,		69.0	),	//GPE_ACTION_KO,
	gpc3Dact(	 69.0,	74.0,	69.0,	73.9,	0.5,	0.1,		74.0	),	//GPE_ACTION_ERECT,
};

static const char gp_man_lws[] =
	"manus\n"
	"bone_mother\n"
	"bone_groin\n"
	"bone_waist\n"
	"bone_throax\n"

	"bone_l.upperarm\n"
	"bone_l.forearm\n"
	"bone_l.hand\n"

	"bone_r.upperarm\n"
	"bone_r.forearm\n"
	"bone_r.hand\n"

	"bone_l.thigh\n"
	"bone_l.shin\n"
	"bone_l.feet\n"

	"bone_r.thigh\n"
	"bone_r.shin\n"
	"bone_r.feet\n"

	"bone_neck\n"
	"bone_head\n\0";


static const char gp_s_lws[] =
		"LoadObjectLayer\n"
		"AddBone\n"
		"AddNullObject\n"
		"AddLight\n"
		"AddCamera\n"
		"NumChannels\n"
		"Channel\n"
		"{\n"
		"Envelope\n"
		"Key\n"
		"}\n"
		"ParentItem\n"
		"BoneName\n"
		"BoneRestPosition\n"
		"BoneRestDirection\n"
		"BoneRestLength\n"
		"Plugin\n"
		"EndPlugin\n"
		;

typedef enum GPE_LWS_COM:int {
		GPE_LWS_COM_LoadObjectLayer,
		GPE_LWS_COM_AddBone,
		GPE_LWS_COM_AddNullObject,
		GPE_LWS_COM_AddLight,
		GPE_LWS_COM_AddCamera,
		GPE_LWS_COM_NumChannels,
		GPE_LWS_COM_Channel,
		GPE_LWS_COM_C_open,
		GPE_LWS_COM_Envelope,
		GPE_LWS_COM_Key,
		GPE_LWS_COM_C_close	,
		GPE_LWS_COM_ParentItem,
		GPE_LWS_COM_BoneName,
		GPE_LWS_COM_BoneRestPosition,
		GPE_LWS_COM_BoneRestDirection,
		GPE_LWS_COM_BoneRestLength,
		GPE_LWS_COM_Plugin,
		GPE_LWS_COM_EndPlugin,
} GPT_LWS_COM;
typedef enum GPE_LWS_iTYP:U4 {
	gpeLWSiTYP_OBJ = 0x10000000,
	gpeLWSiTYP_LIG = 0x20000000,
	gpeLWSiTYP_CAM = 0x30000000,
	gpeLWSiTYP_BON = 0x40000000,
	gpeLWSiTYP_TYP = 0xF0000000,
} GPT_LWS_iTYP;
class gpc3Dkey {
public:
	float val, sec; int spn;
	float aP[6];
	gpc3Dkey(){};
	gpc3Dkey( const char* pS ){
		gpmCLR;
		*this = pS;
	};
	gpc3Dkey& operator = ( const char* pS );
	float operator * ( float s ) {
			if( s == sec )
				return val;
			if( this[1].sec<=s )
				return this[1].val;
			if( this[1].val==val )
				return val;

			return	   (this[1].val-val)
					* ((s-sec)/(this[1].sec-sec))
					+ val;
	}
	size_t sOUT( char* pBUFF, const char* pEND = "\r\n" );
};
class gpc3Dcnl {
public:
	gpcLZY aKEY[9];
	U4 nKEY( U1 c ) { return c > 8 ? 0 : (this ? aKEY[c].nLD(sizeof(gpc3Dkey)): 0); }
	float valKEYs( U1 c, float s );
	gpc3Dkey* pKEYins( U1 c, float s );
	gpc3Dkey* pKEYinsIX( U1 c, I4 i, I4 nE, float s );
};

//#include <ctime>
static const I8 gpaMONTH[] = {
			// 2012
			31,
			29+31,				//29
			31+28+31,
			30+31+28+31,

			31+30+31+28+31,
			30+31+30+31+28+31,
			31+30+31+30+31+28+31,
			31+31+30+31+30+31+28+31,

			30+31+31+30+31+30+31+28+31,
			31+30+31+31+30+31+30+31+28+31,
			30+31+30+31+31+30+31+30+31+28+31,
			31+30+31+30+31+31+30+31+30+31+28+31,
			// 2013
			31,
			28+31,				// 28
			31+28+31,
			30+31+28+31,

			31+30+31+28+31,
			30+31+30+31+28+31,
			31+30+31+30+31+28+31,
			31+31+30+31+30+31+28+31,

			30+31+31+30+31+30+31+28+31,
			31+30+31+31+30+31+30+31+28+31,
			30+31+30+31+31+30+31+30+31+28+31,
			31+30+31+30+31+31+30+31+30+31+28+31,
			// 2014
			31,
			28+31,				// 28
			31+28+31,
			30+31+28+31,

			31+30+31+28+31,
			30+31+30+31+28+31,
			31+30+31+30+31+28+31,
			31+31+30+31+30+31+28+31,

			30+31+31+30+31+30+31+28+31,
			31+30+31+31+30+31+30+31+28+31,
			30+31+30+31+31+30+31+30+31+28+31,
			31+30+31+30+31+31+30+31+30+31+28+31,
			// 2015
			31,
			28+31,				// 28
			31+28+31,
			30+31+28+31,

			31+30+31+28+31,
			30+31+30+31+28+31,
			31+30+31+30+31+28+31,
			31+31+30+31+30+31+28+31,

			30+31+31+30+31+30+31+28+31,
			31+30+31+31+30+31+30+31+28+31,
			30+31+30+31+31+30+31+30+31+28+31,
			31+30+31+30+31+31+30+31+30+31+28+31,
			};

class gpcTIME {
public:
	I8	year,
		mounth,
		day,
		hour,
		minute,
		sec,
		msec;

	gpcTIME( void ) { gpmCLR; }
	I8 get_msec( bool b_touch = false ) {
		std::time_t t = std::time(0);   // get time now
		std::tm* now = std::localtime(&t);

		year = (now->tm_year+1900) - 2012; //local_time.wYear-2012;
		mounth = year*12 + now->tm_mon; // local_time.wMonth;
		day =	year*365 + (year/4) + 1
				+ gpaMONTH[(year%4)*12 + now->tm_mon] + now->tm_mday;

		hour = day*24 + now->tm_hour; //local_time.wHour;
		minute = hour*60  + now->tm_min; //+ local_time.wMinute;
		sec = minute*60   + now->tm_sec; //+ local_time.wSecond;

		return msec = sec*ms2sec;
	}
};


class gpc3Ditm {
	gpc3D*		p3D;
public:
	char sNAME[0x100];
	gpeALF alfNM;
	U4	itmIX,	itmID,
		momIX,	momID,
		trgID,	layID,
		mxIX,	nLEV,
		nNAME,	stkIX;
	F4  	rstXYZ, rstYPR, rstWHD;

	// TRACK -----------------
	F4x4	mxL, mxW, mxiW;
	float sec; gpeACT ACT;

	gpc3Dcnl	cnl;
	gpcLZY	bonLST;
	bool	bNULL;


	gpc3Ditm(){};
	gpc3D* p3Do() { return p3D; }
	gpc3D* p3Dobj( gpcGL* pGL, const char* pP, char *pF );
	char* pNAME( char* pN = NULL );
	U4  nBON(){ return this ? bonLST.nLD(sizeof(U4)) : 0; }
	U4* pBONadd( gpc3Ditm* pBON = NULL );
	F4x4* pMXl( float s );
};

class gpc3Dgym {
	public:
	char sPATH[gpdMAX_PATH], *pFILE,
		 sPUB[gpdMAX_PATH], *pPUB;
	I8x4 id;

	I4x4	olcb;
	gpcLZY	*p_lws,
			itmLST;

	U4 n3Ditm() { return this ? itmLST.nLD(sizeof(gpc3Ditm)) : 0; }
	gpc3Ditm* p3DitmADD( U4 i, U4 id );
	gpc3Ditm* p3Dii( U4 i );
	~gpc3Dgym(){
		gpmDEL(p_lws);
	}
	gpc3Dgym( I4 i, const char* pP, gpeALF alf );
	gpc3Dgym* pLWS( gpcLZY& lws, gpcLZYdct& lwsDCT, gpcLZYdct& bnDCT );
};
class gpc3Dlst {
public:
	gpcLZY lst3D;
	gpcLZYdct bnDCT, lwsDCT;
	gpcLZY lst3Dgym;

	~gpc3Dlst(){
		gpc3D **pp3D = (gpc3D**)lst3D.Ux( 0, sizeof(gpc3D*) );
		I4 i3D = 0, e3D = lst3D.nLD(sizeof(gpc3D*));
		for( I4 n3D = e3D; i3D < n3D; i3D++ ) {
			gpmDEL( pp3D[i3D] );
		}

		gpc3Dgym **pp3Dgym = (gpc3Dgym**)lst3Dgym.Ux( 0, sizeof(gpc3Dgym*) );
		//I4
		i3D = 0, e3D = lst3Dgym.nLD(sizeof(gpc3Dgym*));
		for( I4 n3D = e3D; i3D < n3D; i3D++ ) {
			gpmDEL( pp3Dgym[i3D] );
		}
	}
	gpc3Dlst();
	gpc3D* p3Dix( I4 i ) {
		if( i < 0 ) return NULL;
		gpc3D* p3D = ((gpc3D**)lst3D.Ux(i,sizeof(gpc3D*)))[0];
		return p3D;
	}
	gpc3D* p3DobjADD( gpeALF alf, const char* pP );

	gpc3Dgym* p3DgymIX( I4 i ) {
		if( this ? i < 0 : true )
			return NULL;
		gpc3Dgym** pp3Dgym = ((gpc3Dgym**)lst3Dgym.Ux(0,sizeof(gpc3Dgym*)));
		return pp3Dgym[i];
	}
	gpc3Dgym* p3DgymADD( gpeALF alf, const char* pP );

};
class gpc3Dtrk {
public:
	I8 mSEC; gpeACT ACT;

	gpc3Dtrk(){}
	F4x4* pGYM( F4x4* pMX, gpc3Dgym* p3Dg, gpc3Ditm* p3Di, I8 ms, gpeACT act );

};
class gpMEM {
public:
	I8x2 	aA[0x10];
	I8		*pA, *pD, i8;
	double	aF[0x10], d8;
	gpcLZY	lzyMEM,
			lzyOBJ,
			lzyCLASS,
			lzyCODE,
			lzyBINlnk,
			*pLZYsrcXFND,
			*pLZYsrcXFNDall;
    gpcLZYdctBIN    lzyDCTbin;

	I4		nCD, nXFND,
			iSTK,nDAT,nINST,
			pc, pcCPY;
	U4		msRUN, nDCTscp, nDCTbin;
	gpeCCR	ccr;

	gpINST	*pINST;
	gpcWIN	*pWIN;
	gpcMASS	*pMASS;
	gpcSRC	*pSRC;
	gpGL	*pMgl;
	gpcGL	*pWgl;
	gpCTRL	*pCTRL;

	I4x4	*pFREE, *pALLOC;
	U4		nFREE, nALLOC;
	~gpMEM() {
		gpmDEL( pLZYsrcXFNDall);
		gpmDEL( pMgl );
		gpmDELary(pFREE);
		gpmDELary(pALLOC);
	}
	gpMEM( gpcSRC* pS, gpcWIN* pW, gpcLZY* pSRCstk, I4 i = 0x2000 );
	gpOBJ* OBJfnd( I4 dctID );
	gpOBJ* OBJadd( char* pS, I4 dctID );
	U1*	pUn( I4 iPC = 0, U4 nU1 = 0x100 ) {
		if( this ? iPC < 0 : true )
			return NULL;
		return lzyMEM.Ux( iPC, nU1, true, sizeof(U1) );
	}

	U4 nALL( I4 iPC );
	I4x4* pALL( I4 iPC );
	I4 iFREE( I4 iPC );
	I4 iALL( U4 nA );

	gpPTR* pPTR( I4 i ) { return (gpPTR*)pUn( i, sizeof(gpPTR)); }
	gpPTR* pPTRu1( I4 i ) { return pPTR( i )->pPTRu1( this ); }

	gpOBJ* getOBJptr( U1* pU1, U4 nBYTE, I4 nmID );

	gpINST& inst( gpeOPid op ) {
		gpINST* pI = (gpINST*)lzyCODE.Ux( nCD, sizeof(*pI) );
		pI->op = op;
		nCD++;
		return *pI;
	};
	gpOBJ* pOBJ( gpeALF alf ) {
		if( this ? !alf : true )
			return NULL;
		U4 nO = lzyOBJ.nLD(sizeof(gpOBJ)), sOF;
		if( !nO )
			return NULL;
		gpOBJ* pO0 = gpmLZYvali( gpOBJ, &lzyOBJ );
		for( U4 i = 0; i < nO; i++ )
		{
			gpOBJ& obj = pO0[i];
			if( !(sOF=obj.sOF()) )
				continue;
			if( obj.AN.alf != alf )
				continue;
			return pO0+i;
		}
		return NULL;
	}

	gpINST* instRDY( gpcLZY* pDBG );
	gpINST* instALU();
	U1*		instJSR( U1* p_dst, gpINST& inst );
	I4		instDOit( gpOBJ& obj, U1* pU1 );
	I4		instDOitSLMP( gpcGT* pGT );
	I4		instDOitGSM( gpcGT* pGT );

	gpcLZY* memPRINT( gpcLZY* pPRNT, I4 *pI4, I4 nI4 );
	void	funPRINT();

	void	funFND();
	void	funNEW();
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

    //gpcRES	*apOUT[4];

	gpcLZY	*pMINI, *pDBG;

	gpcMAP	*pMAP;

	gpcSCOOP 	aSCOOP[3];
	gpCORE		*pCORE;

	gpcLZYblk	lzyBLOCK;
	gpMEM		*pMEM,
				*pMEMo;

	gpOBJ* srcOBJfnd( I4 dctID );
	gpOBJ* srcOBJadd( char* pS, I4 dctID );
	gpBLK* srcBLKnew( char* pS, gpeOPid opID, gpROW* pRml, I4 bIDm, I4 bIDmR, I4 mnID  );
	gpBLK* srcBLKup( char* pS, gpBLK* pBMom, gpeOPid opID, I4 mnID ) {
		return srcBLKnew( pS, opID, pBMom->pLASTrow(), pBMom->bID, pBMom->iLAST(), mnID );
	}
	gpBLK* srcBLKinsrt( char* pS, gpBLK* pBLKup, gpeOPid opID, I4 mnID ) {
		if( pBLKup )
			return srcBLKnew( pS, opID, NULL, -1, -1, mnID );

		gpBLK	*pBLKm = lzyBLOCK.pSTPdwn( pBLKup->bIDm ),
				*pBLKi = lzyBLOCK.pNEWblk( opID, pBLKup->bIDm, pBLKup->bIDmR, mnID );

		gpROW	*pRuf = pBLKup->pROW(0,true),
				*pRif = pBLKi->pROW(0,true),
				*pRml = pBLKm->pLASTrow();

		pRml->bIDup = 	pBLKup->bIDm = pBLKi->bID;
						pBLKup->bIDmR = 0; // pBLKi->iLAST();

		*pRif = *pRml;
		pRif->pstOP = opID;

		pBLKi->pNEWrow();
		return pBLKi;
	}
	gpPTR* SRCpPTR( char* pS, gpROW& R );



	///--------------------------
	///			INST
	///--------------------------
	gpBLK*	srcINSTdwn( char* pS, gpBLK *pBLKm, gpBLK* pBLK, gpBLK* pBLKup, I4 mnID );
	gpBLK*	srcINSTdwnO( char* pS, gpBLK *pBLKm, gpBLK* pBLK, gpBLK* pBLKup, I4 mnID );

	gpBLK*	srcINSTmov( char* pS, gpBLK *pBLKm, gpBLK* pBLK );
	gpBLK*	srcINSTanDalf( char* pS, gpBLK *pBLKm, gpBLK* pBLK );
	gpBLK*	srcINSTadd( char* pS, gpBLK *pBLKm, gpBLK* pBLK );
	gpBLK*	srcINSTmul( char* pS, gpBLK *pBLKm, gpBLK* pBLK );

	gpBLK*	srcINSTmov3( char* pS, gpBLK *pBLKm, gpBLK* pBLK );

	//gpBLK*	srcINSTmov2( char* pS, gpBLK *pBLKm, gpBLK* pBLK );
	gpBLK*	srcINSTent2( char* pS, gpBLK *pBLKm, gpBLK* pBLK );
	gpBLK*	srcINSTadd2( char* pS, gpBLK *pBLKm, gpBLK* pBLK );
	gpBLK*	srcINSTmul2( char* pS, gpBLK *pBLKm, gpBLK* pBLK );


	bool 		srcINSTrun(); //gpcMASS* pMASS, gpcWIN* pWIN );
	gpcLZY*		srcINSTmini( gpcLZY* pLZY ); //,gpcMASS* pMASS, gpcWIN* pWIN );
	///--------------------------
	///			BLOCK
	///--------------------------
	///--------------------------
	///			CONST & VAR
	///--------------------------
	gpBLK* srcBLKmNdID( char* pS, gpBLK* pBLK, I4 dctID, I4 mnID );
	gpBLK* srcBLKaryUTF8( gpBLK* pBLK, I4 mnID, char* pS, U4 nS );
	gpBLK* srcBLKaryAN( char* pS, gpBLK* pBLK, I4 dctID, I4 mnID, gpeCsz cAN, const I8x2& AN );
	gpBLK* srcBLKaryNUM( char* pS, gpBLK* pBLK, I4 dctID, I4 mnID, gpeCsz cAN, const I8x2& AN );

	///--------------------------
	///			OPERA
	///--------------------------
	gpBLK* srcBLKbrakS( char* pS, I4 mnID, gpBLK* pBLK, gpeOPid opID );
	gpBLK* srcBLKbrakE( char* pS, I4 mnID, gpBLK* pBLK, gpeOPid opID, gpcLZY* pDBG );
	gpBLK* srcBLKstk( char* pS, I4 mnID, gpBLK* pBLK, gpeOPid opID, gpcLZY* pDBG );
	gpBLK* srcBLKmov( char* pS, I4 mnID, gpBLK* pBLK, gpeOPid opID, gpcLZY* pDBG );
	gpBLK* srcBLKanDalf( char* pS, I4 mnID, gpBLK* pBLK, gpeOPid opID, gpcLZY* pDBG );

	gpBLK* srcBLKmul( char* pS, I4 mnID, gpBLK* pBLK, gpeOPid opID, gpcLZY* pDBG );
	gpBLK* srcBLKadd( char* pS, I4 mnID, gpBLK* pBLK, gpeOPid opID, gpcLZY* pDBG );

	gpBLK* srcBLKout( char* pS, I4 mnID, gpBLK* pBLK, gpeOPid opID, gpcLZY* pDBG ) {
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

		//nVERr = gpmMAX( nHD, nBLD ); //+1;
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
		cr.y = gpmMIN( cr.y, dim.y-1 );

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
	U4x4	srcBRK( gpcLZYdct& dOP, //U1 iSCP,
					bool bNoMini, U1 selID, const char* pVAN = NULL );
	U1		srcSCN( gpcCRS& crs, bool bNoMini, U1 selID );
	I4x2	srcRDY(
						U1x4* pO,
						I4x2 xy, I4x2 fWH,
						I4 fz, I4 zz,
						gpcCRS& crs,
						bool bNoMini, U1 selID
					);
	void 	srcDBG( gpcLZYdct& dOP, U1 iSCP );
	void 	srcCMPLR( gpcLZYdct& dOP, U1 iSCP, gpcWIN* pW, gpcLZY* pSRCstk ); //gpcMASS* pMS );
	U1 	srcBLD( gpcWIN* pW, gpcLZY* pSRCstk ); //
	gpCORE* srcRUN( gpcMASS* pMASS, gpcWIN* pWIN, gpCORE* pMOM = NULL );
	gpcLZY* srcMINI( gpcLZY* pLZY, gpcMASS* pMASS, gpcWIN* pWIN, gpCORE* pMOM = NULL );

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
	U8 iPUB() { return iB()+1; }
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
	bool SRCcmp( U1* pS, U4 nS ) {
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

	gpcLZY** ppCLASS( I8 ix ) {
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
	gpcCLASS( I8 id ) {
		gpmCLR;
		I8 n = 0;
		pLST = pLST->tree_add( id, n );
		if( !pLST )
			return;

		nLST = n;
	}
public:
	gpcCLASS( void ) {
		gpmCLR;
	}

	gpcLZY* pGET( I8 ix ) {
		if( ix >= nLST )
			return NULL;

		gpcLZY** ppC = ppCLASS( ix );
		if( !ppC )
			return NULL;

		return *ppC;
	}
	gpcLZY** ppGET( I8 ix ) {
		if( ix >= nLST )
			return NULL;

		return ppCLASS( ix );
	}

	I8 fnd( I8 id ) {
		if( id ? !this : true )
			return nLST;

		if( nLST )
		if( idFND == id )
			return ixFND;

		return pLST->tree_fnd(id, nLST);
	}
	gpcLZY* p_fnd( I8 id, I8& ix ) {
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
	gpcCLASS* add( I8 id, I8& ix, I8& n ) {
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

	gpcSRC** ppSRC( void ) {
		return (gpcSRC**)(pSRCc ? pSRCc->p_alloc : NULL);
	}
public:
	gpcMAP		mapCR;

	gpeALF		aTGwip[0x100];

	// CMPL ----------------------------
	gpcLZY		CMPL;
	gpcCMPL		PC;
	U4			aPC[0x100], iPC,
				aiDAT[0x100], alDAT;

	// CPLD ----------------------------
	U4	anDICTix[0x1000],
		rstLEV, iLEV, nLEV, topLEV;

	/// DB
	gpcLZYdct	dctDB;
	gpcLZY		lstDB;
	/// aGLcnl --------
	//F4		aGLcnl[0x10];
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

	gpDBlst*	iDBu( gpMEM* pMEM, char *pU, char* sPATH, char* pFILE );
	gpDBlst*	iDB( gpMEM* pMEM, gpPTR *pPi, char* sPATH, char* pFILE );
	gpcLZYdct*	pOPER();
	U4* pM( U4x2& zn, U1 id = 4 ) {
		zn = 0;
		if( !this )
			return NULL;
		U4x4 mpZN;
		U4* p_map = mapCR.MAPalloc( zn, mpZN, id );
		zn = mpZN.a4x2[1];
		return p_map;
	}
	U4 getXFNDzn( const U4x2& zn ) {
		if( !this )
			return 0;

		U4x4 mpZN;
		U4 *pM = mapCR.MAPalloc( zn, mpZN );
		if( !pM )
			return 0;

		U4	i = zn*U4x2( 1, mpZN.z );
		return pM[i];
	}
	U4 getXFNDan( I4x2 an ) {
		if( this ? !an.x : true )
			return 0;
		an.x--;
		return getXFNDzn(an);
	}
	U4 jDOitREF( gpcWIN* pWIN, U4 i, U4& ie, U4 **ppM, U4 **ppC, U4 **ppR, U4* pZ = NULL );

	U4 relLEV( void ) {
		return iLEV-rstLEV;
	}

	gpcCMPL* piLEVmom( void ) { return CMPL.pPC( aPC[iLEV-1] ); }
	gpcCMPL* piLEViPC( void ) { return CMPL.pPC( aPC[iLEV] ); }
	U4 incLEV( void ) {

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
	void tag_add( gpeALF tg, U4 iKID ) {
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
	void tag_sub( gpeALF tg, U4 iKID ) {
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
	U1* justDOitOLD( gpcWIN* pWIN ); //U1* sKEYbuff, I4x4& mouseXY, U4* pKT, I4x4& SRCxycr, I4x4& SRCin );


	gpcMASS&	null();
	gpcMASS&	operator = ( const gpcMASS& b );
	/// -------------------

	gpcMASS(){ gpmCLR; }
	gpcMASS( const U1* pU, U8 nU );
	gpcMASS( const gpcMASS& b ) { *this = b; }

	virtual ~gpcMASS();
	gpcSRC* get( U4 i ) {
		if( i >= nLST )
			return NULL;
		gpcSRC** ppS = ppSRC();
		return ppS ? ppS[i] : NULL;
	}
	gpcSRC* srcFND( U4 xfnd ) {
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
