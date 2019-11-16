#include "gpcpic.h"
#include "gpcSRC.h"

U4 gpcPICall::iFND( U1* pS )
{
	I8x2 alfN(0,14);
	alfN = pS;
	alfN.num = gpfSTR2U8( pS+alfN.num, &pS );

	for( U4 p = 0; p < nPICld; p++ )
	{
		if( !ppPIC[p] )
		{
			if( iPICfr > p )
				iPICfr = p;
			continue;
		}

		if( ppPIC[p]->alfN != alfN )
			continue;

		return p;
	}
	return nPICld;
}
