#include "gpccrs.h"
#include "gpcSRC.h"
bool gpcCRS::miniLOCK( gpcPIC* pPIC, SDL_Renderer* pRNDR, I4x2 allWH ) {
	if( !this )
		return false;

	if( !pPIC || !pRNDR )
		return false;

	if( pLOCK != pMINI )
	{
		gpmDELary( pMINI );
	}


	/*winWH /= I4x2(4,8);
	winWH &= I4x2(2,3);*/
	pLOCK = pPIC->u1x4LOCK( pRNDR, allWH, allWH.x );
	pMINI = pLOCK;

	if( !pLOCK )
	{
		nMINI = 0;
		return false;
	}

	nMINI = pPIC->txWH.a4x2[0].area(); // CRSfrm.a4x2[1].area();

	return true;
}
bool gpcCRS::miniRDYesc( gpcWIN* pWIN, gpcPIC* pPIC ) {
	bool bESC = false; //, bNoMini;
	if( CRSfrm.x >= CRSfrm.z )
	{
		CRSfrm.x = CRSfrm.z;
		bESC = true;
	}
	if( CRSfrm.y >= CRSfrm.w )
	{
		CRSfrm.y = CRSfrm.w;
		bESC = true;
	}
	I4x2 &div = pWIN->wDIVcr( id ).a4x2[0];
	fxyz.z = pPIC->txWH.z;
	if( bESC )
	{
		fxyz.w = (CRSfrm.w-1)*fxyz.z;
		return true;
	}

	U4	divOFFfrm	= pPIC->txWH.a4x2[1].area()/2,
		divOFF	 	= (div.x ? fxyz.z/2: 0)
					+ (div.y ? divOFFfrm/2:0); //pPIC->txWH.a4x2[1].area()/4
	pMNoff = pMINI+divOFF;
	pMNoffFRM = pMNoff+divOFFfrm;
	for( U4 h = 0; h < CRSfrm.w; h++ )
	{
		fxyz.w = h*fxyz.z;
		gpmZnOF( pMNoff+fxyz.w, CRSfrm.z );
		gpmZnOF( pMNoffFRM+fxyz.w, CRSfrm.z );
	}

	return false;
}
U4 gpcCRS::miniRDYmap(
						gpcWIN* pWIN, I4x4& lurdAN,
						U4* &pM,	U4* &pC,	U4* &pR,
						U4* &psCp,	U4* &psRp,	U4& z,
						U4x4& mZN, bool bSHFT
					) {
	gpcMAP* pMAP = pWIN->piMASS ? &(pWIN->piMASS->mapCR) : NULL;
	if( !pMAP )
		return -1;

	//gpcMASS* pMASS = pWIN->piMASS;
	U4 selID = id;
	if( bSHFT )
	if( pWIN->apCRS[pWIN->srcDIV] )
		selID = pWIN->srcDIV; /// Szomszédol!

	I4x4	*p_selAI = (selID == id ? selANIN : pWIN->apCRS[selID]->selANIN),
			&xyCR = aXYuvPC[0];

	lurdAN = I4x4( p_selAI[0].a4x2[0], p_selAI[1].a4x2[0] ).lurd();

	U4x2 rdZN = lurdAN.a4x2[1] - U4x2(1,0);
	U4x4 dim;
	/// nagyon vigyázz itt nem BIZTOS, hogy a saját, PC és pR-rel dolgozik,
	/// hanem ha le van nyomva a SHIFT akor e SRC_DIV-vel
	pMAP->MAPalloc( rdZN, mZN, selID );
	U4	c, r, ie,
		i = pWIN->piMASS->jDOitREF( pWIN, 0, ie, &pM, &pC, &pR, &z );

	bool bNoMini; //, bTRIG = false;
	if( selID ) {
		i = mZN.a4x2[1].sum()*(U4)selID;
		pC += i; pR += i;
	}


	gpcSRC* pSrC = NULL;
	U1 oo;
	std::cout << stdRDY " RDY";
	for( i = 0; i < ie; i++ ) {
		if( !pM[i] )
			continue;
		c = i%z;
		r = i/z;
		bNoMini = (
						   ( c+1 >= lurdAN.x && r >= lurdAN.y )
						&& ( c+1 <= lurdAN.z && r <= lurdAN.w )
					);
		/// TÁMADÁS HO RUKK!!
		dim =	(pSrC = pWIN->piMASS->srcFND( pM[i] ))
				->srcBRK( bNoMini, selID, " \t\r\n" );

		//dim = mass.srcFND( pM[i] )->CRSdim( bNoMini );
		if( pC[c] < dim.x )
			pC[c] = dim.x;
		if( pR[r] < dim.y )
			pR[r] = dim.y;

		if( pSrC ? !bNoMini : true )
			continue;

		oo = pSrC->srcSCN( *this, bNoMini, selID );
		if( oo&1 )
			aCRSonPG[0].a4x2[1] = I4x2(c,r);
		if( oo&2 )
			aCRSonPG[1].a4x2[1] = I4x2(c,r);

	}

	/// pCp pRp ----------------------------------------
	psCp = pWIN->apCRS[selID]->ZNpos( mZN.a4x2[1], pC, pR );
	psRp = pWIN->apCRS[selID]->pRp;
	/// end pCp pRp ----------------------------------------
	I4x2 brdr = I4x2( psCp[mZN.x],psRp[mZN.y] );
	if( (CRSfrm.x+brdr.x < 1) || (CRSfrm.y+brdr.y < 1) ) {
		// ne lehessen messzebb tekerni
		brdr *= -1;
		CRSfrm.a4x2[0].mx( brdr );
		return -1;
	}

	return selID;
}
///------------------------------
///
/// 		miniRDY
///
///------------------------------
static U1 gpsTITLE[0x100];
void gpcCRS::miniRDY(	gpcWIN* pWIN,
						gpcPIC* pPIC, SDL_Renderer* pRNDR,
						bool bSHFT, bool bMOV ) {
	picBG.lzyRST(); bobBG.lzyRST();

	if( !miniLOCK( pPIC, pRNDR, pWIN->wDIVcrALLOCK() ) )
		return;

	if( miniRDYesc(pWIN,pPIC) )
		return;

	I4x4 lurdAN;
	U4x4 mZN, dim;
	U4	*pM, *pC, *pR, *psCp, *psRp,
		z, c, r, i;

	U4 selID = miniRDYmap( pWIN, lurdAN, pM, pC, pR, psCp, psRp, z, mZN, bSHFT );
	if( selID > 3 )
		return;

	/// engem most a kijelölés vége érdekel
	I4x2& CR = aCRSonPG[1].a4x2[1];
	aCRSonPG[3].a4x2[0] = 	aCRSonPG[1].a4x2[0]
							+I4x2(psCp[CR.x],psRp[CR.y]);
	if( bMOV )
	if( aCRSonPG[3].a4x2[1] != aCRSonPG[3].a4x2[0] )
	{
		I4x2 &wh = CRSfrm.a4x2[1];
		I4	x = aCRSonPG[3].a4x2[0].x+CRSfrm.x,
			y = aCRSonPG[3].a4x2[0].y+CRSfrm.y;
		if( x < 0 )
			CRSfrm.x = 0-aCRSonPG[3].a4x2[0].x;
		else if( x >= wh.x )
			CRSfrm.x = 0-(aCRSonPG[3].a4x2[0].x-wh.x);

		if( y < 0 )
			CRSfrm.y = 0-aCRSonPG[3].a4x2[0].y;
		else if( y >= wh.y )
			CRSfrm.y = 0-(aCRSonPG[3].a4x2[0].y-wh.y);

		aCRSonPG[3].a4x2[1] = aCRSonPG[3].a4x2[0];
	}
	bool bON=(id==pWIN->onDIV.x), bNoMini;

	I4x4 &xyCR = aXYuvPC[0];
	gpeCLR	c16bg = gpeCLR_blue,
			c16fr = gpeCLR_blue2,
			c16ch = gpeCLR_blue2;

	gpcSRC* pSRC;
	U1 sSTR[0x20];
	U4 iON = scnZN.a4x2[0]*I4x2(1,z);
	I4x2 onAN = scnZN.a4x2[0]+I4x2(1,0);
	U8 s;
	if( lurdAN.x )
	for( U4 r = lurdAN.y, c, ce; r <= lurdAN.w; r++ )
	{
		if( r < mZN.y )
			xyCR.y = psRp[r];
		else
			xyCR.y = psRp[mZN.y]+((r-mZN.w)*gpdSRC_ROWw);


		for( c = lurdAN.x-1; c < lurdAN.z; c++ ) {
			if( c < mZN.x )
				xyCR.x = psCp[c];
			else
				xyCR.x = psCp[mZN.x]+((c-mZN.z)*gpdSRC_COLw);
			c16fr = (iON == I4x2(c,r)*I4x2(1,z))	? gpeCLR_white
													: (
														(selID == pWIN->srcDIV) 	? gpeCLR_orange
																				: gpeCLR_green
													  );

			xyCR.a4x2[0] += CRSfrm.a4x2[0];
			xyCR.a4x2[1] = I4x2( (c < mZN.x ? pC[c] : gpdSRC_COLw ), (r < mZN.y ? pR[r] : gpdSRC_ROWw ) );

			fxyz.a4x2[0] = (xyCR.a4x2[0]+xyCR.a4x2[1]).MN( CRSfrm.a4x2[1] );

			pMNoffFRM	->pos( xyCR.a4x2[0], fxyz )
						->frmBRDR( xyCR.a4x2[1], c16fr, 0xf, fxyz-I4x4( xyCR.a4x2[0].MX(0), 0 )  );

			if( c16fr != gpeCLR_white )
				continue;

			pMNoffFRM	->pos( xyCR.a4x2[0]+I4x2(1,0), fxyz )
						->print( onAN.pSTRalf4n(sSTR), gpeCLR_white );

		}
	}

	U1* pTITLE = gpsTITLE;
	for( U4 r = 0,a; r < mZN.y; r++ )
	{
		xyCR.y = psRp[r]+CRSfrm.y;
		if( xyCR.y >= CRSfrm.w )
			break;
		if( !pR[r] )
			continue;

		xyCR.w = pR[r];
		if( (xyCR.y + xyCR.w) < 0 )
			continue;

		i = r*z;
		fxyz.y = min( CRSfrm.w, (xyCR.y + xyCR.w) );
		for( U4 c = 0; c < mZN.x; c++ )
		{
			xyCR.x = psCp[c]+CRSfrm.x;
			if( xyCR.x >= CRSfrm.z )
				break;
			if( !pC[c] )
				continue;

			xyCR.z = pC[c];
			if( (xyCR.x + xyCR.z) < 0 )
				continue;

			fxyz.x = min( CRSfrm.z, (xyCR.x + xyCR.z) );
			a = c+1;
			c16fr = bON&(iON == i+c) ? gpeCLR_white : gpeCLR_blue2;
			c16ch = gpeCLR_blue2;

			if( !pM[i+c] ) {
				// nem lesz SRC
				if( !lurdAN.x || r < lurdAN.y || r > lurdAN.w || !((a >= lurdAN.x) && (a <= lurdAN.z )) )
				{
					// NINCSEN kijelölve
					if( bON ? iON != i+c : true )
						continue;

					// de rajta a pointer
					pMNoffFRM	->pos( xyCR.a4x2[0], fxyz )
								->frmBRDR( xyCR.a4x2[1], gpeCLR_white, 0xf, fxyz-I4x4( xyCR.a4x2[0].MX(0), 0 )  );
					pMNoffFRM	->pos( xyCR.a4x2[0]+I4x2(1,0), fxyz )
								->print( onAN.pSTRalf4n(sSTR), gpeCLR_white );
					continue;
				}
				continue;
			}

			pTITLE = gpsTITLE;
			if( pSRC = pWIN->piMASS->srcFND(pM[i+c]) ) {
				//gpcRES* pRES = pSRC->apOUT[3];
				if( pSRC->pMEM  )
				{
					if( pSRC->pMEM->pCTRL )
					{
						if( *(pSRC->pMEM->pCTRL->sNAME) )
							pTITLE += sprintf( (char*)pTITLE, "%s", (pSRC->pMEM->pCTRL->sNAME) );
					}
					if( pSRC->pMEM->pGL )
					{
						if( pSRC->pMEM->pGL->aPICid[0] )
						{
							/// CELL PICTURES BACK GROUND -------------------
							//xyWH azonos a xyPIC[0]-val!
							aXYuvPC[2].a4x2[1].x = pSRC->pMEM->pGL->aPICid[0]-1;
							picBG.lzyADD( &aXYuvPC, sizeof(aXYuvPC), s = -1 );
						}
						if( pSRC->pMEM->pGL->aBOBid[0] )
						{
							aXYuvPC[2].a4x2[1].x = pSRC->pMEM->pGL->aBOBid[0]-1;
							bobBG.lzyADD( &aXYuvPC, sizeof(aXYuvPC), s = -1 );
						}
					}
				}
				/*else if( pRES )
				{
					U4	n = pRES->nFND(),
						i = pRES->iFND( gpeALF_NAME );
					if( i < n )
					{
						gpcALU& alu = pRES->ALU( i );
						if( alu.typ().x&0x10 )
							pTITLE += sprintf( (char*)pTITLE, "%s", (char*)alu.pDAT );

					}
				}*/
				/*if( pSRC->picID )
				{
					/// CELL PICTURES BACK GROUND -------------------
					//xyWH azonos a xyPIC[0]-val!
					aXYuvPC[2].a4x2[1].x = pSRC->picID-1;
					picBG.lzyADD( &aXYuvPC, sizeof(aXYuvPC), s = -1 );
				}*/

			}



			if( !lurdAN.x || r < lurdAN.y || r > lurdAN.w ) {
				// NINCSEN kijelölés az egész sorban
				pMNoffFRM	->pos( xyCR.a4x2[0], fxyz )
							->frmBRDR( xyCR.a4x2[1], c16fr, 0xf, fxyz-I4x4( xyCR.a4x2[0].MX(0), 0 )  );

				pSRC->srcRDY(
									pMNoff,
									xyCR.a4x2[0], fxyz.a4x2[0],
									CRSfrm.z, fxyz.z,
									*this, false, selID
								);

				if( bON ? iON == i+c : false )	// rajta a pointer
					pMNoffFRM	->pos( xyCR.a4x2[0]+I4x2(1,0), fxyz )
								->print( onAN.pSTRalf4n(sSTR), gpeCLR_white );

				if( pTITLE > gpsTITLE )
					pMNoffFRM	->pos( xyCR.a4x2[0]+I4x2(1,0), fxyz )
								->print( gpsTITLE, gpeCLR_white );

				continue;
			}

			bNoMini = ((a >= lurdAN.x) && (a <= lurdAN.z ));
			if( bNoMini )
			{
				c16fr = bED ? gpeCLR_green2 : gpeCLR_cyan;
				c16ch = bED ? gpeCLR_cyan : gpeCLR_blue2;
			} else
				c16fr = c16ch = gpeCLR_blue2;

			if( bON )
			if( iON == i+c )
			{
				c16fr = gpeCLR_white;
			}

			pMNoffFRM	->pos( xyCR.a4x2[0], fxyz )
						->frmBRDR( xyCR.a4x2[1], c16fr, 0xf, fxyz-I4x4( xyCR.a4x2[0].MX(0), 0 )  );


			if( pTITLE > gpsTITLE )
				pTITLE += sprintf( (char*)pTITLE, " " );

			if( a == lurdAN.x && r == lurdAN.y )
				pTITLE += lurdAN.a4x2[0].strALF4N(pTITLE);
			else if( a == lurdAN.z && r == lurdAN.w )
				pTITLE += lurdAN.a4x2[1].strALF4N(pTITLE);
			else if( bON ? iON == i+c : false )
				pTITLE += onAN.strALF4N(pTITLE);

			if( pTITLE > gpsTITLE )
				pMNoffFRM	->pos( xyCR.a4x2[0]+I4x2(1,0), fxyz )
							->print( gpsTITLE, gpeCLR_white );

			pSRC->srcRDY(
								pMNoff,
								xyCR.a4x2[0], fxyz.a4x2[0],
								CRSfrm.z, fxyz.z,
								*this,
								bNoMini, selID
							);
		}
	}
}


