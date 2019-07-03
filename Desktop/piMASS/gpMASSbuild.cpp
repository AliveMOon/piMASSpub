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

char	gpsPRG[] = " \t\r\n\a .,:;!? =<> -+*/%^ &~|@#$ \\ \" \' ()[]{} ",
		gpsTAB[] = "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t",
		*gppTAB = gpsTAB+strlen(gpsTAB);

gpcOPCD::gpcOPCD( const gpcOPCD* pTHIS, const char* pS, char a, char m, I8 i, U8 u, double _d, gpeALF t, gpeALF df )
{
	gpmCLR;

	pSTR = (U1*)pS;
	nADD = a;
	nMUL = m;
	d = _d;
	i8 = (this-pTHIS)+i;

	typ = t;
	nDAT = u;

	def = df;

	nSTR = gpmVAN( pSTR, gpsPRG, nLEN );


}

static const gpcOPCD gpaOPCi[] = {
	// this,	pS,			a, m, i, nDT,				d,		t;
	{ gpaOPCi,	"false", 	0, 0, 0, sizeof(U1), 		0.0, gpeALF_zero },
	{ gpaOPCi,	"true", 	0, 0, 0, sizeof(U1), 		0.0, gpeALF_TRUE },

	{ gpaOPCi,	"U1", 		0, 0, 0, sizeof(U1), 		0.0, gpeALF_TYPE, gpeALF_U },
	{ gpaOPCi,	"U2", 		0, 0, 0, sizeof(U2), 		0.0, gpeALF_TYPE, gpeALF_U },
	{ gpaOPCi,	"U4", 		0, 0, 0, sizeof(U4), 		0.0, gpeALF_TYPE, gpeALF_U },
	{ gpaOPCi,	"U8", 		0, 0, 0, sizeof(U8), 		0.0, gpeALF_TYPE, gpeALF_U },
	{ gpaOPCi,	"I1", 		0, 0, 0, sizeof(I1), 		0.0, gpeALF_TYPE, gpeALF_I },
	{ gpaOPCi,	"I2", 		0, 0, 0, sizeof(I2), 		0.0, gpeALF_TYPE, gpeALF_I },
	{ gpaOPCi,	"I4", 		0, 0, 0, sizeof(I4), 		0.0, gpeALF_TYPE, gpeALF_I },
	{ gpaOPCi,	"I8", 		0, 0, 0, sizeof(I8), 		0.0, gpeALF_TYPE, gpeALF_I },
	{ gpaOPCi,	"F4", 		0, 0, 0, sizeof(float),		0.0, gpeALF_TYPE, gpeALF_D },
	{ gpaOPCi,	"F8", 		0, 0, 0, sizeof(double),	0.0, gpeALF_TYPE, gpeALF_D },

	{ gpaOPCi,	"sizeof(", 	0, 0, 0, 0,					0.0, gpeALF_FUNC },
	{ gpaOPCi,	"if(", 		0, 0, 0, 0,					0.0, gpeALF_FUNC },
	{ gpaOPCi,	"for(", 	0, 0, 0, 0,					0.0, gpeALF_FUNC },
	{ gpaOPCi,	"while(", 	0, 0, 0, 0,					0.0, gpeALF_FUNC },
	{ gpaOPCi,	"switch(",	0, 0, 0, 0,					0.0, gpeALF_FUNC },

	{ gpaOPCi,	"break",	0, 0, 0, 0,					0.0, gpeALF_FUNC },
	{ gpaOPCi,	"continue",	0, 0, 0, 0,					0.0, gpeALF_FUNC },

	{ gpaOPCi,	"class",	0, 0, 0, 0,					0.0, gpeALF_CLASS, gpeALF_TYPE },
	{ gpaOPCi,	"pub",		0, 0, 0, 0,					0.0, gpeALF_FUNC },
	{ gpaOPCi,	"prot",		0, 0, 0, 0,					0.0, gpeALF_FUNC },
	{ gpaOPCi,	"new",		0, 0, 0, 0,					0.0, gpeALF_FUNC },
	{ gpaOPCi,	"del",		0, 0, 0, 0,					0.0, gpeALF_FUNC },

	{ gpaOPCi,	"SYS",		0, 0, 0, 0,					0.0, gpeALF_CLASS },
	{ gpaOPCi,	"GT",		0, 0, 0, 0,					0.0, gpeALF_CLASS },
	{ gpaOPCi,	"PIC",		0, 0, 0, 0,					0.0, gpeALF_CLASS },


};


void gpcMASS::reset( void )
{
	gpmZ( asPRG );
	if( !iLEV )
	{
		U8 s8;
		PC.reset( &CMPL );
		aPC[0] = 0;
		PC.pPC( &CMPL, 0 );
		for( U4 i = 1, iN = gpmN(gpaOPCi), nwLEV = iLEV; i < iN; i++ )
		{
			PC.pPC( &CMPL, 0 )->cmpl_add( &CMPL, gpaOPCi[i].pSTR, gpaOPCi[i].nSTR );
			gpcCMPL* pPC = PC.pPC( &CMPL, i );
			if( !pPC )
				continue;
			pPC->typ = gpaOPCi[i].typ;
			pPC->wip = gpaOPCi[i].def;
			pPC->n_dat = gpaOPCi[i].nDAT;
		}
		iLEV++;
	}
	rstLEV = iLEV;
}
/*void gpcMASS::reset_o( void )
{
	gpmZ( asPRG );
	if( !iLEV )
	{
		U8 s8;

		anDICTix[iLEV] = 0;
		for( U4 i = 0, iN = gpmN(gpaOPCi), nwLEV = iLEV; i < iN; i++ )
		{
			apDICTix[nwLEV] = apDICTix[nwLEV]->dict_add( gpaOPCi[i].pSTR, gpaOPCi[i].nSTR );
			if( anDICTix[nwLEV] >= apDICTix[nwLEV]->nIX() )
				continue;

			apDICTopcd[nwLEV] = apDICTopcd[nwLEV]->lazy_add( &gpaOPCi[anDICTix[nwLEV]], sizeof(*gpaOPCi), s8 = -1, 1 );
			gpcOPCD	&OPCD = *((gpcOPCD*)(apDICTopcd[nwLEV]->p_alloc+s8));
			OPCD.lab = gpfSTR2ALF( OPCD.pSTR, OPCD.pSTR+OPCD.nSTR );

			anDICTix[nwLEV] = apDICTix[nwLEV]->nIX();
		}
		incLEV();
	}
	rstLEV = iLEV;
}*/
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
		*psPRG = mass.asPRG, c,
		*pPUB = gpsSTRpub;

	char sVAN[] = ".";
	U4 	nALF,
		utf8,
		&iLEV = mass.iLEV, alLEV = iLEV,
		*piDAT = mass.aiDAT, &alDAT = mass.alDAT,
		&iPC = mass.iPC, nPC;
	I4 nCHK;
	bool bSIGN = false, bALF;

	U8 	nN, nSKIP, nLEN, nSTR,
		nVAN = 0, s8;

	I2 nADD = 0;
	U8 u8;
	I8 i8;
	double d;
	gpeALF typ;


	mass.reset();
	// be kell jegyezni a rublikának egy saját változót amibe dolgozhat
	// és azon keresztül érhetik el
	cout << endl << "lv fn[mo:iP]nD				// iD/alD";
	gpcCMPL	com = 0, *pFND = NULL, *pMOM = NULL, *pPRNT = NULL, *pNEW = NULL, *pPAR;
	while( pS < pE )				/// 	COMPILER
	{
		pS += gpmNINCS( pS, " \t\r\n" );
		if( pS >= pE )
		{
			// feldolgozni ami még van
			break;
		}

		sVAN[0] = c = *pS;
        if( c < 0x80 ? gpaALFadd[c] : true )
        {
			// UTF8!
			nALF = gpfABCnincs( pSTR = pS, pE, nLEN );
			if( nALF )
			{
				bool bCNTI = false;

				// ALF esélyes ?
				bALF = (nALF == nLEN);


                nSTR = nALF + gpmVAN( pSTR+nALF, gpsPRG, nVAN );
				nLEN += nVAN;

				pSTR = gpmSTRnCPY( pPUB, pSTR, nSTR );

				pPUB += nSTR;
				pS += nSTR;

				nSKIP = gpmNINCS( pS, " \t\r\n" );
				c = pS[nSKIP];
				bCNTI =  c < 0x80 ? gpaALFadd[c] : true;

				if( !bCNTI )
				switch( c )
				{
					case '(':
						// el kell dönteni, hogy ez most fügvény
						// vagy a konstruktorát akarja meghívni
						// hogy inicializálja a példányt
						pPAR = mass.PC.pPC( &mass.CMPL, com.mPC );
						cout << endl;
						com.typ = pPAR->wip == gpeALF_DEC	? gpeALF_FUNC : gpeALF_TYPE;
						com.wip = com.typ == gpeALF_FUNC	? gpeALF_FUNC : gpeALF_INIT;
						*pPUB = c;
						pPUB++;
						if( com.wip == gpeALF_INIT )
						{
							//com.iINI = com.mPC;
							break;
						}

						if( com.typ == gpeALF_FUNC )
							nSTR++;
						break;
					default:
						break;
				}
				*pPUB = 0;
				pPUB++;

				if( !pMOM )
					pMOM = mass.PC.pPC( &mass.CMPL, com.mPC ); //mass.aPC[mass.iLEV] );

				nPC = com.nPC( &mass.CMPL );
				iPC = pMOM->cmpl_find( &mass.CMPL, pSTR, nSTR );
				pFND = pFND->pPC( &mass.CMPL, iPC );
				com.iLEV = iLEV;

				bool bFND = true;
				if( pFND )
				{
					if( !pFND->typ )
					{
						bFND = false;
						// nem talált újat akar
						if( pMOM )
						{
							// van valami mama
							switch( com.typ )
							{
								case gpeALF_CLASS:

									pMOM->cmpl_add( &mass.CMPL, pSTR, nSTR );
									pNEW = pMOM->pPC( &mass.CMPL, nPC );
									if( !pNEW )
										break; // nem jött létre?
									pPRNT = pNEW;


									pNEW->i_str = pSTR-gpsSTRpub;
									pNEW->n_str = nSTR;
									pNEW->wip = gpeALF_DEC; //com.typ;
									pNEW->typ = com.wip; //gpeALF_TYPE;
									pNEW->iLEV = iLEV;

									com.mPC = iPC = nPC;
									com.i_str = pSTR-gpsSTRpub;
									com.n_str = nSTR;

									//mass.incLEV();
									pMOM = NULL;
									break;


								case gpeALF_TYPE:
									pMOM->cmpl_add( &mass.CMPL, pSTR, nSTR );
									pNEW = pMOM->pPC( &mass.CMPL, nPC );
									if( !pNEW )
										break; // nem jött létre?
									pPRNT = pNEW;

									pNEW->i_str = pSTR-gpsSTRpub;
									pNEW->n_str = nSTR;
									if( com.wip == gpeALF_INIT )
									{
										pPAR = pMOM->pPC( &mass.CMPL, com.iDEF );
										pNEW->wip = pNEW->typ = pPAR->typ;
										//pPAR->wip;
										pNEW->iDEF = pPAR->iPC;
										pNEW->n_dat = pPAR->n_dat;
										pNEW->mPC = com.mPC;
										pPRNT->pINFO = (char*)pPUB;
										pPUB += sprintf( pPRNT->pINFO, "stuff(" );
									} else
										pNEW->typ = pNEW->wip = com.wip;

									pNEW->iLEV = iLEV;

									if( pNEW->n_dat = com.n_dat )
									{
										gpcCMPL* pM = pM->pPC( &mass.CMPL, pNEW->mPC);
										while( pM ? (pM->iPC != pM->mPC) : false )
										{
											pM->n_dat += pNEW->n_dat;
											if( pM->typ == gpeALF_FUNC )
											{
												pPRNT->pINFO = (char*)pPUB;
												pPUB += sprintf( pPRNT->pINFO, "f(para)" );

												break;
											}

											pM = pM->pPC( &mass.CMPL, pM->mPC);
										}
									}
									break;


								case gpeALF_FUNC:
									pMOM->cmpl_add( &mass.CMPL, pSTR, nSTR );
									pNEW = pMOM->pPC( &mass.CMPL, nPC );
									if( !pNEW )
										break; // nem jött létre?
									pPRNT = pNEW;
									nN = memcmp( pSTR, gpsSTRpub+pMOM->i_str, nSTR-1 );

									// azt kell megoldani, hogy paraméter listát akarjon készíteni
									pNEW->i_str = pSTR-gpsSTRpub;
									pNEW->n_str = nSTR;
									pNEW->wip = ( nN ) ? gpeALF_DEC : gpeALF_CONSTR;
									pNEW->typ = gpeALF_FUNC;
									pNEW->iLEV = iLEV;

									com.mPC = iPC = nPC;
									com.i_str = pSTR-gpsSTRpub;
									com.n_str = nSTR;

									pMOM = NULL;
									if( nN )
										break;

									pPRNT->pINFO = (char*)pPUB;
									pPUB += sprintf( pPRNT->pINFO, "c{constr()}" );

									// amikor a zárójelet bezárják akkorra kell lenie egy listának a szinten,
									// és rákeresni a lista alapján egy változatra
									break;

								default:
									break;

							}

						}
					} else {
						switch( pFND->typ )
						{
							case gpeALF_zero:
								bFND = true; /// ez mit keres itt?
								break;
							case gpeALF_CLASS:
								// deklarál egy osztályt;
								//mass.incLEV();
								pPRNT = &com;

								com.null();
								com.typ = pFND->typ;
								com.wip = pFND->wip;
								com.mPC = mass.aPC[mass.iLEV];
								com.i_str = pSTR-gpsSTRpub;
								com.n_str = nSTR;


								break;
							case gpeALF_TYPE:
								//com.null();
								if( iPC == mass.aPC[iLEV] )
								{
									bFND = true; /// construktor?
									com.typ = gpeALF_FUNC;
								} else
									com.typ = pFND->typ;
								pPRNT = &com;

								if( pFND->wip == gpeALF_DEC )
								{
									if( pFND->iDEF != pFND->iPC )
										pFND->iDEF = pFND->iPC;
									pFND->wip = gpeALF_DEF;
								}

								com.wip = pFND->wip;
											//gpeALF_DEF;
								com.iDEF = pFND->iDEF;
								com.mPC = mass.aPC[mass.iLEV];
								pMOM = NULL;
								com.i_str = pSTR-gpsSTRpub;
								com.n_str = nSTR;
								com.n_dat =	pFND->n_dat;
							if( com.typ == pFND->typ )
								break;

							case gpeALF_FUNC:


								break;
							default:
								break;
						}
					}

					if( pPRNT )
					{
						if( iPC ? iPC == pPRNT->mPC : false )
						{
							bFND = true; /// construktor?
						}
						*pPUB = 0;
						pPUB++;
						U2 n = sprintf( (char*)pPUB, "%d %d[%d:%d]%db %s%s",
																			pPRNT->iLEV, iPC,
																			pPRNT->mPC, pPRNT->iPC,
																			pPRNT->n_dat,
																			(gppTAB-mass.relLEV()), pSTR


																			); //  );
						if( pPRNT->typ == pPRNT->wip )
							mass.aiDAT[iLEV] += pPRNT->n_dat;

						if( mass.alDAT < mass.aiDAT[iLEV] )
						{
							mass.alDAT = mass.aiDAT[iLEV];
						}

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
				if( bCNTI )
					continue;

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
							mass.decLEV();
							/*if( pPRNT = pPRNT->pPC( &mass.CMPL, iPC ) )
							{

								U2 n = sprintf( (char*)pPUB, "RT%d %d[%d:%d]%db %s%s",
																						iLEV, iPC,
																						pPRNT->mPC, pPRNT->iPC,
																						pPRNT->n_dat,
																						(gppTAB-mass.relLEV()), gpsSTRpub+pPRNT->i_str


																						);
								cout << endl << (char*)pPUB;
							}*/
							nVAN = 0;
							break;




						case '+':
							break;
						case '-':
							break;

						case '*':

							break;
						case '/':
							if( pS[nVAN] == '*'  )
							if( U1* pCOM = (U1*)strstr( (char*)pS+nVAN+1, "*/" ) )
							{
								nVAN = (pCOM-pS)+2;
								break;
							} else {
								nVAN = 0;
								pS = pE;
								break;
							}
							break;

						case ',':	// vessző
							// berakjuk ami létrejött a sorba

							break;
						case '.':	// pont

							break;
					}
				}
				else if( c < 0x3A )
				{
					// 30-39 // számok 0-9

					u8 = gpfSTR2U8( pS-(nVAN+1), &pS );
					if( *pS == '.' )
                    {
						typ = gpeALF_D;
						d = gpmSTR2D( pS ) + (double)u8;		///
						cout << endl << (gppTAB-iLEV)  << d;
					} else {
						if( nADD < 0 )
						{
							typ = gpeALF_I;
							i8 = -u8;							///
							cout << endl << (gppTAB-iLEV) << i8;
						} else {
							cout << endl << (gppTAB-iLEV) << u8;
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
							break;
						case ';':
							// comment
							nVAN = 0;
							break;

						case '<':
							break;
						case '=':

							break;
						case '>':
							break;

						case '?':
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
					mass.decLEV();
					/*if( pPRNT = pPRNT->pPC( &mass.CMPL, iPC ) )
					{

						U2 n = sprintf( (char*)pPUB, "RT%d %d[%d:%d]%db %s%s",
																				iLEV, iPC,
																				pPRNT->mPC, pPRNT->iPC,
																				pPRNT->n_dat,
																				(gppTAB-mass.relLEV()), gpsSTRpub+pPRNT->i_str


																				);
						cout << endl << (char*)pPUB;
					}*/
					nVAN = 0;
					break;

				case '{':
					mass.incLEV();
					nVAN = 0;
					break;
				case '}':
					mass.decLEV();
					/*if( pPRNT = pPRNT->pPC( &mass.CMPL, iPC ) )
					{

						U2 n = sprintf( (char*)pPUB, "RT%d %d[%d:%d]%db %s%s",
																				iLEV, iPC,
																				pPRNT->mPC, pPRNT->iPC,
																				pPRNT->n_dat,
																				(gppTAB-mass.relLEV()), gpsSTRpub+pPRNT->i_str


																				);
						cout << endl << (char*)pPUB;
					}*/
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
