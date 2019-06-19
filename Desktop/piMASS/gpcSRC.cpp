#include "gpcSRC.h"

gpcSRC::gpcSRC()
{
    //ctor
    gpmCLR;
}
gpcSRC& gpcSRC::reset( U1* pS, U1* pSe, U1** ppS )
{
	gpmCLR;
	U8 anLEN[4];
	pA = (pS += gpfVAN( pS, (U1*)"\a", anLEN[0] ));
	if( *pA == '\a' )									//...A
	{
		pA++;											//...aA
		pB = pA+gpfVAN( pA, (U1*)"\a", anLEN[0] );		//...aA..B
		if( *pB == '\a' )
		{
			pS = pB+1;									//...aA..BS
			pS += gpfVAN( pS, (U1*)"\a", anLEN[1] );	//...aA..B... . .   . . ...S
		} else
			pS = pB;
	}
	nL = pS-pA;
	if( ppS )
		*ppS = pS;

	return *this;
}

gpcSRC& gpcSRC::operator = ( gpcSRC& B )
{
	if( this == &B )
		return *this; // handle self assignment

	nL = &B ? B.nL : 0;
	if( !nL )	// B kampec;
	{
		// hagyjuk a pA-t hátha hamarosan mással töltjük meg
		return *this;
	}
	U1* BpA = B.pA;
	U8 i = B.iB();
	// fontos, hogy itt legyenek, mielött a pA-val kezdünk valamit

	//assignment operator
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
	gpmDELary(pA);
    //dtor
}



gpcSRCcache::~gpcSRCcache()
{
	gpcSRC** ppSRC = (gpcSRC**)(p_cache ? p_cache->p_alloc : NULL);

	if( ppSRC )
	for( U4 n = p_cache->n_load/sizeof(U14*), i = 0; i < n; i++ )
	{
		gpmDEL( ppSRC[i] );
	}

	gpmDEL(p_cache);
	gpmDEL(p_lst);
}


gpcSRC* gpcSRCcache::add_fnd( gpcSRC* pSRC, U4 xfnd, U4& is, U4& n )
{
	n = is = nLST;

	if( nLST )
	{
		is = p_lst->tree_fnd(xfnd, nLST);
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

	p_lst = p_lst->tree_add(xfnd, nLST);
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

	p_cache = p_cache->lazy_add( &pFND, sizeof(pFND), s = -1);
	return pFND;
}
gpcSRCcache::gpcSRCcache( const U1* pU, U8 nU )
{
	gpmCLR;
	if(!nU)
		return;
	if( pU ? !*pU : true )
		return;

	U1	*pS = (U1*)pU,
		*pSe = pS+nU;
	gpcSRC tmp;
	U4 is, n, xfnd = 0;
	while( pS < pSe ? *pS : false )
	{
		tmp.reset( pS, pSe, &pS );
		if(!tmp.nL)
			continue;
		add_fnd( &tmp, xfnd, is, n );
		xfnd = n;
	}
}
