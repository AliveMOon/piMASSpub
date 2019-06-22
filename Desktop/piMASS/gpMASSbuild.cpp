#include "gpcSRC.h"

void gpcSRC::hd( gpcMASS& mass, gpeALF* pTGpub )
{
	if( nHD == nVER )
		return;

	U1* pS = pA;
	U8	oSW = bSW;
	bSW &= ~gpeMASSclrMSK;
	gpeALF alf, *pTGdie = pTG;
	U4 nX, nTGdie = nTG;

	pTG = NULL;
	nTG = 0;

	while( pB-pS )
	{
		pS += gpmNINCS( pS, " \t\r\n" );
		if( pB <= pS )
			break;
		nX = *pS = '#';
		if( nX )
		{
			pS++;
			if( pB <= pS )
				break;

			nX += gpmNINCS( pS, "#" );
			if( pB <= pS+nX )
				break;
			pS += nX;
			alf = gpfSTR2ALF( pS, pB, &pS );
			if( alf < gpeALF_A )
				continue;

			if( !pTGpub )
				pTGpub = mass.aTGwip;

			for( U4 i = 0; i < nTG; i++ )
			{
				if( pTGpub[i] != alf )
					continue;

				alf = gpeALF_zero;
				break;
  			}
  			if( alf < gpeALF_A )
				continue; // már van a listában

			if( pTGdie )
			for( U4 i = 0; i < nTGdie; i++ )
            {
				if( pTGdie[i] != alf )
					continue;
				nTGdie--;
				if( i == nTGdie )
				{
					nTGdie;
					gpmDELary(pTGdie);
				}
				pTGdie[i] = pTGdie[nTGdie];
				i--;
            }

			pTGpub[nTG] = alf;
            nTG++;
			continue;
		}

		alf = gpfSTR2ALF( pS, pB, &pS );
		switch( alf )
		{
			case gpeALF_SUB:
				bSW |= gpeMASSsubMSK;
				continue;
			case gpeALF_RET:
				bSW |= gpeMASSretMSK;
				continue;
			case gpeALF_ENTER:
				bSW |= gpeMASSentrMSK;
				continue;
			case gpeALF_UNSEL:
				bSW |= gpeMASSunselMSK;
				continue;
			case gpeALF_INP:
				bSW |= gpeMASSinpMSK;
				continue;
			case gpeALF_PASS:
				bSW |= gpeMASSpassMSK;
				continue;
			case gpeALF_OFF: // keret kikapcsolás
				bSW |= gpeMASSoffMSK;
				continue;
			case gpeALF_CRSL: // bal oldali cursor jele				bSW |= gpeMASScrslMSK;
				continue;
			case gpeALF_CRSR: // jobb oldali cursor jele
				bSW |= gpeMASScrsrMSK;
				continue;
		}
	}

	if( pTGdie )
	{
		for( U4 i = 0; i < nTGdie; i++ )
		{
			if( pTGdie[i] < gpeALF_zero )
				continue;
			// kiszedni a teg listából az SRC-t
			mass.tg_sub( pTGdie[i], IX );
		}
		gpmDELary(pTGdie);
	}

	if( pTGpub )
	{
		pTG = new gpeALF[nTG+1];
		nTGdie = nTG;
		for( U4 i = nTG = 0; i < nTGdie; i++ )
		{
			if( pTGpub[i] < gpeALF_zero )
				continue;
			pTG[nTG] = pTGpub[i];
			// betenni a teg listába az SRC-t

			nTG++;
		}
		if( !nTG )
			gpmDELary(pTG);
		gpmDELary(pTGdie);
	}

	nHD = nVER;
}
void gpcSRC::build( gpcMASS& mass )
{
	hd( mass );
	U1* pS = pB;
	while( pS-pA < nL )
	{

	}
	nBLD = nVER;
}
