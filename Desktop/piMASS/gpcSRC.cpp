#include "gpcSRC.h"
#include "gpccrs.h"



gpcSRC::gpcSRC()
{
    //ctor
    gpmCLR;
}
gpcSRC& gpcSRC::SRCcpy( U1* pS, U1* pSe )
{
	if( pSe <= pS )
		return *this;
	nL = pSe-pS;
	U1* pKL = nA ? pA : NULL;
	if( nA < nL )
	{
		gpmDELary(pA);
		nA = gpmPAD( nL+1, 0x10 );
		pA = new U1[nA];
	}

	gpmMEMCPY( pA, pS, nL );
	pA[nL] = 0;
	U8 nLEN;
	pB = pA + gpfVAN( pA, (U1*)"\a", nLEN );
	if( *pB == '\a' )
		pB++;

	return *this;
}
gpcSRC& gpcSRC::reset( U1* pS, U1* pSe, U1** ppS, U4x4& _spcZN, U4 nADD )
{
	gpmCLR;
	spcZN = _spcZN;
	_spcZN.x += nADD;

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
static const U4 gpnZ = ((gpcSRC*)NULL)->nCLR();

gpcSRC& gpcSRC::operator = ( gpcSRC& B )
{
	if( this == &B )
		return *this; // handle self assignment
	//assignment operator

	/// elopunk mindent most
	gpmMEMCPY( this, &B, 1 );
	gpmZn( (U1*)&B.pALFtg, gpnZ );
 	/*pALFtg = B.pALFtg;	B.pALFtg = NULL;
	nALFtg = B.nALFtg;	B.nALFtg = 0;
	IX = B.IX;
	retIX = B.retIX;
	spc = B.spc;
	bSW = B.bSW;
	pEXE = B.pEXE; B.pEXE = NULL;
	pRES = B.pRES; B.pRES = NULL;
	pMINI = B.pMINI; B.pMINI = NULL;
	pBIG = B.pBIG; B.pBIG = NULL;
	nVER = B.nVER;
	nBLD = B.nBLD;
	nHD = B.nHD;*/


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
	if( nA )			// ha nA == 0 nm mi foglaltuk
		gpmDELary(pA);
	gpmDELary(pALFtg);
	gpmDELary(pEXE);
	gpmDELary(pRES);
	gpmDELary(pMINI);
	gpmDELary(pBIG);
	gpmDELary(pMAP);
}

I4x4 gpcSRC::CRSmini( U1x4* pO, U4x4* pCx2, I4x4 xy, I4 fx, I4 fy, I4 fz, U4* pC64, gpcCRS& crs, gpeCLR bg, gpeCLR fr, gpeCLR ch  )
{
	if( this ?
				   ( fx <= 0 	||	fy <= 0 )
				|| ( xy.x >= fx ||	xy.y >= fy )
				: true )
		return xy;

	I4x4 cxy = xy;
	U1x4 c;
	//c.u4 = pC64[15];
	U1 nx, aC[] = " ";
	I4 cr, n, rr;
	for( I4 r = max(xy.y,0); r < fy; r++ )
	{
		rr = r*fz;
		for( I4 c = max(xy.x,0); c < fx; c++ )
		{
			cr = rr+c;
			pO[cr].y = 0;
		}
	}

	if( !(bSW&gpeMASSoffMSK) )
	if( xy.x < fx && xy.y < fy )
	{

		// UP
		if( xy.y >= 0 )
		{
			rr = xy.y*fz;
			for( I4 c = max(xy.x,0); c < fx; c++ )
			{
				cr = rr+c;
				pO[cr].x = fr;
				pO[cr].y |= 1;
			}
		}
		//DWN
		{
			rr = (fy-1)*fz;
			for( I4 c = max(xy.x,0); c < fx; c++ )
			{
				cr = rr+c;
				pO[cr].x = fr;
				pO[cr].y |= 4;
			}
		}

		// LEFT
		//if( xy.x >= 0 )
		{
			for( I4 r = max(xy.y,0), rr = max(xy.x,0) + r*fz ; r < fy; r++, rr += fz )
			{
				pO[rr].x = fr;
				pO[rr].y |= 8;
			}
		}
		// RIGHT
		{
			for( I4 r = max(xy.y,0), rr = fx-1 + r*fz; r < fy; r++, rr += fz )
			{
				pO[rr].x = fr;
				pO[rr].y |= 2;
			}
		}

	}
	cr = dim.w;

	bool bON = false;
	I4 nFILL;

	for( U1* pC = pSRCstart(), *pAL = pSRCalloc() , *pCe = pC+dim.w; pC < pCe; pC++ )
	{
		if( cxy.y >= fy )
			break;

		cr = cxy.x + cxy.y*fz;
		if( this == crs.apSRC[0] )
		if( crs.apSRC[0] == crs.apSRC[1] )
		if( pC-pAL == crs.anSTR[0] )
		{
			pO[cr].y |= crs.anSTR[0]==crs.anSTR[1] ? 0x4 : 0x8;
			bON = true;
		}

		if( bON )
		if( cxy.x >= 0 && cxy.x < fx )
		if( cr >= 0 && cr < fy*fz )
		{
			pO[cr].y |= 0x10;
			pO[cr].x = ch;
			if( *pC == '\t' )
			{
				if( pO[cr].y&0x10 && pO[cr].x == ch )
					pO[cr].z = bg;
				else
					pO[cr].z = ch;

				pO[cr].w = '.' - ' ';
			}

		}

		if( bON )
		if( this == crs.apSRC[1] )
		if( pC-pAL >= crs.anSTR[1] )
		{
			pO[cr].y &= 0xf;
			if( crs.anSTR[0] != crs.anSTR[1] )
				pO[cr].y |= 8;
			bON = false;
		}

		switch( *pC )
		{
			case '\r':
				if( pC[1] != '\n' )
				{
					cxy.x = xy.x;
					continue;
				}
				pC++;
				cxy.x = xy.x;
				cxy.y++;
				continue;
			case '\n':
				cxy.x = xy.x;
				cxy.y++;
				continue;
			case '\a':
				cxy.y++;
				cxy.x = xy.x;
				continue;
			case '\t':
				cxy.x = xy.x + ((cxy.x-xy.x)/4 + 1)*4;
				continue;

				/*aC[0] = *pC;
				n = gpmNINCS( pC+1, aC );
				if( !bON || (cxy.y < 0) )
				{
					cxy.x = xy.x + ((cxy.x-xy.x)/4 + n)*4 + 4;
					pC += n;
					continue;
				}

				if( this == crs.apSRC[1] )
				if( ((pC-pAL)+1+n) >= crs.anSTR[1] )
				{
					pO[cr].y &= 0xf;
					if( crs.anSTR[0] != crs.anSTR[1] )
						pO[cr].y |= 8;
					bON = false;
					n = crs.anSTR[1]-((pC-pAL)+1);
				}

				cxy.x++;
				nFILL = (xy.x + ((cxy.x-xy.x)/4 + n)*4 + 4) - cxy.x;

				//if( cxy.x < 0 )
				if( cxy.x+nFILL < 1 )
				{
					cxy.x += nFILL;
					pC += n;
					continue;
				}
				else if( cxy.x < 0)
				{
					nFILL += cxy.x;
					cxy.x = 0;
				}

				for(
						U4	cr = cxy.x + cxy.y*fz,
							cre = min( cxy.x+nFILL, fx) + cxy.y*fz;

						cr < cre;

						cr++
					)
				{
					pO[cr].y |= 0x10;
					pO[cr].x = ch;
				}
				cxy.x += nFILL;
				pC += n;
				continue;*/

			case ' ':
				cxy.x++;
				continue;
		}

		if( *pC < ' ' )
			continue;

		nx = *pC;
		if( nx & 0x80 )
		{
			pC++;
		} else
			nx = 0;

		if( cxy.x >= fx || cxy.x < 0 || cxy.y < 0 )
		{
			cxy.x++;
			continue;
		}


		cr = cxy.x + cxy.y*fz;

		//pO[cr] = c;
		if( pO[cr].y&0x10 && pO[cr].x == ch )
			pO[cr].z = bg;
		else
			pO[cr].z = ch;

		pO[cr].w = *pC - ' ';
		cxy.x++;

		if( !nx )
			continue;
		pO[cr].w += (nx&4)>>2;

	}
	return cxy;
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

	pSRCc = pSRCc->lzy_add( &pFND, sizeof(pFND), s = -1);
	return pFND;
}


