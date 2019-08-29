#include "gpcSRC.h"



gpcSRC::gpcSRC()
{
    //ctor
    gpmCLR;
}
gpcSRC& gpcSRC::reset( U1* pS, U1* pSe, U1** ppS, U4x4& spc )
{
	gpmCLR;
	space = spc;
	spc.x++;

	if( pS ? !*pS : true )
	{
		if( !ppS )
		return *this;

		*ppS = pS;
		return *this;
	}
	U8 anLEN[4];
	//pS += gpfVAN( pS, (U1*)"\a", anLEN[0] );
	pA = pS;
	if( *pA == '\a' )									//...A\a
		pA++;											//...\aA
	pS = (pB = pA + gpfVAN( pA, (U1*)"\a", anLEN[0] ));	//...\aA..B
	if( *pB == '\a' )
		pS++;											//...\aA..B\aS
	nL = gpfVAN( pS, (U1*)"\a", anLEN[1] );
	if( !pS[nL] )
		pS+=nL;											//...\aA..B\a... . .   . . ...S0
	else if( pS[nL] != '\a' )
		pS += gpfVAN( pS, (U1*)"\a", anLEN[1], true );	//...\aA..B\a... . .   . . ...S?
	else
		pS+=nL;											//...\aA..B\a... . .   . . ...S\a


	nL = pS-pA;
	if( !ppS )
		return *this;

	*ppS = pS;
	return *this;
}

gpcSRC& gpcSRC::operator = ( gpcSRC& B )
{
	if( this == &B )
		return *this; // handle self assignment
	//assignment operator

	/// elopunk mindent most
	pALFtg = B.pALFtg;	B.pALFtg = NULL;
	nALFtg = B.nALFtg;	B.nALFtg = 0;
	IX = B.IX;
	retIX = B.retIX;
	space = B.space;
	bSW = B.bSW;
	pEXE = B.pEXE; B.pEXE = NULL;
	pRES = B.pRES; B.pRES = NULL;
	pMINI = B.pMINI; B.pMINI = NULL;
	pBIG = B.pBIG; B.pBIG = NULL;
	nVER = B.nVER;
	nBLD = B.nBLD;
	nHD = B.nHD;


	qBLD();

	nL = &B ? B.nL : 0;
	if( !nL )	// B kampec;
	{
		// hagyjuk a pA-t hátha hamarosan mással töltjük meg
		return *this;
	}
	U1* BpA = B.pA;
	U8 i = B.iB();
	// fontos, hogy itt legyenek, mielött a pA-val kezdünk valamit

	if( nA < (nL+2) )
	{
		// ha nA == 0 volt azt jelenti nem foglalva volt, ha nem valahonnan kölcsönözve
		if( nA )	// csak akkor felszabadítható ha van mérete, egyébként simán elfelejthető
			gpmDELary(pA);

		nA = gpmPAD( nL+2, 0x10 );
		pA = new U1[nA];
	}
	if( !pA )
	{
		// nem ölég a memoria?
		gpmCLR;
		return *this;
	}

	// nL = B.nL; ema vót
	// nL = B.nL; e má vót
	if( i < nL )
	{
		// nL van
		// i kissebb ez egyértelmű
		pB = pA+i;
		gpmMEMCPY( pA, BpA, nL );
		pA[nL] = 0;
		return *this;
	}

	// aszondja nem egyértelmű akkor legyen az javít
	// potoljuk // az elejére rakunk egyet mert kell
	pB = pA;
	*pB = '\a';
	gpmMEMCPY( pB+1, BpA, nL );
	nL++;
	pA[nL] = 0;

	for( U8 i = 1; i < nL; i++ )
	{
		if( pB[i] != '\a' )
			continue;

		pB[i] = '!'; // ez nagyon nem stimmel figyelmeztet
	}

	return *this;
}

gpcSRC::gpcSRC( gpcSRC& B )
{
	gpmCLR;
	*this = B;
}

gpcSRC::~gpcSRC()
{
	if( nA )
		gpmDELary(pA);
	gpmDELary(pALFtg);
	gpmDELary(pEXE);
	gpmDELary(pRES);
	gpmDELary(pMINI);
	gpmDELary(pBIG);
}



gpcMASS::~gpcMASS()
{
	gpcSRC** ppS = ppSRC();

	if( ppS )
	for( U4 n = pSRCc->n_load/sizeof(U1x4*), i = 0; i < n; i++ )
	{
		gpmDEL( ppS[i] );
	}

	gpmDEL(pSRCc);
	gpmDEL(pLST);
}


gpcSRC* gpcMASS::SRCadd( gpcSRC* pSRC, U4 xfnd, U4& is, U4& n )
{
	n = is = nLST;

	if( nLST )
	{
		is = pLST->tree_fnd(xfnd, nLST);
		if( is < nLST )
		{
			gpcSRC	**ppS = ppSRC(); //,
					//*pOUT = NULL;
			if( !ppS )
				return NULL;

			if( !ppS[is] )
			{
				if( !pSRC )
					return NULL;

				pFND = ppS[is] = new gpcSRC( *pSRC );
			}
			else if( !pSRC )
			{
				gpmDEL( ppS[is] );
				xFND = xfnd;
				return pFND = NULL;
			}

			xFND = xfnd;
			*pFND = *pSRC;
			return pFND;
		}
		// azaz a ha nem lett is < nLST
		// akkor hozzá fog adni
	}

	pLST = pLST->tree_add(xfnd, nLST);
	n = nLST;
	if( is >= nLST ) // is és nLST - ha továbbra is egyenlő akkor nem tudta hozzá adni
		return NULL;

	if( nALLOC > is )
	{
		// probáltak ide már foglalni korábban csak talán leRESETelték a listát
		// megnézzük van e pointer
		gpcSRC** ppS = ppSRC();
		pFND = ppS[is];
		if( pFND )
		{
			xFND = xfnd;
			if( !pSRC )
			{
				gpmDEL( ppS[is] );
				return pFND = NULL;
			}

			*pFND = *pSRC;
			return pFND;
		}
	} else {
		nALLOC = nLST;
	}

	U8 s;

	xFND = xfnd;
	pFND = new gpcSRC( *pSRC );

	pSRCc = pSRCc->lazy_add( &pFND, sizeof(pFND), s = -1);
	return pFND;
}


