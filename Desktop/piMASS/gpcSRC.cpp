#include "gpcSRC.h"
#include "gpccrs.h"



gpcSRC::gpcSRC()
{
    //ctor
    gpmCLR;
}

gpcSRC& gpcSRC::SRCcpy( U1* pS, U1* pSe )
{
	U1* pKL = nA ? pA : NULL;
	if( pSe <= pS )
	{
        // kill
        gpmDELary(pKL);
        pB = pA = NULL;
        nA = nL = 0;
		return *this;
	}
	nL = pSe-pS;
	if( nA < nL+2 )
	{
		gpmDELary(pKL);
		nA = gpmPAD( nL+2, 0x10 );
		pA = new U1[nA];
	}
	U8 nLEN;
	U4 iA = gpfVAN( pS, (U1*)"\a", nLEN );
	if( iA >= nL )
	{
		iA = sizeof(" \a");
		gpmMcpyOF( pA, " \a", iA );
		iA--;
	} else
		iA = 0;

	gpmMcpyOF( pA+iA, pS, nL );
	nL += iA;
	pA[nL] = 0;

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
	if( pS+nL >= pSe )
	{
		nL = pSe-pS;
		pS = pSe;
	} else if( !pS[nL] )
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
	gpmMcpyOF( this, &B, 1 );
	gpmZn( (U1*)&B.pALFtg, gpnZ );

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
		gpmMcpyOF( pA, BpA, nL );
		pA[nL] = 0;
		return *this;
	}

	// aszondja nem egyértelmű akkor legyen az javít
	// potoljuk // az elejére rakunk egyet mert kell
	pB = pA;
	*pB = '\a';
	gpmMcpyOF( pB+1, BpA, nL );
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
	for( U1 i = 0; i < gpmN(apOUT); i++ )
		gpmDELary(apOUT[i]);

	gpmDELary(pMINI);
	gpmDELary(pBIG);
	gpmDELary(pMAP);

}
gpcSRC* gpcSRC::SRCfrm(	U1x4* p1, const I4x4& xy, gpeCLR fr, const I4x4& fxyz ) //, I4 fz )
{
	if( this ?
				   ( fxyz.x <= 0 ||	fxyz.y <= 0 )
				|| ( xy.x >= fxyz.x ||	xy.y >= fxyz.y )
				: true )
		return NULL;

	if( !p1 ) // ki lehet vele kapcsolni a keret rajzolást ha nem kap pointert
		return this;

	for( I4 r = max(xy.y,0); r < fxyz.y; r++ )
	for( I4 c = max(xy.x,0) + r*fxyz.z, s = c-xy.x, ce = c+fxyz.x-s; c < ce; c++ )
			p1[c] = 0;

	if( bSW&gpeMASSoffMSK )
		return this;

	// UP
	if( xy.y >= 0 )
	for( I4 yz = xy.y*fxyz.z, c = max(xy.x,0) + yz, ce = yz + fxyz.x; c < ce; c++ )
	{
		p1[c].z = fr;
		p1[c].w |= 0x1;
	}
	/*if( pS1 )
	{
		I4 yz = xy.y*fxyz.z, c = max(xy.x,0) + yz + 1, ce = yz + fxyz.x - 1, cs = c;
		while( *pS1 )
		{
			p1[c].z = fr;
			p1[c].w = *pS1;
			c++;
			if( c >= ce )
				break;
			pS1++;
		}
	}*/
	//DWN
	for( I4 yz = (fxyz.y-1)*fxyz.z, c = max(xy.x,0) + yz, ce = yz + fxyz.x; c < ce; c++ )
	{
		p1[c].z = fr;
		p1[c].w |= 0x4;
	}

	// LEFT
	for( I4 r = max(xy.y,0), rr = max(xy.x,0) + r*fxyz.z ; r < fxyz.y; r++, rr += fxyz.z )
	{
		p1[rr].z = fr;
		p1[rr].w |= 0x8;
	}

	// RIGHT
	for( I4 r = max(xy.y,0), rr = fxyz.x-1 + r*fxyz.z; r < fxyz.y; r++, rr += fxyz.z )
	{
		p1[rr].z = fr;
		p1[rr].w |= 0x2;
	}


	return this;
}
I4x2 gpcSRC::SRCmini(
						U1x4* pO,  I4x2 xy,

						I4 fx,
						I4 fy,

						I4 fz, I4 zz,

						gpcCRS& crs,
						gpeCLR bg, //gpeCLR fr,
						gpeCLR ch,
						bool bNoMini
					)
{
	if( this ?
				   ( fx <= 0 	||	fy <= 0 )
				|| ( xy.x >= fx ||	xy.y >= fy )
				: true )
		return xy;

	I4x2 cxy = xy;
	U1x4 c;
	//c.u4 = pC64[15];
	U1 nx, aC[] = " ";
	I4 cr, n, rr;
	//for( I4 r = max(xy.y,0); r < fy; r++ )
	//for( I4 c = max(xy.x,0) + r*zz, ce = c+fx; c < ce; c++ )
	//		pO[c] = 0;


	bool bON = false, bSEL = false;
	I4 nFILL;

	for( U1* pC = pSRCstart( bNoMini ), *pAL = pSRCalloc( bNoMini ) , *pCe = pC+dim.w; pC < pCe; pC++ )
	{
		if( cxy.y >= fy )
			break;

		cr = cxy.x + cxy.y*zz;
		if( this == crs.apSRC[0] )
		if( crs.apSRC[0] == crs.apSRC[1] )
		if( pC-pAL == crs.iSTR.x )
		{
			if( cr > 0 )
				pO[cr].z |= 0x10;
			//pO[cr].y |= crs.iSTR.x==crs.iSTR.y ? 0x4 : 0x8;
			bON = true;
		}

		if( bON )
		if( cxy.x >= 0 && cxy.x < fx )
		if( cr >= 0 && cr < fy*zz )
		{
			//pO[cr].y |= 0x10;
			//pO[cr].x = ch;
			if( *pC == '\t' )
			{
				if( pO[cr].y&0x10 && pO[cr].x == ch )
					pO[cr].z = bg;
				else
					pO[cr].z = ch;

				pO[cr].w = '.' - ' ';
			}
			pO[cr].z |= 0x10;
		}

		if( bON )
		if( this == crs.apSRC[1] )
		if( pC-pAL >= crs.iSTR.y )
		{
			//pO[cr].y &= 0xf;
			//if( crs.iSTR.x != crs.iSTR.y )
			//	pO[cr].y |= 8;
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
			case ' ':
				cxy.x++;
				continue;
			case '_':
				if( cr < 1 )
					continue;
				pO[cr-1].y = 1;
				if( (pO[cr-1].w/0x20)%2 )
					cxy.x++;

				continue;
			case '#':
				if( cr < 1 )
						continue;
				pO[cr-1].y = 2;
				continue;
		}

		if( *pC < ' ' )
			continue;

		nx = *pC;
		if( nx&0x80 )
			pC++;
		else
			nx = 0;

		if( cxy.x >= fx || cxy.x < 0 || cxy.y < 0 )
		{
			cxy.x++;
			continue;
		}


		cr = cxy.x + cxy.y*zz;

		if( (pO[cr].y&0x10) && (pO[cr].x==ch) )
			pO[cr].z |= bg;
		else
			pO[cr].z |= ch;

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
	gpmDEL(pCAM);
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

	pSRCc = pSRCc->lzyADD( &pFND, sizeof(pFND), s = -1);
	return pFND;
}


