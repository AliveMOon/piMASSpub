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
gpeALF gpfSTR2ALF( U1* pS, U1* p_end, U1** ppS )
{
	if( pS ? !*pS : true )
	{
		if( ppS )
			*ppS = pS;

		return gpeALF_zero;
	}

	I8 alf = 0;
	U1 c;
	if( p_end < pS )
		p_end = pS + gpmSTRLEN(pS);

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

U8 gpfALFA2STR(char* p_out, I8 d0)
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
		//d1 = d0%gpdALF;
		//d1 += '@';
		//p_buff--;
		//*p_buff = d1;
		//d0 /= gpdALF;
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
gpcSRC::gpcSRC()
{
    //ctor
    gpmCLR;
}
gpcSRC& gpcSRC::reset( U1* pS, U1* pSe, U1** ppS )
{
	gpmCLR;
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
	gpmDELary(pA);
    //dtor
}



gpcMASS::~gpcMASS()
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


gpcSRC* gpcMASS::add( gpcSRC* pSRC, U4 xfnd, U4& is, U4& n )
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

gpcMASS::gpcMASS( const U1* pU, U8 nU )
{
	gpmCLR;
	if(!nU)
		return;
	if( pU ? !*pU : true )
		return;

	U1	*pS = (U1*)pU,
		*pSe = pS+nU;
	gpcSRC tmp;
	U4 is, n, xadd = 1, id, mom = 0;
	nSP = 1;

	while( pS < pSe ? *pS : false )
	{
		tmp.reset( pS, pSe, &pS );
		if(!tmp.nL)
			continue;

		if( tmp.bSUB() )
		{
			mom = nSP;
            nSP++;
            aSP44[nSP].null();
		}
		else if( tmp.bENTR() )
		{
			aSP44[nSP].x = 0;
			aSP44[nSP].y++;
			if( aSP44[mom].w < aSP44[nSP].y )
				aSP44[mom].w = aSP44[nSP].y;
		} else {
			aSP44[nSP].x++;
			if( aSP44[mom].z < aSP44[nSP].x )
				aSP44[mom].z = aSP44[nSP].x;
		}
		tmp.space = aSP44[nSP];
        apSP[nSP] = add( &tmp, xadd, aSPix[nSP], n );

		while( apSP[nSP]->bRET() )
		{
			apSP[mom]->space = aSP44[mom];
			apSP[mom]->retIX = aSPix[nSP];
			mom--;
			nSP--;
		}

		xadd++;

	}
}
