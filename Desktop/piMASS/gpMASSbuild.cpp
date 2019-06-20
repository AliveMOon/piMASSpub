#include "gpcSRC.h"

void gpcSRC::hd( void )
{
	if( nHD == nVER )
		return;

	U1* pS = pA;
	bSW &= ~gpeMASSclrMSK;
	gpeALF alf;
	while( pB-pS )
	{
		pS += gpmNINCS( pS, " \t\r\n" );
		if( pB <= pS )
			break;
		alf = gpfSTR2ALF( pS, pB, &pS );
		switch( alf )
		{
			case gpeALF_ENTER:
				bSW |= gpeMASSentrMSK;

		}
	}

	nHD = nVER;
}
void gpcSRC::build( void )
{
	hd();
	U1* pS = pB;
	while( pS-pA < nL )
	{

	}
	nBLD = nVER;
}
