#include "gpcSRC.h"
#include "gpccrs.h"
///-------------------------------------------------------------
///
///                         gpMEM
///
///-------------------------------------------------------------
U4 gpMEM::nALL( I4 iPC ) {
	if( this ? (iPC<0) : true )
		return 0;
	I4x4* pA = pALL(iPC);
	return pA ? pA->y : 0;
}
I4x4* gpMEM::pALL( I4 iPC ) {
	if( iPC < 0 )
		return NULL;

	for( U4 i = 0; i < nALLOC; i++ ) {
		if( !pALLOC[i].n )
			continue;

		if( pALLOC[i].i > iPC )
			continue;
		if( (iPC-pALLOC[i].i) >= pALLOC[i].n )
			continue;

		return pALLOC+i;
	}
	return NULL;
}
I4 gpMEM::iFREE( I4 iPC ) {
	if( this ? iPC < 0 : true )
		return -1;

	I4x4* pA = pALL(iPC);
	if(!pA)
		return -1;
	iPC = pA->i;
	U4 iF = nFREE;
	for( U4 i = 0; i < nFREE; i++ ) {
		if( !pFREE[i].n ) {
			if( iF < i )
				continue;

			iF = i;
			continue;
		}

		if( pFREE[i].n+pFREE[i].i != iPC ) {
			if( pFREE[i].i != iPC+pA->n )
				continue;

			pFREE[i].i = iPC;
			pFREE[i].n += pA->n;
			pA->n = 0;
			return pA->i = -1;
		}

		pFREE[i].n += pA->n;
		pA->n = 0;
		return pA->i = -1;
	}

	if( iF < nFREE ) {
		pFREE[iF] = *pA;
		pA->n = 0;
		return pA->i = -1;
	}

	I4x4* pKILL = pFREE;
	nFREE++;
	pFREE = new I4x4[nFREE];
	if( pKILL ) {
		gpmMcpyOF( pFREE, pKILL, iF );
		delete[] pKILL;
	}
	pFREE[iF] = *pA;
	pA->n = 0;
	return -1;
}
I4 gpMEM::iALL( U4 nA ) {
	if( this ? nA < 1 : true )
		return -1;
	I4	nA0x10 = gpmPAD( nA+1, 0x10 ),
		iA = -1;
	for( U4 i = 0; i < nFREE; i++ ) {
		if( pFREE[i].n < nA0x10 )
			continue;

		iA = pFREE[i].i;
		if( (pFREE[i].n-nA0x10) > 0x10 ) {
			// lecíp
			pFREE[i].n -= nA0x10;
			pFREE[i].i += nA0x10;
		} else {
			// egészet
			nA0x10 = pFREE[i].n;
			pFREE[i].n = 0;
		}
		break;
	}
	if( iA < 0 ) {
		iA = nDAT;
		nDAT += nA0x10;
	}
	U4 a = 0;
	for( ; a < nALLOC; a++ ) {
		if( pALLOC[a].n )
			continue;

		pALLOC[a].i = iA;
		pALLOC[a].n = nA0x10;
		U1* pA = pUn( iA, nA0x10 );
		pA[nA] = 0;
		return iA;
	}

	I4x4* pKILL = pALLOC;
	nALLOC= a+1;
	pALLOC = new I4x4[nALLOC];
	if( pKILL ) {
		gpmMcpyOF( pALLOC, pKILL, a );
		delete[] pKILL;
	}

	pALLOC[a].i = iA;
	pALLOC[a].n = nA0x10;

	U1* pA = pUn( iA, nA0x10 );
	pA[nA] = 0;
	return iA;
}
///--------------------------
///			gpMEM::gpOBJ
///--------------------------
gpOBJ* gpMEM::OBJfnd( I4 dctID ) {
	if( !this )
		return NULL;
	gpOBJ* pO0 = gpmLZYvali( gpOBJ, &lzyOBJ );
	if( !pO0 )
		return NULL;
	for( U4 nO = lzyOBJ.nLD(sizeof(gpOBJ)), o = 0; o < nO; o++ ) {
		if( pO0[o].dctID != dctID )
			continue;
		return pO0+o;
	}
	return NULL;
}

gpOBJ* gpMEM::OBJadd( char* pS, I4 dctID ) {
	gpOBJ *pO = OBJfnd(dctID);
	if( pO )
		return pO;

	U4 nO = lzyOBJ.nLD(sizeof(gpOBJ));
	pO = (gpOBJ*)lzyOBJ.Ux( nO, sizeof(*pO) );
	if( !pO )
		return NULL;
	pO->pMEM = this;
	pO->dctID = dctID;
	pO->iPTR = -1;
	gpPTR* pPTR = pO->pPTR();
	return pO;
}
///--------------------------
///			gpOBJ
///--------------------------

///-------------------------------------------------------------
///
///                         gpcSRC
///
///-------------------------------------------------------------
gpcSRC::gpcSRC() {
    //ctor
    gpmCLR;
}
///-------------------------------------------------------------
///  gpcSRC :: (NUM UTF8 AN ALF) -> OBJ(dctID)
///-------------------------------------------------------------
gpOBJ* gpcSRC::srcOBJfnd( I4 dctID ) { return pMEM->OBJfnd(dctID); }
gpOBJ* gpcSRC::srcOBJadd( char* pS,I4 dctID ) { return pMEM->OBJadd(pS,dctID); }
///-------------------------------------------------------------
///  gpcSRC :: (NUM UTF8 AN ALF) -> OBJ(dctID) + pBLK->pROW[i]
///-------------------------------------------------------------
gpBLK* gpcSRC::srcBLKnew( char* pS, gpeOPid opID, gpROW* pRml, I4 bIDm, I4 bIDmR, I4 mnID  ) {
	gpBLK	*pBLK = lzyBLOCK.pNEWblk( opID, bIDm, bIDmR, mnID )->pRST( pMEM );
	gpROW	*pRf = pBLK->pROW(0,true);
	if( !pRf )
		return pBLK;

	if( !pRml )
		return pBLK;

	*pRf = *pRml;
	if( opID != pRml->pstOP )
		pRf->pstOP = opID;
	if( gpaOPgrp[opID] != gpeOPid_begin )
		pBLK->pNEWrow();

	pRml->pstOP = gpeOPid_nop;
	pRml->bIDup = pBLK->bID;
	return pBLK;
}

gpBLK* gpcSRC::srcBLKmNdID( char* pS, gpBLK* pBLK, I4 dctID, I4 mnID ) {
	gpOBJ *pO = pMEM->OBJadd(pS,dctID);
	if( !pO )
		return pBLK;

	if( !pBLK )
		pBLK = srcBLKnew( pS, gpeOPid_stk, NULL, -1, -1, mnID );

	gpROW* pRl = pBLK->pLSTrow();
	if( !pRl )
		return pBLK;
	if( pBLK->opIDgrp() == gpeOPid_begin )
	{
		pBLK = srcBLKup( pS, pBLK, gpeOPid_nop, mnID );
		//srcBLKnew( pS, gpeOPid_nop, pRl, pBLK->bID, pBLK->iLAST(), mnID );
		pRl = pBLK->pLSTrow();
	}

	*pRl = pO;
	if( pRl->mnID == mnID )
		return pBLK;

	pRl->mnID = mnID;
	return pBLK;
}

gpBLK* gpcSRC::srcBLKaryNUM( char* pS, gpBLK* pBLK, I4 dctID, I4 mnID, gpeCsz cAN, const I8x2& AN ) {

	pBLK = srcBLKmNdID( pS, pBLK, dctID, mnID );
	gpOBJ* pO = srcOBJadd( pS, dctID );
	if( !pO )
		return pBLK;

	gpPTR* pP = pO->pPTRu1();
	pP->x = pP->y = 1;
	pP->z = 0;
	pP->cID = cAN;
	U4 nS = sizeof(AN);

	I4x4* pALL = pMEM->pALL( pP->iPC );
	if( (pALL?pALL->n:0) < nS )
	{
		pMEM->iFREE( pP->iPC );
		pP->iPC = pMEM->iALL( nS );
	}
	U1* pA = pMEM->pUn( pP->iPC, nS );
	gpmMcpy( pA, &AN, nS );
	return pBLK;
}
gpBLK* gpcSRC::srcBLKaryUTF8( gpBLK* pBLK, I4 mnID, char* pS, U4 nS ) {
	I4 mNdID = mnID;
	if( mNdID > 0 )
		mNdID *= -1;
	else
		mNdID *= -1;

	pBLK = srcBLKmNdID( pS, pBLK, mNdID, mnID );
	gpOBJ* pO = srcOBJadd( pS, mNdID );
	if( !pO )
		return pBLK;

	gpPTR* pP = pO->pPTRu1();
	I4x4* pALL = pMEM->pALL( pP->iPC );
	if( (pALL?pALL->n:0) < nS )
	{
		pMEM->iFREE( pP->iPC );
		pP->iPC = pMEM->iALL( nS );
	}
	U1* pA = pMEM->pUn( pP->iPC, nS );
	gpmMcpy( pA, pS, nS );
	pP->x = nS;
	pP->y = 1;
	pP->z = 0;
	pP->cID = gpeCsz_b;

	return pBLK;
}
gpBLK* gpcSRC::srcBLKaryAN( char* pS, gpBLK* pBLK, I4 dctID, I4 mnID, gpeCsz cAN, const I8x2& AN ) {

	pBLK = srcBLKmNdID( pS, pBLK, dctID, mnID );
	gpOBJ* pO = srcOBJadd( pS, dctID );
	if( !pO )
		return pBLK;

	pO->AN = AN;
	pO->cAN = cAN;
	gpPTR* pP = pO->pPTRu1();
	return pBLK;
}
gpcSRC& gpcSRC::SRCcpy( U1* pS, U1* pSe ) {
	U1* pKL = nA ? pA : NULL;
	if( pSe <= pS )
	{
        // kill
        gpmDELary(pKL);
        pB = pA = NULL;
        nA = n_ld(0);
		return *this;
	}
	n_ld(pSe-pS);
	if( nA < n_ld_add()+2 )
	{
		gpmDELary(pKL);
		nA = gpmPAD( n_ld_add()+2, 0x10 );
		pA = new U1[nA];
	}
	U8 nLEN;
	U4 iA = gpfVAN( pS, (U1*)"\a", nLEN );
	if( iA >= n_ld_add() )
	{
		iA = sizeof(" \a");
		gpmMcpyOF( pA, " \a", iA );
		iA--;
	} else
		iA = 0;

	gpmMcpyOF( pA+iA, pS, n_ld_add() );
	//n_ld += iA;
	pA[n_ld_add(iA)] = 0;

	pB = pA + gpfVAN( pA, (U1*)"\a", nLEN );
	if( *pB == '\a' )
		pB++;

	return *this;
}
gpcSRC& gpcSRC::reset( U1* pS, U1* pSe, U1** ppS, U4x4& _spcZN, U4 iADD ) {
	gpmCLR;
	spcZN = _spcZN;
	_spcZN.x += iADD;

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
	n_ld(gpfVAN( pS, (U1*)"\a", anLEN[1] ));
	if( pS+n_ld_add() >= pSe )
	{
		n_ld(pSe-pS);
		pS = pSe;
	} else if( !pS[n_ld_add()] )
		pS+=n_ld_add();											//...\aA..B\a... . .   . . ...S0
	else if( pS[n_ld_add()] != '\a' )
		pS += gpfVAN( pS, (U1*)"\a", anLEN[1], true );	//...\aA..B\a... . .   . . ...S?
	else
		pS+=n_ld_add();											//...\aA..B\a... . .   . . ...S\a


	n_ld(pS-pA);
	if( !ppS )
		return *this;

	*ppS = pS;
	return *this;
}
static const U4 gpnZ = ((gpcSRC*)NULL)->nCLR();

gpcSRC& gpcSRC::operator = ( gpcSRC& B ) {
	if( this == &B )
		return *this; // handle self assignment
	//assignment operator

	/// elopunk mindent most
	gpmMcpyOF( this, &B, 1 );
	gpmZnOF( (U1*)&B.pALFtg, gpnZ );

	qBLD();

	n_ld( &B ? B.n_ld_add() : 0 );
	if( !n_ld_add() )	// B kampec;
	{
		// hagyjuk a pA-t hátha hamarosan mással töltjük meg
		return *this;
	}
	U1* BpA = B.pA;
	U8 i = B.iB();
	// fontos, hogy itt legyenek, mielött a pA-val kezdünk valamit

	if( nA < (n_ld_add()+2) )
	{
		// ha nA == 0 volt azt jelenti nem foglalva volt, ha nem valahonnan kölcsönözve
		if( nA )	// csak akkor felszabadítható ha van mérete, egyébként simán elfelejthető
			gpmDELary(pA);

		nA = gpmPAD( n_ld_add()+2, 0x10 );
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
	if( i < n_ld_add() )
	{
		// nL van
		// i kissebb ez egyértelmű
		pB = pA+i;
		gpmMcpyOF( pA, BpA, n_ld_add() );
		pA[n_ld_add()] = 0;
		return *this;
	}

	// aszondja nem egyértelmű akkor legyen az javít
	// potoljuk // az elejére rakunk egyet mert kell
	pB = pA;
	*pB = '\a';
	gpmMcpyOF( pB+1, BpA, n_ld_add() );
	//n_ld++;
	pA[n_ld_add(1)] = 0;

	for( U8 i = 1; i < n_ld_add(); i++ )
	{
		if( pB[i] != '\a' )
			continue;

		pB[i] = '!'; // ez nagyon nem stimmel figyelmeztet
	}

	return *this;
}

gpcSRC::gpcSRC( gpcSRC& B ) {
	gpmCLR;
	*this = B;
}

gpcSRC::~gpcSRC() {
	if( nA )			// ha nA == 0 nm mi foglaltuk
		gpmDELary(pA);
	gpmDELary(pALFtg);
	gpmDELary(pEXE0);
	gpmDELary(pABI);
	for( U1 i = 0; i < gpmN(apOUT); i++ )
		gpmDELary(apOUT[i]);

	gpmDELary(pMINI);
	//gpmDELary(pBIG);
	gpmDELary(pMAP);
	gpmDEL(pCORE);
}

gpcSRC* gpcSRC::SRCfrm(	U1x4* p1, const I4x4& xy, gpeCLR fr, const I4x4& fxyz ) {
	if( this ?
				   ( fxyz.x <= 0 ||	fxyz.y <= 0 )
				|| ( xy.x >= fxyz.x ||	xy.y >= fxyz.y )
				: true )
		return NULL;

	if( !p1 ) // ki lehet vele kapcsolni a keret rajzolást ha nem kap pointert
		return this;

	for( I4 r = gpmMAX(xy.y,0); r < fxyz.y; r++ )
	for( I4 c = gpmMAX(xy.x,0) + r*fxyz.z, s = c-xy.x, ce = c+fxyz.x-s; c < ce; c++ )
			p1[c] = 0;

	if( bSW&gpeMASSoffMSK )
		return this;

	// UP
	if( xy.y >= 0 )
	for( I4 yz = xy.y*fxyz.z, c = gpmMAX(xy.x,0) + yz, ce = yz + fxyz.x; c < ce; c++ )
	{
		p1[c].z = fr;
		p1[c].w |= 0x1;
	}
	//DWN
	for( I4 yz = (fxyz.y-1)*fxyz.z, c = gpmMAX(xy.x,0) + yz, ce = yz + fxyz.x; c < ce; c++ )
	{
		p1[c].z = fr;
		p1[c].w |= 0x4;
	}
	// LEFT
	for( I4 r = gpmMAX(xy.y,0), rr = gpmMAX(xy.x,0) + r*fxyz.z ; r < fxyz.y; r++, rr += fxyz.z )
	{
		p1[rr].z = fr;
		p1[rr].w |= 0x8;
	}
	// RIGHT
	for( I4 r = gpmMAX(xy.y,0), rr = fxyz.x-1 + r*fxyz.z; r < fxyz.y; r++, rr += fxyz.z )
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
						bool bNoMini, U1 selID	) {

	if( this ?
				   ( fx <= 0 	||	fy <= 0 )
				|| ( xy.x >= fx ||	xy.y >= fy )
				: true )
		return xy;

	I4x2 cxy = xy;
	U1x4 c;
	U1 nx, aC[] = " ";
	I4 cr, n, rr;
	U1	b01 =	 (((U1)(this==crs.apSRC[0]))<<1)
				| ((U1)(this==crs.apSRC[1]));

	bool	bON = false,
			bONpre, bSEL = false;
	I4 nFILL;

	for( U1	*pC = pSRCstart( bNoMini, selID ),
			*pAL = pSRCalloc( bNoMini, selID ),
			*pCe = pC+dim.w;

			pC < pCe;

			pC++	) {

		if( cxy.y >= fy )
			break;
		bONpre = bON;
		cr = cxy.x + cxy.y*zz;

		if( b01 == 3 )
		if( pC-pAL == crs.iSTR.x )
		{
			if( cr > 0 )
				pO[cr].z |= 0x10;
			bON = true;
		}

		if( bON )
		if( cxy.x >= 0 && cxy.x < fx )
		if( cr >= 0 && cr < fy*zz )
		{
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
		if( b01&1 ) //this == crs.apSRC[1] )
		if( pC-pAL >= crs.iSTR.y )
		{
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
				if( bON||bONpre )
					break;
				if( cr < 1 )
					continue;
				pO[cr-1].y = 1;
				if( (pO[cr-1].w/0x20)%2 )
					cxy.x++;

				continue;
			case '#':
				if( bON||bONpre )
					break;
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

///-------------------------------------------------------------
///
///                         gpcMASS
///
///-------------------------------------------------------------
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


gpcSRC* gpcMASS::SRCadd( gpcSRC* pSRC, U4 xfnd, U4& is, U4& n ) {
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


