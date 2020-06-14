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
