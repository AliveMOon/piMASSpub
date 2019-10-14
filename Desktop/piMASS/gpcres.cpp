
#include "gpcres.h"
extern U1 gpaALFadd[];


gpcIS::gpcIS( gpcRES* pM )
{
	gpmCLR;
	pMOM = pM;
}
gpcIS::~gpcIS()
{
	null();
}

gpcRES* gpcRES::compiAN( U1* pS, U1* pE, gpcLZYdct* pDICT )
{
	if( !this )
	{
		gpcRES* pTHIS = new gpcRES;
		if( !this )
			return NULL;

		return pTHIS->compiAN( pS, pE, pDICT );
	}
	null();
	U4x4 xyWH = 0;
	U4 deep = 0;
	U1 *pU = pS, d;
	U8 nLEN;

	for( pS += gpmNINCS( pS, " \t\r\n" ); pS < pE ? *pS : false; pS += gpmNINCS( pS, " \t\r\n" ) )
	{
		pS++;
		switch( pS[-1] ) {
			case ',': {	// vessző OSZLOPOK
					if( deep )
						break;

					xyWH.x++;
					if( xyWH.z >= xyWH.x )
						break;
					// bővíteni kell
					xyWH.z = xyWH.x;
				} break;
			case ';': {	// SOROK
					if( deep )
						break;

					xyWH.x = 0;
					xyWH.y++;
				} break;



			case ')': {
						if( !d )
						{
							pE = pS;
							break;
						}
						if( d != '(' )
							break;

						if( deep )
						{
							deep--;
							if( deep )
								break;

							d = 0;
						} else
							pE = pS;
						break;
					}
			case '}': {
						if( !d ) {
							pE = pS;
							break;
						}

						if( d != '{' )
							break;

						if( deep ) {
							deep--;
							if( deep )
								break;

							d = 0;
						} else
							pE = pS;
						break;
					}
			case ']': {
						if( !d ) {
							pE = pS;
							break;
						}

						if( d != ']' )
							break;

						if( deep ) {
							deep--;
							if( deep )
								break;

							d = 0;
						} else
							pE = pS;
						break;
					}
			case '(':
			case '{':
			case '[': {
					if( !d )
					{
						d = pS[-1];
						deep = 1;
						break;
					}

					if( d == pS[-1] )
							deep++;

				} break;



			case '/': {
					switch( *pS ) {
						case '*':
							pS = (U1*)strstr( (char*)pS+1, "*/" );
							if( pS ? (pS >= pE) : true )
							{
								pS = pE;
								break;
							}

							pS += 2;
							break;

						case '/':
							pS += gpmVAN( pS, "\n", nLEN );
							break;
					}
				} break;
			case '\"': {
					pS += gpmVAN( pS, "\"", nLEN );
					if( *pS == '\"' )
					{
						pS++;
						break;
					}
					pS = pE;
				} break;
			case '\'': {
					pS += gpmVAN( pS, "\'", nLEN );
					if( *pS == '\'' )
					{
						pS++;
						break;
					}
					pS = pE;
				} break;
		}
	}

	if( pE > pS )
		pE = pS;

	pS = pU;



	return this;
}


gpcREStrs* gpcREStrs::REScompiAN( U1* pS, U1* pE, U4* pMAP, gpcLZYdct* pDICT )
{
	if( !this )
	{
		return new gpcREStrs( pS, pE );
	}
	null();

	U4x4 xyWH = 0;
	I4x4 nOP = 0;
	U4 deep = 0;
	U1* pU = pS, *pB, d;
	U8 nLEN;
	*pMAP = 0;
	U4 nM = 1;
	for( pS += gpmNINCS( pS, " \t\r\n" ); pS < pE ? *pS : false; pS += gpmNINCS( pS, " \t\r\n" ) )
	{
		pS++;
		switch( pS[-1] ) {
			case ',':	// vessző OSZLOPOK
				if( deep )
					break;

				xyWH.x++;
				if( xyWH.z >= xyWH.x )
					break;
				// bővíteni kell
				xyWH.z = xyWH.x;
				break;

			case ';':	// SOROK
				if( deep )
					break;

				xyWH.x = 0;
				xyWH.y++;
				break;

			case ')': {
						if( !d )
						{
							pE = pS;
							break;
						}
						if( d != '(' )
							break;

						if( deep )
						{
							deep--;
							if( deep )
								break;

							d = 0;
						} else
							pE = pS;
						break;
					}

			case '}': {
						if( !d )
						{
							pE = pS;
							break;
						}

						if( d != '{' )
							break;

						if( deep )
						{
							deep--;
							if( deep )
								break;

							d = 0;
						} else
							pE = pS;
						break;
					}

			case ']': {
						if( !d )
						{
							pE = pS;
							break;
						}

						if( d != ']' )
							break;

						if( deep )
						{
							deep--;
							if( deep )
								break;

							d = 0;
						} else
							pE = pS;
						break;
					}

			case '(':
			case '{':
			case '[':
				if( !d )
				{
					d = pS[-1];
					deep = 1;
					break;
				}

				if( d == pS[-1] )
						deep++;

				break;

			case '/':
				switch( *pS )
				{
					case '*':
						pS = (U1*)strstr( (char*)pS+1, "*/" );
						if( pS ? (pS >= pE) : true )
						{
							pS = pE;
							break;
						}

						pS += 2;
						break;

					case '/':
						pS += gpmVAN( pS, "\n", nLEN );
						break;
				}
				break;
			case '\"':
				pS += gpmVAN( pS, "\"", nLEN );
				if( *pS == '\"' )
				{
					pS++;
					break;
				}
				pS = pE;
				break;
			case '\'':
				pS += gpmVAN( pS, "\'", nLEN );
				if( *pS == '\'' )
				{
					pS++;
					break;
				}
				pS = pE;
				break;
		}
	}

	if( pE > pS )
		pE = pS;

	pS = pU;

    xyWH.a4x2[1] += 1;

    a = xyWH.a4x2[1].x;
    n = xyWH.a4x2[1].y;
	an = a*n;

	xyWH.a4x2[0] = 0;

	typ = gpcNET4_NULL;

	bool bABC;
	U1 c, *pBEGIN;
	U8 nNINCS, nVAN, u8;
	I8 i8;
	double d8;

	for( pS += gpmNINCS( pS, " \t\r\n" ); pS < pE ? *pS : false; pS += gpmNINCS( pS, " \t\r\n" ) )
	{
		c = *pS;
		pS++;

		switch( c )
		{


			case ')': {
						if( !d )
						{
							pE = pS;
							break;
						}
						if( d != '(' )
							break;

						if( deep )
						{
							deep--;
							if( deep )
								break;


							pBEGIN = NULL;
							d = 0;
							nOP = 0;
						} else
							pE = pS;
						break;
					}

			case '}': {
						if( !d )
						{
							pE = pS;
							break;
						}

						if( d != '{' )
							break;

						if( deep )
						{
							deep--;
							if( deep )
								break;

							pBEGIN = NULL;
							d = 0;
							nOP = 0;
						} else
							pE = pS;
						break;
					}

			case ']': {
						if( !d )
						{
							pE = pS;
							break;
						}

						if( d != '[' )
							break;

						if( deep )
						{
							deep--;
							if( deep )
								break;


							pBEGIN = NULL;
							d = 0;
							nOP = 0;
						} else
							pE = pS;
						break;
					}

			case '(':
			case '{':
			case '[':
				if( !d )
				{
					pBEGIN = pS;
					d = c;
					deep = 1;
					break;
				}

				if( d == c )
						deep++;

				break;


			case '\"':
				pS += gpmVAN( pS, "\"", nLEN );
				if( *pS == '\"' )
				{
					pS++;
					break;
				}
				pS = pE;
				break;
			case '\'':
				pS += gpmVAN( pS, "\'", nLEN );
				if( *pS == '\'' )
				{
					pS++;
					break;
				}
				pS = pE;
				break;


			case ',':	// vessző OSZLOPOK
				if( deep )
					break;

				nOP = 0;
				xyWH.x++;
				if( xyWH.z >= xyWH.x )
					break;
				// bővíteni kell
				xyWH.z = xyWH.x;
				break;

			case ';':	// SOROK
				if( deep )
					break;

				nOP = 0;
				xyWH.x = 0;
				xyWH.y++;
				break;

			case '+':
				if( !deep )
					 nOP.x++;
				break;
			case '-':
				if( !deep )
					 nOP.x--;
				break;

			case '/':
				switch( *pS )
				{
					case '*':
						pS = (U1*)strstr( (char*)pS+1, "*/" );
						if( pS ? (pS >= pE) : true )
						{
							pS = pE;
							break;
						}

						pS += 2;
						break;

					case '/':
						pS += gpmVAN( pS, "\n", nLEN );
						break;
				}
				break;

			default:
				if( deep )
					break;

				pB = pS-1;
				u8 = gpfSTR2U8( pS-1, &pS );
				if( *pS == '.' )
				{
					d8 = gpmSTR2D( pS ) + u8;
					pS += gpmNINCS( pS, ".0123456789fF" );

                    equAN( xyWH.a4x2[0], nOP.x < 0 ? -d8 : d8 );
					break;
				}

				if( pB == pS )
				{
					// visza lépet mert egyetlen számot sem talált

					// és nem is azért mert '.' azaz pöst
					// ? nem valós számot szeretett volna a felhasználó
					pS++;
					break;
				}

				if( nOP.x > -1 )
				{
					equAN( xyWH.a4x2[0], u8 );
					break;
				}

				i8 = u8;
				equAN( xyWH.a4x2[0], -i8 );
				break;

		}

		continue;

		/// off ----------------------

		if( bABC = gpmbABC( pS[-1], gpaALFadd) ) {

		}



	}


	return this;
}

gpcREStrs::gpcREStrs(  U1* pS, U1* pE )
{
	gpmCLR;
	if( pE > pS ? !*pS : true )
	{
		return;
	}

	REScompiAN( pS, pE );

}


gpcREStrs::gpcREStrs()
{
	gpmCLR;
}

gpcREStrs::~gpcREStrs()
{
	null();
}

gpcREStrs& gpcREStrs::operator = ( const gpcREStrs& b )
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
				pDAT = (U1*)new gpcREStrs( *((gpcREStrs*)b.pDAT) );
				return *this;
			}

			pDAT = (U1*)new gpcREStrs[an];
			for( gpcREStrs* pR = (gpcREStrs*)pDAT, *pRe = pR+an, *pS = (gpcREStrs*)b.pDAT; pR < pRe; pR++, pS++ )
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

/*gpcREStrs& gpcREStrs::operator = ( const gpcREStrs& b )
{
	if (this == &b ) return *this; // handle self assignment

	*this = b;
	return *this;
}*/
