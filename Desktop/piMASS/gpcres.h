#ifndef GPCRES_H
#define GPCRES_H

#include "piMASS.h"
#include "gpcSCHL.h"
#include "gpcOPCD.h"
#include "gpcSRC.h"
class gpcRES;

class gpcIS
{
	gpcRES* pMOM;
public:
	void*	pDAT;
	gpeALF 	alf;
	U1x4	typ;
	U4x4	AN;

	gpcIS( gpcRES* pM = NULL );
	~gpcIS();

	gpcIS( const gpcIS& b )
	{
		gpmCLR;
		*this = b;
	}
	gpcIS* null( gpcRES* pM = NULL )
	{
		if( !this )
			return this;
        if( pMOM == pM ? pDAT : NULL )
			delete[] pDAT;

        gpmCLR;
        return this;
	}
	U4 nALL()
	{
		AN.z = AN.a4x2[0].area();
		if( !typ.w )
			typ.w = typ.area_yz()*(typ.x&0xf);

		AN.w = AN.z;
		if( typ.w > 1 )
			AN.w *= typ.w;

		return AN.w;
	}
	gpcIS& operator = ( const gpcIS& b )
	{
		if( this == &b )
			return *this;

		null();

		if( !&b )
			return *this;

        gpmMEMCPY( this, &b, sizeof(*this) );
		pMOM = NULL;
		if( !pDAT )
		{
			return *this;
		}

		pDAT = new U1[nALL()];
        memcpy( pDAT, b.pDAT, AN.w );

        return *this;
	}

};

class gpcRES
{
	gpeALF	*pID;
	U1x4	*pTYP;
	U4x4	*pAN;
	U8x4	*pTREE;
	U4		*pTx;
	void**	ppDAT;
	U4		n, t, i, ig;
	gpcIS*	pIS;

public:
	gpcRES& null()
	{
		if( ppDAT )
		for( U4 j = 0; j < n; j++ )
		{
			if( !ppDAT[j] )
				continue;
			pGET( j )->null( this );
		}
        gpmDELary( pID );
		gpmDELary( pTYP );
		gpmDELary( pAN );
		gpmDELary( pTREE );
		gpmDELary( pTx );
		gpmDELary( pIS );
	}
	gpcRES()
	{
        gpmCLR;
	}
	~gpcRES()
	{
		null();
	}
	gpcRES* compiAN( U1* pS, U1* pE, gpcLZYdct* pDICT = NULL );

	gpcIS* pGET( U4 _i )
	{
		if( this ? (_i >= n) : true )
			return NULL;

		if( !pIS )
		{
			pIS = new gpcIS( this );
			if( !pIS )
				return NULL;
		}

		if( ppDAT ? ppDAT[_i] : NULL )
		if( pIS->pDAT == ppDAT[_i] )
		{
			return pIS;
		}

		pIS->alf	= pID ? pID[_i] : gpeALF_null;
		pIS->typ	= pTYP ? pTYP[_i] : 0;
		pIS->AN 	= pAN ? pAN[_i] : 0;
		pIS->pDAT	= ppDAT ? ppDAT[_i] : NULL;

		return pIS;
	}
    U4 iGET( gpeALF alf )
    {
		if( !this )
			return 0;

		if( !n )
			return n;

		if( i < n )
		if( pID[i] == alf )
			return i;

		ig = n;
		if( pTREE )
		{
			ig = pTREE->tree_fnd( alf, t );
			if( ig < t )
				return i = pTx[ig];

			for( U4 j = 0, x; j < n; j++ )
			{
				if( !pID[j] )
					continue;

				if( ig >= n )
				if( pID[j] == alf )
				{
					// azt remélem ettől, hogy amit gyakrabban keresnek az elöbre kerüljön
					pTx[t] = ig = j;
					x = pTREE->tree_fnd( pID[j], t );
					if( x >= t )
						t = pTREE->tree_add( pID[j], t );		// nem volt benne a listában
					break;
				}

				x = pTREE->tree_fnd( pID[j], t );
				if( x < t )
					continue;

				pTx[t] = j;
				t = pTREE->tree_add( pID[j], t );
			}

			if( ig < n )
				i = ig;	// megtalálta a ciklusban

			return ig;
		}

		pTREE	= new U8x4[n];
		pTx		= new U4[n];
		gpmZn( pTREE, n );
		t = 0;
		for( U4 j = 0; j < n; j++ )
		{
			if( !pID[j] )
				continue;

			if( ig >= n )
			if( pID[j] == alf )
			{
				pTx[t] = ig = j;
				t = pTREE->tree_add( pID[j], t );
				break; // azt remélem ettől, hogy amit gyakrabban keresnek az elöbre kerüljön
			}

			pTx[t] = j;
			t = pTREE->tree_add( pID[j], t );

		}
		if( ig < n )
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
