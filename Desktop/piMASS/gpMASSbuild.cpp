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
U8 inline gpfALF2STR( char* p_out, I8 d0 )
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
gpeALF inline gpfSTR2ALF( U1* pS, U1* p_end, U1** ppS )
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

	U1* pS = pA;
	U8	oSW = bSW;
	bSW &= ~gpeMASSclrMSK;
	gpeALF alf, *pTGdie = pALFtg;
	U4 nX, nTGdie = nALFtg;

	pALFtg = NULL;
	nALFtg = 0;
	gpfALF2STR( (char*)gpsSTRpub, space.x+1 );
	psHD += sprintf( psHD, "\r\n-----------------\r\nHD:[%s%d] spc:%s V:%d H:%d C:%d \r\nbSW:0x%0.8x",
						gpsSTRpub, space.y, space.str( psHD+0x100, "," ), nVER, nHD, nBLD,
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



gpcCMPLlev*	gppLEV = NULL;
void gpcSRC::cmpi( gpcMASS& mass, bool bDBG )
{
	hd( mass );

	gpfALF2STR( (char*)gpsSTRpub, space.x+1 );

	psHD = gpsHD;
	psHD[0] = 0;
	psHD += sprintf( 	psHD, "\r\nComPI:[%s%d] spc:%s V:%d H:%d B:%d \r\nbSW:0x%0.8x",
						gpsSTRpub, space.y,
						space.str( psHD+0x100), nVER, nHD, nBLD,
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



	U4	nSTRpool = 0,
		nSTR = gpfALF2STR( (char*)pSTR, (I8) (nALFtg ? pALFtg[0] : gpeALF_PRG) ),
		iTHIS = mass.PC.cmpl_find( &mass.CMPL, pSTR, nSTR ), iFND, iR, iNEW;
	U8 nLEN, nVAN, nALF;
	gpcCMPL *pMOM = mass.piLEVmom(), *pTHIS = pMOM, *pFND = NULL, aR[8],
			*pI4 = pMOM->pPC( &mass.CMPL, pMOM->cmpl_find( &mass.CMPL, (U1*)"I4", 2 )),	// talán sima int legyen?
			*pNEW = NULL;
	gpmZ(aR);

	if( pMOM->iPC != iTHIS )
	{
		iTHIS = mass.CMPL.nPC();
		pMOM->cmpl_add( &mass.CMPL, pSTR, nSTR );
		pTHIS = pMOM->pPC( &mass.CMPL, iTHIS );
		pTHIS->typ = pTHIS->wip = gpeALF_PRG;
	}
	U4 &nDAT = pTHIS->n_dat;
	nDAT = 0;

	mass.incLEV();
	char	*pCOUT = NULL, sVAN[] = ".";
	bool bABC;
	cout << endl << "ASM:nP:lv[fd]\tstr\ttyp.sz\r\n-----------------------\r\n";

	U4 floorLEV = mass.iLEV;

	if( !gppLEV )
		gppLEV = gppLEV->newROOT();
	gpcCMPLlev* pLEV = gppLEV->get( floorLEV );
	gpcLAZY* pCMPL = &mass.CMPL;

	//nSTR = 0;
	pSTR = NULL;
	gpeALF alf, mxALF;

	if( pTHIS )
	{
		pCOUT = pTHIS->sDECL( pPUB, gppTAB-1 + floorLEV-pLEV->iLEV, gpsNDAT, &mass.CMPL );
		/*if( pCOUT )
			cout << "ASM:" << pCOUT << "\t" << pLEV->iDAT << "/" << nDAT << "\r\n";*/
	}

	for( pS += gpmNINCS( pS, " \t\r\n" ); pS <= pE; pS += gpmNINCS( pS, " \t\r\n" ) )
	{
		if( !nSTR )
			pSTR = NULL;
		else if( !pSTR )
			nSTR = 0;

		if( pFND )
		{
		/// -----------------------------
		/// 		IGEN TALÁLAT
		/// -----------------------------

			if( pFND->wip == gpeALF_OPER )
			{
				switch( pFND->typ )
				{

					case gpeALF_BEGIN:
/// 2. CLASS sNAME pDEF -> pMOM -----------------------------
						if( pLEV->pDEF )
							pMOM = pLEV->pDEF;
						pLEV = pLEV->inc( pMOM, '+' );
						pLEV->AoBclr();
						break;
					case gpeALF_END:
						pMOM = (pLEV = pLEV->dec(floorLEV))->pMOM;
						if( pMOM )
						{
							switch( pMOM->wip )
							{
								case gpeALF_CONSTR:
								case gpeALF_FUNC:
								case gpeALF_CYCLE:
									pMOM = (pLEV = pLEV->dec(floorLEV))->pMOM;
									break;
								default:
									break;

							}

						}
						if( !pMOM )
							pMOM = pTHIS;
						pLEV->AoBclr();
						break;


					case gpeALF_BRAKS:
						if( pLEV->pDEF )
							pMOM = pLEV->pDEF;
						pLEV = pLEV->inc( pMOM, '+' );
						pLEV->AoBclr();
						break;

					case gpeALF_BRAKE:
						//pMOM = (pLEV = pLEV->dec(floorLEV))->pMOM;
						//if( !pMOM )
						//	pMOM = pTHIS;

						pLEV->pDEF = NULL;
						pLEV->AoBclr();
						break;

					case gpeALF_NEWROW:
						pLEV->AoBclr();
						break;
					case gpeALF_STK:
						pLEV->AoBclr();
						break;

					case gpeALF_CLASS:
						// osztájt akar valaki dec/def-iniálni?
						/// egy új CLASS élete azzal kezdödik hogy
/// 0. CLASS -> pOP -------------------------------
					default:
						pLEV->pOP = pFND;
						break;

				}

			} else {
				switch( pFND->typ )
				{
                    case gpeALF_DEF:
						pLEV->pDEF = pFND;

						break;
					case gpeALF_FUNC:
						pLEV->pFUNC = pFND;

						break;
					default:
						break;
				}

			}

		}
		else if( nSTR )
		{
			/// -----------------------------
			/// 		NEM TALÁLAT
			/// -----------------------------

				// de van valami amit létrehozhatunk
				iNEW = pCMPL->nPC();
				bool bFUN = pSTR[nSTR-1] == '(';
				if( !pLEV->pOP )
				{

					pMOM->cmpl_add(pCMPL, pSTR, nSTR );
					pNEW = pMOM->pPC(pCMPL, iNEW );
					if( pNEW )
					{
						gpcCMPL& def = pLEV->pDEF ? *pLEV->pDEF : *pI4;
						if( bFUN )
						{
							pNEW->wip = gpeALF_FUNC;
							pNEW->iDEF = def.iPC;

							char* p_str = pMOM->pPC( pCMPL, pMOM->mPC )->p_kid->sSTRix( pMOM->iKD, "Oxo" );
							U4 n_str = strlen( p_str );
							if( n_str == nSTR-1 )
							{
								nSTR = ((char*)gp_memcmp( p_str, pSTR, n_str ))-p_str;
                                if( nSTR == n_str )
                                {
									pNEW->wip = gpeALF_CONSTR;
									pNEW->iDEF = pNEW->mPC;
                                }
							}
							pNEW->typ = def.typ;
							pLEV = pLEV->inc( pMOM, '+' );
							pMOM = pNEW;
							pLEV->AoBclr();
							pLEV = pLEV->inc( pMOM );

						} else {
							pNEW->wip = def.wip;
							pNEW->typ = def.typ;
							pNEW->iDEF = def.iPC;
							if( pNEW->n_dat = def.n_dat )
							{
								pMOM->n_dat += pNEW->n_dat;
								pLEV->iDAT += pNEW->n_dat;
							}

						}
						pFND = pNEW;
					}
				} else {
					switch( pLEV->pOP->typ )
					{
						case gpeALF_CLASS:
								pMOM->cmpl_add(pCMPL, pSTR, nSTR );
								pNEW = pMOM->pPC(pCMPL, iNEW );
								if( !pNEW )
									break;
/// 1. CLASS sNAME -> pDEF -----------------------------
								pNEW->wip = gpeALF_CLASS;
								pNEW->typ = gpeALF_DEF;
								pNEW->iDEF = pNEW->iPC;

								pLEV->pOP = NULL;

								pFND =
								pLEV->pDEF = pNEW;
								break;
						case gpeALF_FUNC:

								break;
						default:
								break;
					}
				}

			pCOUT = NULL;
		}
		if( pFND )
		{
			pCOUT = pFND->sDECL( pPUB, gppTAB-1 + floorLEV-pLEV->iLEV, gpsNDAT, &mass.CMPL );
			pFND = NULL;
		}
		if( nDAT < pLEV->iDAT )
			nDAT = pLEV->iDAT;

		if( pCOUT )
			cout << "ASM:" << pCOUT << "\t" << pLEV->iDAT << "/" << nDAT << "\r\n";

		/// --------------------------------
		/// ITT a VÉGE
		if( pS >= pE )
			break;
		/// --------------------------------

		c = *pS;
		if( bABC = gpmbABC(c, gpaALFadd) )
        {
			// UTF8!
			nALF = gpfABCnincs( pSTR = pS, pE, nLEN, gpaALFadd );
			if( nALF )
			{
				alf = gpfSTR2ALF( pSTR, pSTR+nSTR );
				if( mxALF < alf )
					mxALF = alf;
				nSTR = nALF + gpmVAN( pSTR+nALF, gpsPRG, nVAN );
				nLEN += nVAN;

				pSTR = gpmSTRnCPY( pPUB, pSTR, nSTR );
				pPUB += nSTR;
				pS += nSTR;
				pS += gpmNINCS( pS, " \t\r\n" );
				if( *pS == '(' )
				{
					*pPUB = *pS;
					nSTR++;
					pS++;
				}

				iFND = pMOM->cmpl_find( &mass.CMPL, pSTR, nSTR );
				if( !iFND )
				{

					/// NEMTALÁLT
					pFND = NULL;
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



