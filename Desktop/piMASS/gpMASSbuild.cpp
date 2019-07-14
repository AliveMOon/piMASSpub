#include "gpcSRC.h"
U1 gpaALFadd[0x100];
U1* gpf_aALF_init( void )
{

	for( int i = 0; i < 0x100; i++ )
	{
		if( i >= 'A' && i <= 'Z')
		{
			// egész biztos kisbetüt többet használnak
			gpaALFadd[i] = 'A'-1;//GPD_UP;
			continue;
		}
		else if( i >= 'a' && i <= 'z' )
		{
			gpaALFadd[i] = 'a'-1;
			continue;
		}
		gpaALFadd[i] = 0;
	}
	return gpaALFadd;
}
U8 inline gpfABCnincs( U1* p_str, U1* pE, U8& nLEN )
{
	nLEN = 0;
	if( (p_str < pE) ? !*p_str : true )
		return 0;

	U1* pS = p_str;
	while( pS < pE )
	{
		if( (*pS)&0x80 )
		{
			if( (*pS)&0x40 )
				nLEN++;
			pS++;
			continue;
		}
		if( !gpaALFadd[*pS] )
			break;

		nLEN++;
		pS++;
	}
	return pS-p_str;
}
U8 gpfALF2STR( char* p_out, I8 d0 )
{

	if( !p_out )
		return 0;
	if( !d0 )
	{
		*p_out = 0;
		return 0;
	}
	char	lx_s_buff[0x40],
            *p_buff = lx_s_buff + 0x3f,
            *p_end = p_buff;

	*p_end = 0;
	bool b_minus = false;
	if( d0 < 0 )
	{
		b_minus = true;
		d0 *= -1;
	}

	I8 d1;
	while( d0 )
	{
		d1 = d0;
		d0 = (d0-1)/gpdALF;
		p_buff--;
		*p_buff = (d1-d0*gpdALF)+'\`';
	}

	if( b_minus )
	{
		p_buff--;
		*p_buff = '-';
	}
	U2 n = p_end-p_buff;
	memcpy( p_out, p_buff, n+1 );
	return n;
}
gpeALF gpfSTR2ALF( U1* pS, U1* p_end, U1** ppS )
{
	if( p_end < pS )
	{
		if( ppS )
			*ppS = p_end;
		return gpeALF_zero;
	}

	if( pS ? !*pS : true )
	{
		if( ppS )
			*ppS = pS;

		return gpeALF_zero;
	}

	U8 alf = 0;
	U1 c;

	while( pS < p_end)
	{
		c = *(U1*)pS;
		if( !gpaALFadd[c] )
			break;
		c -= gpaALFadd[c];

		alf *= gpdALF;
		alf += c;

		pS++;
	}

	if (ppS)
		*ppS = pS;

	return (gpeALF)alf;
}
char gpsHD[0x1000], *psHD;
void gpcSRC::hd( gpcMASS& mass, gpeALF* pTGpub )
{
	if( !this )
		return;

	psHD = gpsHD;
	psHD[0] = 0;
	if( nHD == nVER )
	{
		if( !nVER )
			nVER = 1;
		else
			return;
	}

	U1* pS = pA;
	U8	oSW = bSW;
	bSW &= ~gpeMASSclrMSK;
	gpeALF alf, *pTGdie = pALFtg;
	U4 nX, nTGdie = nALFtg;

	pALFtg = NULL;
	nALFtg = 0;

	psHD += sprintf( psHD, "\r\nHD: %s V:%d H:%d C:%d \r\nbSW:0x%0.8x",
						space.str( psHD+0x100, "," ), nVER, nHD, nBLD,
						bSW );

	while( pB-pS )
	{
		pS += gpmNINCS( pS, " \t\r\n" );
		if( pB <= pS )
			break;
		nX = *pS == '#';
		if( nX )	// #TAG
		{
			pS++;
			if( pB <= pS )
				break;

			nX = gpmNINCS( pS, "#" );
			if( pB <= pS+nX )
				break;
			pS += nX;
			alf = gpfSTR2ALF( pS, pB, &pS );
			if( alf < gpeALF_A )
				continue;

			if( !pTGpub )
				pTGpub = mass.aTGwip;

			for( U4 i = 0; i < nALFtg; i++ )
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
					nTGdie = 0;
					gpmDELary(pTGdie);
					break;
				}
				pTGdie[i] = pTGdie[nTGdie];
				i--;
            }

			pTGpub[nALFtg] = alf;
            nALFtg++;
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
			default:
					break;
		}
	}

	if( pTGdie )
	{
		for( U4 i = 0; i < nTGdie; i++ )
		{
			if( pTGdie[i] < gpeALF_A )
				continue;
			// kiszedni a teg listából az SRC-t
			mass.tag_sub( pTGdie[i], IX );
		}
		gpmDELary(pTGdie);
	}

	U8 bOFF = ~(gpeMASSmainMSK|gpeMASSprgMSK);
	if( pTGpub )
	{
		pALFtg = new gpeALF[nALFtg+1];
		nTGdie = nALFtg;
		nALFtg = 0;
		psHD += sprintf( psHD, "\r\nnTG:%d", nTGdie );

		for( U4 i = 0; i < nTGdie; i++ )
		{
			if( pTGpub[i] < gpeALF_A )
				continue;
			pALFtg[nALFtg] = pTGpub[i];
			psHD += sprintf( psHD, " #" );
			psHD += gpfALF2STR( psHD, (I8)pTGpub[i] );
			switch( pALFtg[nALFtg] )
			{
				case gpeALF_MAIN:
					bSW |= gpeMASSmainMSK;
					bOFF |= gpeMASSmainMSK;
					break;
				case gpeALF_PRG:
					bSW |= gpeMASSprgMSK;
					bOFF |= gpeMASSprgMSK;
					break;
				default:
					break;
			}
			// betenni a teg listába az SRC-t
			mass.tag_add( pALFtg[nALFtg], IX );			///
			nALFtg++;
		}


		if( !nALFtg )
			gpmDELary(pALFtg);
		gpmDELary(pTGdie);
	}
	psHD += sprintf( psHD, "\r\nOFF:0x%0.8x",
						bOFF );
	bSW &= bOFF;
	nHD = nVER;
	psHD += sprintf( psHD, "\r\nbSW:0x%0.8x",
						bSW );
	if( psHD > gpsHD )
		cout << gpsHD;
	cout << "." ;
}

char	gpsPRG[] = gpdPRGsep, //" \t\r\n\a .,:;!? =<> -+*/%^ &~|@#$ \\ \" \' ()[]{} ",
		gpsTAB[] = "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t",
		*gppTAB = gpsTAB+strlen(gpsTAB),
		gpsNDAT[] = "-bwllqqqqxxxxxxxx";


U1 gpsSTRpub[0x10000];
void gpcSRC::cmpi( gpcMASS& mass, bool bDBG )
{
	hd( mass );

	psHD = gpsHD;
	psHD[0] = 0;
	psHD += sprintf( 	psHD, "\r\nComPI: %s V:%d H:%d B:%d \r\nbSW:0x%0.8x",
						space.str( psHD+0x100), nVER, nHD, nBLD,
						bSW );
	if( psHD > gpsHD )
	{
		cout << gpsHD;
		psHD = gpsHD;
		psHD[0] = 0;
	}
	cout << "." << endl ;

	U1	*pPUB = mass.reset( gpsSTRpub ),
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
		pTHIS = pMOM->pPC( &mass.CMPL, iTHIS );
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
			pCOUT = pFND->sDECL( pPUB, gpsNDAT, &mass.CMPL );
			pFND = NULL;
		} else
			pCOUT = NULL;

		if( pCOUT )
			cout << "ASM:" << pCOUT << "\r\n";

		c = *pS;
		if( bABC = gpmbABC(c) )
        {
			// UTF8!
			nALF = gpfABCnincs( pSTR = pS, pE, nLEN );
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
				pFND = pMOM->pPC( &mass.CMPL, iFND );
			}
			continue;
		}

		iFND = pMOM->cmpl_best( &mass.CMPL, pS, pE-pS );
		pFND = pMOM->pPC( &mass.CMPL, iFND );
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

	psHD = gpsHD;
	psHD[0] = 0;
	psHD += sprintf( 	psHD, "\r\nComPI: %s V:%d H:%d B:%d \r\nbSW:0x%0.8x",
						space.str( psHD+0x100), nVER, nHD, nBLD,
						bSW );
	if( psHD > gpsHD )
	{
		cout << gpsHD;
		psHD = gpsHD;
		psHD[0] = 0;
	}
	cout << "." << endl ;

	U1	*pPUB = mass.reset( gpsSTRpub ),
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
		pTHIS = pMOM->pPC( &mass.CMPL, iTHIS );
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
			pCOUT = pFND->sDECL( pPUB, gpsNDAT, &mass.CMPL );
			pFND = NULL;
		} else
			pCOUT = NULL;

		if( pCOUT )
			cout << "ASM:" << pCOUT << "\r\n";

		c = *pS;
		if( bABC = gpmbABC(c) )
        {
			// UTF8!
			nALF = gpfABCnincs( pSTR = pS, pE, nLEN );
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
				pFND = pMOM->pPC( &mass.CMPL, iFND );
			}
			continue;
		}

		iFND = pMOM->cmpl_best( &mass.CMPL, pS, pE-pS );
		pFND = pMOM->pPC( &mass.CMPL, iFND );
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

void gpcSRC::cmpi_trash2( gpcMASS& mass, bool bDBG )
{
	hd( mass );

	psHD = gpsHD;
	psHD[0] = 0;
	psHD += sprintf( 	psHD, "\r\nComPI: %s V:%d H:%d B:%d \r\nbSW:0x%0.8x",
						space.str( psHD+0x100), nVER, nHD, nBLD,
						bSW );
	if( psHD > gpsHD )
	{
		cout << gpsHD;
		psHD = gpsHD;
		psHD[0] = 0;
	}
	cout << "." << endl ;

	U1	*pS = *pB == '\a' ? pB+1 : pB,
		*pSe, *pE = pA+nL, *pSTR = NULL,
		*psPRG = mass.asPRG, c;

	char sVAN[] = ".", *pINFO;
	U1* pPUB = mass.reset( gpsSTRpub );
	gpcCMPL	aR[8];

	U4 	nALF,
		utf8,
		&iLEV = mass.iLEV, alLEV = iLEV,
		*piDAT = mass.aiDAT, &alDAT = mass.alDAT,
		&iPC = mass.iPC,
		nPC = mass.CMPL.nPC(),
		iFND, iOP, nR = 0;
	I4 nCHK;
	bool bSIGN = false, bALF, bABC;

	U8 	nN, nSKIP, nLEN, nSTR,
		nVAN = 0, s8;

	I2 nADD = 0;
	U8 u8;
	I8 i8;
	double d;
	gpeALF typ;



	// be kell jegyezni a rublikának egy saját változót amibe dolgozhat
	// és azon keresztül érhetik el
	//cout << endl << "lv fn[iP:mm]nD				// iD/alD\r\n";
	cout << endl << "ASM:nP:lv tp dc.n iP\r\n";
	pSTR = pPUB;
	nSTR = gpfALF2STR( (char*)pSTR, (I8)pALFtg[0] );

	U4 iTHIS = mass.PC.cmpl_find( &mass.CMPL, pSTR, nSTR );
	gpcCMPL *pMOM = mass.piLEVmom(), *pTHIS = pMOM;
	if( pMOM->iPC != iTHIS )
	{
		iTHIS = mass.CMPL.nPC();
		pMOM->cmpl_add( &mass.CMPL, pSTR, nSTR );
		pTHIS = pMOM->pPC( &mass.CMPL, iTHIS );
		pTHIS->typ = pTHIS->wip = gpeALF_PRG;
	}
	mass.incLEV();



	gpcCMPL	com = 0,
			*pFND = NULL,
			*pPRNT = NULL, *pNEW = NULL,
			*pDC_A = NULL, *pDC_B = pTHIS,
			*pDF_A = NULL,
			*pDF_B = NULL,
			*pDFop = NULL,

			*pPAR,

			*pA = NULL, *pOP = NULL,
			*pB = NULL, *piLEVpc = NULL;

	com.iLEV = mass.iLEV;
	com.mPC = pMOM->iPC;

	gpmZ(aR);
	char* pCOUT;
	//while( pS < pE )				/// 	COMPILER
	for( pS += gpmNINCS( pS, " \t\r\n" ); pS < pE; pS += gpmNINCS( pS, " \t\r\n" ) )
	{
		pCOUT = NULL;
		bool bOP = pDFop ? (pDFop->wip == gpeALF_OPER) : false;

		if( bOP )
		{
			if( pDF_B )
			{
				if( !pDF_A )
					pCOUT = pDF_B->sDECL( pPUB, gpsNDAT, &mass.CMPL );
				else
					pCOUT = pDFop->sASM( gpsSTRpub, pPUB, gpsNDAT, &mass.CMPL, pDF_A, pDF_B );

				pDF_A = pDF_B;
			}

			gpcCMPL& iLEVpc = pDF_B ? *pDF_B : *mass.piLEViPC();
			switch( pDFop->typ )
			{
					case gpeALF_STK:
						pCOUT = pDF_A->sDECL( pPUB, gpsNDAT, &mass.CMPL );

						pDF_A = pDF_B = pDFop = NULL;
						nR = 0;
						break;

					case gpeALF_PNT:
						pDF_A = pDF_B = pDFop = NULL;

                        nR = 0;
						break;

					case gpeALF_BEGIN:	// {

						pDF_A = pDF_B = pDFop =
						pB = pA = pOP = NULL;

						if( pDC_B )
						{
							mass.iPC = pMOM->iPC;
							pMOM = pDC_B;
						}
						mass.incLEV();

						nR = 0;
						break;
					case gpeALF_END:	// }
						pDF_A = pDF_B = pDFop = NULL;

						pMOM = mass.piLEVmom();
						mass.decLEV();

						if( gpcCMPL* pDWN = pMOM->sKIDlst(  gpsSTRpub, pPUB, &mass.CMPL, c ) )
						if( *pPUB )
								cout << endl << pPUB;



						nR = 0;
						break;

					case gpeALF_BRAKS:	// (
						pDF_A = pDF_B = pDFop = NULL;

						mass.incLEV();
						nR = 0;
						break;


					case gpeALF_BRAKE:	// )
						pDF_A = pDF_B = pDFop = NULL;

						/// itt volt valami DSTs izé
						if( gpcCMPL* pDWN = iLEVpc.sKIDlst( gpsSTRpub, pPUB, &mass.CMPL, c ) )
						{
							if( *pPUB )
								cout << endl << pPUB;
						}

						mass.asPRG[iLEV] = c;
						iPC = mass.aPC[iLEV];

						nR = 0;
						break;



					case gpeALF_COMS: // "/*"
						if( U1* pCOM = (U1*)strstr( (char*)pS, "*/" ) )
						{
							pS = pCOM+2;
						} else {
							pS = pE;
						}
						continue;
					case gpeALF_COM: // "//"
						pS += gpmVAN( pS, "\r\n", nLEN );
						continue;


					//case gpeALF_MOV: // =
						/// itt volt valami DSTs izé
					default:
						break;

			}

		}


		if( pDC_A != pDC_B )
		{
				pDC_A = pDC_B;
				pCOUT = pDC_A->sDECL( pPUB, gpsNDAT, &mass.CMPL );
				pDF_A = pDFop = pDF_B = NULL;
		}

		if( pCOUT )
			cout << "ASM:" << pCOUT << "\r\n";

		sVAN[0] = c = *pS;
        if( bABC = gpmbABC(c) )
        {
			// UTF8!
			nALF = gpfABCnincs( pSTR = pS, pE, nLEN );
			if( nALF )
			{
				// ALF esélyes ?
				bALF = (nALF == nLEN);


                nSTR = nALF + gpmVAN( pSTR+nALF, gpsPRG, nVAN );
				nLEN += nVAN;

				pSTR = gpmSTRnCPY( pPUB, pSTR, nSTR );

				pPUB += nSTR;
				pS += nSTR;


				nSKIP = gpmNINCS( pS, " \t\r\n" );
				c = pS[nSKIP];
				bABC = gpmbABC(c);
				/*if( pMOM ? (com.mPC != pMOM->iPC) : false  )
					pMOM = NULL;*/

				if( !pMOM )
				{
					pMOM = mass.PC.pPC( &mass.CMPL, com.mPC ); //mass.aPC[mass.iLEV] );
					/*if( pMOM )
					{
						cout << endl << "mom:" << pMOM->iPC << (char*)gpsSTRpub+pMOM->i_str;
					}*/
				}

				if( !bABC )
				switch( c )
				{
					case '(':
						// el kell dönteni, hogy ez most fügvény
						// vagy a konstruktoráról szól a kifejezés
						*pPUB = c;
						pPUB++;
						pB = pA = pOP = NULL;
						nR = 0;
						//com.wip = pMOM->wip;
						if( pMOM->wip == gpeALF_DEC )
						{
							// csak deklarálva van még a mama
							nN = memcmp( pSTR, gpsSTRpub+pMOM->iPUB, nSTR );

							com.typ = nN ? gpeALF_FUNC : gpeALF_CONSTR;
							com.wip = gpeALF_DEC;
							nSTR++;
							// wip DEC
							// typ FUNC / CONSTR
						} else {
							com.wip = gpeALF_INIT;
							com.typ = gpeALF_CLASS;
							// wip INIT
							// typ CLASS
						}


						break;

					case '[':
						// forrás tömböt akar
						*pPUB = c;
						pPUB++;
						pB = pA = pOP = NULL;
						nR = 0;

						nSTR++;

						com.typ = gpeALF_ARR;
						// DEF
						// ARRAY
						break;

					default:
						break;
				}
				*pPUB = 0;
				pPUB++;


				nPC = mass.CMPL.nPC();
				iPC = pMOM->cmpl_find( &mass.CMPL, pSTR, nSTR );
				pFND = pFND->pPC( &mass.CMPL, iPC );
				com.iLEV = iLEV;

				//bool bFND = true;
				if( pFND )
				{
					if( !pFND->wip )
					{
///--------------------------------------------------------
///						NEM talált
///--------------------------------------------------------
						//bFND = false;
						// nem talált újat akar
						if( pMOM )
						{
							// van valami mama
							switch( com.wip )
							{
								case gpeALF_DEC:
									pMOM->cmpl_add( &mass.CMPL, pSTR, nSTR );
									pNEW = pMOM->pPC( &mass.CMPL, nPC );
									if( !pNEW )
										break; // nem jött létre?


									pDC_B = pNEW;

									pPRNT = NULL; //pNEW;


/// 1. STEP
// DECLARED CLASS -------------------
									pNEW->wip = pDC_A ? pDC_A->wip : com.wip; 	// DEC
									pNEW->typ = pDC_A ? pDC_A->typ : com.typ; 	// CLASS
									//pNEW->iLEV = iLEV;
									pNEW->iPUB = pSTR-gpsSTRpub;	// name
									pNEW->n_str = nSTR;

									/// pNEW->iDEF = 0;
									pNEW->iDEF = pNEW->iDEC = pNEW->iPC;

									com.mPC = iPC =
									com.iDEC = pNEW->iPC;	// DEC pc
									com.iPUB = pNEW->iPUB;
									com.n_str = pNEW->n_str;

									//pMOM = NULL;
									if( pPRNT )
									if( com.typ == gpeALF_CONSTR )
									{
										pPRNT->pINFO = (char*)pPUB;
										pPUB += sprintf( pPRNT->pINFO, "c{constr()}" );
									}

									break;



								case gpeALF_INIT:
								case gpeALF_DEF:
									pMOM->cmpl_add( &mass.CMPL, pSTR, nSTR );
									pNEW = com.pPC( &mass.CMPL, nPC );
									if( !pNEW )
										break; // nem jött létre?
									pDF_B = pNEW;

									//pPRNT = NULL; //pNEW;
									//pB = com.pPC( &mass.CMPL, com.iDEF );


/// 3. STEP
// DEFINE STUFF x,y,z,etc.... -------------------
									pNEW->wip = gpeALF_DEF; // ? pDEF->wip;
									pNEW->typ = pDC_B->typ;
									//pNEW->iLEV = iLEV;
									pNEW->iPUB = pSTR-gpsSTRpub;	// name
									pNEW->n_str = nSTR;

									pNEW->iDEF = pDC_B->iPC;
									if( com.wip == gpeALF_INIT )
									{
/// 4. STEP
// DEFINE&INIT STUFF x,y,z,etc.... -------------------
										pNEW->iINI = pNEW->iDEF;
									}
									/// com.iDEC -- ha 2.-nél ne bátottuk?
									/// ugyan az mint a mass.aPC[iLEV]?
									//iPC = pNEW->iPC;
									//pNEW->mPC = mass.aPC[iLEV];

									if( pNEW->n_dat = pB->n_dat )
									{
										gpcCMPL* pM = pM->pPC( &mass.CMPL, pNEW->mPC);
										while( pM ? (pM->iPC != pM->mPC) : false )
										{
											pM->n_dat += pNEW->n_dat;
											if( pM->wip != gpeALF_DEC )
											{
												pM = pM->pPC( &mass.CMPL, pM->mPC);
												continue;
											}

											if( !pPRNT )
												break;

											pPRNT->pINFO = (char*)pPUB;
											pPUB += sprintf(
																pPRNT->pINFO,

																pMOM->typ == gpeALF_FUNC
																|| pMOM->typ == gpeALF_CONSTR

																? "f(para)"
																: "stuff"

															);
											break;
										}
									}

									break;

								default:
									break;

							}

						}
					} else {
///--------------------------------------------------------
///					IGEN talált
///--------------------------------------------------------
/// ismerte WIP megmondja mit akar csinálni
						switch( pFND->wip )
						{
							case gpeALF_zero:
								break;
							case gpeALF_DEC:
								pDC_B = pFND;
								pDF_A = pDF_B = pDFop = NULL;
								// deklarál egy osztályt;
								//mass.incLEV();
								if( !pDC_B->n_dat )
								{
									//com.null();

/// 0. STEP
// DEC new CLASS ------------------------
									pPRNT = NULL; //&com;

									com.iDEC = pDC_B->iPC; // 0; 			// false?
									com.wip = pDC_B->wip; 	// pFND->wip;	// DEC new
									com.typ = pDC_B->typ;	// CLASS

									//com.mPC = mass.aPC[mass.iLEV];	//
									com.iPUB = pSTR-gpsSTRpub;
									com.n_str = nSTR;


									break;
								}

								//com.null();
								if( pFND->wip == gpeALF_DEC )
									pFND->wip = gpeALF_DEF;

								com.iDEC = pDC_B->iPC;

						///case gpeALF_DEF:
/// 2. STEP
// DEFINE CLASS -------------------
								//pPRNT = &com;

								com.wip = pFND->wip; 			//pFND->wip;	// DEF
								com.typ = pFND->typ;			// CLASS/FUNC
								com.iDEF = pFND->iPC;			// iDEF pl: U1
								/// com.iDEC -- ne bátsd?


								com.mPC = pMOM->iPC;
								com.iPUB = pSTR-gpsSTRpub;
								com.n_str = nSTR;
								com.iLEV = pMOM->iLEV+1;

								/// itt volt valami DSTs izé
								if( !pB )
								{
									pB = pFND;
									if( pFND->wip == gpeALF_DEF )
										pPRNT = NULL;
								}
								else if( pOP ? pOP->wip == gpeALF_OPER : false )
								{
									pA = pB;
									pB = pFND;
									pPRNT = pA;
								}

								break;
case gpeALF_DEF:
/// 2. STEP
// DEFINE CLASS -------------------
								pPRNT = NULL;
								if( !pDF_B )
									pDF_A = pDF_B;

								pDF_B = pFND;
								break;

								pPRNT = &com;

								com.wip = pFND->wip; 			//pFND->wip;	// DEF
								com.typ = pFND->typ;			// CLASS/FUNC
								com.iDEF = pFND->iPC;			// iDEF pl: U1
								/// com.iDEC -- ne bátsd?


								com.mPC = mass.aPC[mass.iLEV];
								com.iPUB = pSTR-gpsSTRpub;
								com.n_str = nSTR;
								com.iLEV = mass.iLEV;

								/// itt volt valami DSTs izé
								if( !pB )
								{
									pB = pFND;
									if( pFND->wip == gpeALF_DEF )
										pPRNT = NULL;
								}
								else if( pOP ? pOP->wip == gpeALF_OPER : false )
								{
									pA = pB;
									pB = pFND;
									pPRNT = pA;
								}

								break;
							case gpeALF_FUNC:


								break;
							default:
								break;
						}
					}

					if( pPRNT )
					if( pPRNT == pA )
					{
						char* pCOUT = pOP->sASM( gpsSTRpub, pPUB, gpsNDAT, &mass.CMPL, pA, pB );
						cout << pCOUT;
						pPRNT = pOP = pA = NULL;
					} else {

						*pPUB = 0;
						pPUB++;
						U2 n = sprintf( (char*)pPUB, "%0.2d %0.2d[%0.2d:%0.2d]%0.2db %s%s",
											pPRNT->iLEV, iPC,
											pPRNT->iPC, pPRNT->mPC,
											pPRNT->n_dat,
											(gppTAB-mass.relLEV()), pSTR
										);

						if( pPRNT != &com )
							mass.aiDAT[iLEV] += pPRNT->n_dat;

						if( mass.alDAT < mass.aiDAT[iLEV] )
							mass.alDAT = mass.aiDAT[iLEV];

						gpfVAN( pPUB, NULL, nLEN );
						nLEN += gpfVANn( pPUB, (U1*)"\t" )*3;
						cout 	<< endl
								<< (char*)pPUB << ( gppTAB-(8-min(8, (nLEN/4) )) )
								<< "//"<< ((pPRNT->typ == pPRNT->wip ) ? "*": "" ) << mass.aiDAT[iLEV]
								<< "/" << mass.alDAT << " - ";

						if( pPRNT->pINFO )
							cout << pPRNT->pINFO;

						pPRNT = NULL;
					}
				}
				pS += nSKIP;
				if( bABC )
					continue; // következő szó?

				sVAN[0] = c;
			}
		}

		nPC = mass.CMPL.nPC();
		gpcCMPL& iLEVpc = pDF_B ? *pDF_B : *mass.piLEViPC();

		if( pA && pOP && pB )
		{
			char* pCOUT = pOP->sASM( gpsSTRpub, pPUB, gpsNDAT, &mass.CMPL, pA, pB );
			cout << pCOUT;
			pA = pOP = NULL;
		}

		iOP = iLEVpc.cmpl_best( &mass.CMPL, pS, pE-pS );
		pOP = pOP->pPC( &mass.CMPL, iOP );
		if( pOP->wip == gpeALF_OPER )
		{
			pDFop = pOP;
			pS += pOP->n_str;
			continue;


			switch( pOP->typ )
			{
					case gpeALF_BEGIN:	// {
						pDF_A = pDF_B = pDFop =
						pB = pA = pOP = NULL;

						mass.incLEV();
						nR = nVAN = 0;
						break;
					case gpeALF_END:	// }
						if( pB )
							cout << "->STK";
						pDF_A = pDF_B = pDFop =
						pB = pA = pOP = NULL;

						if( gpcCMPL* pDWN = iLEVpc.sKIDlst(  gpsSTRpub, pPUB, &mass.CMPL, c ) )
						if( *pPUB )
								cout << endl << pPUB;

						mass.decLEV();
						nR = nVAN = 0;
						break;

					case gpeALF_BRAKS:	// (
						pDF_A = pDF_B = pDFop =
						pB = pA = pOP = NULL;

						mass.incLEV();
						nR = nVAN = 0;
						break;

					case gpeALF_BRAKE:	// )
						pDF_A = pDF_B = pDFop =
						pB = pA = pOP = NULL;

						/// itt volt valami DSTs izé
						if( gpcCMPL* pDWN = iLEVpc.sKIDlst( gpsSTRpub, pPUB, &mass.CMPL, c ) )
						{
							if( *pPUB )
								cout << endl << pPUB;
						}

						mass.asPRG[iLEV] = c;
						iPC = mass.aPC[iLEV];

						nR = nVAN = 0;
						break;



					case gpeALF_COMS: // "/*"
						if( U1* pCOM = (U1*)strstr( (char*)pS+nSKIP, "*/" ) )
						{
							pS = pCOM+2;
							nSKIP = 0;
						} else {
							nSKIP = 0;
							pS = pE;
						}
						break;
					case gpeALF_COM: // "//"
						nSKIP += gpmVAN( pS+nSKIP, "\r\n", nLEN );
						break;


					//case gpeALF_MOV: // =
						/// itt volt valami DSTs izé
					default:
						/*if(	gpcCMPL* pM = pOP->pPC( &mass.CMPL, pOP->mPC ) )
						{
							cout << endl << pM->p_kid->sSTRix( pOP->iKD );


							//pA = NULL;
						}*/
						nSKIP += gpmNINCS( pS+nSKIP, " \t\r\n" );
						break;

			}
			pS += nSKIP;

			c = *pS;
			if( (c < 0x30) || (c >= 0x3A)  )
				continue;

			sVAN[0] = c;
		}

		pS++;
		nVAN = gpmNINCS( pS, sVAN );
		if( pS + nVAN > pE )
			nVAN = pE-pS;

		if( c < 0x40 )
		{
			if( c < 0x20 )
			{

			} else {
				if( c < 0x30 )
				{
					// 20-2F
					switch(c)
					{
						case ' ':
							break;
						case '!':
							break;
						case '#':
							break;
						case '$':
							break;
						case '%':
							break;
						case '&':
							break;

						case '\"':
							break;
						case '\'':
							break;


						case '(':
							mass.incLEV();
							nVAN = 0;
							break;

						case ')':
							/// itt volt valami DSTs izé

							if( gpcCMPL* pDWN = iLEVpc.sKIDlst( gpsSTRpub, pPUB, &mass.CMPL, c ) )
							{
								if( *pPUB )
									cout << endl << pPUB;

							}

							mass.asPRG[iLEV] = c;
							iPC = mass.aPC[iLEV];

							nVAN = 0;
							break;




						case '+':
							/// itt volt valami DSTs izé

							break;
						case '-':
							/// itt volt valami DSTs izé

							break;

						case '*':
							/// itt volt valami DSTs izé

							break;
						case '/':
							if( pS[nVAN] != '*'  )
							{
								if( !nVAN )
								{
									/// itt volt valami DSTs izé

									com.nMUL--;
									break;
								}

								nVAN += gpmVAN( pS+nVAN, "\r\n", nLEN );
								break;
							}
							if( U1* pCOM = (U1*)strstr( (char*)pS+nVAN+1, "*/" ) )
							{
								nVAN = (pCOM-pS)+2;
							} else {
								nVAN = 0;
								pS = pE;
							}
							break;

						case ',':	// vessző
							// berakjuk ami létrejött a sorba
							/// itt volt valami DSTs izé

							cout << //(pB ?
									"->STK"  //: ",")
									;
							pA = pB = pOP = NULL;
							nR = nVAN = 0;
							break;
						case '.':	// pont

							break;
					}
				}
				else if( c < 0x3A )
				{
					//cout << endl << (gppTAB-mass.relLEV());
					// 30-39 // számok 0-9
					/// itt volt valami DSTs izé
					if( pOP ? pOP->wip != gpeALF_OPER : true )
						pB = NULL;

					pA = pB;

					pDF_B = pB = aR+(nR%8);
					nR++;

					pB->wip = gpeALF_REG;
					pB->u8 = gpfSTR2U8( pS-(nVAN+1), &pS );
					if( *pS == '.' )
                    {
						pB->typ = gpeALF_D;
						pB->d = gpmSTR2D( pS ) + (double)u8;		///
						//cout << d;
					} else {
						if( nADD < 0 )
						{
							pB->typ = gpeALF_I;
							pB->i8 = -pB->u8;							///
							//cout << i8;
						} else {
							//cout << u8;
							pB->typ = gpeALF_U;
						}
						d = 0;
                    }



                    nVAN = 0;

					/*if( pA && pOP )
					{
						char* pCOUT = pOP->sASM( gpsSTRpub, pPUB, gpsNDAT, &mass.CMPL, pA, pB );
						cout << pCOUT;
						pA = NULL;
					}*/


				} else {
					switch(c)
					{
						// 3A-3F
						case ':':
							nVAN = 0;
							break;
						case ';':
							// mass.decLEV();
							// comment
							/// itt volt valami DSTs izé
							pDF_A = pDF_B = pDFop =
							pB = pA = pOP = NULL;
							nR = nVAN = 0;
							break;

						case '<':
							/// itt volt valami DSTs izé

							break;
						case '=':
							/// itt volt valami DSTs izé

							//mass.incLEV();
							break;
						case '>':
							/// itt volt valami DSTs izé

							break;
						case '?':
							nVAN = 0;
							break;
					}
				}
			}
		} else {
			// 40-7F
			switch(c)
			{
				// 3A-3F
				case '@':
					break;
				case '\'':
					break;

				case '[':
					mass.incLEV();
					nVAN = 0;
					break;
				case ']':
					if( gpcCMPL* pDWN = iLEVpc.sKIDlst(  gpsSTRpub, pPUB, &mass.CMPL, c ) )
					if( *pPUB )
							cout << endl << pPUB;

					mass.decLEV();
					nVAN = 0;
					break;

				case '{':
					mass.incLEV();
					nVAN = 0;
					break;
				case '}':
					if( gpcCMPL* pDWN = iLEVpc.sKIDlst(  gpsSTRpub, pPUB, &mass.CMPL, c ) )
					if( *pPUB )
							cout << endl << pPUB;

					mass.decLEV();
					nVAN = 0;
					break;



				case '|':
					break;
				case '\\':
					break;

				case '^':
					break;
				case '_':
					break;

				case '~':
					break;
				case 0x7f:
					break;
			}
		}

		pS += nVAN;
	}	/// while( pS < pE )
	cout << endl << "alDAT:" << mass.alDAT << "BLD/VER:"<< nBLD << nVER;
	cout << endl;
	nBLD = nVER;
}
