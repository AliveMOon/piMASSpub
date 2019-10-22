#ifndef GPCRES_H
#define GPCRES_H

#include "piMASS.h"
#include "gpcSCHL.h"
#include "gpcOPCD.h"
#include "gpcSRC.h"
class gpcRES;

enum gpeTYP:U4
{
	// x[7s,6f,5r,4p? : 3-0 nBYTE = 1<<(x&0xf) ]
	// yz dimxy
	// w size
	gpeTYP_U1 	= MAKE_ID( 0x00, 1, 1, 1 ),
	gpeTYP_U4 	= MAKE_ID( 0x02, 1, 1, 4 ),
	gpeTYP_I4 	= MAKE_ID( 0x82, 1, 1, 4 ),
	gpeTYP_F  	= MAKE_ID( 0xc2, 1, 1, 4 ),
	gpeTYP_U8 	= MAKE_ID( 0x03, 1, 1, 8 ),
	gpeTYP_I8 	= MAKE_ID( 0x83, 1, 1, 8 ),
	gpeTYP_D  	= MAKE_ID( 0xc3, 1, 1, 8 ),

	gpeTYP_A	= MAKE_ID( 0x93, 1, 1, 8 ),
	gpeTYP_AN 	= MAKE_ID( 0x93, 2, 1, 16 ),


	gpeTYP_U14 	= MAKE_ID( 0x00, 4, 1, 0 ),
	gpeTYP_U44 	= MAKE_ID( 0x02, 4, 1, 0 ),
	gpeTYP_I44 	= MAKE_ID( 0x82, 4, 1, 0 ),
};

class gpcALU
{
public:
	U4		nALL, iA;
	gpcRES	*pRM;
	void	*pDAT;
	I1x4	op;
	U1x4	typ;	// typ:
					// x[7s,6f,5r,4p? 	: 3-0 nBYTE = 1<<(x&0xf) ]
					// yz[ dimXY ] 		, w[] = nBYTE*dimXY
	U4x4	AN;
	gpeALF 	alf;

	gpcALU(){ gpmCLR; };
	gpcALU( gpcRES* pM ); //= NULL );
	~gpcALU();

	gpcALU( const gpcALU& b )
	{
		gpmCLR;
		*this = b;
	}
	gpcALU& null()
	{
		gpmCLR;
        return *this;
	}
	U4 nDIM()
	{
		return typ.area_yz();
	}
	U4 nT1()
	{
		return 1<<(typ.x&0xf);
	}
	U4 nLOAD()
	{
		if( !AN.z )
			AN.z = AN.a4x2[0].area();

		// typ:
		// x[7s,6f,5r,4p? 	: 3-0 nBYTE = 1<<(x&0xf) ]
		// yz[ dimXY ] 		, w[] = nBYTE*dimXY
		if( !typ.w )
			typ.w = nDIM()<<(typ.x&0xf);

		AN.w = AN.z;
		if( typ.w > 1 )
			AN.w *= typ.w;
		// AN
		// x-A y-n
		// z = a*n

		return AN.w;
	}
	gpcALU& operator = ( const gpcALU& b )
	{
		if( this == &b )
			return *this;

		null();

		if( !&b )
			return *this;

        gpmMEMCPY( this, &b, 1 );
		if( !pDAT )
		{
			return *this;
		}
		pRM = NULL;				// pResMom
		nALL = nLOAD();
		pDAT = new U1[nALL];
        memcpy( pDAT, b.pDAT, AN.w );

        return *this;
	}
	gpcALU& equ( gpcRES* pTHIS, U4x2 xy, U1x4 ty4, I1x4 op4, U8 u8, U2 dm = 0 );
	gpcALU& equ( gpcRES* pTHIS, U4x2 xy, U1x4 ty4, I1x4 op4, I8 i8, U2 dm = 0 );
	gpcALU& equ( gpcRES* pTHIS, U4x2 xy, U1x4 ty4, I1x4 op4, double u8, U2 dm = 0 );
};

class gpcADR
{
public:
	gpcRES*	pRM;
	I4		dp, iA, nA;
	I8x2	an;

	gpcADR(){ gpmCLR; };

	gpcADR( gpeALF a )
	{
		gpmCLR;
		an.alf = a;
	}
	gpcADR& operator = ( gpeALF a )
	{
		gpmCLR;
		an.alf = a;
		return *this;
	}

	gpcADR& operator = ( gpcRES* pM );


	gpcADR( gpeALF a, gpcRES* pM )
	{
		*this = a;
		*this = pM;
	}

	gpcALU& ALU( gpcRES* pM );


};

class gpcRES
{
	gpeALF	*pALF;
	I1x4	*pOP;	// x add/sub // y mul/div // z[7rem:mod 6not:! 5or:| 4and:& ]  //w ?
					// w size

	U1x4	*pTYP;	// x[7s,6f,5r,4p? : 3-0 nBYTE = 1<<(x&0xf) ]
					// yz dimxy
	U4x4	*pAN;
	U8x4	*pTREE;
	U4		*pTx,

			*pnALL,
			nA, t, i, ig, iLEV;

	void	**ppDAT;
	gpcALU	alu;
	gpcRES* pMOM;

public:

	U4 iL()
	{
		return iLEV;
	}
	gpcRES* pRM()
	{
		return pMOM;
	}
	gpcRES& null();

	gpcRES( gpcRES* pM = NULL )
	{
        gpmCLR;
        if( !pM )
			return;

		if( pMOM = pM )
			iLEV = pMOM->iLEV+1;
	}

	~gpcRES()
	{
		null();
	}

	gpcRES* compiEASY( U1* pS, U1* pE, U1** ppE, gpcRES* pMOM );
	gpcRES* compiHARD( U1* pS, U1* pE, U1** ppE, gpcRES* pMOM );

	gpcALU& ALU( U4 iA )
	{
		alu = this;

		if( iA >= nA )
			return alu;

		alu.iA 		= iA;
		alu.alf		= pALF	? pALF[iA]	: gpeALF_null;
		alu.op		= pOP	? pOP[iA]	: 0;
		alu.typ		= pTYP	? pTYP[iA]	: 0;	// x[7s,6f,5r,4p? : 3-0 nBYTE = 1<<(x&0xf) ]
												// yz dimxy
		alu.AN 		= pAN	? pAN[iA]	: 0;
		alu.pDAT	= ppDAT	? ppDAT[iA]	: NULL;
		alu.nALL	= pnALL ? pnALL[iA]	: 0;
		return alu;
	}
	gpcRES* chg( gpcALU& ali )
	{
		if( !this )
			return NULL;

		if( ali.iA >= nA )
			return this;

		if( pALF[ali.iA] == ali.alf )
		{
			if( !ppDAT )
			{
				ppDAT = new void*[nA];
			}
			else if( ppDAT[ali.iA] != ali.pDAT )
				gpmDELary( ppDAT[ali.iA] );
			ppDAT[ali.iA] = ali.pDAT;
			if( pOP )
				pOP[ali.iA] = ali.op;
			if( pTYP )
				pTYP[ali.iA] = ali.typ;
			if( pAN )
				pAN[ali.iA] = ali.AN;

		} else
			gpmDELary( ali.pDAT );

		return this;
	}
	gpcALU& ADD( gpeALF alf, U4 typ, U4 op ) {
		U4 nCPY = nA;
		nA++;


		/// ---------------------------------------------
		void* pKILL = ppDAT;
		ppDAT = new void*[nA];
		if( pKILL )
		{
			memcpy( ppDAT, pKILL, nCPY*sizeof(*ppDAT) );
			delete[] pKILL;
		}
		ppDAT[nCPY] = NULL;

		pKILL = pnALL;
		pnALL = new U4[nA];
		if( pKILL )
		{
			memcpy( pnALL, pKILL, nCPY*sizeof(*pnALL) );
			delete[] pKILL;
		}
		pnALL[nCPY] = 0;
		/// ---------------------------------------------


		pKILL = pALF;
		pALF = new gpeALF[nA];
		if( pKILL )
		{
			memcpy( pALF, pKILL, nCPY*sizeof(*pALF) );
			delete[] pKILL;
		}
		pALF[nCPY] = alf;

		pKILL = pOP;
		if( op || pKILL )
		{
			pOP = new I1x4[nA];
			pOP->u4 = 0;
			gpfMEMSET( pOP+1, nCPY, pOP, sizeof(*pOP) );
			if( pKILL )
			{
				memcpy( pOP, pKILL, nCPY*sizeof(*pOP) );
				delete[] pKILL;
			}
			pOP[nCPY].u4 = op;
		}

		pKILL = pTYP;
		pTYP = new U1x4[nA];
		if( pKILL )
		{
			memcpy( pTYP, pKILL, nCPY*sizeof(*pTYP) );
			delete[] pKILL;
		}
		pTYP[nCPY].u4 = typ;

		pKILL = pAN;
		pAN = new U4x4[nA];
		if( pKILL )
		{
			memcpy( pAN, pKILL, nCPY*sizeof(*pAN) );
			delete[] pKILL;
		}
		pAN[nCPY] = 0;

		pKILL = pTREE;
		pTREE = new U8x4[nA];
		if( pKILL )
		{
			memcpy( pTREE, pKILL, nCPY*sizeof(*pTREE) );
			delete[] pKILL;
		}
		pTREE[nCPY].null();

		pKILL = pTx;
		pTx = new U4[nA];
		if( pKILL )
		{
			memcpy( pTx, pKILL, nCPY*sizeof(*pTx) );
			delete[] pKILL;
		}
		pTx[nCPY] = 0;

		return ALU( nCPY );
	}

	U4 nFND()
	{
		return this ? nA : 0;
	}
    U4 iFND( gpeALF alf )
    {
		if( !this )
			return 0;

		if( !nA )
			return nA;

		if( i < nA )
		if( pALF[i] == alf )
			return i;

		ig = nA;
		if( pTREE )
		{
			ig = pTREE->tree_fnd( alf, t );
			if( ig < t )
				return i = pTx[ig];

			for( U4 j = 0, x; j < nA; j++ )
			{
				if( !pALF[j] )
					continue;

				if( ig >= nA )
				if( pALF[j] == alf )
				{
					// azt remélem ettől, hogy amit gyakrabban keresnek az elöbre kerüljön
					pTx[t] = ig = j;
					x = pTREE->tree_fnd( pALF[j], t );
					if( x >= t )
						t = pTREE->tree_add( pALF[j], t );		// nem volt benne a listában
					break;
				}

				x = pTREE->tree_fnd( pALF[j], t );
				if( x < t )
					continue;

				pTx[t] = j;
				t = pTREE->tree_add( pALF[j], t );
			}

			if( ig < nA )
				i = ig;	// megtalálta a ciklusban

			return ig;
		}

		pTREE	= new U8x4[nA];
		pTx		= new U4[nA];
		gpmZn( pTREE, nA );
		t = 0;
		for( U4 j = 0; j < nA; j++ )
		{
			if( !pALF[j] )
				continue;

			if( ig >= nA )
			if( pALF[j] == alf )
			{
				pTx[t] = ig = j;
				t = pTREE->tree_add( pALF[j], t );
				break; // azt remélem ettől, hogy amit gyakrabban keresnek az elöbre kerüljön
			}

			pTx[t] = j;
			t = pTREE->tree_add( pALF[j], t );

		}
		if( ig < nA )
			i = ig;	// megtalálta a ciklusban

		return ig;



    }


};





















class gpcREStrs
{
public:
	union
	{
		struct
		{
			gpeALF	id;		// 0	8
			gpeNET4 typ;	// 8	4	// [0] SIZ  [1] UIFD [2][3] SIZxN
			U4		a,		// 12	4	// ha a == 0 akkor regiszter
			// dif ---------------
					n;		// 16	4
			U8		an;		// 20	8	// chace
			U1*		pDAT;	// 28	?
							// 32
		};

		struct
		{
			gpeALF	_id;	// 0	8
			U1	 	aT[4];	// 8	4	// *aT egy elem mérete [1] UIFD [2][3] SIZxN
			U4		_a,		// 12	4	// ha a == 0 akkor regiszter
			// dif ---------------
					aU[4]; 	// 16
							// 32
		};

	};


	gpcREStrs(  U1* pS, U1* pE );
	gpcREStrs* REScompiAN( U1* pS, U1* pE, U4* pMAP = NULL, gpcLZYdct* pDICT = NULL );

	gpcREStrs( U1 u1 )
	{
		a = 0;
		null();
		typ = gpeNET4_U11;
		aU[0] = u1;
	}

	U8 nDAT( void ) {
		if( this ? !a : true )
			return 0;
		return an * (*aT);
	}
	gpcREStrs& load( U1* pB, U8 nCPY ) {
		U8 nO = nDAT();
		if( (nO < nCPY) || (nO > nCPY*2) )
		{
			nO = 0;
			if( an < 2 )
				delete pDAT;
			else
				delete[] pDAT;
			pDAT = NULL;
		}

		if( !pDAT )
			pDAT = new U1[nCPY];
		if( !pB )
			gpmZn( pDAT, nCPY );
		else
			gpmMEMCPY( pDAT, pB, nCPY );
		return *this;
	}

	gpcREStrs& null();
	gpcREStrs& operator = ( const gpcREStrs& b );

	~gpcREStrs();
	gpcREStrs();
	gpcREStrs( const gpcREStrs& b )
	{
		*this = b;
	}


	gpcREStrs& operator = ( gpeALF alf )
	{
		null();
		typ = gpeNET4_ALF;
		*(I8*)aU = alf;
		return *this;
	}
	gpcREStrs& operator = ( U8 b )
	{
		null();
		typ = gpeNET4_U81;
		*(U8*)aU = b;
		return *this;
	}
	gpcREStrs& operator = ( I8 b )
	{
		null();
		typ = gpeNET4_I81;
		*(I8*)aU = b;
		return *this;
	}
	gpcREStrs& operator = ( U1x4 b )
	{
		null();
		typ = gpeNET4_U14;
		*(U1x4*)aU = b;
		return *this;
	}
	gpcREStrs& operator = ( double d )
	{
		null();
		typ == gpeNET4_D81;
		*(double*)aU = d;
		return *this;
	}
	gpcREStrs& operator = ( const U4x2& b )
	{
		null();
		typ = gpeNET4_U42;
		*(U4x2*)aU = b;
		return *this;
	}
	gpcREStrs& operator = ( const U4x4& b )
	{
		null();
		typ = gpeNET4_U44;
		*(U4x4*)aU = b;
		return *this;
	}
	gpcREStrs& operator = ( const I4x2& b )
	{
		null();
		typ = gpeNET4_I42;
		*(I4x2*)aU = b;
		return *this;
	}
	gpcREStrs& operator = ( const I4x4& b )
	{
		null();
		typ = gpeNET4_I44;
		*(I4x4*)aU = b;
		return *this;
	}
	gpcREStrs& operator = ( const F4& b )
	{
		null();
		typ = gpeNET4_I44;
		*(F4*)aU = b;
		return *this;
	}

	gpcREStrs& equ( gpeALF* pB, U4 _a, U4 _n ) {
		U8	nAN = _a*_n;
		if( nAN < 2 )
			return ( nAN ) ? (*this = *pB) : null();

		load( (U1*)pB, nAN * sizeof(*pB) );

		typ = gpeNET4_ALF;
        a = _a;
        n = _n;
        an = nAN;

		return *this;
	}
	gpcREStrs& equ( U1* pB, U4 _a, U4 _n ) {
		U8	nAN = _a*_n;
		if( nAN < 2 )
			return ( nAN ) ? (*this = (U8)*pB) : null();

		load( (U1*)pB, nAN * sizeof(*pB) );

		typ = gpeNET4_U11;
        a = _a;
        n = _n;
        an = nAN;

		return *this;
	}

	gpcREStrs& equ( U2* pB, U4 _a, U4 _n ) {
		U8	nAN = _a*_n;
		if( nAN < 2 )
			return ( nAN ) ? (*this = (U8)*pB) : null();

		load( (U1*)pB, nAN * sizeof(*pB) );

		typ = gpeNET4_U21;
        a = _a;
        n = _n;
        an = nAN;

		return *this;
	}
	gpcREStrs& equ( U4* pB, U4 _a, U4 _n ) {
		U8	nAN = _a*_n;
		if( nAN < 2 )
			return ( nAN ) ? (*this = (U8)*pB) : null();

		load( (U1*)pB, nAN * sizeof(*pB) );

		typ = gpeNET4_U41;
        a = _a;
        n = _n;
        an = nAN;

		return *this;
	}
	gpcREStrs& equ( U8* pB, U4 _a, U4 _n ) {
		U8	nAN = _a*_n;
		if( nAN < 2 )
			return ( nAN ) ? (*this = *pB) : null();

		load( (U1*)pB, nAN * sizeof(*pB) );

		typ = gpeNET4_U81;
        a = _a;
        n = _n;
        an = nAN;

		return *this;
	}

	gpcREStrs& equ( U1x4* pB, U4 _a, U4 _n ) {
		U8	nAN = _a*_n;
		if( nAN < 2 )
			return ( nAN ) ? (*this = *pB) : null();

		load( (U1*)pB, nAN * sizeof(*pB) );

		typ = gpeNET4_U14;
        a = _a;
        n = _n;
        an = nAN;

		return *this;
	}
	gpcREStrs& equ( U4x2* pB, U4 _a, U4 _n ) {
		U8	nAN = _a*_n;
		if( nAN < 2 )
			return ( nAN ) ? (*this = *pB) : null();

		load( (U1*)pB, nAN * sizeof(*pB) );

		typ = gpeNET4_U42;
        a = _a;
        n = _n;
        an = nAN;

		return *this;
	}

	gpcREStrs& equ( U4x4* pB, U4 _a, U4 _n ) {
		U8	nAN = _a*_n;
		if( nAN < 2 )
			return ( nAN ) ? (*this = *pB) : null();

		load( (U1*)pB, nAN * sizeof(*pB) );

		typ = gpeNET4_U44;
        a = _a;
        n = _n;
        an = nAN;

		return *this;
	}

	gpcREStrs& equ( I4x2* pB, U4 _a, U4 _n ) {
		U8	nAN = _a*_n;
		if( nAN < 2 )
			return ( nAN ) ? (*this = *pB) : null();

		load( (U1*)pB, nAN * sizeof(*pB) );

		typ = gpeNET4_I42;
        a = _a;
        n = _n;
        an = nAN;

		return *this;
	}

	gpcREStrs& equ( I4x4* pB, U4 _a, U4 _n ) {
		U8	nAN = _a*_n;
		if( nAN < 2 )
			return ( nAN ) ? (*this = *pB) : null();

		load( (U1*)pB, nAN * sizeof(*pB) );

		typ = gpeNET4_I44;
        a = _a;
        n = _n;
        an = nAN;

		return *this;
	}

	gpcREStrs& equ( F4* pB, U4 _a, U4 _n ) {
		U8	nAN = _a*_n;
		if( nAN < 2 )
			return ( nAN ) ? (*this = (F4)*pB) : null();

		load( (U1*)pB, nAN * sizeof(*pB) );

		typ = gpeNET4_F44;
        a = _a;
        n = _n;
        an = nAN;

		return *this;
	}
	gpcREStrs& equ( D4* pB, U4 _a, U4 _n ) {
		U8	nAN = _a * _n;

		load( (U1*)pB, nAN * sizeof(*pB) );

		typ = gpeNET4_D84;
        a = _a;
        n = _n;
        an = nAN;

		return *this;
	}



	gpcREStrs& EXPAND( const U4x2& xy )
	{
		if( !xy.sum() )
			return *this;

		U1	a16[0x10],
			*pKILL = NULL;
		if( !a )
			memcpy( a16, aU, *aT );
		else {
			if( xy.x < a && xy.y < n )
				return *this;
			pKILL = pDAT;
		}


		U8	nSRC = a*(*aT),	// ha reg volt akor 0*(*aT) azaz büdös agy ZERO
			nDST = (xy.x+1)*(*aT),
			nNEW = nDST*(xy.y+1);

		pDAT = new U1[nNEW];
		gpmZn(pDAT,nNEW);

		if( !nSRC )
			memcpy( pDAT, a16, *aT );
		else for( U1* pS = pKILL, *pD = pDAT, *pSe = pS + nSRC*n; pS < pSe; pS += nSRC, pD += nDST )
				gpmMEMCPY( pD, pS, nSRC );


		a = xy.x+1;
		n = xy.y+1;
		an = a*n;
		return *this;
	}

	U4x2 WH( void )
	{
		if( typ != gpeNET4_RES )
			return U4x2( a, n );

		U4	*pA = new U4[a+n],
			*pN = pA+a;
		*pA = 1;
		gpfMEMSET( pA+1, a+n-1, pA, sizeof(*pA) );
		gpcREStrs* pR = (gpcREStrs*)pDAT;
		U8 i = 0;
		U4x2 wh;
		for( U4 y = 0; y < n; y++ )
		{
			for( U4 x = 0; x < a; x++, i++ )
			{
				wh = pR[i].WH();
				if( wh.x < 1 )
					continue;

				if( pA[x] < wh.x )
					pA[x] = wh.x;

				if( pN[y] >= wh.y )
					continue;

				pN[y] = wh.y;
			}
		}
		wh = 0;
		for( i = 0; i < a; i++ )
			wh.x += pA[i];
		for( i = 0; i < n; i++ )
			wh.y += pN[i];

		delete[] pA;

		return wh;
	}
	U8 u8( U4x2 xy )
	{
		U8 o = 0;

		if( typ != gpeNET4_RES )
		{
			o = *(U8*)aU;
		}
		return o;
	}
	gpcREStrs& res2u8( void )
	{
		/// nincs befejezve
		if( typ == gpeNET4_RES )
		{
			gpcREStrs* pR = (gpcREStrs*)pDAT;
			U8 i = 0;
			U4x2 	wh = WH(),
					xy;

			U8	nNEW = wh.area(),
				*pU8 = new U8[nNEW], iDST = 0;
			i = 0;
			for( U4 y = 0, whx = wh.x, hy = 1; y < n; y++ )
			{
				for( U4 x = 0; x < a; x++, i++ )
				{
					wh = pR[i].WH();
					if( hy < wh.y )
						hy = wh.y;
					for( xy.y = 0; xy.y < wh.y; xy.y++ )
					for( xy.x = 0; xy.x < wh.x; xy.x++ )
					{
						pU8[iDST+(xy*U4x2(1,whx))] = pR[i].u8( xy );
					}
				}
				iDST += whx*hy;
				hy = 1;
			}
			null();

			typ = gpeNET4_U81;
			a = wh.x;
			n = wh.x;
			an = a*n;
			pDAT = (U1*)pU8;

			delete[] pR;
		}


		return *this;

 	}

	gpcREStrs& equAN( U4x2 xy, U8 u8 )
	{
		if( !typ )
			*this = (U8)0;

		EXPAND( xy );

		if( !a )
			return *this = u8;

		U8 yax = ((U8)xy.y)*a + xy.x;
        switch( typ )
        {
			case gpeNET4_U81:
				((U8*)pDAT)[yax] = u8;
				return *this;

			case gpeNET4_U84:
				((U8x4*)pDAT)[yax] = u8;
				return *this;


			case gpeNET4_D81:
				((double*)pDAT)[yax] = u8;
				return *this;
			case gpeNET4_RES:
				((gpcREStrs*)pDAT)[yax] = u8;
				return *this;
		}
											//0011223344556677
		if( u8 < 							0x8000000000000000 )
		{
			if( u8 < 						0x100000000 )
			{
				if( u8 < 					0x80000000 )
				{
					if( u8 < 				0x10000 )
					{
						if( u8 < 			0x8000 )
						{
							if( u8 < 		0x100 )
							{
								if( u8 < 	0x80 )
								{
									switch( typ )
									{
										case gpeNET4_I11:
											((I1*)pDAT)[yax] = u8;
											return *this;
										/*case gpeNET4_I14:
											((I14*)pDAT)[yax] = u8;
											return *this;*/
									}
								}

								switch( typ )
								{
									case gpeNET4_U11:
										((U1*)pDAT)[yax] = u8;
										return *this;
									case gpeNET4_U14:
										((U1x4*)pDAT)[yax] = u8; // ez jó mert 0x100 nál kissebb azaz kitörli a többit
										return *this;
								}
							}
							switch( typ )
							{
								case gpeNET4_I21:
									((I2*)pDAT)[yax] = u8;
									return *this;
							}
						}
						switch( typ )
						{
							case gpeNET4_U21:
								((U2*)pDAT)[yax] = u8;
								return *this;
						}
					}

					switch( typ )
					{
						case gpeNET4_I41:
							((I4*)pDAT)[yax] = u8;
							return *this;
						case gpeNET4_I42:
							((I4x2*)pDAT)[yax] = u8;
							return *this;
						case gpeNET4_I44:
							((I4x4*)pDAT)[yax] = u8;
							return *this;
					}
				}

				switch( typ )
				{
					case gpeNET4_U41:
						((U4*)pDAT)[yax] = u8;
						return *this;
					case gpeNET4_U42:
						((U4x2*)pDAT)[yax] = u8;
						return *this;
					case gpeNET4_U44:
						((U4x4*)pDAT)[yax] = (U4)u8;
						return *this;
					case gpeNET4_F41:
						((float*)pDAT)[yax] = u8;
						return *this;
					case gpeNET4_F44:
						((F4*)pDAT)[yax] = u8;
						return *this;
				}
			}

			switch( typ )
			{
				case gpeNET4_I81:
					((I8*)pDAT)[yax] = u8;
					return *this;

				case gpeNET4_I84:
					((I8x4*)pDAT)[yax] = u8;
					return *this;
			}
		}

		// nem talált neki helyet
		/// most legyen az egész RES?






		return *this;
	}

};
/*class gpcREStrs
{
	public:
		gpcREStrs();
		virtual ~gpcREStrs();
		gpcREStrs& operator=(const gpcREStrs& other);

	protected:

	private:
};*/

#endif // GPCRES_H
