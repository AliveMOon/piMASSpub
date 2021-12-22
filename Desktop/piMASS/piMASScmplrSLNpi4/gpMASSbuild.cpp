#include "gpcSRC.h"
#include "gpccrs.h"
U1 gpaALFsub[0x100];
U1 gpaALFsub2[0x100];
char gpaALF_H_sub[0x100];
char	gpsPRG[] = gpdPRGsep, //" \t\r\n\a .,:;!? =<> -+*/%^ &~|@#$ \\ \" \' ()[]{} ",
		gpsTAB[] = "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t",
		*gppTAB = gpsTAB+strlen(gpsTAB),
		gpsNDAT[] = "-bwllqqqqxxxxxxxx";
int alfLEN( gpeALF alf ) {
	if( !alf )
		return 0;

	if( alf <= gpeALF_4 ) {
		if( alf <= gpeALF_2 ) {
			if( alf <= gpeALF_Z )
				return 1;
			return 2;
		}
		if( alf <= gpeALF_3 )
				return 3;
		return 4;
	}

	if( alf <= gpeALF_8 ) {
		if( alf <= gpeALF_6 ) {
			if( alf <= gpeALF_5 )
				return 5;
			return 6;
		}
		if( alf <= gpeALF_7 )
				return 7;
		return 8;
	}

	if( alf <= gpeALF_12 ) {
		if( alf <= gpeALF_10 ) {
			if( alf <= gpeALF_9 )
				return 9;
			return 10;
		}
		if( alf <= gpeALF_11 )
				return 11;
		return 12;
	}
	if( alf <= gpeALF_13 )
		return 13;
	return 14;
}
U1* gpf_aALF_init( void ) {
	for( int i = 0; i < 0x100; i++ )
	{
		if( i >= 'A' && i <= 'Z')
		{
			// egész biztos kisbetüt többet használnak
			gpaALFsub[i] = 'A'-1;//GPD_UP;
			continue;
		}
		else if( i >= 'a' && i <= 'z' )
		{
			gpaALFsub[i] = 'a'-1;
			continue;
		}
		gpaALFsub[i] = 0;
	}
	gpmMcpy( gpaALFsub2, gpaALFsub, sizeof(gpaALF_H_sub) );
	gpmMcpy( gpaALF_H_sub, gpaALFsub, sizeof(gpaALF_H_sub) );
	gpaALFsub2['\n']	= ('\n'-'E')+('A'-1);
	gpaALFsub2['\r']	= ('\r'-'E')+('A'-1);
	gpaALFsub2['+']		= ('+'-'E')+('A'-1);
	gpaALFsub2['\"']	= ('\"'-'E')+('A'-1);
	gpaALFsub2[',']		= (','-'E')+('A'-1);

	gpaALF_H_sub['#']	= ('#'-'E')+('A'-1);
	gpaALF_H_sub['_']	= ('_'-'E')+('A'-1);
	return gpaALFsub;
}
char* gpfSTRdrop( char* p_dst, char* p_str, char* p_end, const char* p_drop ) {
	if( !p_end )
		p_end = p_str+gpmSTRLEN(p_str);
	if( !p_drop )
		return (char*)gpmSTRnCPY( p_dst, p_str, p_end-p_str );
	U8 nL;
	size_t n, m;
	char* pD = (char*)p_dst;
	p_str += gpmMIN( p_end-p_str, gpmNINCS(p_str,p_drop) );
	while( p_str < p_end ) {
		n = gpmMIN( p_end-p_str, gpmVAN(p_str,p_drop, NULL )); //,nL) );

		gpmSTRnCPY( p_dst, p_str, n );
		p_dst += n;
		p_str += n;
		p_str += gpmMIN( p_end-p_str, gpmNINCS(p_str,p_drop) );
	}
	*p_dst = 0;
	return pD;
}

gpeALF gpfSTR2ALF( const U1* pS, const U1* p_end, U1** ppS, char skip ) {
	if( p_end < pS ) {
		if( ppS )
			*ppS = (U1*)p_end;
		return gpeALF_null;
	}

	if( pS ? !*pS : true ) {
		if( ppS )
			*ppS = (U1*)pS;

		return gpeALF_null;
	}

	U8 alf = 0;
	U1 c, utf;

	while( pS < p_end) {
		c = *(U1*)pS;
		//utf = 0x80;
		if( (c>>6) == 2 ) { // ha van UTF8 szemét
			pS++;
			continue;
		}

		if( (c>>6) == 3 ) {	// UTF8 HEAD
			//pS++;
			c = 'e';
		}
		if( skip )
		if( c == skip ){
			pS++;
			continue;
		}

		if( !gpaALFsub[c] )
			break;

		c -= gpaALFsub[c];

		alf *= (U8)gpdALF;
		alf += (U8)c;

		pS++;
	}

	if (ppS)
		*ppS = (U1*)pS;

	return (gpeALF)alf;
}
U1		gpsSTRpub[0x10000];
char 	gpsHD[0x1000], *psHD;
void gpcSRC::hd( gpcMASS* pMASS, gpeALF* pTGpub ) {
	if( !this )
		return;

	psHD = gpsHD;
	psHD[0] = 0;
	if( nHD == nVERr )
	{
		if( !nVERr )
			nVERr = 1;
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
						gpsSTRpub, spcZN.y, spcZN.pSTR( psHD+0x100, "," ), nVERr, nHD, nBLD,
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
				pTGpub = pMASS->aTGwip;

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
			case gpeALF_sub:
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
			pMASS->tag_sub( pTGdie[i], IX );
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
			pMASS->tag_add( pALFtg[nALFtg], IX );			///
			nALFtg++;
		}


		if( !nALFtg )
			gpmDELary(pALFtg);
		gpmDELary(pTGdie);
	}

	psHD += sprintf(	psHD, "\r\nOFF:0x%0.8x",
						bOFF );
	bSW &= bOFF;
	nHD = nVERr;
	if( bSW&gpeMASSznMSK )
	if( bSW&gpeMASSentrMSK )
		bSW &= ~gpeMASSentrMSK;
	psHD += sprintf( 	psHD, "\r\nbSW:0x%0.8x",
						bSW );
	if( psHD > gpsHD )
		if(bSTDcout){gpdCOUT << gpsHD;}
	if(bSTDcout){gpdCOUT << ".";}
}
gpcLZYdct* gpcMASS::pOPER() {
	if(!this)
		return NULL;
	if( OPER.nIX() )
		return &OPER;

	char *pSi, *pSe;
	U4 iOP, iOPe = OPER.nIX();
	/// -------------------------------------------------------
	/// OPER
	/// -------------------------------------------------------
	if( !iOPe ) {
		mxOP = gpeALF_null;
		/// ha nincsen még kitöltve az OPER lista feltöltjük
		pSe = strchr( (char*)gpasOP[0], ' ' );
		OPER.dctMILLadd( (U1*)gpasOP[0], pSe-gpasOP[0] );
		aOP[0].num = 14;
		aOP[0] = pSe+1;
		mxOP = aOP[0].alf;
		iOPe++;

		for( U4 i = 1, ie = gpmN(gpasOP); i < ie; i++ ) {
			pSe = strchr( (char*)gpasOP[i], ' ' );
			if( !pSe )
				continue;
			U4 n = pSe-gpasOP[i], nn = 0;
			iOP = 0;
			iOP = OPER.dctMILLfnd( (U1*)gpasOP[i], pSe-gpasOP[i], iOPe );
			if( iOP < iOPe )
			if( OPER.nSTRix(iOP) < n )
				iOP = iOPe;

			if( iOP >= iOPe ) {
				iOP = iOPe;
				OPER.dctMILLadd( (U1*)gpasOP[i], pSe-gpasOP[i] );
				aOP[iOP].num = 14;
				aOP[iOP] = pSe+1;
				iOPe++;
				if( mxOP < aOP[iOP].alf )
					mxOP = aOP[iOP].alf;
			}

		}
		iOPe = OPER.nIX();
	}

	return &OPER;
}
/// 3 main.win.res.bldcmplr.instRUN.instALU
U1 gpcSRC::srcBLD( gpcWIN* pW, gpcLZY* pSRCstk ) {
	if( !this )
		return 0;
	if( msBLD ? msBLD > pW->mSEC.x : true )
		return 2; // még várjon az építéssel

	msBLD = 0;
	if( !(*aSCP).nLNK() )
		return 0;

	U1 iSCP = 0;
	srcCMPLR( *pW->piMASS->pOPER(), iSCP, pW, pSRCstk );
	return 1;
}






