#include "gpcSRC.h"
extern U1 gpaALFsub[];

I8x2& I8x2::operator = ( const U1* pS )
{
	if( num ? ( pS ? !*pS : true ) : true )
		return null();
	if( num > 14 )
		num = 14;
	U1* pE = (U1*)pS + num;
	alf = gpfSTR2ALF( pS, pE, &pE );
	num = pE-pS;
	return *this;
}
I8x2& I8x2::operator = ( const char* pS )
{
	return *this = (U1*)pS;
}
U1 I8x2::cdrMILL( const char* pS, U4 nS )
	{
		num = nS;
		*this = pS;
		char *pSi = (char*)pS+num, *pSe;
		U1 typ = alf ? 0x10:0;
		if( typ ? (num >= nS) : true )
			return typ;

		num = gpfSTR2I8( pSi, &pSe );
		if(pSe > pSi)
			typ |= 0x20;

		if( pSe-pS < nS )
		if( *pSe == 'x' || *pSe == 'X' )
		{
			// vector
			switch( pSe[1] )
			{
				case '2':
					typ |= 1;
					break;

				case '3':
				case '4':
					typ |= 2;
					break;

				case '5':
				case '6':
				case '7':
				case '8':
					typ |= 3;
					break;

				case '9':
				case 'a':
				case 'b':
				case 'c':
				case 'd':
				case 'e':
				case 'f':
				case 'A':
				case 'B':
				case 'C':
				case 'D':
				case 'E':
				case 'F':
					typ |= 4;
					break;

				default:
					break;
			}
		}
		return typ;
	}
