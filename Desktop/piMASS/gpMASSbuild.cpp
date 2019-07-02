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

char gpsPRG[] = " \t\r\n\a .,:;!? =<> -+*/%^ &~|@#$ \\ \" \' ()[]{} ";
char gpsTAB[] = "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t",

	 *gppTAB = gpsTAB+strlen(gpsTAB);
/*gpcOPCD::gpcOPCD( const char* pS, char a, char m, I8 i, U8 u, double _d, gpeALF t )
{
	pSTR = (U1*)pS;
	nADD = a;
	nMUL = m;
	i8 = i;
	u8 = u;
	d = _d;
	typ = t;

	nSTR = gpmVAN( pSTR, gpsPRG, nLEN );

}*/


/*gpcOPCD gpaOPC[] = {
	{ "false", 	0, 0, 0, size(U1), 		0.0, gpeALF_U },
	{ "true", 	0, 0, 0, size(U1), 		0.0, gpeALF_U },

	{ "U2", 	0, 0, 0, size(U2), 		0.0, gpeALF_U },

	{ "U1", 	0, 0, 0, size(U1), 		0.0, gpeALF_U },
	{ "U2", 	0, 0, 0, size(U2), 		0.0, gpeALF_U },
	{ "U4", 	0, 0, 0, size(U4), 		0.0, gpeALF_U },
	{ "U8", 	0, 0, 0, size(U8), 		0.0, gpeALF_U },
	{ "I1", 	0, 0, 0, size(U1), 		0.0, gpeALF_I },
	{ "I2", 	0, 0, 0, size(U2), 		0.0, gpeALF_I },
	{ "I4", 	0, 0, 0, size(U4), 		0.0, gpeALF_I },
	{ "I8", 	0, 0, 0, size(U8), 		0.0, gpeALF_I },
	{ "F4", 	0, 0, 0, size(F4), 		0.0, gpeALF_F },
	{ "F8", 	0, 0, 0, size(double),	0.0, gpeALF_D },

	{ "if", 		0, 0, 0, 0,	0.0, gpeALF_FUNC },
	{ "for", 		0, 0, 0, 0,	0.0, gpeALF_FUNC },
	{ "while", 		0, 0, 0, 0,	0.0, gpeALF_FUNC },
	{ "switch",		0, 0, 0, 0,	0.0, gpeALF_FUNC },
	{ "break",		0, 0, 0, 0,	0.0, gpeALF_FUNC },
	{ "continue",	0, 0, 0, 0,	0.0, gpeALF_FUNC },
	{ "class",		0, 0, 0, 0,	0.0, gpeALF_FUNC },
	{ "pud",		0, 0, 0, 0,	0.0, gpeALF_FUNC },
	{ "prot",		0, 0, 0, 0,	0.0, gpeALF_FUNC },

	{ "SYS",		0, 0, 0, 0,	0.0, gpeALF_FUNC },
	{ "GT",			0, 0, 0, 0,	0.0, gpeALF_FUNC },
	{ "PIC",		0, 0, 0, 0,	0.0, gpeALF_FUNC },


};*/
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
	{ gpaOPCi,	"true", 	0, 0, 0, sizeof(U1), 		0.0, gpeALF_CONST },

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

	{ gpaOPCi,	"sizeof", 	0, 0, 0, 0,					0.0, gpeALF_FUNC },
	{ gpaOPCi,	"if", 		0, 0, 0, 0,					0.0, gpeALF_FUNC },
	{ gpaOPCi,	"for", 		0, 0, 0, 0,					0.0, gpeALF_FUNC },
	{ gpaOPCi,	"while", 	0, 0, 0, 0,					0.0, gpeALF_FUNC },
	{ gpaOPCi,	"switch",	0, 0, 0, 0,					0.0, gpeALF_FUNC },

	{ gpaOPCi,	"break",	0, 0, 0, 0,					0.0, gpeALF_FUNC },
	{ gpaOPCi,	"continue",	0, 0, 0, 0,					0.0, gpeALF_FUNC },

	{ gpaOPCi,	"class",	0, 0, 0, 0,					0.0, gpeALF_CLASS, gpeALF_TYPE },
	{ gpaOPCi,	"pud",		0, 0, 0, 0,					0.0, gpeALF_FUNC },
	{ gpaOPCi,	"prot",		0, 0, 0, 0,					0.0, gpeALF_FUNC },
	{ gpaOPCi,	"new",		0, 0, 0, 0,					0.0, gpeALF_FUNC },
	{ gpaOPCi,	"del",		0, 0, 0, 0,					0.0, gpeALF_FUNC },

	{ gpaOPCi,	"SYS",		0, 0, 0, 0,					0.0, gpeALF_CLASS },
	{ gpaOPCi,	"GT",		0, 0, 0, 0,					0.0, gpeALF_CLASS },
	{ gpaOPCi,	"PIC",		0, 0, 0, 0,					0.0, gpeALF_CLASS },


};
U4 gpcCMPL::cmpl_find( gpcLAZY* pCMPL, U1* pS, U4 nS )
{
	if( this ? !pCMPL : true )
		return 0;

	gpcCMPL** ppC = (gpcCMPL**)(pCMPL->n_load ? pCMPL->p_alloc : NULL);
	if( !ppC )
		return 0;

	gpcCMPL* pC = this;
	U4 ifPC, nPC, *pPC = NULL;

	while( pC )
	{
		ifPC = pC->p_kid->dict_find( pS, nS, nPC );

		if( ifPC >= nPC )
		{
			if( pC->mPC == pC->iPC )
				return 0;
			pC = ppC[pC->mPC];
			continue;
		}
		U4x4 *p_ix0 = ((U4x4*)pC->p_kid->ix.p_alloc);
		if( p_ix0[ifPC].y != nS )
		{
			if( pC->mPC == pC->iPC )
				return 0;
			pC = ppC[pC->mPC];
			continue;
		}

		if( pC->p_iPC ? !pC->p_iPC->p_alloc : true )
			return 0;

		return ((U4*)pC->p_iPC->p_alloc)[ifPC];
	}
	return 0;
}
gpcLAZY* gpcCMPL::cmpl_add( gpcLAZY* pCMPL, U1* pS, U4 nS )
{

	U4 fnd = cmpl_find( pCMPL, pS, nS );
	if( fnd )
		return pCMPL;

	if( this ? !pCMPL : true )
		return pCMPL;

	U4 w = p_kid->nIX();
	p_kid = p_kid->dict_add( pS, nS );
	if( w >= p_kid->nIX() )
		return pCMPL;

	U8 iPC8 = -1;

	pCMPL = pCMPL->lazy_add( NULL, sizeof(this), iPC8 );
	gpcCMPL	**ppC = (gpcCMPL**)pCMPL->p_alloc;
	if( !ppC )
	{
		return pCMPL;
	}
	gpcCMPL* pC = new gpcCMPL( iPC );
	if( !pC )
		return pCMPL;

	pC->iPC = iPC8/sizeof(this);
	ppC[pC->iPC] = pC;


	p_iPC = p_iPC->lazy_add( &pC->iPC, sizeof(pC->iPC), iPC8 = -1 );

	return pCMPL;
}

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
}
void gpcMASS::reset_o( void )
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
}
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
		utf8, iDAT = 0, nPC, iPC, &iLEV = mass.iLEV, alLEV = iLEV;
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
	cout << endl << "fnd [mom:iPC iD:nD]";
	gpcCMPL	com = 0, *pFND, *pMOM = NULL, *pPRNT;
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
				pPRNT = NULL;
				// ALF esélyes ?
				bALF = (nALF == nLEN);


                nSTR = nALF + gpmVAN( pSTR+nALF, gpsPRG, nVAN );
				nLEN += nVAN;

				gpmSTRnCPY( pPUB, pSTR, nSTR )[nSTR] = 0;
				pSTR = pPUB;
				pPUB += nSTR+1;
				if( !pMOM )
					pMOM = mass.PC.pPC( &mass.CMPL, com.mPC ); //mass.aPC[mass.iLEV] );

				nPC = com.nPC( &mass.CMPL );
				iPC = pMOM->cmpl_find( &mass.CMPL, pSTR, nSTR );
				pFND = pFND->pPC( &mass.CMPL, iPC );

				if( pFND )
				{
					switch( pFND->typ )
					{
						case gpeALF_zero:
							{
								if( pMOM )
								{
									switch( com.typ )
									{
										case gpeALF_CLASS:

											pMOM->cmpl_add( &mass.CMPL, pSTR, nSTR );
											pFND = pMOM->pPC( &mass.CMPL, nPC );
											if( !pFND )
												break; // nem jött létre?

											pPRNT = pFND;
											pFND->i_str = pSTR-gpsSTRpub;
											pFND->n_str = nSTR;
											pFND->wip = gpeALF_DEC; //com.typ;
											pFND->typ = com.wip; //gpeALF_TYPE;

											iLEV++;

											com.mPC = nPC;
											pMOM = NULL;
											break;

										case gpeALF_TYPE:
											pMOM->cmpl_add( &mass.CMPL, pSTR, nSTR );
											pFND = pMOM->pPC( &mass.CMPL, nPC );
											if( !pFND )
												break; // nem jött létre?
											pPRNT = pFND;
											pFND->i_str = pSTR-gpsSTRpub;
											pFND->n_str = nSTR;
											pFND->typ = pFND->wip = com.wip;
											if( pFND->n_dat = com.n_dat )
											{
												gpcCMPL* pM = pM->pPC( &mass.CMPL, pFND->mPC);
												while( pM ? (pM->iPC != pM->mPC) : false )
												{
													pM->n_dat += pFND->n_dat;
													pM = pM->pPC( &mass.CMPL, pM->mPC);
												}
											}
											break;
										default:
											break;

									}

								}
							}
							break;
						case gpeALF_CLASS:
							// deklarál egy osztályt;
							if( iLEV )
							{
								iLEV--;
								pMOM = NULL;
							}
							com.null();
							com.typ = pFND->typ;
							com.wip = pFND->wip;
							com.mPC = mass.aPC[mass.iLEV];
							pPRNT = &com;


							break;
						case gpeALF_TYPE:
							//com.null();
							com.typ = pFND->typ;
							com.wip = pFND->wip;
							com.mPC = mass.aPC[mass.iLEV];
							com.n_dat =	pFND->n_dat;
							pPRNT = &com;
							break;
						default:
							break;
					}

					if( pPRNT )
					{
						U2 n = sprintf( (char*)pPUB, "%d iPC[%d:%d %d:%d]%s%s ", iPC, pPRNT->iPC, pPRNT->mPC, iDAT, pPRNT->n_dat, (gppTAB-iLEV), pSTR ); //  );
						if( pPRNT->wip == gpeALF_DEF  )
							iDAT += pPRNT->n_dat;
						gpfVAN( pPUB, NULL, nLEN );
						nLEN += gpfVANn( pPUB, (U1*)"\t" )*3;
						cout << endl << (char*)pPUB << ( gppTAB-(8-min(8, (nLEN/4))) ) << "//";
					}
				}
				pS += nSTR;
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
							break;

						case ')':
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
					} else {
						if( nADD < 0 )
						{
							typ = gpeALF_I;
							i8 = -u8;							///
						}
						else
							typ = gpeALF_U;
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

					nVAN = 0;
					break;
				case ']':
					nVAN = 0;
					break;

				case '{':
					nVAN = 0;
					break;
				case '}':
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
}
void gpcSRC::cmpi_trash( gpcMASS& mass, bool bDBG )
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
		*pSe, *pE = pA+nL, *pSTR = NULL;
	char sVAN[] = ".";
	U4 	nALF,
		utf8;
	I4 nCHK;
	bool bSIGN = false, bALF;

	U8 	nN, nSKIP, nLEN,
		nVAN = 0, s8;

	mass.reset();
	/*gpmZ( mass.asPRG );
	if( !mass.iLEV )
	{

		mass.anDICTix[mass.iLEV] = 0;
        for( U4 i = 0, in = gpmN(gpaOPCi), mlD = mass.iLEV; i < in; i++ )
        {
			mass.apDICTix[mlD] = mass.apDICTix[mlD]->dict_add( (char*)gpaOPCi[i].pSTR, gpaOPCi[i].nSTR );
			if( mass.anDICTix[mlD] >= mass.apDICTix[mlD]->nIX() )
				continue;

			mass.apDICTopcd[mlD] = mass.apDICTopcd[mlD]->lazy_add( &gpaOPCi[mass.anDICTix[mlD]], sizeof(*gpaOPCi), s8 = -1, 1 );
			gpcOPCD	&OPCD = *((gpcOPCD*)(mass.apDICTopcd[mlD]->p_alloc+s8));
			OPCD.lab = gpfSTR2ALF( OPCD.pSTR, OPCD.pSTR+OPCD.nSTR );


			mass.anDICTix[mlD] = mass.apDICTix[mlD]->nIX();
		}
		mass.iLEV++;
	}
	mass.nLEV = mass.iLEV+1;*/

	U4	iPC0 = 0, iPC = iPC0,
		iDAT = 0, ixDCT, nDCT = 0,
		iL,

		&iLEV = mass.iLEV, &nLEV = mass.nLEV, prevLEV;

	endD = (strtD = iLEV)+1;

	U1		*psPRG = mass.asPRG, c,
			*pPUB = gpsSTRpub;

	gpcOPCD		*pPRG = mass.aPRG;

	gpcLAZY		**ppDICTopcd = mass.apDICTopcd;
	gpcLZYdct	**ppDICTix = mass.apDICTix;
	U4	*pnDICTix = mass.anDICTix,
		*pLEVsp = mass.aLEVsp;

	double d = 0;
	while( pS < pE )				/// 	COMPILER
	{
		pS += gpmNINCS( pS, " \t\r\n" );
		if( pS >= pE )
		{
			// feldolgozni ami még van
			break;
		}
		prevLEV = iLEV;

		//iLEV = nLEV-1;

		sVAN[0] = c = *pS;
        if( c < 0x80 ? gpaALFadd[c] : true )
        {
			// UTF8!
			nALF = gpfABCnincs( pPRG[iPC].pSTR = pS, pE, pPRG[iPC].nLEN );
			if( nALF )
			{
				// ALF esélyes ?
				bALF = (nALF == pPRG[iPC].nLEN);


                pPRG[iPC].nSTR = nALF + gpmVAN( pPRG[iPC].pSTR+nALF, gpsPRG, nVAN );
				pS += pPRG[iPC].nSTR;
				pPRG[iPC].nLEN += nVAN;

				gpmSTRnCPY( pPUB, pPRG[iPC].pSTR, pPRG[iPC].nSTR )[pPRG[iPC].nSTR] = 0;
				iL = iLEV;

				if( pPRG[iPC].nWIP == gpePRG_newclass )
				{
					U4 ixDCT = pnDICTix[iL];
					ppDICTix[iL] = ppDICTix[iL]->dict_add( pPRG[iPC].pSTR, pPRG[iPC].nSTR );
					if( ixDCT >= ppDICTix[iL]->nIX() )
					{
						// nem sikerült
						continue;
					}
					// egyébként opcd-t is kap
                    ppDICTopcd[ iL ] = ppDICTopcd[ iL ]->lazy_add( &pPRG[iPC], sizeof(&pPRG[iPC]), s8 = -1, 1 );
					gpcOPCD	&nwOPCD = *((gpcOPCD*)(ppDICTopcd[iL]->p_alloc+s8));
					nwOPCD.lab = gpfSTR2ALF( nwOPCD.pSTR, nwOPCD.pSTR+nwOPCD.nSTR );
					pnDICTix[iL] = ppDICTix[iL]->nIX();

				} else {

					for( ; iL <= iLEV; iL-- )
					{
						ixDCT = ppDICTix[iL]->dict_find( pPUB, pPRG[iPC].nSTR, pnDICTix[iL] );
						if( ixDCT >= pnDICTix[iL] )
							continue;

						if(
							gpcOPCD* pOPCD = 	ppDICTopcd[iL]->p_alloc
												? ((gpcOPCD*)ppDICTopcd[iL]->p_alloc)+ixDCT
												: NULL
						)
						{
							gpcOPCD& opcd = *pOPCD;
							if( pPRG[iPC].nSTR != opcd.nSTR )
								continue;

							pPRG[iPC].typ = opcd.typ;
							pPRG[iPC].nDAT = opcd.nDAT;
							switch( opcd.typ )
							{
								case gpeALF_COMMAND:
									pPRG[iPC].nWIP = gpePRG_newclass;
									pPRG[iPC].nDAT = 0;
									mass.incLEV();
									pLEVsp[iLEV] = iPC;
									break;
								case gpeALF_TYPE:
									pPRG[iPC].nWIP = gpePRG_typ_name;
									pLEVsp[iLEV] = iPC;
									break;
								case gpeALF_VAR:
									pPRG[iPC].nWIP = gpePRG_var_name;
									break;
								default:
									pPRG[iPC].nWIP = 0;

							}
							pPRG[iPC].nDAT = opcd.nDAT;
						}

						break;
					}
				}
				iPC0 = pLEVsp[ iLEV ];


				if( iPC0 != iPC )
				if( iL >= iLEV )
				{
					// nem talált
					pPRG[iPC].nWIP	= pPRG[iPC0].nWIP; // gpePRG_var_name;
					pPRG[iPC].typ	= pPRG[iPC0].typ;
					pPRG[iPC].nDAT	= pPRG[iPC0].nDAT;
					pPRG[iPC].iSTR	= pPRG[iPC0].iSTR;
					for( U2 w = 0, we = min(gpePRG_end, pPRG[iPC].iSTR ); w < we; w++ )
					{
						pPRG[iPC].aTYP[w] = pPRG[iPC0].aTYP[w];
					}
				}

				pPRG[iPC].aTYP[pPRG[iPC].iSTR] = pPRG[iPC].typ;
				pPRG[iPC].apSTR[pPRG[iPC].iSTR] = pPUB;
				pPRG[iPC].iDAT = iDAT;
				pPRG[iPC].iLEV = iL;
				pPRG[iPC].ixDCT = ixDCT;
				pPRG[iPC].iSTR++;

				pPUB += pPRG[iPC].nSTR+1;
				continue;
			}

        }

		if( pPRG[iPC].iSTR )
        {

			if( pPRG[iPC].nADD > 0 )
				cout << " + ";
			else if( pPRG[iPC].nADD < 0 )
				cout << " - ";
			if( pPRG[iPC].nMUL > 0 )
				cout << "*";
			else if( pPRG[iPC].nMUL < 0 )
				cout << "/";
			U2 n = sprintf( (char*)pPUB, "iPC[%d:%d %d:%d]%s", iPC, iLEV, iDAT, pPRG[iPC].nDAT, (gppTAB-iLEV)  );
			iDAT += pPRG[iPC].nDAT;
			//cout << endl << "iPC[" << iPC << ":" << iLEV << "]" << (gppTAB-iLEV) ;
			for( U2 w = 0, we = min(gpePRG_end, pPRG[iPC].iSTR+1 ); w < we; w++ )
			{
				if( !pPRG[iPC].apSTR[w] )
				{
					//cout << sVAN << endl;
					//if( pPRG[iPC].pSTR )
					//	cout << pPRG[iPC].pSTR;
					continue;
				}
				n += sprintf( (char*)pPUB+n, "%s ", pPRG[iPC].apSTR[w] );
				//cout << pPRG[iPC].apSTR[w] << " " ;
			}
			gpfVAN( pPUB, NULL, nLEN );
			nLEN += gpfVANn( pPUB, (U1*)"\t" )*3;
			cout << endl << (char*)pPUB << ( gppTAB-(8-min(8, (nLEN/4))) ) << "//";

			for( U2 w = 0, we = min(gpePRG_end, pPRG[iPC].iSTR+1 ); w < we; w++ )
			if( pPRG[iPC].aTYP[w] )
			{
				gpmALF2STR( pPUB,pPRG[iPC].aTYP[w] );
				pPUB[3] = 0;
				cout << (char*)pPUB;

			}

			//cout << endl;



			switch( pPRG[iPC].typ )
			{
				case gpeALF_STR:
						cout << (gppTAB-iLEV) << "\"...\"" << endl;
						break;
				case gpeALF_I:
						cout << (gppTAB-iLEV) << pPRG[iPC].i8 << endl;
						break;
				case gpeALF_U:
						cout << (gppTAB-iLEV) << pPRG[iPC].u8 << endl;
						break;
				case gpeALF_D:
						cout << (gppTAB-iLEV) << pPRG[iPC].d << endl;
						break;
				default:
					break;
				/*default:
						psPRG[iPC+1] = 0;
						cout << (gppTAB-iLEV) << (char*)psPRG+iPC << endl;*/
			}


			iPC++;
			pPRG[iPC].null();
			pPRG[iPC].iLEV = iLEV;

        }

		//cout << (gppTAB-iLEV) << (char*)sVAN << endl;
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
							psPRG[iPC] = c;
							pPRG[iPC].typ = gpeALF_STR;
							pPRG[iPC].pSTR = pS+nVAN;

                            nVAN += gpmVAN( pPRG[iPC].pSTR, sVAN, pPRG[iPC].nLEN );
                            pPRG[iPC].nSTR = (pS+nVAN) - pPRG[iPC].pSTR;
							if( pS[nVAN] == c )
								nVAN++;
							pPRG[iPC].nWIP = 1;
							break;
						case '\'':
							psPRG[iPC] = c;
							pPRG[iPC].typ = gpeALF_U;
							pPRG[iPC].u8 = gpfUTF8( pS+nVAN, NULL );

                            nVAN += gpmVAN( pS+nVAN, sVAN, nN );
                            if( pS[nVAN] == c )
								nVAN++;

							pPRG[iPC].nWIP = 1;
							break;


						case '(':
							psPRG[iPC] = c;
							pPRG[iPC].typ = gpeALF_FUNC;

							mass.incLEV();

							pPRG[iPC].nWIP = 1;
							nVAN = 0;
							break;

						case ')':
							psPRG[iPC] = c;
							pPRG[iPC].typ = gpeALF_FUNC;

							mass.decLEV();

							pPRG[iPC].nWIP = 1;
							nVAN = 0;
							break;




						case '+':
							pPRG[iPC].nADD += nVAN+1;
							break;
						case '-':
							pPRG[iPC].nADD -= nVAN+1;
							break;

						case '*':
							nCHK = pPRG[iPC].nMUL;
							pPRG[iPC].nMUL += nVAN+1;
							if( nCHK < 0 )
							if( (pPRG[iPC].nMUL-nCHK) > 1 )
							{
								// ez biza /*..//** vagy valami ilyesmi
								U1* pSS = (U1*)strstr( (char*)pS, "*/" ); // megkeressük a végét
								nSKIP = pSS ? (pSS-pS)+2 : (pE-pS);
								nSKIP += gpmNINCS( pS+nSKIP, "*/" );
								if( nSKIP > pE-pS )
									nVAN = pE-pS;
								else
									nVAN = nSKIP;
								break;
							}
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

							pPRG[iPC].nMUL -= nVAN+1;
							/*if( pPRG[iPC].nMUL < -1 )
							{
								pPRG[iPC].nMUL = 0;
								/// comment skip next line
								nSKIP = gpmVAN( pS, "\r\n\a", nN );
								nSKIP += gpmNINCS( pS+nSKIP, "\r\n" );
								if( nSKIP > pE-pS )
									nVAN = pE-pS;
								else
									nVAN = nSKIP;

								break;
							}*/

							break;

						case ',':	// vessző
							// berakjuk ami létrejött a sorba
							psPRG[iPC] = c;

							break;
						case '.':	// pont
							psPRG[iPC] = c;

							break;

					}
				}
				else if( c < 0x3A )
				{
					// 30-39 // számok 0-9
                    pPRG[iPC].u8 = gpfSTR2U8( pS-(nVAN+1), &pS );
					if( *pS == '.' )
                    {
						pPRG[iPC].typ = gpeALF_D;
						pPRG[iPC].d = gpmSTR2D( pS ) + (double)pPRG[iPC].u8;		///
						if( pPRG[iPC].nADD < 0 )
							pPRG[iPC].d *= -1;
                    } else {
						if( pPRG[iPC].nADD < 0 )
						{
							pPRG[iPC].typ = gpeALF_I;
							pPRG[iPC].i8 = -pPRG[iPC].u8;							///
						}
						else
							pPRG[iPC].typ = gpeALF_U;
						pPRG[iPC].d = 0;
                    }
                    nVAN = 0;

					pPRG[iPC].nWIP = 1;
				} else {
					switch(c)
					{
						// 3A-3F
						case ':':
							break;
						case ';':
							// comment
							nVAN += gpmVAN( pS, "\r\n\a", nN );
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
					psPRG[iPC] = c;
					pPRG[iPC].typ = gpeALF_FUNC;

					mass.incLEV();

					pPRG[iPC].nWIP = 1;
					nVAN = 0;
					break;
				case ']':
					psPRG[iPC] = c;
					pPRG[iPC].typ = gpeALF_FUNC;

					mass.decLEV();

					pPRG[iPC].nWIP = 1;
					nVAN = 0;
					break;

				case '{':
					psPRG[iPC] = c;
					pPRG[iPC].typ = gpeALF_FUNC;

					mass.incLEV();

					pPRG[iPC].nWIP = 1;
					nVAN = 0;
					break;
				case '}':
					psPRG[iPC] = c;
					pPRG[iPC].typ = gpeALF_FUNC;

					mass.decLEV();

					pPRG[iPC].nWIP = 1;
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

		if( prevLEV > iLEV )
		{
			iPC0 = pLEVsp[ iLEV ];
			pPRG[iPC0].nDAT = iDAT-pPRG[iPC0].iDAT;
			U2 n = sprintf( (char*)pPUB, "RET[%d:%d %d:%d]%s", iPC0, iLEV, iDAT, pPRG[iPC0].nDAT, (gppTAB-iLEV)  );
			gpfVAN( pPUB, NULL, nLEN );
			nLEN += gpfVANn( pPUB, (U1*)"\t" )*3;
			cout << endl << (char*)pPUB << ( gppTAB-(8-min(8, (nLEN/4))) ) << "//";
		}

	}
	nBLD = nVER;
}
