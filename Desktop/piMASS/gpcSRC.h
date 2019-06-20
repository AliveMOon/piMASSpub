#ifndef GPCSRC_H
#define GPCSRC_H

#include "piMASS.h"
enum gpeMASSsw:U8
{
	gpeMASSzero,
	gpeMASSsub = gpeMASSzero,
	gpeMASSret,
	gpeMASSentr,
	gpeMASSunsel,
	gpeMASSinp,
	gpeMASSpass,

	gpeMASSalert,

	gpeMASSsubMSK = 1<<gpeMASSsub,
	gpeMASSretMSK = 1<<gpeMASSret,
	gpeMASSentrMSK = 1<<gpeMASSentr,
	gpeMASSunselMSK = 1<<gpeMASSunsel,
	gpeMASSinpMSK = 1<<gpeMASSinp,
	gpeMASSpassMSK = 1<<gpeMASSpass,
	gpeMASSclrMSK = (1<<gpeMASSalert)-1,
	gpeMASSalertMSK,

};
inline U4 gpfUTF8( const U1* pS, U1** ppS )
{
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
inline U8 gpfVAN( const U1* pU, const U1* pVAN, U8& nLEN, bool bDBG = false )
{
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
	gpcLAZY* pTREE = NULL;
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
#define gpmVAN( d, v, l ) gpfVAN( (d), (U1*)v, l )
I8 inline gpfSRC2I8( U1* p_str, U1** pp_str = NULL )
{
	if( !p_str )
		return 0;
	U8 nLEN;
    p_str += gpmVAN(p_str, "+-0123456789xXbBdD", nLEN );
	I8 i8 = strtol( (char*)p_str, (char**)&p_str, 10 );
	if( !i8 )
	{
		switch( *p_str )
		{
			case 'x':
			case 'X':
				p_str++;
				i8 = strtol( (char*)p_str, (char**)&p_str, 16 );
				break;
			case 'b':
			case 'B':
				p_str++;
				i8 = strtol( (char*)p_str, (char**)&p_str, 2 );
				break;
			case 'd':
			case 'D':
				p_str++;
				i8 = strtol( (char*)p_str, (char**)&p_str, 10 );
				break;
		}
	}
	if( pp_str )
		*pp_str = p_str;
	return i8;
}
class gpcSRC
{
public:
    U1  *pA, *pB;
    U8	nL, nA, bSW;
    U44	space;
    U4	retIX, nTG;
    gpeALF* pTG;

    bool qBLD( void )
    {
		if( nVER > nBLD )
			return true;	// már kérte valaki

		nVER = max( nHD, nBLD )+1;
		return false; // mi kérjük elösször
    }

    bool bSUB( gpcMASS& mass )
    {
		if( !this )
			return false;

		hd( mass );

		// beljebb lép
		return bSW&gpeMASSsubMSK;
    }
    bool bRET( gpcMASS& mass )
    {
		if( !this )
			return false;

		hd( mass );

		// kijebb lép
		return bSW&gpeMASSretMSK;
    }
    bool bENTR( gpcMASS& mass )
    {
		if( !this )
			return false;

		hd( mass );

		// új sort kezd a táblázatban
		return bSW&gpeMASSentrMSK;
    }
    bool bUNsel( gpcMASS& mass )
    {
		if( !this )
			return false;

		hd( mass );

		// pointerrel ne lehessen kijelölni
		// pl. rajzolásnál hasznos, meg gombnál
		return bSW&gpeMASSunselMSK;
    }

	bool bIN( gpcMASS& mass )
    {
		if( !this )
			return false;

		// input rublika
		hd( mass );

		return bSW&gpeMASSinpMSK;
    }

    bool bPASS( gpcMASS& mass )
    {
		if( !this )
			return false;

		hd( mass );

		// csillagokat kell írni a betű helyett?

		return bSW&gpeMASSpassMSK;
    }
    bool bALERT( void )
    {
		if( !this )
			return false;

		return bSW&gpeMASSalertMSK;
    }
    void hd( gpcMASS& mass, gpeALF* pTGpub = NULL );
    void build( gpcMASS& mass );

    gpcSRC();
    virtual ~gpcSRC();
	U8 iB( void )
	{
		if( !pA || !nL )
			return 0;

		if( pB < pA )
			pB = pA;

		U8 i = pB-pA;
		if( i )
		if( i >= nL )
		{
			if( i == nL )
				return nL; // ezzel jelezük, ha valaki már kereste
							// azaz nem 0

			pB = ( pA ? pA+nL : NULL );
			return nL;
		}

		if( pB[i] == '\a' )
			return i;	// ez a fasza

		// még van esély keressük meg, hátha há
		U8 nLEN;
		i = gpfVAN( pA, (U1*)"\a", nLEN );
		if( i )
		if( i >= nL )
		{
			if( i == nL )
				return nL;	// i = nL -> ezzel jelezük, hogy kerestük de biza nem vót // szar

			pB = ( pA ? pA+nL : NULL );
			return nL;
		}

		// na meguszta talált egyet
		pB = pA+i;
		return i;
	}
	gpcSRC& reset( U1* pC, U1* pE, U1** ppSRC );
    gpcSRC( gpcSRC& B );
    gpcSRC& operator = ( gpcSRC& B );
protected:
    U4	nVER, nBLD, nHD;
private:
};

class gpcMASS
{
	gpcLAZY	*pSRCc,
			*pSRCl;
	U4		nLST, xFND, nALLOC, nSP,
			aSPix[0x100];
	gpcSRC	*pFND,
			*apSP[0x100];
	U44 	aSP44[0x100];

	gpcSRC** ppSRC( void )
	{
		return (gpcSRC**)(pSRCc ? pSRCc->p_alloc : NULL);
	}
public:
	gpeALF	aTGwip[0x100];
	gpcMASS( const U1* pU, U8 nU );
	virtual ~gpcMASS();

	gpcSRC* fnd( U4 xfnd )
	{
		if(!this)
			return NULL;

		if( nLST )
		if( xFND == xfnd )
			return pFND;

		U4 iFND = pSRCl->tree_fnd(xfnd, nLST);
		if(iFND >= nLST)
			return NULL;

		gpcSRC** ppS = ppSRC();
		if( ppS ? !ppS[iFND] : true )
			return pFND;

		pFND = ppS[iFND];
		xFND = xfnd;

		return pFND;
	}

	gpcSRC* add( gpcSRC* pSRC, U4 xfnd, U4& is, U4& n );

};

#endif // GPCSRC_H
