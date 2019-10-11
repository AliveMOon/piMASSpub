
#include "gpcres.h"

gpcRES::gpcRES()
{
	gpmCLR;
}

gpcRES::~gpcRES()
{
	null();
}

gpcRES& gpcRES::operator = ( const gpcRES& b )
{
	if (this == &b )
		return *this; // handle self assignment

	if( !b.typ )
		return null();

	memcpy( this, &b, sizeof(b) );
	if( !a )
		return *this;

	switch( typ )
	{
		case gpeNET4_MAS:
			return null();

			/// ki van kapcsolva
			/// MASS
			if( an < 2 )
			{
				pDAT = (U1*)new gpcMASS( *((gpcMASS*)b.pDAT) );
				return *this;
			}

			pDAT = (U1*)new gpcMASS[an];
			for( gpcMASS* pR = (gpcMASS*)pDAT, *pRe = pR+an, *pS = (gpcMASS*)b.pDAT; pR < pRe; pR++, pS++ )
				*pR = *pS;

			return *this;

		case gpeNET4_RES:
			if( an < 2 )
			{
				pDAT = (U1*)new gpcRES( *((gpcRES*)b.pDAT) );
				return *this;
			}

			pDAT = (U1*)new gpcRES[an];
			for( gpcRES* pR = (gpcRES*)pDAT, *pRe = pR+an, *pS = (gpcRES*)b.pDAT; pR < pRe; pR++, pS++ )
				*pR = *pS;

			return *this;

		default:
			if(	!*aT )
				return null();

			pDAT = new U1[*aT * an];
			memcpy( pDAT, b.pDAT, *aT * an );
			return *this;
	}

	return *this;
}

/*gpcRES& gpcRES::operator = ( const gpcRES& b )
{
	if (this == &b ) return *this; // handle self assignment

	*this = b;
	return *this;
}*/
