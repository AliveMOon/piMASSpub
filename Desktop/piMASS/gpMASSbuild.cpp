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
	d = _d;
	i8 = (this-pTHIS)+i;

	typ = t;
	nDT = u;

	nSTR = gpmVAN( pSTR, gpsPRG, nLEN );


}

static const gpcOPCD gpaOPCi[] = {
	// this,	pS,			a, m, i, nDT,				d,		t;
	{ gpaOPCi,	"false", 	0, 0, 0, sizeof(U1), 		0.0, gpeALF_CONST },
	{ gpaOPCi,	"true", 	0, 0, 0, sizeof(U1), 		0.0, gpeALF_CONST },

	{ gpaOPCi,	"U1", 		0, 0, 0, sizeof(U1), 		0.0, gpeALF_TYU },
	{ gpaOPCi,	"U2", 		0, 0, 0, sizeof(U2), 		0.0, gpeALF_TYU },
	{ gpaOPCi,	"U4", 		0, 0, 0, sizeof(U4), 		0.0, gpeALF_TYU },
	{ gpaOPCi,	"U8", 		0, 0, 0, sizeof(U8), 		0.0, gpeALF_TYU },
	{ gpaOPCi,	"I1", 		0, 0, 0, sizeof(U1), 		0.0, gpeALF_TYI },
	{ gpaOPCi,	"I2", 		0, 0, 0, sizeof(U2), 		0.0, gpeALF_TYI },
	{ gpaOPCi,	"I4", 		0, 0, 0, sizeof(U4), 		0.0, gpeALF_TYI },
	{ gpaOPCi,	"I8", 		0, 0, 0, sizeof(U8), 		0.0, gpeALF_TYI },
	{ gpaOPCi,	"F4", 		0, 0, 0, sizeof(float),		0.0, gpeALF_TYF },
	{ gpaOPCi,	"F8", 		0, 0, 0, sizeof(double),	0.0, gpeALF_TYF },

	{ gpaOPCi,	"sizeof", 	0, 0, 0, 0,					0.0, gpeALF_FUNC },
	{ gpaOPCi,	"if", 		0, 0, 0, 0,					0.0, gpeALF_FUNC },
	{ gpaOPCi,	"for", 		0, 0, 0, 0,					0.0, gpeALF_FUNC },
	{ gpaOPCi,	"while", 	0, 0, 0, 0,					0.0, gpeALF_FUNC },
	{ gpaOPCi,	"switch",	0, 0, 0, 0,					0.0, gpeALF_FUNC },

	{ gpaOPCi,	"break",	0, 0, 0, 0,					0.0, gpeALF_FUNC },
	{ gpaOPCi,	"continue",	0, 0, 0, 0,					0.0, gpeALF_FUNC },

	{ gpaOPCi,	"class",	0, 0, 0, 0,					0.0, gpeALF_COMMAND },
	{ gpaOPCi,	"pud",		0, 0, 0, 0,					0.0, gpeALF_FUNC },
	{ gpaOPCi,	"prot",		0, 0, 0, 0,					0.0, gpeALF_FUNC },
	{ gpaOPCi,	"new",		0, 0, 0, 0,					0.0, gpeALF_FUNC },
	{ gpaOPCi,	"del",		0, 0, 0, 0,					0.0, gpeALF_FUNC },

	{ gpaOPCi,	"SYS",		0, 0, 0, 0,					0.0, gpeALF_CLASS },
	{ gpaOPCi,	"GT",		0, 0, 0, 0,					0.0, gpeALF_CLASS },
	{ gpaOPCi,	"PIC",		0, 0, 0, 0,					0.0, gpeALF_CLASS },


};
char gpsSTRpub[0x10000];

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
		*pSe, *pE = pA+nL, *pSTR = NULL;
	char sVAN[] = ".";
	U4 	nALF,
		utf8;
	I4 nCHK;
	bool bSIGN = false, bALF;

	U8 	nN, nSKIP,
		nVAN = 0, s8;

	if( !mass.lD )
	{

		mass.anDICT[mass.lD] = 0;
        for( U4 i = 0, in = gpmN(gpaOPCi), mlD = mass.lD; i < in; i++ )
        {
			mass.apDICT[mlD] = mass.apDICT[mlD]->dict_add( (char*)gpaOPCi[i].pSTR, gpaOPCi[i].nSTR );
			if( mass.anDICT[mlD] >= mass.apDICT[mlD]->nIX() )
				continue;

			mass.apDICTopcd[mlD] = mass.apDICTopcd[mlD]->lazy_add( &gpaOPCi[mass.anDICT[mlD]], sizeof(*gpaOPCi), s8 = -1, 1 );
			gpcOPCD	&OPCD = *((gpcOPCD*)(mass.apDICTopcd[mlD]->p_alloc+s8));
			OPCD.lab = gpfSTR2ALF( OPCD.pSTR, OPCD.pSTR+OPCD.nSTR );


			mass.anDICT[mlD] = mass.apDICT[mlD]->nIX();
		}
		mass.lD++;
	}

	U4	iPC0 = 0, iPC = iPC0+1,
		iDT = 0,
		lvDCT, ixDCT, nDCT = 0,
		nDICT = mass.lD+1, nDICTmn = nDICT;


	gpmZ( mass.asPRG );
	U1		*psPRG = mass.asPRG, c,
			*pPUB = (U1*)gpsSTRpub;

	gpcOPCD	*pPRG = mass.aPRG;
	gpcLZYdct	**ppDICT = mass.apDICT;
	double d = 0;
	while( pS < pE )				/// 	COMPILER
	{
		pS += gpmNINCS( pS, " \t\r\n" );
		if( pS >= pE )
		{
			// feldolgozni ami még van
			break;
		}

		lvDCT = nDICT-1;

        c = *pS;
        if( c < 0x80 ? gpaALFadd[c] : true )
        {
			// UTF8!
			nALF = gpfABCnincs( pPRG[iPC].pSTR = pS, pE, pPRG[iPC].nLEN );
			if( nALF )
			{
				// ALF esélyes ?
				gpcOPCD	&prg = pPRG[iPC];

				bALF = (nALF == prg.nLEN);
                prg.nSTR = nALF + gpmVAN( prg.pSTR+nALF, gpsPRG, nVAN );
				pS += prg.nSTR;
				prg.nLEN += nVAN;

				gpmSTRnCPY( pPUB, prg.pSTR, prg.nSTR )[prg.nSTR] = 0;
				prg.apSTR[prg.nWIP] = pPUB;
				prg.alvDCT[0] = lvDCT;
				prg.nWIP++;

				if( bDBG ) 	///
					cout << (gppTAB-lvDCT) << pPUB << endl;

				pPUB += prg.nSTR+1;
				continue;
			}

        }
		if( pPRG[iPC].nWIP )
        {
			if( pPRG[iPC].nADD > 0 )
				cout << " + ";
			else if( pPRG[iPC].nADD < 0 )
				cout << " - ";
			if( pPRG[iPC].nMUL > 0 )
				cout << "*";
			else if( pPRG[iPC].nMUL < 0 )
				cout << "/";

			if( pPRG[iPC].nWIP == 1 )
			switch( pPRG[iPC].typ )
			{
				case gpeALF_STR:
						cout << (gppTAB-lvDCT) << "\"...\"" << endl;
						break;
				case gpeALF_I:
						cout << (gppTAB-lvDCT) << pPRG[iPC].i8 << endl;
						break;
				case gpeALF_U:
						cout << (gppTAB-lvDCT) << pPRG[iPC].u8 << endl;
						break;
				case gpeALF_D:
						cout << (gppTAB-lvDCT) << pPRG[iPC].d << endl;
						break;
				/*default:
						psPRG[iPC+1] = 0;
						cout << (gppTAB-lvDCT) << (char*)psPRG+iPC << endl;*/
			}
			iPC++;
			pPRG[iPC].null();
			pPRG[iPC].alvDCT[0] = lvDCT;
			cout << "iPC" << iPC << ":" << endl;
        }

		sVAN[0] = c;
		cout << (gppTAB-lvDCT) << (char*)sVAN << endl;
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

							nDICT++;
							lvDCT = nDICT-1;
							pPRG[iPC].nWIP = 1;
							nVAN = 0;
							break;

						case ')':
							psPRG[iPC] = c;
							pPRG[iPC].typ = gpeALF_FUNC;
							if( nDICT > nDICTmn )
							{
								nDICT--;
								lvDCT = nDICT-1;
							}
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
							pPRG[iPC].nMUL -= nVAN+1;
							if( pPRG[iPC].nMUL < -1 )
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
					/*if( bDBG )
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
					}*/
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

					nDICT++;
					lvDCT = nDICT-1;
					pPRG[iPC].nWIP = 1;
					nVAN = 0;
					break;
				case ']':
					psPRG[iPC] = c;
					pPRG[iPC].typ = gpeALF_FUNC;
					if( nDICT > nDICTmn )
					{
						nDICT--;
						lvDCT = nDICT-1;
					}
					pPRG[iPC].nWIP = 1;
					nVAN = 0;
					break;

				case '{':
					psPRG[iPC] = c;
					pPRG[iPC].typ = gpeALF_FUNC;

					nDICT++;
					lvDCT = nDICT-1;
					pPRG[iPC].nWIP = 1;
					nVAN = 0;
					break;
				case '}':
					psPRG[iPC] = c;
					pPRG[iPC].typ = gpeALF_FUNC;
					if( nDICT > nDICTmn )
					{
						nDICT--;
						lvDCT = nDICT-1;
					}
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

	}
	nBLD = nVER;
}
