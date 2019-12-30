#include "gpccrs.h"
#include "gpcSRC.h"

///------------------------------
///
/// 		miniRDY
///
///------------------------------
void gpcCRS::miniRDY( gpcWIN& win, U1 iDIV, gpcMASS& mass, gpcPIC* pPIC, SDL_Renderer* pRNDR )
{
	if( miniOFF( pPIC, pRNDR ) )
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
	gpmZn( pMINI, nMINI );
	if( bESC )
		return;

	I4x4 xyWH = 0;
	gpcMAP* pMAP = &mass.mapCR;
	if( !pMAP )
		return;


	// nem a 0 cella a vezér hanem az 1-es
	//if( lurd.a4x2[1].x > ie )

	I4x4	selAN0AN1( selANIN[0].a4x2[0], selANIN[1].a4x2[0] ),
			lurdAN = selAN0AN1.lurd();
	U4x2	spcZN = lurdAN.a4x2[1] - U4x2(1,0);
	U4x4	mCR, dim;

	U4	*pM = pMAP->MAPalloc( spcZN, mCR, id ),//pMAP->pMAP,
		*pC = pMAP->pCOL,
		*pR = pMAP->pROW, i, ie = pC-pM, c, r;
	if( id )
	{
		i = pMAP->mapZN44.a4x2[1].sum()*(U4)id;
		pC += i;
		pR += i;
	}


//	bool bNoMini = false;

	for( i = 0; i < ie; i++ )
	{
		if( !pM[i] )
			continue;
		c = i%pMAP->mapZN44.z;
		r = i/pMAP->mapZN44.z;
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
	for( U4 c = 0; c < pMAP->mapZN44.x; c++ )
		xyWH.z += pC[c];

	for( U4 r = 0; r < pMAP->mapZN44.y; r++ )
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

	gpcSRC	//tmp,
			//*pEDIT = NULL,
			*pSRC = NULL; //,
			//*pS2 = NULL;

	xyWH.y = CRSfrm.y;
	gpeCLR	c16bg = gpeCLR_blue,
			c16fr = gpeCLR_blue2,
			c16ch = gpeCLR_blue2;

	for( U4 r = 0; r < pMAP->mapZN44.y; xyWH.y += pR[r], r++ )
	{
		if( xyWH.y >= CRSfrm.w )
			break;
		if( !pR[r] )
			continue;
		if( (xyWH.y + (int)pR[r]) < 0 )
			continue;

		xyWH.x = CRSfrm.x;
		for( U4 c = 0; c < pMAP->mapZN44.x; xyWH.x += pC[c], c++ )
		{
			if( xyWH.x >= CRSfrm.z )
				break;
			if( !pC[c] )
				continue;

			if( xyWH.x +  pC[c] < 0 )
				continue;

			i = c + r*pMAP->mapZN44.z;
			if( !pM[i] )
				continue;

			pSRC = mass.SRCfnd( pM[i] );

			bNoMini = false;
			if( !lurdAN.x )
				c16fr = c16ch = gpeCLR_blue2;
			else if(
							( c+1 >= lurdAN.x	&& r >= lurdAN.y )
						&&	( c+1 <= lurdAN.z	&& r <= lurdAN.w )
					)
			{
				bNoMini = true;
				c16fr = bED ? gpeCLR_green2 : gpeCLR_cyan;
				c16ch = bED ? gpeCLR_cyan : gpeCLR_blue2;
			} else {
				c16fr = c16ch = gpeCLR_blue2;
			}

			pSRC->CRSmini(
								pMINI, aCRS, xyWH,

								min(CRSfrm.z, xyWH.x+(int)pC[c]),
								min(CRSfrm.w, xyWH.y+(int)pR[r]),

								CRSfrm.z,
								//gpaC64,
								*this,
								c16bg, c16fr, c16ch,
								bNoMini
							);

		}
	}
}
