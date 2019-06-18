#ifndef GPCSRC_H
#define GPCSRC_H

#include "piMASS.h"

inline U4 gpfUTF8( const U1* pU, U1** ppU )
{
	U4 utf8 = *pU;
	if( !(utf8&0x80) )
	{
		if( ppU )
			(*ppU) = ((U1*)pU) + (!!utf8);
		return utf8;
	}

	if( (utf8&0xc0) == 0x80 )
	{
		// azaz hibás 0x40-nek igaznak kéne lenie
		while( (utf8&0xc0) == 0x80 )
		{
			pU++;
			utf8 = *pU;
			if( !utf8 )
			{
				if( !ppU )
					return 0;

				*ppU = (U1*)pU;
				return 0;
			}
		}

		if( !ppU )
			return '.';

		*ppU = (U1*)pU;
		return '.';
	}

	U4	u0 = utf8<<1, n = 0;
	U4	static const aADD[] = { 0,0x80, 0x800, 0x10000 };

	while( u0&0x80 )
	{
		n++;
		u0 <<= 1;
		utf8 <<= 6;
		utf8 |= pU[n]&0x3f;
	}
	utf8 += aADD[n];
	if( !ppU )
		return utf8;
	pU += n+1;
	*ppU = (U1*)pU;
	return utf8;
}
inline U8 gpfVAN( const U1* pU, const U1* pVAN, U8& nLEN )
{
	nLEN = 0;
	if( pU ? !*pU : true )
		return 0;

	U1 *pN = (U1*)pU;
	if( !pVAN )
	{
		// strlen!!
		while( *pN )
		{
			if( (*pN&0xc0) != 0x80 )
				nLEN++;	// csak a 0x80 asokat nem számoljuk bele mert azok tötike karakterek
			pN++;
		}
		return pN-pU;
	}
	U8 nVAN = gpfVAN( pVAN, NULL, nLEN ); // utf8len
	bool abVAN[0x80];
	gpmZ( abVAN );
	*abVAN = true;

	if( nVAN == nLEN )
	{
		//van-ban nincsen UTF8 akkor turbó
		for( U8 v = 0; v < nLEN; v++ )
		{
			abVAN[pVAN[v]] = true;
		}
		while( !abVAN[*pN] )
			pN++;

		return nLEN = pN-pU;
	}
	U4 u4, nT = 0;
	U1	*pV = (U1*)pVAN,
		*pE = pV+nVAN;
	gpcLAZY* pTREE = NULL;
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
	while( *pN )
	{
		u4 = gpfUTF8( pN, &pN );
		if( u4 < 0x80 )
		if( abVAN[u4] )
				break;

		if( pTREE->tree_fnd(u4, nT) < nT )
			break;

		nLEN++;
	}

	return pN-pU;
}
I8 inline gpfSRC2I8( U1* p_str, U1** pp_str = NULL )
{
	if( !p_str )
		return 0;
	U8 nLEN;
    p_str += gpfVAN(p_str, (U1*)"+-0123456789xXbBdD", nLEN );
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
    U4	nA;
    gpcSRC();
    virtual ~gpcSRC();

protected:
private:
    gpcSRC(const gpcSRC& other){}
    gpcSRC& operator=(const gpcSRC& b)
    {
        if( this == &b )
            return *this; // handle self assignment
        //assignment operator
        return *this;
    }
};

#endif // GPCSRC_H
