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
		*gppTAB = gpsTAB+strlen(gpsTAB);


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

	U1	*pS = *pB == '\a' ? pB+1 : pB,
		*pSe, *pE = pA+nL, *pSTR = NULL,
		*psPRG = mass.asPRG, c;

	char sVAN[] = ".", *pINFO;
	U4 	nALF,
		utf8,
		&iLEV = mass.iLEV, alLEV = iLEV,
		*piDAT = mass.aiDAT, &alDAT = mass.alDAT,
		&iPC = mass.iPC, nPC;
	I4 nCHK;
	bool bSIGN = false, bALF, bABC;

	U8 	nN, nSKIP, nLEN, nSTR,
		nVAN = 0, s8;

	I2 nADD = 0;
	U8 u8;
	I8 i8;
	double d;
	gpeALF typ;


	U1* pPUB = mass.reset( gpsSTRpub );
	// be kell jegyezni a rublikának egy saját változót amibe dolgozhat
	// és azon keresztül érhetik el
	cout << endl << "lv fn[iP:mm]nD				// iD/alD";
	gpcCMPL	com = 0, *pFND = NULL, *pMOM = NULL, *pPRNT = NULL, *pNEW = NULL, *pDEF, *pPAR, *pDST = NULL;
	while( pS < pE )				/// 	COMPILER
	{
		pS += gpmNINCS( pS, " \t\r\n" );
		if( pS >= pE )
		{
			// feldolgozni ami még van
			break;
		}

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
				if( pMOM ? (com.mPC != pMOM->iPC) : false  )
					pMOM = NULL;

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

						//com.wip = pMOM->wip;
						if( pMOM->wip == gpeALF_DEC )
						{
							// csak deklarálva van még a mama
							nN = memcmp( pSTR, gpsSTRpub+pMOM->i_str, nSTR );

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


				nPC = com.nPC( &mass.CMPL );
				iPC = pMOM->cmpl_find( &mass.CMPL, pSTR, nSTR );
				pFND = pFND->pPC( &mass.CMPL, iPC );
				com.iLEV = iLEV;

				//bool bFND = true;
				if( pFND )
				{
					if( !pFND->wip )
					{
//--------------------------------------------------------
//					NEM talált
//--------------------------------------------------------
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
									pPRNT = pNEW;
									if( com.typ == gpeALF_CONSTR )
									{
										pPRNT->pINFO = (char*)pPUB;
										pPUB += sprintf( pPRNT->pINFO, "c{constr()}" );
									}
/// 1. STEP // DECLARED CLASS -------------------
									pNEW->wip = com.wip; 	// DEC
									pNEW->typ = com.typ; 	// CLASS
									pNEW->iLEV = iLEV;
									pNEW->i_str = pSTR-gpsSTRpub;	// name
									pNEW->n_str = nSTR;

									/// pNEW->iDEF = 0;
									pNEW->iDEC = com.iDEC;
									com.mPC = iPC =
									com.iDEC = pNEW->iPC;	// DEC pc
									com.i_str = pNEW->i_str;
									com.n_str = pNEW->n_str;

									pMOM = NULL;
									break;



								case gpeALF_INIT:
								case gpeALF_DEF:
									pMOM->cmpl_add( &mass.CMPL, pSTR, nSTR );
									pNEW = com.pPC( &mass.CMPL, nPC );
									if( !pNEW )
										break; // nem jött létre?
									pPRNT = pNEW;
									pDEF = com.pPC( &mass.CMPL, com.iDEF );


/// 3. STEP // DEFINE STUFF x,y,z,etc.... -------------------
									pNEW->wip = gpeALF_DEF; // ? pDEF->wip;
									pNEW->typ = pDEF->typ;
									pNEW->iLEV = iLEV;
									pNEW->i_str = pSTR-gpsSTRpub;	// name
									pNEW->n_str = nSTR;

									pNEW->iDEF = pDEF->iPC;
									if( com.wip == gpeALF_INIT )
									{
/// 4. STEP // DEFINE&INIT STUFF x,y,z,etc.... -------------------
										pNEW->iINI = pNEW->iDEF;
									}
									/// com.iDEC -- ha 2.-nél ne bátottuk?
									/// ugyan az mint a mass.aPC[iLEV]?
									iPC = pNEW->iPC;
									pNEW->mPC = mass.aPC[iLEV];

									if( pNEW->n_dat = pDEF->n_dat )
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
//--------------------------------------------------------
//					IGEN talált
//--------------------------------------------------------
/// ismerte WIP megmondja mit akar csinálni
						switch( pFND->wip )
						{
							case gpeALF_zero:
								break;
							case gpeALF_DEC:
								// deklarál egy osztályt;
								//mass.incLEV();
								if( !pFND->n_dat )
								{
									com.null();

/// 0. STEP // DEC new CLASS ------------------------
									pPRNT = &com;

									com.iDEC = pFND->iPC; // 0; 			// false?
									com.wip = gpeALF_DEC; 	// pFND->wip;	// DEC new
									com.typ = pFND->typ;	// CLASS

									com.mPC = mass.aPC[mass.iLEV];	//
									com.i_str = pSTR-gpsSTRpub;
									com.n_str = nSTR;


									break;
								}

								com.null();
								com.iDEC = pFND->iPC;

							case gpeALF_DEF:
/// 2. STEP // DEFINE CLASS -------------------


								pPRNT = &com;

								com.wip = gpeALF_DEF; 			//pFND->wip;	// DEF
								com.typ = pFND->typ;			// CLASS/FUNC
								com.iDEF = pFND->iPC;			// iDEF pl: U1
								/// com.iDEC -- ne bátsd?


								com.mPC = mass.aPC[mass.iLEV];
								com.i_str = pSTR-gpsSTRpub;
								com.n_str = nSTR;
								com.iLEV = mass.iLEV;

								if( I1 o = pDST->sDST( pPUB, iPC, (char*)gpsSTRpub,  (char*)(gppTAB-mass.relLEV()),  (char*)pSTR ) )
								{
									cout << endl << pPUB+o;
									pPRNT = NULL;
								}
								/*if( pDST )
								{

									char	*pOP = (char*)pPUB, *pINFO = pOP,
											o = pDST->sOP( pOP );
									sprintf( pINFO = pOP+o, "%0.2d %0.2d[%0.2d:%0.2d]%0.2db %s %s %s %s",
														pDST->iLEV, iPC,
														pDST->iPC, pDST->mPC,
														pDST->n_dat,
														(gppTAB-mass.relLEV()), gpsSTRpub+pDST->i_str, pOP, pSTR

													);
									cout << endl << pINFO;
									pPRNT = NULL;
									//cout << endl << pDST->iLEV << "[" << pDST->iPC << ":" << pDST->mPC << "]" << (gppTAB - pDST->iLEV) << "=" ;
								}*/
								break;

							case gpeALF_FUNC:


								break;
							default:
								break;
						}
					}

					if( pPRNT )
					{
						/*if( iPC ? iPC == pPRNT->mPC : false )
						{
							bFND = true; /// construktor?
						}*/
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
							if( I1 o = pDST->sDST( pPUB, iPC,  (char*)gpsSTRpub,  (char*)(gppTAB-mass.relLEV()),  (char*)pSTR ) )
							{
								cout << endl << pPUB+o;
								pPRNT = NULL;
							}
							if( gpcCMPL* pDWN = mass.piLEVpc()->pLIST( gpsSTRpub, pPUB, &mass.CMPL, c ) )
							{
								if( *pPUB )
									cout << endl << pPUB;

								/*if( pDWN->wip == gpeALF_DEF && pDWN->iDEF )
								if(	gpcCMPL* pDEF = pDEF->pPC( &mass.CMPL, pDWN->iDEF )->pLIST( gpsSTRpub, pPUB, &mass.CMPL ) )
								if( *pPUB )
										cout << endl << pPUB;*/
							}

							mass.asPRG[iLEV] = c;
							iPC = mass.aPC[iLEV];

							nVAN = 0;
							break;




						case '+':
							if( !pDST )
								pDST = pMOM->pPC( &mass.CMPL, iPC );
							if( pDST )
								pDST->op |= 0x10;
							break;
						case '-':
							if( !pDST )
								pDST = pMOM->pPC( &mass.CMPL, iPC );
							if( pDST )
								pDST->op |= 8;
							break;

						case '*':
							if( !pDST )
								pDST = pMOM->pPC( &mass.CMPL, iPC );
							if( pDST )
								pDST->op |= 0x20;
							break;
						case '/':
							if( pS[nVAN] != '*'  )
							{
								if( !nVAN )
								{
									if( !pDST )
										pDST = pMOM->pPC( &mass.CMPL, iPC );
									if( pDST )
										pDST->op |= 0x40;
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
							if( !pDST )
							{
								com.op = 0;
								nVAN = 0;
								break;
							}

							if( I1 o = pDST->sDST( pPUB, iPC,  (char*)gpsSTRpub,  (char*)(gppTAB-mass.relLEV()),  (char*)pSTR ) )
							{
								cout << endl << pPUB+o;
								pPRNT = NULL;
							}
							pDST->op = 0;
							pDST = NULL;
							nVAN = 0;
							break;
						case '.':	// pont

							break;
					}
				}
				else if( c < 0x3A )
				{
					// 30-39 // számok 0-9
					//if( pDST )
					pPRNT = pDST ? pDST : &com;
					pPRNT->op |= pPRNT->op ? 0 : 1;
					if( I1 o = pPRNT->sDST( pPUB, iPC,  (char*)gpsSTRpub,  (char*)(gppTAB-mass.relLEV()) ) )
					{
						cout << endl << pPUB+o;
						pPRNT->op = 0;
						pPRNT = NULL;
					}


					u8 = gpfSTR2U8( pS-(nVAN+1), &pS );
					if( *pS == '.' )
                    {
						typ = gpeALF_D;
						d = gpmSTR2D( pS ) + (double)u8;		///
						cout << d;
					} else {
						if( nADD < 0 )
						{
							typ = gpeALF_I;
							i8 = -u8;							///
							cout << i8;
						} else {
							cout << u8;
							typ = gpeALF_U;
						}
						d = 0;
                    }
                    nVAN = 0;


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
							if( !pDST )
							{
								com.op = 0;
								nVAN = 0;
								break;
							}

							if( I1 o = pDST->sDST( pPUB, iPC,  (char*)gpsSTRpub,  (char*)(gppTAB-mass.relLEV()),  (char*)pSTR ) )
							{
								cout << endl << pPUB+o;
								pPRNT = NULL;
							}
							pDST->op = 0;
							pDST = NULL;
							nVAN = 0;
							break;

						case '<':
							if( !pDST )
								pDST = pMOM->pPC( &mass.CMPL, iPC );
							if( pDST )
								pDST->op |= 2;
							break;
						case '=':
							if( !pDST )
								pDST = pMOM->pPC( &mass.CMPL, iPC );
							(pDST ? pDST->op : com.op ) |= 1;
							//mass.incLEV();
							break;
						case '>':
							if( !pDST )
								pDST = pMOM->pPC( &mass.CMPL, iPC );
							if( pDST )
								pDST->op |= 4;
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
					if( gpcCMPL* pDWN = mass.piLEVpc()->pLIST(  gpsSTRpub, pPUB, &mass.CMPL, c ) )
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
					if( gpcCMPL* pDWN = mass.piLEVpc()->pLIST(  gpsSTRpub, pPUB, &mass.CMPL, c ) )
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
