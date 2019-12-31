#include "gpccrs.h"
#include "gpcSRC.h"

///------------------------------
///
/// 		miniRDYgl
///
///------------------------------
void gpcCRS::miniRDYgl( gpcWIN& win, U1 iDIV, gpcMASS& mass, gpcPIC* pPIC, SDL_Renderer* pRNDR )
{
	if( miniOFFgl( pPIC, pRNDR, win.winSIZ.a4x2[0] ) )
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
	SDL_Rect div = win.wDIV( id );
	U4 off = (div.x ? pPIC->txWH.x/8: 0) + (div.y ? pPIC->txWH.a4x2[0].area()/10: 0) ;

	for( U4 h = 0; h < CRSfrm.w; h++ )
		gpmZn( pMINI + off + h*pPIC->txWH.x, CRSfrm.z );
	if( bESC )
		return;

	gpcMAP* pMAP = &mass.mapCR;
	if( !pMAP )
		return;



	I4x4	xyWH = 0,
			lurdAN = I4x4( selANIN[0].a4x2[0], selANIN[1].a4x2[0] ).lurd();

	U4x2	spcZN = lurdAN.a4x2[1] - U4x2(1,0);
	U4x4	mCR, dim;

	U4	*pM = pMAP->MAPalloc( spcZN, mCR, id ),
		*pC = pMAP->pCOL,
		*pR = pMAP->pROW,
		i, ie = pC-pM, c, r, z = mCR.z;

	if( id )
	{
		i = mCR.a4x2[1].sum()*(U4)id;
		pC += i;
		pR += i;
	}

	for( i = 0; i < ie; i++ )
	{
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
	for( U4 c = 0; c < mCR.x; c++ )
		xyWH.z += pC[c];

	for( U4 r = 0; r < mCR.y; r++ )
		xyWH.w += pR[r];

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

	//gpcSRC *pSRC = NULL;

	xyWH.y = CRSfrm.y;
	gpeCLR	c16bg = gpeCLR_blue,
			c16fr = gpeCLR_blue2,
			c16ch = gpeCLR_blue2;


	for( U4 r = 0; r < mCR.y; xyWH.y += pR[r], r++ )
	{
		if( xyWH.y >= CRSfrm.w )
			break;
		if( !pR[r] )
			continue;
		if( (xyWH.y + (int)pR[r]) < 0 )
			continue;

		xyWH.x = CRSfrm.x;
		i = r*z;
		ie = min(CRSfrm.w, xyWH.y+(int)pR[r]);
		for( U4 c = 0; c < mCR.x; xyWH.x += pC[c], c++ )
		{
			if( xyWH.x >= CRSfrm.z )
				break;
			if( !pC[c] )
				continue;

			if( xyWH.x + pC[c] < 0 )
				continue;

			//i = c + r*z;
			if( !pM[i+c] )
				continue;

			if( !lurdAN.x || r < lurdAN.y || r > lurdAN.w )
			{
				mass.SRCfnd( pM[i+c] )->CRSmini(
												pMINI+off, xyWH,

												min(CRSfrm.z, xyWH.x+(int)pC[c]),	// fx
												ie,									// fy

												CRSfrm.z, pPIC->txWH.x,				// fz zz
												*this,
												c16bg, gpeCLR_blue2, gpeCLR_blue2,
												false
											);
				continue;
			}

			bNoMini = (c+1 >= lurdAN.x) && (c+1 <= lurdAN.z );
			if( bNoMini )
			{
				c16fr = bED ? gpeCLR_green2 : gpeCLR_cyan;
				c16ch = bED ? gpeCLR_cyan : gpeCLR_blue2;
			} else
				c16fr = c16ch = gpeCLR_blue2;



			mass.SRCfnd( pM[i+c] )->CRSmini(
											pMINI + off, xyWH,

											min(CRSfrm.z, xyWH.x+(int)pC[c]),
											ie,

											CRSfrm.z, pPIC->txWH.x,				// fz zz
											*this,
											c16bg, c16fr, c16ch,
											bNoMini
										);

		}
	}
}
