#include "gpccrs.h"
#include "gpcSRC.h"
bool gpcCRS::miniLOCK( gpcPIC* pPIC, SDL_Renderer* pRNDR, I4x2 allWH ) {
	if( !this )
		return true;

	if( !pPIC || !pRNDR )
		return true;

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
		return true;
	}

	nMINI = pPIC->txWH.a4x2[0].area(); // CRSfrm.a4x2[1].area();
	return false;
}
///------------------------------
///
/// 		miniRDYgl
///
///------------------------------
U1 gpsTITLE[0x100];
void gpcCRS::miniRDYgl( gpcWIN& win, gpcMASS& mass, gpcPIC* pPIC, SDL_Renderer* pRNDR, bool bSHFT )
{
	if( miniLOCK( pPIC, pRNDR, win.wDIVcrALLOCK() ) )
		return;
	//U4 xFND;
	bool bESC = false, bNoMini;
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
	I4x2 &div = win.wDIVcr( id ).a4x2[0];
	I4x4 fxyz;
	fxyz.z = pPIC->txWH.z;

	U4	off = 	  (div.x ? fxyz.z/2: 0)
				+ (div.y ? pPIC->txWH.a4x2[1].area()/4: 0),
		offFRM = pPIC->txWH.a4x2[1].area()/2;

	for( U4 h = 0; h < CRSfrm.w; h++ )
	{
		fxyz.w = h*fxyz.z;
		gpmZnOF( pMINI + off + fxyz.w, CRSfrm.z );
		gpmZnOF( pMINI + off + fxyz.w + offFRM, CRSfrm.z );
	}

	if( bESC )
		return;

	gpcMAP* pMAP = &mass.mapCR;
	if( !pMAP )
		return;

	U4 selID = id;
	if( win.apCRS[win.srcDIV] )
	if( bSHFT )
		selID = win.srcDIV;

	I4x4	*p_selAI = (selID == id ? selANIN : win.apCRS[selID]->selANIN),

			&xyCR = aXYuvPC[0],

			lurdAN = I4x4( p_selAI[0].a4x2[0], p_selAI[1].a4x2[0] ).lurd();
	U4x2	spcZN = lurdAN.a4x2[1] - U4x2(1,0);
	U4x4	mZN, dim;


	gpmZ( aXYuvPC );
	aXYuvPC[1].a4x2[1] = I4x2(1,1);
	aXYuvPC[2].a4x2[0] = I4x2(0,1);
	picBG.lzyRST();
	bobBG.lzyRST();


	/// nagyon vigyázz itt nem BIZTOS, hogy a saját, PC és pR-rel dolgozik,
	/// hanem ha le van nyomva a SHIFT akor e SRC_DIV-vel
	I4	*pM = (I4*)pMAP->MAPalloc( spcZN, mZN, selID ),
		*pC = (I4*)pMAP->pCOL,
		*pR = (I4*)pMAP->pROW,
		i, ie = pC-pM, c, r, z = mZN.z;

	if( selID )
	{
		i = mZN.a4x2[1].sum()*(U4)selID;
		pC += i;
		pR += i;
	}

	for( i = 0; i < ie; i++ ) {
		if( !pM[i] )
			continue;
		c = i%z;
		r = i/z;
		bNoMini = (
						   ( c+1 >= lurdAN.x	&& r >= lurdAN.y )
						&& ( c+1 <= lurdAN.z	&& r <= lurdAN.w )
					);

		dim = mass.SRCfnd( pM[i] )->CRSdim( bNoMini );
		if( pC[c] < dim.x )
			pC[c] = dim.x;
		if( pR[r] < dim.y )
			pR[r] = dim.y;
	}

	/// pCp pRp ----------------------------------------
	I4	*psCp = win.apCRS[selID]->ZNpos( mZN.a4x2[1], pC, pR ), // mapZN.ALLOC-al dolgozik
		*psRp = win.apCRS[selID]->pRp;
	/// end pCp pRp ----------------------------------------
	I4x2 brdr = I4x2( psCp[mZN.x],psRp[mZN.y] );


	if( (CRSfrm.x+brdr.x < 1) || (CRSfrm.y+brdr.y < 1) )
	{
		// ne lehessen messzebb tekerni
		brdr *= -1;
		CRSfrm.a4x2[0].mx( brdr );
		return ;
	}

	bool bON = id == win.onDIV.x;

	gpeCLR	c16bg = gpeCLR_blue,
			c16fr = gpeCLR_blue2,
			c16ch = gpeCLR_blue2;

	gpcSRC* pSRC;
	U1 sSTR[0x20];
	U4 iON = scnZN.a4x2[0]*I4x2(1,z);
	I4x2 onAN = scnZN.a4x2[0]+I4x2(1,0);
	//U1x4* pPOS;
	U8 s;

	if( lurdAN.x )
	for( U4 r = lurdAN.y, c, ce; r <= lurdAN.w; r++ )
	for( c = lurdAN.x-1; c < lurdAN.z; c++ )
	{
		if( c < mZN.x )
			xyCR.x = psCp[c];
		else
			xyCR.x = psCp[mZN.x]+((c-mZN.z)*gpdSRC_COLw);

		if( r < mZN.y )
			xyCR.y = psRp[r];
		else
			xyCR.y = psRp[mZN.y]+((r-mZN.w)*gpdSRC_ROWw);

		c16fr = (iON == I4x2(c,r)*I4x2(1,z)) ? gpeCLR_white
											: (
													(selID == win.srcDIV) ? gpeCLR_orange
																		: gpeCLR_green
											);

		xyCR.a4x2[0] += CRSfrm.a4x2[0]; //frm.xy
		xyCR.a4x2[1] = I4x2( (c < mZN.x ? pC[c] : gpdSRC_COLw ), (r < mZN.y ? pR[r] : gpdSRC_ROWw ) );

		fxyz.a4x2[0] = (xyCR.a4x2[0]+xyCR.a4x2[1]).MN( CRSfrm.a4x2[1] );

		pMINI[off+offFRM]	.pos( xyCR.a4x2[0], fxyz )
							->frmBRDR( xyCR.a4x2[1], c16fr, 0xf, fxyz-I4x4( xyCR.a4x2[0].MX(0), 0 )  );
		if( c16fr != gpeCLR_white )
			continue;

		pMINI[off+offFRM].pos( xyCR.a4x2[0]+I4x2(1,0), fxyz )->print( onAN.strA4N(sSTR), gpeCLR_white );

	}

	/*xyWH.a4x2[0] = CRSfrm.a4x2[0];
	xyWH.z = psCp[mZN.x];
	xyWH.w = psRp[mZN.y];*/
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
			/*xyWH.x = psCp[c]+CRSfrm.x;

			if( xyWH.x >= CRSfrm.z )
				break;
			if( !pC[c] )
				continue;

			if( xyWH.x + pC[c] < 0 )
				continue;*/

			xyCR.x = psCp[c]+CRSfrm.x;
			if( xyCR.x >= CRSfrm.z )
				break;
			if( !pC[c] )
				continue;

			xyCR.z = pC[c];
			if( (xyCR.x + xyCR.z) < 0 )
				continue;

			//fxyz.x = min(CRSfrm.z, xyWH.x+pC[c]);
			fxyz.x = min( CRSfrm.z, (xyCR.x + xyCR.z) );
			a = c+1;
			c16fr = bON&(iON == i+c) ? gpeCLR_white : gpeCLR_blue2;
			c16ch = gpeCLR_blue2;

			if( !pM[i+c] )
			{
				// nem lesz SRC
				if( !lurdAN.x || r < lurdAN.y || r > lurdAN.w || !((a >= lurdAN.x) && (a <= lurdAN.z )) )
				{
					// NINCSEN kijelölve
					if( bON ? iON != i+c : true )
						continue;

					// de rajta a pointer
					pMINI[off+offFRM].pos( xyCR.a4x2[0], fxyz )->frmBRDR( xyCR.a4x2[1], gpeCLR_white, 0xf, fxyz-I4x4( xyCR.a4x2[0].MX(0), 0 )  );
					pMINI[off+offFRM].pos( xyCR.a4x2[0]+I4x2(1,0), fxyz )->print( onAN.strA4N(sSTR), gpeCLR_white );
					continue;
				}
				continue;
			}

			pTITLE = gpsTITLE;
			if( pSRC = mass.SRCfnd(pM[i+c]) )
			{
				gpcRES* pRES = pSRC->apOUT[3];
				if( pRES )
				{
					U4	n = pRES->nFND(),
						i = pRES->iFND( gpeALF_NAME );
					if( i < n )
					{
						gpcALU& alu = pRES->ALU( i );
						if( alu.typ().x&0x10 )
							pTITLE += sprintf( (char*)pTITLE, "%s", (char*)alu.pDAT );

					}
				}
				if( pSRC->picID )
				{
					/// CELL PICTURES BACK GROUND -------------------
					//xyWH azonos a xyPIC[0]-val!
					aXYuvPC[2].a4x2[1].x = pSRC->picID-1;
					picBG.lzyADD( &aXYuvPC, sizeof(aXYuvPC), s = -1 );
				}
				if( pSRC->bobID )
				{
					aXYuvPC[2].a4x2[1].x = pSRC->bobID-1;
					bobBG.lzyADD( &aXYuvPC, sizeof(aXYuvPC), s = -1 );
				}
			}



			if( !lurdAN.x || r < lurdAN.y || r > lurdAN.w )
			{
				// NINCSEN kijelölés az egész sorban
				pMINI[off+offFRM].pos( xyCR.a4x2[0], fxyz )
							->frmBRDR( xyCR.a4x2[1], c16fr, 0xf, fxyz-I4x4( xyCR.a4x2[0].MX(0), 0 )  );

				pSRC->SRCmini(
									pMINI+off,

									xyCR.a4x2[0],

									fxyz.x,fxyz.y,

									CRSfrm.z, fxyz.z,				// fz zz
									*this,
									c16bg, //gpeCLR_blue2,
									gpeCLR_blue2,
									false
							);

				if( bON ? iON == i+c : false )	// rajta a pointer
					pMINI[off+offFRM].pos( xyCR.a4x2[0]+I4x2(1,0), fxyz )->print( onAN.strA4N(sSTR), gpeCLR_white );

				if( pTITLE > gpsTITLE )
					pMINI[off+offFRM].pos( xyCR.a4x2[0]+I4x2(1,0), fxyz )->print( gpsTITLE, gpeCLR_white );

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

			pMINI[off+offFRM].pos( xyCR.a4x2[0], fxyz )
							->frmBRDR( xyCR.a4x2[1], c16fr, 0xf, fxyz-I4x4( xyCR.a4x2[0].MX(0), 0 )  );


			if( pTITLE > gpsTITLE )
				pTITLE += sprintf( (char*)pTITLE, " " );

			if( a == lurdAN.x && r == lurdAN.y )
				pTITLE += lurdAN.a4x2[0].strA4Nplus(pTITLE); // pMINI[off+offFRM].pos( xyCR.a4x2[0]+I4x2(1,0), fxyz )->print( lurdAN.a4x2[0].strA4N(sSTR), c16fr );
			else if( a == lurdAN.z && r == lurdAN.w )
				pTITLE += lurdAN.a4x2[1].strA4Nplus(pTITLE); // pMINI[off+offFRM].pos( xyCR.a4x2[0]+I4x2(1,0), fxyz )->print( lurdAN.a4x2[1].strA4N(sSTR), c16fr );
			else if( bON ? iON == i+c : false )
				pTITLE += onAN.strA4Nplus(pTITLE); //  pMINI[off+offFRM].pos( xyCR.a4x2[0]+I4x2(1,0), fxyz )->print( onAN.strA4N(sSTR), gpeCLR_white );


			if( pTITLE > gpsTITLE )
				pMINI[off+offFRM].pos( xyCR.a4x2[0]+I4x2(1,0), fxyz )->print( gpsTITLE, gpeCLR_white );

			pSRC->SRCmini(
							pMINI + off,

							xyCR.a4x2[0], //pMINI + off + offFRM,

							fxyz.x,fxyz.y,

							CRSfrm.z, fxyz.z,				// fz zz

							*this,
							c16bg, //c16fr,
							c16ch,
							bNoMini
						);
		}
	}
}

void gpcCRS::miniRDYgl_old( gpcWIN& win, gpcMASS& mass, gpcPIC* pPIC, SDL_Renderer* pRNDR, bool bSHFT ) {
	if( miniLOCK( pPIC, pRNDR, win.wDIVcrALLOCK() ) )
		return;
	//U4 xFND;
	bool bESC = false, bNoMini;
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
	I4x2 &div = win.wDIVcr( id ).a4x2[0];
	I4x4 fxyz;
	fxyz.z = pPIC->txWH.z;

	U4	off = 	  (div.x ? fxyz.z/2: 0)
				+ (div.y ? pPIC->txWH.a4x2[1].area()/4: 0),
		offFRM = pPIC->txWH.a4x2[1].area()/2;

	for( U4 h = 0; h < CRSfrm.w; h++ )
	{
		fxyz.w = h*fxyz.z;
		gpmZnOF( pMINI + off + fxyz.w, CRSfrm.z );
		gpmZnOF( pMINI + off + fxyz.w + offFRM, CRSfrm.z );
	}

	if( bESC )
		return;

	gpcMAP* pMAP = &mass.mapCR;
	if( !pMAP )
		return;

	U4 selID = id;
	if( win.apCRS[win.srcDIV] )
	if( bSHFT )
		selID = win.srcDIV;

	I4x4	*p_selAI = (selID == id ? selANIN : win.apCRS[selID]->selANIN),

			&xyWH = aXYuvPC[0],

			lurdAN = I4x4( p_selAI[0].a4x2[0], p_selAI[1].a4x2[0] ).lurd();
	U4x2	spcZN = lurdAN.a4x2[1] - U4x2(1,0);
	U4x4	mZN, dim;


	gpmZ( aXYuvPC );
	aXYuvPC[1].a4x2[1] = I4x2(1,1);
	aXYuvPC[2].a4x2[0] = I4x2(0,1);
	picBG.lzyRST();


	/// nagyon vigyázz itt nem BIZTOS, hogy a saját, PC és pR-rel dolgozik,
	/// hanem ha le van nyomva a SHIFT akor e SRC_DIV-vel
	I4	*pM = (I4*)pMAP->MAPalloc( spcZN, mZN, selID ),
		*pC = (I4*)pMAP->pCOL,
		*pR = (I4*)pMAP->pROW,
		i, ie = pC-pM, c, r, z = mZN.z;

	if( selID )
	{
		i = mZN.a4x2[1].sum()*(U4)selID;
		pC += i;
		pR += i;
	}

	for( i = 0; i < ie; i++ ) {
		if( !pM[i] )
			continue;
		c = i%z;
		r = i/z;
		bNoMini = (
						   ( c+1 >= lurdAN.x	&& r >= lurdAN.y )
						&& ( c+1 <= lurdAN.z	&& r <= lurdAN.w )
					);

		dim = mass.SRCfnd( pM[i] )->CRSdim( bNoMini );
		if( pC[c] < dim.x )
			pC[c] = dim.x;
		if( pR[r] < dim.y )
			pR[r] = dim.y;
	}

	/// pCp pRp ----------------------------------------
	I4	*psCp = win.apCRS[selID]->ZNpos( mZN.a4x2[1], pC, pR ), // mapZN.ALLLOC-al dolgozik
		*psRp = win.apCRS[selID]->pRp;

	/// end pCp pRp ----------------------------------------

	/*for( U4 c = 0; c < mZN.x; c++ )
		xyWH.z += pC[c];

	for( U4 r = 0; r < mZN.y; r++ )
		xyWH.w += pR[r];*/

	xyWH.z = psCp[mZN.x];
	xyWH.w = psRp[mZN.y];

	if( CRSfrm.x+xyWH.z < 1 )
	{
		CRSfrm.x = -xyWH.z;
		bESC = true;
	}
	if( CRSfrm.y+xyWH.w < 1 )
	{
		CRSfrm.y = -xyWH.w;
		bESC = true;
	}
	if( bESC )
		return;

	bool bON = id == win.onDIV.x;

	//xyWH.y = CRSfrm.y;
	gpeCLR	c16bg = gpeCLR_blue,
			c16fr = gpeCLR_blue2,
			c16ch = gpeCLR_blue2;

	gpcSRC* pSRC;
	U1 sSTR[0x20];
	U4 iON = scnZN.a4x2[0]*I4x2(1,z);
	I4x2 onAN = scnZN.a4x2[0]+I4x2(1,0);
	//U1x4* pPOS;
	U8 s;

	if( lurdAN.x )
	for( U4 r = lurdAN.y, c, ce; r <= lurdAN.w; r++ )
	for( c = lurdAN.x-1; c < lurdAN.z; c++ )
	{
		if( c < mZN.x )
			xyWH.x = psCp[c];
		else
			xyWH.x = psCp[mZN.x]+((c-mZN.z)*gpdSRC_COLw);

		if( r < mZN.y )
			xyWH.y = psRp[r];
		else
			xyWH.y = psRp[mZN.y]+((r-mZN.w)*gpdSRC_ROWw);

		c16fr = (iON == I4x2(c,r)*I4x2(1,z)) ? gpeCLR_white
											: (
													(selID == win.srcDIV) ? gpeCLR_orange
																		: gpeCLR_green
											);

		xyWH.a4x2[0] += CRSfrm.a4x2[0];
		//xyWH.a4x2[1] = I4x2( pC[c], pR[r] );

		fxyz.x = min(CRSfrm.z, (xyWH.x + (c < mZN.x ? pC[c] : gpdSRC_COLw )) );
		fxyz.y = min(CRSfrm.w, (xyWH.y + (r < mZN.y ? pR[r] : gpdSRC_ROWw )) );

		pMINI[off+offFRM].pos( xyWH.a4x2[0], fxyz )
			->frmBRDR( xyWH.a4x2[1], c16fr, 0xf, fxyz-I4x4( xyWH.a4x2[0].MX(0), 0 )  );
		if( c16fr != gpeCLR_white )
			continue;

		pMINI[off+offFRM].pos( xyWH.a4x2[0]+I4x2(1,0), fxyz )->print( onAN.strA4N(sSTR), gpeCLR_white );

	}

	xyWH.a4x2[0] = CRSfrm.a4x2[0];
	xyWH.z = psCp[mZN.x];
	xyWH.w = psRp[mZN.y];

	for( U4 r = 0,a; r < mZN.y; r++ )
	{
		xyWH.y = psRp[r]+CRSfrm.y;

		if( xyWH.y >= CRSfrm.w )
			break;
		if( !pR[r] )
			continue;
		if( (xyWH.y + pR[r]) < 0 )
			continue;

		i = r*z;
		fxyz.y = min(CRSfrm.w, xyWH.y+(int)pR[r]);
		for( U4 c = 0; c < mZN.x; c++ )
		{
			xyWH.x = psCp[c]+CRSfrm.x;

			if( xyWH.x >= CRSfrm.z )
				break;
			if( !pC[c] )
				continue;

			if( xyWH.x + pC[c] < 0 )
				continue;

			fxyz.x = min(CRSfrm.z, xyWH.x+pC[c]);
			a = c+1;
			c16fr = bON&(iON == i+c) ? gpeCLR_white : gpeCLR_blue2;
			c16ch = gpeCLR_blue2;

			if( !pM[i+c] )
			{
				// nimlesz SRC
				if( !lurdAN.x || r < lurdAN.y || r > lurdAN.w || !((a >= lurdAN.x) && (a <= lurdAN.z )) )
				{
					// NINCSEN kijelölve
					if( bON ? iON != i+c : true )
						continue;

					// de rajta a pointer
					pMINI[off+offFRM].pos( xyWH.a4x2[0], fxyz )->frmBRDR( xyWH.a4x2[1], gpeCLR_white, 0xf, fxyz-I4x4( xyWH.a4x2[0].MX(0), 0 )  );
					pMINI[off+offFRM].pos( xyWH.a4x2[0]+I4x2(1,0), fxyz )->print( onAN.strA4N(sSTR), gpeCLR_white );
					continue;
				}
				continue;
			}
			if( pSRC = mass.SRCfnd( pM[i+c] ) )
			if( pSRC->picID )
			{
				/// CELL PICTURES BACK GROUND -------------------
				//xyWH azonos a xyPIC[0]-val!
				aXYuvPC[2].a4x2[1].x = pSRC->picID-1;
				picBG.lzyADD( &aXYuvPC, sizeof(aXYuvPC), s = -1 );
			}



			if( !lurdAN.x || r < lurdAN.y || r > lurdAN.w )
			{
				// NINCSEN kijelölés az egész sorban
				pMINI[off+offFRM].pos( xyWH.a4x2[0], fxyz )
							->frmBRDR( xyWH.a4x2[1], c16fr, 0xf, fxyz-I4x4( xyWH.a4x2[0].MX(0), 0 )  );

				pSRC->SRCmini(
									pMINI+off, //pMINI + off + offFRM,

									xyWH.a4x2[0],

									fxyz.x,fxyz.y,

									CRSfrm.z, fxyz.z,				// fz zz
									*this,
									c16bg, //gpeCLR_blue2,
									gpeCLR_blue2,
									false
							);

				if( bON ? iON == i+c : false )	// rajta a pointer
					pMINI[off+offFRM].pos( xyWH.a4x2[0]+I4x2(1,0), fxyz )->print( onAN.strA4N(sSTR), gpeCLR_white );

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

			//pMINI[off+offFRM].pos( xyWH.a4x2[0], fxyz )->frm( xyWH.a4x2[1], c16fr, 0xf, fxyz-I4x4( xyWH.a4x2[0], 0 )  );
			pMINI[off+offFRM].pos( xyWH.a4x2[0], fxyz )
							->frmBRDR( xyWH.a4x2[1], c16fr, 0xf, fxyz-I4x4( xyWH.a4x2[0].MX(0), 0 )  );
			/*pSRC->SRCfrm(
							pMINI + off + offFRM,

							xyWH,

							c16fr,
							fxyz

						);*/
			if( a == lurdAN.x && r == lurdAN.y )
				pMINI[off+offFRM].pos( xyWH.a4x2[0]+I4x2(1,0), fxyz )->print( lurdAN.a4x2[0].strA4N(sSTR), c16fr );
			else if( a == lurdAN.z && r == lurdAN.w )
				pMINI[off+offFRM].pos( xyWH.a4x2[0]+I4x2(1,0), fxyz )->print( lurdAN.a4x2[1].strA4N(sSTR), c16fr );
			else if( bON ? iON == i+c : false )
				pMINI[off+offFRM].pos( xyWH.a4x2[0]+I4x2(1,0), fxyz )->print( onAN.strA4N(sSTR), gpeCLR_white );

			pSRC->SRCmini(
							pMINI + off, xyWH.a4x2[0], //pMINI + off + offFRM,

							fxyz.x,fxyz.y,

							CRSfrm.z, fxyz.z,				// fz zz

							*this,
							c16bg, //c16fr,
							c16ch,
							bNoMini
						);
		}
	}
}
