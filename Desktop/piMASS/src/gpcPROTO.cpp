
#include "gpcSRC.h"
extern U1 gpaALFadd[];
extern U1 gpsSTRpub[]; //[0x10000];
extern char gpsHD[]; //[0x1000];
extern char	gpsTAB[];
extern char *gppTAB;
//extern
;
extern char gpsNDAT[];
extern char gpsPRG[];



void gpcSRC::cmpi_undo00( gpcMASS& mass, bool bDBG )
{
	hd( mass );

	char *psHD = gpsHD;
	psHD[0] = 0;
	psHD += sprintf( 	psHD, "\r\nComPI: %s V:%d H:%d B:%d \r\nbSW:0x%0.8x",
						spcZN.str( psHD+0x100), nVER, nHD, nBLD,
						bSW );
	if( psHD > gpsHD )
	{
		cout << gpsHD;
		psHD = gpsHD;
		psHD[0] = 0;
	}
	cout << "." << endl ;

	U1	*pPUB = mass.msRST( gpsSTRpub ),
		*pS = *pB == '\a' ? pB+1 : pB,
		*pSe, *pE = pA+nL, *pSTR = pPUB, c, nADD = 0;



	U4	nSTR = gpfALF2STR( (char*)pSTR, (I8) (nALFtg ? pALFtg[0] : gpeALF_PRG) ),
		iTHIS = mass.PC.cmpl_find( &mass.CMPL, pSTR, nSTR ), iFND, iR;
	U8 nLEN, nVAN, nALF;
	gpcCMPL *pMOM = mass.piLEVmom(), *pTHIS = pMOM, *pFND = NULL, aR[8];
	gpmZ(aR);

	if( pMOM->iPC != iTHIS )
	{
		iTHIS = mass.CMPL.nPC();
		pMOM->cmpl_add( &mass.CMPL, pSTR, nSTR );
		pTHIS = mass.CMPL.pPC( iTHIS );
		pTHIS->typ = pTHIS->wip = gpeALF_PRG;
	}
	mass.incLEV();
	char	*pCOUT = NULL, sVAN[] = ".";
	bool bABC;
	cout << endl << "ASM:nP:lv tp dc.n iP\r\n";

	for( pS += gpmNINCS( pS, " \t\r\n" ); pS < pE; pS += gpmNINCS( pS, " \t\r\n" ) )
	{

		if( pFND )
		{
			pCOUT = pFND->sLOG( pPUB, NULL, gpsNDAT, &mass.CMPL );
			pFND = NULL;
		} else
			pCOUT = NULL;

		if( pCOUT )
			cout << "ASM:" << pCOUT << "\r\n";

		c = *pS;
		if( bABC = gpmbABC(c, gpaALFadd) )
        {
			// UTF8!
			nALF = gpfABCnincs( pSTR = pS, pE, nLEN, gpaALFadd );
			if( nALF )
			{
				nSTR = nALF + gpmVAN( pSTR+nALF, gpsPRG, nVAN );
				nLEN += nVAN;

				pSTR = gpmSTRnCPY( pPUB, pSTR, nSTR );
				pPUB += nSTR;
				pS += nSTR;

				iFND = pMOM->cmpl_find( &mass.CMPL, pSTR, nSTR );
				if( !iFND )
				{
					/// NEMTALÁLT
					continue;
				}
				pFND = mass.CMPL.pPC( iFND );
			}
			continue;
		}

		iFND = pMOM->cmpl_best( &mass.CMPL, pS, pE-pS );
		pFND = mass.CMPL.pPC( iFND );
		if( pFND->wip == gpeALF_OPER )
		{
			pS += pFND->n_str;
			switch( pFND->typ )
			{
				case gpeALF_COMS: // "/*"
					if( U1* pCOM = (U1*)strstr( (char*)pS, "*/" ) )
					{
						pS = pCOM+2;
					} else {
						pS = pE;
					}
					pFND = NULL;
					continue;
				case gpeALF_COM: // "//"
					pS += gpmVAN( pS, "\r\n", nLEN );
					pFND = NULL;
					continue;
				default:
					break;
			}
			continue;
		}

		if( (c < 0x30) || (c >= 0x3A)  )
		{
			sVAN[0] = c;
			pS += gpmNINCS( pS, sVAN );
			pFND = NULL;
			continue;
		}

		pFND = aR+(iR%gpmN(aR));
		pFND->iKD = iR%gpmN(aR);
		iR++;

		pFND->wip = gpeALF_REG;
		pFND->u8 = gpfSTR2U8( pS, &pS );
		if( *pS == '.' )
		{
			pFND->typ = gpeALF_D;
			pFND->d = gpmSTR2D( pS ) + (double)pFND->u8;
		}
		else if( nADD < 0 )
		{
			pFND->typ = gpeALF_I;
			pFND->i8 = -pFND->u8;							///
		} else
			pFND->typ = gpeALF_U;


	}

	cout << endl << "alDAT:" << mass.alDAT << "BLD/VER:"<< nBLD << nVER;
	cout << endl;
	nBLD = nVER;
}
void gpcSRC::cmpi_SKELETON( gpcMASS& mass, bool bDBG )
{

	hd( mass );

	char *psHD = gpsHD;
	psHD[0] = 0;
	psHD += sprintf( 	psHD, "\r\nComPI: %s V:%d H:%d B:%d \r\nbSW:0x%0.8x",
						spcZN.str( psHD+0x100), nVER, nHD, nBLD,
						bSW );
	if( psHD > gpsHD )
	{
		cout << gpsHD;
		psHD = gpsHD;
		psHD[0] = 0;
	}
	cout << "." << endl ;

	U1	*pPUB = mass.msRST( gpsSTRpub ),
		*pS = *pB == '\a' ? pB+1 : pB,
		*pSe, *pE = pA+nL, *pSTR = pPUB, c, nADD = 0;



	U4	nSTR = gpfALF2STR( (char*)pSTR, (I8) (nALFtg ? pALFtg[0] : gpeALF_PRG) ),
		iTHIS = mass.PC.cmpl_find( &mass.CMPL, pSTR, nSTR ), iFND, iR;
	U8 nLEN, nVAN, nALF;
	gpcCMPL *pMOM = mass.piLEVmom(), *pTHIS = pMOM, *pFND = NULL, aR[8];
	gpmZ(aR);

	if( pMOM->iPC != iTHIS )
	{
		iTHIS = mass.CMPL.nPC();
		pMOM->cmpl_add( &mass.CMPL, pSTR, nSTR );
		pTHIS = mass.CMPL.pPC( iTHIS );
		pTHIS->typ = pTHIS->wip = gpeALF_PRG;
	}
	mass.incLEV();
	char	*pCOUT = NULL, sVAN[] = ".";
	bool 	bABC;
	cout << endl << "ASM:nP:lv tp dc.n iP\r\n";

	for( pS += gpmNINCS( pS, " \t\r\n" ); pS < pE; pS += gpmNINCS( pS, " \t\r\n" ) )
	{

		if( pFND )
		{
			pCOUT = pFND->sLOG( pPUB, NULL, gpsNDAT, &mass.CMPL );
			pFND = NULL;
		} else
			pCOUT = NULL;

		if( pCOUT )
			cout << "ASM:" << pCOUT << "\r\n";

		c = *pS;
		if( bABC = gpmbABC(c, gpaALFadd) )
        {
			// UTF8!
			nALF = gpfABCnincs( pSTR = pS, pE, nLEN, gpaALFadd );
			if( nALF )
			{
				nSTR = nALF + gpmVAN( pSTR+nALF, gpsPRG, nVAN );
				nLEN += nVAN;

				pSTR = gpmSTRnCPY( pPUB, pSTR, nSTR );
				pPUB += nSTR;
				pS += nSTR;

				iFND = pMOM->cmpl_find( &mass.CMPL, pSTR, nSTR );
				if( !iFND )
				{
					/// NEMTALÁLT
					continue;
				}
				pFND = mass.CMPL.pPC( iFND );
			}
			continue;
		}

		iFND = pMOM->cmpl_best( &mass.CMPL, pS, pE-pS );
		pFND = mass.CMPL.pPC( iFND );
		if( pFND->wip == gpeALF_OPER )
		{
			pS += pFND->n_str;
			switch( pFND->typ )
			{
				case gpeALF_COMS: // "/*"
					if( U1* pCOM = (U1*)strstr( (char*)pS, "*/" ) )
					{
						pS = pCOM+2;
					} else {
						pS = pE;
					}
					pFND = NULL;
					continue;
				case gpeALF_COM: // "//"
					pS += gpmVAN( pS, "\r\n", nLEN );
					pFND = NULL;
					continue;
				default:
					break;
			}
			continue;
		}

		if( (c < 0x30) || (c >= 0x3A)  )
		{
			sVAN[0] = c;
			pS += gpmNINCS( pS, sVAN );
			pFND = NULL;
			continue;
		}

		pFND = aR+(iR%gpmN(aR));
		iR++;

		pFND->wip = gpeALF_REG;
		pFND->u8 = gpfSTR2U8( pS, &pS );
		if( *pS == '.' )
		{
			pFND->typ = gpeALF_D;
			pFND->d = gpmSTR2D( pS ) + (double)pFND->u8;
		}
		else if( nADD < 0 )
		{
			pFND->typ = gpeALF_I;
			pFND->i8 = -pFND->u8;							///
		} else
			pFND->typ = gpeALF_U;


	}

	cout << endl << "alDAT:" << mass.alDAT << "BLD/VER:"<< nBLD << nVER;
	cout << endl;
	nBLD = nVER;
}
