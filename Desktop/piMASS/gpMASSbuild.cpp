#include "gpcSRC.h"
<<<<<<< HEAD

=======
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
U8 gpfALF2STR(char* p_out, I8 d0)
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
>>>>>>> 11d5ed1eddd0cbdb7e509d0bfcd871249b3b5667
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
<<<<<<< HEAD
		nX = (*pS == '#');
		if( nX )
=======
		nX = *pS == '#';
		if( nX )	// #TAG
>>>>>>> 11d5ed1eddd0cbdb7e509d0bfcd871249b3b5667
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
<<<<<<< HEAD
void gpcSRC::build( gpcMASS& mass )
{
	hd( mass );
	U1* pS = pB;
	while( pS-pA < nL )
	{
=======

char gpsPRG[] = " \t\r\n\a .,!? =<> -+*/%^ &~|@#$ \\ \" \' ()[]{} ";
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
gpcOPCD::gpcOPCD( const gpcOPCD* pTHIS, const char* pS, char a, char m, I8 i, U8 u, double _d, gpeALF t )
{
	gpmCLR;

	pSTR = (U1*)pS;
	nADD = a;
	nMUL = m;
	i8 = (this-pTHIS)+i;
	u8 = u;
	d = _d;
	typ = t;

	nSTR = gpmVAN( pSTR, gpsPRG, nLEN );

}

static const gpcOPCD gpaOPCi[] = {
	// this,	pS,			a, m, i, u,				d,		t;
	{ gpaOPCi,	"false", 	0, 0, 0, sizeof(U1), 	0.0, gpeALF_CONST },
	{ gpaOPCi,	"true", 	0, 0, 0, sizeof(U1), 	0.0, gpeALF_CONST },

	{ gpaOPCi,	"U1", 	0, 0, 0, sizeof(U1), 		0.0, gpeALF_TYU },
	{ gpaOPCi,	"U2", 	0, 0, 0, sizeof(U2), 		0.0, gpeALF_TYU },
	{ gpaOPCi,	"U4", 	0, 0, 0, sizeof(U4), 		0.0, gpeALF_TYU },
	{ gpaOPCi,	"U8", 	0, 0, 0, sizeof(U8), 		0.0, gpeALF_TYU },
	{ gpaOPCi,	"I1", 	0, 0, 0, sizeof(U1), 		0.0, gpeALF_TYI },
	{ gpaOPCi,	"I2", 	0, 0, 0, sizeof(U2), 		0.0, gpeALF_TYI },
	{ gpaOPCi,	"I4", 	0, 0, 0, sizeof(U4), 		0.0, gpeALF_TYI },
	{ gpaOPCi,	"I8", 	0, 0, 0, sizeof(U8), 		0.0, gpeALF_TYI },
	{ gpaOPCi,	"F4", 	0, 0, 0, sizeof(float),		0.0, gpeALF_TYF },
	{ gpaOPCi,	"F8", 	0, 0, 0, sizeof(double),	0.0, gpeALF_TYF },

	{ gpaOPCi,	"sizeof", 		0, 0, 0, 0,	0.0, gpeALF_FUNC },
	{ gpaOPCi,	"if", 			0, 0, 0, 0,	0.0, gpeALF_FUNC },
	{ gpaOPCi,	"for", 			0, 0, 0, 0,	0.0, gpeALF_FUNC },
	{ gpaOPCi,	"while", 		0, 0, 0, 0,	0.0, gpeALF_FUNC },
	{ gpaOPCi,	"switch",		0, 0, 0, 0,	0.0, gpeALF_FUNC },

	{ gpaOPCi,	"break",		0, 0, 0, 0,	0.0, gpeALF_FUNC },
	{ gpaOPCi,	"continue",		0, 0, 0, 0,	0.0, gpeALF_FUNC },

	{ gpaOPCi,	"class",		0, 0, 0, 0,	0.0, gpeALF_FUNC },
	{ gpaOPCi,	"pud",			0, 0, 0, 0,	0.0, gpeALF_FUNC },
	{ gpaOPCi,	"prot",			0, 0, 0, 0,	0.0, gpeALF_FUNC },
	{ gpaOPCi,	"new",			0, 0, 0, 0,	0.0, gpeALF_FUNC },
	{ gpaOPCi,	"del",			0, 0, 0, 0,	0.0, gpeALF_FUNC },

	{ gpaOPCi,	"SYS",		0, 0, 0, 0,	0.0, gpeALF_CLASS },
	{ gpaOPCi,	"GT",		0, 0, 0, 0,	0.0, gpeALF_CLASS },
	{ gpaOPCi,	"PIC",		0, 0, 0, 0,	0.0, gpeALF_CLASS },


};
char gpsSTRpub[0x1000];
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
	cout << "." ;

	U1	*pS = *pB == '\a' ? pB+1 : pB,
		*pSe, *pE = pA+nL, *pSTR = NULL;
	char sVAN[] = ".";
	U4 	nALF,
		utf8;
	I4 nCHK;
	bool bSIGN = false, bALF;

	U8 	nN, nSKIP,
		nVAN = 0;

	if( !mass.nSPdct )
	{
        for( U4 i = 0, in = gpmN(gpaOPCi); i < in; i++ )
        {
			*mass.apSPdct = (*mass.apSPdct)->dict_add( (char*)gpaOPCi[i].pSTR, gpaOPCi[i].nSTR );
        }
		*mass.anSPdct = (*mass.apSPdct)->nIX();
		*mass.apSPdtcOPCD = (*mass.apSPdtcOPCD)->lazy_add( gpaOPCi, sizeof(gpaOPCi), nN = -1, 1 );
	}

	U2	iPC = 0,
		nSPdct = mass.nSPdct+1;
	U4 iDAT;


	gpmZ( mass.aSPc );
	U1		*psPRG = mass.aSPc, c;
	gpcOPCD	*pPRG = mass.aSPb;
	gpcLZYdct	**ppSPdct = mass.apSPdct;
	double d = 0;
	while( pS < pE )				/// 	COMPILER
	{
		pS += gpmNINCS( pS, " \t\r\n" );
		if( pS >= pE )
		{
			// feldolgozni ami még van
			break;
		}

        c = *pS;
        if( c < 0x80 ? gpaALFadd[c] : true )
        {
			U4 ixDCT = -1, nDCT;

			// UTF8!
			nALF = gpfABCnincs( pPRG[iPC].pSTR = pS, pE, pPRG[iPC].nLEN );
			if( nALF )
			{
				// ALF esélyes ?
				bALF = (nALF == pPRG[iPC].nLEN);

                pPRG[iPC].nSTR = nALF + gpmVAN( pPRG[iPC].pSTR+nALF, gpsPRG, nN );
				pPRG[iPC].nLEN += nN;
				gpmSTRnCPY( gpsSTRpub, pPRG[iPC].pSTR, pPRG[iPC].nSTR )[pPRG[iPC].nSTR] = 0;
				pS += pPRG[iPC].nSTR;

				if( bDBG ) 	///
					cout << "STR: " << gpsSTRpub << " nSTR:" <<  pPRG[iPC].nSTR << endl << gppTAB-iPC;
				U2 sp;
				for( sp = nSPdct-1; sp < nSPdct ; sp-- )
				{
					ixDCT = ppSPdct[sp]->dict_find( gpsSTRpub, pPRG[iPC].nSTR, nDCT );
					if( ixDCT >= nDCT )
						continue; //nincsen;
					// ez mi ez?
                    // hol van mekkor a stb...
                    // jó ez most nekünk?

                    gpcOPCD* pOPCD = mass.apSPdtcOPCD[sp] ? (gpcOPCD*)mass.apSPdtcOPCD[sp]->p_alloc : NULL;
					if( !pOPCD )
						continue;
					else {

						switch( pOPCD[ixDCT].typ )
						{
							case gpeALF_TYU:
								// unsign type
								pPRG[iPC].nWIP = 0;

								cout << "TYPE UNSIGN";
								break;
							case gpeALF_TYI:
								// signed
								pPRG[iPC].nWIP = 0;
								cout << "TYPE SIGN";
								break;
							case gpeALF_TYF:
								// float
								pPRG[iPC].nWIP = 0;
								cout << "TYPE FLOAT";
								break;

							case gpeALF_CLASS:
								// osztály azaz vegyi gyümi
								pPRG[iPC].nWIP = 0;
								cout << "CLASS";

								break;
							case gpeALF_CONST:
								pPRG[iPC] = pOPCD[ixDCT];
								pPRG[iPC].nWIP = 2;
								break;
						}
						switch( pPRG[iPC].nWIP )
						{
							case 0:
								// TYPE
								pPRG[iPC].typ = pOPCD[ixDCT].typ;
								pPRG[iPC].i8 = iDAT;
								pPRG[iPC].u8 = pOPCD[ixDCT].u8;
								pPRG[iPC].nWIP = 1;
								break;
							case 1:
								// NAME // alloc
                                iDAT += pPRG[iPC].u8;
								break;


						}
						cout << pOPCD[ixDCT].pSTR;
					}
					break;
				}

				if( sp >= nSPdct )
				{
					sp = nSPdct-1;
					// most agyunk nekie
					ixDCT = ppSPdct[sp]->dict_add( gpsSTRpub, pPRG[iPC].nSTR )->x();
					mass.anSPdct[sp] = mass.apSPdct[sp]->nIX();
				}
				continue;
			}

        }
		sVAN[0] = c;
		cout << (char*)sVAN;
		pS++;
		nVAN = gpmNINCS( pS, sVAN );
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
							break;
						case '\'':
							psPRG[iPC] = c;
							pPRG[iPC].typ = gpeALF_U;
							pPRG[iPC].u8 = gpfUTF8( pS+nVAN, NULL );

                            nVAN += gpmVAN( pS+nVAN, sVAN, nN );
                            if( pS[nVAN] == c )
								nVAN++;
							break;




						case '(':
							memset( psPRG+iPC, c, nVAN+1 );
							pPRG[iPC].typ = pPRG[iPC].lab ? gpeALF_FUNC : gpeALF_STK;
							iPC++;
							pPRG[iPC].null();
							if( !nVAN )
								break;
							pPRG[iPC].typ = gpeALF_STK;
							gpfMEMSET( &pPRG[iPC+1], nVAN-1, &pPRG[iPC], sizeof(*pPRG) );
							iPC += nVAN;
							break;
						case ')':
							psPRG[iPC] = c;
							if( bDBG ) 	///
								cout << psPRG[iPC] << endl  << gppTAB-iPC;
							iPC--;
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
							pPRG[iPC].nMUL -= nVAN+1;
							if( pPRG[iPC].nMUL < -2 )
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
							}

							break;

						case ',':	// vessző
							// berakjuk ami létrejött a sorba
							if( pPRG[iPC].nWIP == 1 )
							{

							}
							break;
						case '.':	// pont

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
					if( bDBG )
					{
						if( pPRG[iPC].nADD > 0 )
							cout << " + ";
						else if( pPRG[iPC].nADD < 0 )
							cout << " - ";
						if( pPRG[iPC].nMUL > 0 )
							cout << "*";
						else if( pPRG[iPC].nMUL < 0 )
							cout << "/";

						switch( pPRG[iPC].typ )
						{
							case gpeALF_U:
								cout << pPRG[iPC].u8;
								break;
							case gpeALF_I:
								cout << pPRG[iPC].i8;
								break;
							case gpeALF_D:
								cout << pPRG[iPC].d;
								break;
						}
					}
					cout << endl  << gppTAB-iPC;
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
					pPRG[iPC].typ = gpeALF_A;
					iPC++;
					pPRG[iPC].null();
					break;
				case ']':
					psPRG[iPC] = c;
					if( bDBG ) 	///
						cout << psPRG[iPC] << endl << gppTAB-iPC;
					iPC--;
					nVAN = 0;
					break;

				case '{':
					memset( psPRG+iPC, c, nVAN+1 );
					pPRG[iPC].typ = gpeALF_BLK;
					iPC++;
					pPRG[iPC].null();
					if( !nVAN )
						break;
					pPRG[iPC].typ = gpeALF_BLK;
					gpfMEMSET( &pPRG[iPC+1], nVAN-1, &pPRG[iPC], sizeof(*pPRG) );
					iPC += nVAN;
					break;
				case '}':
					psPRG[iPC] = c;
					if( bDBG ) 	///
						cout << psPRG[iPC] << endl << gppTAB-iPC;
					iPC--;
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
>>>>>>> 11d5ed1eddd0cbdb7e509d0bfcd871249b3b5667

	}
	nBLD = nVER;
}
