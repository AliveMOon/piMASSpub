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
/*U8 inline gpfABCnincs( U1* p_str, U1* pE, U8& nLEN )
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
}*/
U8 gpfALF2STR( U1* p_out, I8 d0 )
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
		return gpeALF_null;
	}

	if( pS ? !*pS : true )
	{
		if( ppS )
			*ppS = pS;

		return gpeALF_null;
	}

	U8 alf = 0;
	U1 c, utf;

	while( pS < p_end)
	{
		c = *(U1*)pS;
		utf = 0x80;
		if( c&utf )
		{
			utf >>= 1;
			while( c&utf )
            {
				utf >>= 1;
				pS++;
            }
			// most egyenlőre minden UTF-et kicserélünk e betüre;
			c = 'e';


		}
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
U1		gpsSTRpub[0x10000];
char 	gpsHD[0x1000], *psHD;
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

	U1* pS = pA, *pSS;
	U8	oSW = bSW;
	bSW &= ~gpeMASSclrMSK;
	gpeALF alf, *pTGdie = pALFtg;
	U4 nX, nTGdie = nALFtg;

	pALFtg = NULL;
	nALFtg = 0;
	gpfALF2STR( gpsSTRpub, spcZN.x+1 );
	psHD += sprintf( 	psHD, "\r\n-----------------\r\nHD:[%s%d] spc:%s V:%d H:%d C:%d \r\nbSW:0x%0.8x",
						gpsSTRpub, spcZN.y, spcZN.str( psHD+0x100, "," ), nVER, nHD, nBLD,
						bSW );

	while( pB > pS )
	{
		if( (*pS&0x80) )
		{
			pS++;
			continue;
		}
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

				alf = gpeALF_null;
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
		if( *pS >= '0' && *pS <= '9' )
		{
			///! ennek majd ciklusnak kell lenie!
			///! mélységet lehessen adni
			if( bSW &= gpeMASSznMSK )
			{
				gpfSTR2U8( pS, &pS );
				continue;
			}

			spcZN.x = (U4)alf-1;
			spcZN.y = gpfSTR2U8( pS, &pS );
			bSW |= gpeMASSznMSK;
			/*if( spcZN.x )
				continue;
			if( !spcZN.y )
				continue;
			bSW |= gpeMASSentrMSK;*/
			continue;
		}
		switch( alf )
		{
			case gpeALF_null:
				pS++;
				continue;
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
				continue;
				pS++;
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
			psHD += gpfALF2STR( (U1*)psHD, (I8)pTGpub[i] );
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

	psHD += sprintf(	psHD, "\r\nOFF:0x%0.8x",
						bOFF );
	bSW &= bOFF;
	nHD = nVER;
	if( bSW&gpeMASSznMSK )
	if( bSW&gpeMASSentrMSK )
		bSW &= ~gpeMASSentrMSK;
	psHD += sprintf( 	psHD, "\r\nbSW:0x%0.8x",
						bSW );
	if( psHD > gpsHD )
		cout << gpsHD;
	cout << "." ;
}

char	gpsPRG[] = gpdPRGsep, //" \t\r\n\a .,:;!? =<> -+*/%^ &~|@#$ \\ \" \' ()[]{} ",
		gpsTAB[] = "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t",
		*gppTAB = gpsTAB+strlen(gpsTAB),
		gpsNDAT[] = "-bwllqqqqxxxxxxxx";



gpcCMPLlev*	gppLEV = NULL;
void gpcSRC::cmpi( gpcMASS& mass, bool bDBG )
{
	hd( mass );

	gpfALF2STR( gpsSTRpub, spcZN.x+1 );

	psHD = gpsHD;
	psHD[0] = 0;
	psHD += sprintf( 	psHD, "\r\nComPI:[%s%d] spc:%s V:%d H:%d B:%d \r\nbSW:0x%0.8x",
						gpsSTRpub, spcZN.y,
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
		*pSe, *pE = pA+nL, *pSTR = pPUB, c, nADD = 0,
		*pSTRpool = NULL;
	gpcLAZY* pCMPL = &mass.CMPL;


	U4	nSTRpool = 0,
		nSTR = gpfALF2STR( pSTR, (I8) (nALFtg ? pALFtg[0] : gpeALF_PRG) ),
		iTHIS = mass.PC.cmpl_find( pCMPL, pSTR, nSTR ), iFND, iR, iNEW, iSPARE, iREDIR;
	U8 nLEN, nVAN, nALF;
	gpcCMPL *pMOM = mass.piLEVmom(), *pTHIS = pMOM, *pFND = NULL, aR[8],
			*pI4 = pCMPL->pPC( pMOM->cmpl_find( pCMPL, (U1*)"I4", 2 ) ),
					//pMOM->pPC( pCMPL, pMOM->cmpl_find( pCMPL, (U1*)"I4", 2 )),	// talán sima int legyen?
			*pNEW = NULL, *pSPARE, *pTMP, *pREDIR,
			*pMm = NULL,
			*pDm = NULL, *pDc = NULL, *pDd = NULL, *pDot = NULL, *pDotM = NULL;
	gpmZ(aR);

	if( pMOM->iPC != iTHIS )
	{
		iTHIS = mass.CMPL.nPC();
		pMOM->cmpl_add( pCMPL, pSTR, nSTR );
		pTHIS = pCMPL->pPC( iTHIS );
		pTHIS->typ = pTHIS->wip = gpeALF_PRG;
	}
	U4 &nDAT = pTHIS->n_dat;
	nDAT = 0;

	mass.incLEV();
	char	*pCOUT = NULL, sVAN[] = ".";
	bool bABC, bENTER;
	cout << endl << "nP:MM.iK[fd]dt\tstr\ttyp.sz\r\n-----------------------\r\n";

	U4 floorLEV = mass.iLEV;

	if( !gppLEV )
		gppLEV = gppLEV->newROOT();
	gpcCMPLlev* pLEV = gppLEV->get( floorLEV );


	//nSTR = 0;
	pSTR = NULL;
	gpeALF alf, mxALF;

	if( pTHIS )
	{
		pLEV->pMOM = pTHIS;
		pCOUT = pTHIS->sLOG( pPUB, gppTAB-1 + floorLEV-pLEV->iLEV, gpsNDAT, &mass.CMPL );
		cout << pCOUT << "\t" << pLEV->iDAT << "/" << nDAT << "L" << pLEV->iLEV <<"\r\n"<<endl;
		pCOUT = NULL;
	}

	for( pS += gpmNINCS( pS, " \t\r\n" ); pS <= pE; pS += gpmNINCS( pS, " \t\r\n" ) )
	{
		if( !nSTR )
			pSTR = NULL;
		else if( !pSTR )
			nSTR = 0;

		if( pFND ) {
		/// -----------------------------
		/// 		IGEN TALÁLAT
		/// -----------------------------

			if( pFND->wip == gpeALF_OPER )
			{
				switch( pFND->typ )
				{
					case gpeALF_DOT:

						if( pLEV->pDEF )
							pMOM = pLEV->pDEF;					// pl. 81:02[80] U1x4 ez lesz az anya, ebbe akarok def.

						if( !pLEV->pDotM )
						{
							pLEV->pDotM = pMOM;
							if( pMOM->iDEF != pMOM->iPC )
								pLEV->pDot = pCMPL->pPC( pMOM->iDEF, gpsSTRpub );
							pLEV->iOFF = 0;
						} /*else {
							if( pMOM->iDEF != pMOM->iPC )
								pLEV->pDOT = pCMPL->pPC( pMOM->iDEF, gpsSTRpub );
							pLEV->iOFF = 0;
						}*/
						if( pLEV->pCALL )
							pFND = pLEV->pCALL;

						pLEV->AoBclr();
						break;
					case gpeALF_BEGIN:							// '{'
/// 2. CLASS sNAME pDEF -> pMOM -----------------------------	// pl. class U1x4 {
						if( pLEV->pDEF )
							pMOM = pLEV->pDEF;					// pl. 81:02[80] U1x4 ez lesz az anya, ebbe akarok def.

						if( pLEV->pCALL )
							pFND = pLEV->pCALL;

						pLEV->AoBCclr();
						break;
					case gpeALF_END:
						pMOM = (pLEV = pLEV->dec(floorLEV))->pMOM;
						if( pLEV->pFND )
							pFND = pLEV->pFND;

						if( !pMOM )
							pMOM = pTHIS;
						pLEV->AoBclr();
						break;


					case gpeALF_BRAKS:
						if( pLEV->pDEF )
							pMOM = pLEV->pDEF;
						pLEV = pLEV->inc( pMOM );
						pLEV->AoBclr();
						break;

					case gpeALF_BRAKE:
						pLEV->DAoBclr();
						if( pLEV->pFND )
						{
							pLEV->pCALL = pLEV->pFND;

							pFND =
							pLEV->pFND = NULL;
						}
						break;
					case gpeALF_DIME:
						if( pLEV->pCALL )
						{
							// ez a zárójel pCALL-ja
							pFND = pLEV->pCALL;
							pLEV->pCALL = NULL;
						}

						if( pLEV->pFND )
						{
							pFND = pLEV->pFND;
							pLEV->pFND = NULL;
							pMOM = ( pLEV = pLEV->dec(floorLEV) )->pMOM;
							if( pFND->wip == gpeALF_ARRAY )
							{
								pFND->i_dat = pLEV->iDAT;
								U4 nD = pFND->n_dat;
								if( nD )
								{
									pMOM->n_dat += nD;
									pLEV->iDAT += nD;
								}
							}

						}
						pLEV->AoBclr();
						break;
					case gpeALF_NEWROW:
						if( !pLEV->pCALL )
						{
							pLEV->AoBclr();
							break;
						}


						if( pSPARE = pCMPL->pSPARE( pLEV->pCALL->iSPARE, gpeALF_FUNC ) )
						{
							pFND = pSPARE;
							if( pTMP = pCMPL->pPC( pFND->iDEF ) )
							if( pFND->n_dat != pTMP->n_dat )
								pFND->n_dat = pTMP->n_dat;
						} else
							pFND = pLEV->pCALL;


						pMOM = ( pLEV = pLEV->dec(floorLEV) )->pMOM;
						pLEV->AoBCclr();
						break;
					case gpeALF_STK:
						pLEV->AoBclr();
						break;

					case gpeALF_CLASS:
						// osztájt akar valaki dec/def-incializálni?
						/// egy új CLASS élete azzal kezdödik hogy
/// 0. CLASS -> pOP ------------------------------- // pl. class
					default:
						pLEV->pOP = pFND;
						break;

				}


			} else {
				// IGEN találat NEM operator
				switch( pFND->typ )
				{
                    case gpeALF_DEF:
/// 3. CLASS pLEV->pDEF = "U1" -----------------------------	// pl. class U1x4 { U1
						pLEV->pDEF = pFND;

						break;
					case gpeALF_FUNC:
						pLEV->pCALL = pFND;
						break;
					default:
						{
							switch( pFND->wip )
							{
								case gpeALF_CYCLE:
									pLEV = pLEV->inc( pMOM, pFND );
									//pMOM = pFND;
									pLEV->DAoBclr();
									//pLEV = pLEV->inc( pMOM );
									pFND = NULL;
									break;
								case gpeALF_FUNC:
									pLEV = pLEV->inc( pMOM, pFND );
									//pMOM = pFND;
									pLEV->DAoBclr();
									//pLEV = pLEV->inc( pMOM );
									pFND = NULL;
									break;
								case gpeALF_REG:
									break;
								default:
									break;
							}
						}
						break;
				}


			}

		}
		else if( nSTR ) {
			/// -----------------------------
			/// 		NINCS találat VAN string
			/// -----------------------------

			iNEW = pCMPL->nPC();
			if( pLEV->pOP )
			{
				/// NINCS találat VAN string VAN operator
				switch( pLEV->pOP->typ )
				{
					case gpeALF_CLASS:
							pMOM->cmpl_add(pCMPL, pSTR, nSTR );
							pNEW = pCMPL->pPC( iNEW );
							if( !pNEW )
								break;
/// 1. CLASS sNAME -> pDEF ----------------------------- // pl. class U1x4
							pNEW->wip = gpeALF_CLASS;
							pNEW->typ = gpeALF_DEF;
							pNEW->iPUB = pSTR-gpsSTRpub;
							pNEW->iDEF = pNEW->iPC;

							/// ezt az operátort törlöm ???
							//pLEV->pOP = NULL;
							/// ezt az operátort törlöm ???

							pFND =
							pLEV->pDEF = pNEW;			// pl. U1x4
							break;
					case gpeALF_FUNC:

							break;
					default:
							break;
				}
			} else {
				/// NINCS találat VAN string NINCS operator
				pMOM->cmpl_add(pCMPL, pSTR, nSTR );
				pNEW = pCMPL->pPC( iNEW );
				if( pNEW )
				{
					gpcCMPL& def = pLEV->pDEF ? *pLEV->pDEF : *pI4;
					pNEW->iDEF = def.iPC;
					pNEW->typ = def.typ;
					pNEW->iPUB = pSTR-gpsSTRpub;

					switch( pSTR[nSTR-1] )
					{
						case '(':
							// van a végén zárójel
							pNEW->wip = gpeALF_FUNC;

							{
								U1* pSTRmom = (U1*)pCMPL->pPC( pMOM->mPC )->p_kid->sSTRix( pMOM->iKD, "Oxo" );
								U4 nSTRmom = gpmSTRLEN( pSTRmom );
								if( nSTRmom+1 == nSTR )
								{
									nSTRmom = (gpmMEMCMP( pSTRmom, pSTR, nSTRmom ))-pSTRmom;
									if( nSTR == nSTRmom+1 )
									{
										pNEW->wip = gpeALF_CONSTR;
										pNEW->iDEF = pNEW->mPC;
									}
								}
							}

							if( nSTR > 1 )
							if( pNEW->wip != gpeALF_CONSTR )
							{
								// ha nem lett constructor
								// funkciónak látszó izé
								// de attól még lehet VáLTOZó inicializálás is
								// ezért csinálunk annak egy TARTALéKOT háha
								iSPARE = pCMPL->nPC();
								pMOM->cmpl_add(pCMPL, pSTR, nSTR-1 );
								pSPARE = pCMPL->pPC( iSPARE );
								if( pSPARE )
								{
									pNEW->iSPARE = iSPARE;
									pSPARE->wip = gpeALF_CLASS;
									pSPARE->typ = def.typ;
									pSPARE->iDEF = def.iPC;
									pSPARE->iPUB = pNEW->iPUB;

								}
							}

							pLEV = pLEV->inc( pMOM, pNEW );
							pMOM = pNEW;
							pLEV->DAoBclr();
							pFND = NULL;
							break;
						case '[':
							pNEW->wip = gpeALF_ARRAY;
							pNEW->n_dat = def.n_dat;

							iREDIR = pCMPL->nPC();
							pMOM->cmpl_add(pCMPL, pSTR, nSTR-1 );
							pREDIR = pCMPL->pPC( iREDIR );
							if( pREDIR )
							{
								// REDIR meg arra lesz jó, hogy pl aIZE[]
								// megtaláljuk a sizeof(aIZE) vel is
								// vagy *aIZE, aIZE->, etc....
								pREDIR->iPUB = pNEW->iPUB;
								pREDIR->iDEF = def.iPC;
								pREDIR->n_dat = def.n_dat;
								pREDIR->wip = pNEW->wip;
								pREDIR->typ = pNEW->typ;
								pREDIR->iREDIR = pNEW->iPC;
							}

							if( pLEV->pDEF )
								pMOM = pLEV->pDEF;

							pLEV = pLEV->inc( pMOM, pNEW );
							pMOM = pNEW;
							pLEV->DAoBclr();
							pFND = NULL;
							break;

						default:
/// 4. CLASS pNEW->wip = gpeALF_CLASS, typ = gpeALF_DEF-----------------------------	// pl. class U1x4 { U1 x
							pNEW->wip = def.wip;
							if( pNEW->n_dat = def.n_dat )
							{
								pNEW->i_dat = pLEV->iDAT;
								pMOM->n_dat += pNEW->n_dat;
								pLEV->iDAT += pNEW->n_dat;
							}
							pFND = pNEW;
							break;
					}
				}
			}
		}



		if( pFND )
		{
			pCOUT = pFND->sLOG( pPUB, gppTAB-1 + floorLEV-pLEV->iLEV, gpsNDAT, pCMPL );
		}
		if( nDAT < pLEV->iDAT )
			nDAT = pLEV->iDAT;

		if( pCOUT )
		{
			cout << pCOUT << "\t" << pLEV->iDAT << "/" << nDAT << "L" << pLEV->iLEV <<"\r\n";
			pCOUT = NULL;
		}

		if(
			const char* pLAB = pFND ?
										pNEW == pFND 	?
															"\t\t\t\t\t\t| NEW"
														:
															(pFND->wip == gpeALF_OPER ? "\t\t\t\t\t\t| opF" : "\t\t\t\t\t\t| FND" )
									:
										( pSTR ? "\t\t\t\t\t\t| STR" : NULL )
		)
		{
			if( !pSTR )
				pSTR = gpsSTRpub+pFND->iPUB;
			else if( pFND )
				pSTR = gpsSTRpub+pFND->iPUB;
			bENTER = false;
			if( pDm != pLEV->pMOM )
			{
				if( !bENTER )
					bENTER = true;

				cout	<<  pLAB << " "
						<< (char*)pSTR << " mom: "
								<< ( pLEV->pMOM ? (char*) gpsSTRpub+pLEV->pMOM->iPUB	: "NULL" )
						<< " -> "
										<< ( pDm ? (char*) gpsSTRpub+pDm->iPUB	: "NULL" )
						<< endl;
				pDm = pLEV->pMOM;
			}
			if( pDc != pLEV->pCALL )
			{
				if( !bENTER )
					bENTER = true;

				cout 	<<  pLAB << " "
						<< (char*)pSTR << " cal: "
								<< ( pLEV->pCALL ? (char*) gpsSTRpub+pLEV->pCALL->iPUB	: "NULL" )
						<< " -> "
								<< ( pDc ? (char*) gpsSTRpub+pDc->iPUB	: "NULL" )
						<< endl;
				pDc = pLEV->pCALL;
			}

			if( pDot != pLEV->pDot ) {
				if( !bENTER )
					bENTER = true;
				//if( pDotM ? pDotM->iPC ==  )
				cout 	<<  pLAB << " "
						<< (char*)pSTR << " dot: "
								<< ( pLEV->pDotM ? (char*) gpsSTRpub+pLEV->pDotM->iPUB : "NULL" )
						<< "."
								<< ( pDot ? (char*) gpsSTRpub+pDot->iPUB : "NULL" )
						<< endl;
				pDot = pLEV->pDot;
			}

			if( pMm != pMOM )
			{
				if( !bENTER )
					bENTER = true;

				cout 	<<  pLAB << " "
						<<  pSTR << " Mom: "
								<< ( pMOM ?  (char*) gpsSTRpub+pMOM->iPUB : "NULL" )
						<< " -> "
								<< ( pMm ? (char*)gpsSTRpub+pMm->iPUB : "NULL" )
						<< endl;
				pMm = pMOM;
			}
			if( pDd != pLEV->pDEF )
			{
				if( !bENTER )
					bENTER = true;

				cout 	<< pLAB << " "
						<< (char*)pSTR << " def: "
								<< ( pLEV->pDEF ? (char*) gpsSTRpub+pLEV->pDEF->iPUB	: "NULL" )
						<< " -> "
								<< ( pDd ? (char*) gpsSTRpub+pDd->iPUB	: "NULL" )
						<< endl;
				pDd = pLEV->pDEF;
			}
			else if( pFND ? pNEW == pFND : false )
			{
				if( !bENTER )
					bENTER = true;

				cout 	<< pLAB << " "
						//<< (char*)pSTR << " new: "
								<< (char*) gpsSTRpub+pMOM->iPUB
						<< "."
								<< (char*) gpsSTRpub+pNEW->iPUB
						<< endl;
			}

			if( bENTER )
				cout << "\t\t\t\t\t\t*---------- ----  ---   --    -" << endl;

		}



		pNEW = pFND = NULL;

		/// --------------------------------
		/// ITT a VÉGE
		if( pS >= pE )
			break;
		/// --------------------------------

		c = *pS;
		if( bABC = gpmbABC(c, gpaALFadd) ) {
			// UTF8!
			nALF = gpfABCnincs( pSTR = pS, pE, nLEN, gpaALFadd );
			if( !nALF )
				continue;

			alf = gpfSTR2ALF( pSTR, pSTR+nSTR );
			if( mxALF < alf )
				mxALF = alf;
			nSTR = nALF + gpmVAN( pSTR+nALF, gpsPRG, nVAN );
			nLEN += nVAN;

			pSTR = gpmSTRnCPY( pPUB, pSTR, nSTR );
			pPUB += nSTR;
			pS += nSTR;
			pS += gpmNINCS( pS, " \t\r\n" );

			switch( *pS )
			{
				case '(':
					*pPUB = *pS;
					pPUB++;
					nSTR++;
					pS++;
					break;
				case '[':
					*pPUB = *pS;
					pPUB++;
					nSTR++;
					pS++;
					break;
			}
			pSTR[nSTR] = 0;
			pPUB++;
			if( pLEV->pDot ? pLEV->pDot->iDEF == pLEV->pDot->iPC : false )
			{
				iFND = pLEV->pDot->cmpl_find( pCMPL, pSTR, nSTR );
				pFND = pLEV->pDot = pCMPL->pPC( iFND, gpsSTRpub );
				if( pFND )
					pLEV->iOFF += pFND->i_dat;
				continue;
			}

			iFND = pMOM->cmpl_find( pCMPL, pSTR, nSTR );
			if( !iFND )
			{
				/// NEMTALÁLT
				pFND = NULL;
				continue;
			}

			pFND = pCMPL->pPC( iFND, gpsSTRpub );
			continue;
		}

		iFND = pMOM->cmpl_best( pCMPL, pS, pE-pS );
		pFND = pCMPL->pPC( iFND );
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
					nSTR = 0;
					continue;
				case gpeALF_COM: // "//"
					pS += gpmVAN( pS, "\r\n", nLEN );
					pFND = NULL;
					nSTR = 0;
					continue;
				case gpeALF_STR:
					{
						U1* p_str = pPUB, *p_se = p_str, nSTR = 0;
						if( pSTRpool+nSTRpool < p_str )
						{
							/// itt kéne elrakni ha még nem tette meg más
							pSTRpool = p_str;
							nSTRpool = 0;
						}
						while( (pS < pE) ? (*pS != '\"' ) : false )
						{
							pSTR = pS+gpmVAN( pS, "\"", nLEN );
							if( pSTR < pE )
							{
								nSTR = pSTR-pS;
								memcpy( p_se, pS, nSTR );
								p_se += nSTR;
								pS += nSTR;
								if( pS[-1] == '\\' )
								{
									p_se[-1] = *pS;
									pS++;
                                }
								continue;
							}

							nSTR = pE-pS;
							memcpy( p_se, pS, nSTR );
							p_se += nSTR;
							pS = pE;
						}
						nSTRpool += p_se-p_str;
						if( pS < pE )
							pS++;
					}
					continue;
				case gpeALF_DOT:
					continue;
				default:
					break;
			}
			continue;
		}

		if( (c < '0') || (c > '9')  )
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



