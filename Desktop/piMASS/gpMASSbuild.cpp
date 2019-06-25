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

void gpcSRC::hd( gpcMASS& mass, gpeALF* pTGpub )
{
	if( !this )
		return;

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

	U8 bOFF = ~(gpeMASSmainMSK||gpeMASSprgMSK);
	if( pTGpub )
	{
		pALFtg = new gpeALF[nALFtg+1];
		nTGdie = nALFtg;
		nALFtg = 0;
		for( U4 i = 0; i < nTGdie; i++ )
		{
			if( pTGpub[i] < gpeALF_A )
				continue;
			pALFtg[nALFtg] = pTGpub[i];
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
	bSW &= bOFF;
	nHD = nVER;
}

char gpsPRG[] = " \t\r\n\a .,!? =<> -+*/%^ &~|@#$ \\ \" \' ()[]{} ";
char gpsTAB[] = "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t",
	 *gppTAB = gpsTAB+strlen(gpsTAB);
char gpsSTRpub[0x1000];
void gpcSRC::cmpi( gpcMASS& mass, bool bDBG )
{
	hd( mass );

	U1* pS = pB, *pSe, *pE = pA+nL, *pSTR = NULL;
	char sVAN[] = ".";
	U4 	//nN = n,
		utf8;
	I4 nCHK;
	bool bSIGN = false;
	//I8	i8 = 0, nMUL = 0, nADD;
	U8 	nN, nSKIP,
		nVAN = 0;
		//, nSTR, nLEN;

	U2 nSP = 0;
	gpmZ( mass.aSPc );
	U1		*pSPc = mass.aSPc, c;
	gpcOPCD	*pSPb = mass.aSPb;

	double d = 0;
	while( pS < pE )
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
			// UTF8!
			pSPb[nSP].nSTR = gpfABCnincs( pSPb[nSP].pSTR = pS, pE, pSPb[nSP].nLEN );
			if( pSPb[nSP].nSTR )
			{
				pSPb[nSP].nSTR += gpmVAN( pSPb[nSP].pSTR+pSPb[nSP].nSTR, gpsPRG, nN );
				pSPb[nSP].nLEN += nN;
				gpmSTRnCPY( gpsSTRpub, pSPb[nSP].pSTR, pSPb[nSP].nSTR )[pSPb[nSP].nSTR] = 0;
				if( bDBG ) 	///
					cout << "STR: " << gpsSTRpub << " nSTR:" <<  pSPb[nSP].nSTR << endl << gppTAB-nSP;

				pS += pSPb[nSP].nSTR;
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
							pSPc[nSP] = c;
							pSPb[nSP].typ = gpeALF_STR;
							pSPb[nSP].pSTR = pS+nVAN;

                            nVAN += gpmVAN( pSPb[nSP].pSTR, sVAN, pSPb[nSP].nLEN );
                            pSPb[nSP].nSTR = (pS+nVAN) - pSPb[nSP].pSTR;
							if( pS[nVAN] == c )
								nVAN++;
							break;
						case '\'':
							pSPc[nSP] = c;
							pSPb[nSP].typ = gpeALF_U;
							pSPb[nSP].u8 = gpfUTF8( pS+nVAN, NULL );

                            nVAN += gpmVAN( pS+nVAN, sVAN, nN );
                            if( pS[nVAN] == c )
								nVAN++;
							break;




						case '(':
							memset( pSPc+nSP, c, nVAN+1 );
							pSPb[nSP].typ = pSPb[nSP].lab ? gpeALF_FUNC : gpeALF_STK;
							nSP++;
							pSPb[nSP].null();
							if( !nVAN )
								break;
							pSPb[nSP].typ = gpeALF_STK;
							gpfMEMSET( &pSPb[nSP+1], nVAN-1, &pSPb[nSP], sizeof(*pSPb) );
							nSP += nVAN;
							break;
						case ')':
							pSPc[nSP] = c;
							if( bDBG ) 	///
								cout << pSPc[nSP] << endl  << gppTAB-nSP;
							nSP--;
							nVAN = 0;
							break;




						case '+':
							pSPb[nSP].nADD += nVAN+1;
							break;
						case '-':
							pSPb[nSP].nADD -= nVAN+1;
							break;

						case '*':
							nCHK = pSPb[nSP].nMUL;
							pSPb[nSP].nMUL += nVAN+1;
							if( nCHK < 0 )
							if( (pSPb[nSP].nMUL-nCHK) > 1 )
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
							pSPb[nSP].nMUL -= nVAN+1;
							if( pSPb[nSP].nMUL < -2 )
							{
								pSPb[nSP].nMUL = 0;
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
                    pSPb[nSP].u8 = gpfSTR2U8( pS-(nVAN+1), &pS );
					if( *pS == '.' )
                    {
						pSPb[nSP].typ = gpeALF_D;
						pSPb[nSP].d = gpmSTR2D( pS ) + (double)pSPb[nSP].u8;		///
						if( pSPb[nSP].nADD < 0 )
							pSPb[nSP].d *= -1;
                    } else {
						if( pSPb[nSP].nADD < 0 )
						{
							pSPb[nSP].typ = gpeALF_I;
							pSPb[nSP].i8 = -pSPb[nSP].u8;							///
						}
						else
							pSPb[nSP].typ = gpeALF_U;
						pSPb[nSP].d = 0;
                    }
                    nVAN = 0;
					if( bDBG )
					{
						if( pSPb[nSP].nADD > 0 )
							cout << " + ";
						else if( pSPb[nSP].nADD < 0 )
							cout << " - ";
						if( pSPb[nSP].nMUL > 0 )
							cout << "*";
						else if( pSPb[nSP].nMUL < 0 )
							cout << "/";

						switch( pSPb[nSP].typ )
						{
							case gpeALF_U:
								cout << pSPb[nSP].u8;
								break;
							case gpeALF_I:
								cout << pSPb[nSP].i8;
								break;
							case gpeALF_D:
								cout << pSPb[nSP].d;
								break;
						}
					}
					cout << endl  << gppTAB-nSP;
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
					pSPc[nSP] = c;
					pSPb[nSP].typ = gpeALF_A;
					nSP++;
					pSPb[nSP].null();
					break;
				case ']':
					pSPc[nSP] = c;
					if( bDBG ) 	///
						cout << pSPc[nSP] << endl << gppTAB-nSP;
					nSP--;
					nVAN = 0;
					break;

				case '{':
					memset( pSPc+nSP, c, nVAN+1 );
					pSPb[nSP].typ = gpeALF_BLK;
					nSP++;
					pSPb[nSP].null();
					if( !nVAN )
						break;
					pSPb[nSP].typ = gpeALF_BLK;
					gpfMEMSET( &pSPb[nSP+1], nVAN-1, &pSPb[nSP], sizeof(*pSPb) );
					nSP += nVAN;
					break;
				case '}':
					pSPc[nSP] = c;
					if( bDBG ) 	///
						cout << pSPc[nSP] << endl << gppTAB-nSP;
					nSP--;
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
