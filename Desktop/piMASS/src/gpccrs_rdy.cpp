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
void gpcCRS::miniRDYgl( gpcWIN& win, gpcMASS& mass, gpcPIC* pPIC, SDL_Renderer* pRNDR )
{
	if( miniLOCK( pPIC, pRNDR, win.wDIVcrALL() ) )
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
	I4x2 div = win.wDIVcr( id ).a4x2[0];
	I4x4 fxyz;
	fxyz.z = pPIC->txWH.x;

	U4	//zz = pPIC->txWH.x,
		off = 	  (div.x ? fxyz.z/2: 0)
				+ (div.y ? pPIC->txWH.a4x2[0].area()/4: 0),
		offFRM = pPIC->txWH.a4x2[0].area()/2;

	for( U4 h = 0; h < CRSfrm.w; h++ )
	{
		fxyz.w = h*fxyz.z;
		gpmZn( pMINI + off + fxyz.w, CRSfrm.z );
		gpmZn( pMINI + off + fxyz.w + offFRM, CRSfrm.z );
	}

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

	xyWH.y = CRSfrm.y;
	gpeCLR	c16bg = gpeCLR_blue,
			c16fr = gpeCLR_blue2,
			c16ch = gpeCLR_blue2;

	gpcSRC* pSRC;
	U1 sSTR[0x20];
	bool bON = id == win.onDIV.x;
	U4 iON = scnZN.a4x2[0]*I4x2(1,z);
	I4x2 onAN = scnZN.a4x2[0]+I4x2(1,0);
	for( U4 r = 0,a; r < mCR.y; xyWH.y += pR[r], r++ )
	{
		if( xyWH.y >= CRSfrm.w )
			break;
		if( !pR[r] )
			continue;
		if( (xyWH.y + (int)pR[r]) < 0 )
			continue;

		xyWH.x = CRSfrm.x;
		i = r*z;
		fxyz.y = min(CRSfrm.w, xyWH.y+(int)pR[r]);
		for( U4 c = 0; c < mCR.x; xyWH.x += pC[c], c++ )
		{
			if( xyWH.x >= CRSfrm.z )
				break;
			if( !pC[c] )
				continue;

			if( xyWH.x + pC[c] < 0 )
				continue;

			fxyz.x = min(CRSfrm.z, xyWH.x+(int)pC[c]);
			a = c+1;
			c16fr = bON&(iON == i+c) ? gpeCLR_white : gpeCLR_blue2;
			c16ch = gpeCLR_blue2;

			if( !pM[i+c] )
			{
				if( !lurdAN.x || r < lurdAN.y || r > lurdAN.w || !((a >= lurdAN.x) && (a <= lurdAN.z )) )
				{
					if( bON ? iON != i+c : true )
					continue;

					pMINI[off+offFRM].pos( xyWH.a4x2[0], fxyz )->frm( xyWH.a4x2[1], gpeCLR_white, 0xf, fxyz-I4x4( xyWH.a4x2[0], 0 )  );
					pMINI[off+offFRM].pos( xyWH.a4x2[0]+I4x2(1,0), fxyz )->print( onAN.strA4N(sSTR), gpeCLR_white );
					continue;
				}

				pMINI[off+offFRM].pos( xyWH.a4x2[0], fxyz )->frm( xyWH.a4x2[1], c16fr, 0xf, fxyz-I4x4( xyWH.a4x2[0], 0 )  );
				if( a == lurdAN.x && r == lurdAN.y )
					pMINI[off+offFRM].pos( xyWH.a4x2[0]+I4x2(1,0), fxyz )->print( lurdAN.a4x2[0].strA4N(sSTR), c16fr );
				else if( a == lurdAN.z && r == lurdAN.w )
					pMINI[off+offFRM].pos( xyWH.a4x2[0]+I4x2(1,0), fxyz )->print( lurdAN.a4x2[1].strA4N(sSTR), c16fr );
				continue;
			}
			pSRC = mass.SRCfnd( pM[i+c] );

			if( !lurdAN.x || r < lurdAN.y || r > lurdAN.w )
			{
				pSRC->SRCfrm(
								pMINI + off + offFRM,

								xyWH,

								c16fr,
								fxyz
						);
				pSRC->SRCmini(
									pMINI+off, //pMINI + off + offFRM,

									xyWH,

									fxyz.x,fxyz.y,

									CRSfrm.z, fxyz.z,				// fz zz
									*this,
									c16bg, //gpeCLR_blue2,
									gpeCLR_blue2,
									false
							);
				if( bON ? iON == i+c : false )
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

			pSRC->SRCfrm(
							pMINI + off + offFRM,

							xyWH,

							c16fr,
							fxyz

						);
			if( a == lurdAN.x && r == lurdAN.y )
				pMINI[off+offFRM].pos( xyWH.a4x2[0]+I4x2(1,0), fxyz )->print( lurdAN.a4x2[0].strA4N(sSTR), c16fr );
			else if( a == lurdAN.z && r == lurdAN.w )
				pMINI[off+offFRM].pos( xyWH.a4x2[0]+I4x2(1,0), fxyz )->print( lurdAN.a4x2[1].strA4N(sSTR), c16fr );
			else if( bON ? iON == i+c : false )
				pMINI[off+offFRM].pos( xyWH.a4x2[0]+I4x2(1,0), fxyz )->print( onAN.strA4N(sSTR), gpeCLR_white );

			pSRC->SRCmini(
							pMINI + off, xyWH, //pMINI + off + offFRM,

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
