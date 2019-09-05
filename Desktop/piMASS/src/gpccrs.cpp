
#include "gpccrs.h"
#include "gpcSRC.h"

I4x4 gpcCRS::srcXYCR( gpcWIN& win, U1 iDIV, gpcMASS& mass, const I4x2& _xy )
{
	SDL_Rect div = win.wDIV( iDIV );
	if( div.w < 1 )
	{
		div = win.wDIV( iDIV );
		return 0;
	}
	I4x2 cr( div.w/CRSfrm.z, div.h/CRSfrm.w );
	I4x4 o = CRSfrm & cr;
	if( !this )
		return o;

	I4x2 xy = _xy - o.a4x2[0] - I4x2(div.x,div.y);
	o = I4x4( xy, xy/cr );
	if( gpcMAP* pMAP = &mass.mapCR )
	{
		U4	*pC = pMAP->pCOL,
			*pR = pMAP->pROW;

		scnAN.null();
		for( scnAN.x = 0; scnAN.x < pMAP->map44.x; scnAN.x++ )
		{
			scnAN.z += pC[scnAN.x];
			if( o.z >= scnAN.z )
				continue;

			scnIN.z = pC[scnAN.x]*cr.x;
			scnIN.x = xy.x - (scnAN.z*cr.x - scnIN.z);
			break;
		}
		if( scnAN.x >= pMAP->map44.x )
		{
			scnIN.z = cr.x*9;
			scnIN.x = xy.x - (scnAN.z*cr.x);
			scnAN.x = pMAP->map44.x + 1 + scnIN.x/scnIN.z;
			scnIN.x %= scnIN.z;
		} else
			scnAN.x++; // ALF 'A' == 1

		for( scnAN.y = 0; scnAN.y < pMAP->map44.y; scnAN.y++ )
		{
			scnAN.w += pR[scnAN.y];
			if( o.w >= scnAN.w )
				continue;

			scnIN.w = pR[scnAN.y]*cr.y;
			scnIN.y = xy.y - (scnAN.w*cr.y - scnIN.w);
			break;
		}
		if( scnAN.y >= pMAP->map44.y )
		{
			scnIN.w = cr.y;
			scnIN.y = xy.y - (scnAN.w*cr.y);
			scnAN.y = pMAP->map44.y + scnIN.y/scnIN.w;
			scnIN.y %= scnIN.w;
		}
	}

	return o;

}
void gpcCRS::CRSsel( gpcWIN& win, U1 iDIV, gpcMASS& mass, bool bSH )
{
	SDL_Rect div = win.wDIV( iDIV );
	I4x2 cr( div.w/CRSfrm.z, div.h/CRSfrm.w );

	gpcMAP* pMAP = &mass.mapCR;
	if( !pMAP )
		return;

	U4	*pM = pMAP->pMAP,
		an = scnAN.a4x2[0]*I4x2( 1, pMAP->map44.z ) - 1;
	if( !pM[an] )
		return;

	U4 xFND = pM[an];
	gpcSRC* pSRC = mass.SRCfnd( xFND );
	if( !pSRC )
		return;

	selANCR[1].a4x2[0] = scnAN.a4x2[0];		//AN
	selANCR[1].a4x2[1] = scnIN.a4x2[0]/cr;	//IN
	anSTR[1] = pSRC->CRSminiCR( selANCR[1].a4x2[1] );

	apSRC[1] = pSRC;

	if( bSH )
	{
		U4	t1 = selANCR[1].a4x2[0]*I4x2(1,pMAP->map44.z),
			t0 = selANCR[0].a4x2[0]*I4x2(1,pMAP->map44.z);
		if( t0 > t1  )
		{
			I4x4 tmp = selANCR[0];
			selANCR[0] = selANCR[1];
			selANCR[1] = tmp;

			apSRC[1] = apSRC[0];
			apSRC[0] = pSRC;

			t0 = anSTR[1];
			anSTR[1] = anSTR[0];
			anSTR[0] = t0;
			return;
		}

		if( anSTR[1] < anSTR[0] )
		{
			t0 = anSTR[1];
			anSTR[1] = anSTR[0];
			anSTR[0] = t0;
		}

		return; // ha le van nyomva a shift akkor meg akarjuk örizni a sel[0]-t.
	}

	selANCR[0] = selANCR[1];
	apSRC[0] = apSRC[1];
	anSTR[0] = anSTR[1];

}

gpcCRS::gpcCRS( gpcWIN& win, I4 mag0 )
{
	//ctor
	gpmCLR;
	/*SDL_Rect div = win.wDIV(0);
	CRSfrm.z = div.w/win.chrPIC.w;
	CRSfrm.w = div.h/win.chrPIC.h;*/
	CRSfrm.a4x2[1] = win.winFRM( 0 );
	if( mag0 < 2 )
		return;

	CRSfrm.z *= mag0;
	CRSfrm.w *= mag0;
}

gpcCRS::~gpcCRS()
{
	//dtor
}

/*gpcCRS::gpcCRS(const gpcCRS& other)
{
	//copy ctor
}

gpcCRS& gpcCRS::operator=(const gpcCRS& rhs)
{
	if (this == &rhs) return *this; // handle self assignment
	//assignment operator
	return *this;
}*/
U1 gpsHUN[] =
" A       E   I  "
"UOoO  O   U U   "
" a       e   i  "
"uUuo  o   u u   "
" '       '   '  "
":\"\"'  :   ' :   "
" '       '   '  "
":\"\"'  :   ' :   "
"0123456789abcdef";

bool gpcCRS::miniOFF( void )
{
	if( nMINI == CRSfrm.a4x2[1].area() )
		return !nMINI;

	gpmDELary( pMINI );
	nMINI = CRSfrm.z*CRSfrm.w;
	if( !nMINI )
		return true;	// nincsen mérete ki lett kapcsolva?

	pCRS = pMINI = new U1x4[nMINI];
	gpmZn( pMINI, nMINI );
	return false; // resized mini, print new
}
U4 gpaC64[] = {
	0, 			0xffffffff,
	0xff880000,	0xffaaffee,
	0xffcc44cc, 0xff00cc55,
	0xff0000aa, 0xffeeee77,
	0xffdd8855, 0xff664400,
	0xffff7777, 0x33333333,
	0xff777777, 0xffaaff66,
	0xff0088ff, 0xffbbbbbb,
};
bool gpcCRS::miniDRW( gpcWIN& win, U1 iDIV )
{
	SDL_Rect src = win.chrPIC, div = win.wDIV( iDIV ), dst;
	if( this ? bESC : true )
	{
		SDL_FillRect( win.pSRFwin, &div, gpaC64[14] ); // 0x000000AA );
		return false;
	}

	dst.x = div.x,
	dst.y = div.y;

	if( CRSfrm.x > 0 )
	{
		dst.w = (CRSfrm.x*div.w)/CRSfrm.z;
		dst.h = div.h;
		SDL_FillRect( win.pSRFwin, &dst, gpaC64[14] );
	}

	if( CRSfrm.y > 0 )
	{
		dst.h = (CRSfrm.y*div.h)/CRSfrm.w;
		dst.w = div.w;
		SDL_FillRect( win.pSRFwin, &dst, gpaC64[14] );
	}

	if( nMINI != CRSfrm.a4x2[1].area() )
	{
		gpmDELary( pMINI );
		nMINI = CRSfrm.a4x2[1].area();
		if( !nMINI )
			return false;	// nincsen mérete ki lett kapcsolva?

		pCRS = pMINI = new U1x4[nMINI];
		gpmZn( pMINI, nMINI );
		return true; // resized mini, print new
	}

	dst = div;

	dst.x = CRSfrm.x > 0 ? (CRSfrm.x*div.w)/CRSfrm.z : 0;
	dst.y = CRSfrm.y > 0 ? (CRSfrm.y*div.h)/CRSfrm.w : 0;
	dst.w -= dst.x;
	dst.h -= dst.y;
	dst.x += div.x;
	dst.y += div.y;

	SDL_FillRect( win.pSRFwin, &dst, gpaC64[6] ); // 0x000000AA );

	dst.w = div.w/CRSfrm.z;
	dst.h = div.h/CRSfrm.w;

	U4 cx = src.w*8, cy = src.h*32, scx, scy;

	U1 c,d, cc;
	if( dst.w != src.w || dst.h != src.h )
	{
		for( U4 i = 0; i < nMINI; i++ )
		{
			if( pMINI[i].y )
			{
				dst.x = (i%CRSfrm.z)*dst.w + div.x;
				dst.y = (i/CRSfrm.z)*dst.h + div.y;
				c = pMINI[i].y+0xb0;
				cc = pMINI[i].x;
				scx = (cc%gpeCLR_violet)*cx;
				scy = (cc/gpeCLR_violet)*cy;
				src.x = (c%8)*src.w + scx;
				src.y = (c/8)*src.h + scy;

				SDL_BlitScaled( win.pSRFchar, &src, win.pSRFwin, &dst );
			}

			c = pMINI[i].w;
			cc = pMINI[i].z;
			scx = (cc%gpeCLR_violet)*cx;
			scy = (cc/gpeCLR_violet)*cy;
			if( c )
			{
				dst.x = (i%CRSfrm.z)*dst.w + div.x;
				dst.y = (i/CRSfrm.z)*dst.h + div.y;

				if( c > 0x60 )
				{
					d = gpsHUN[c-0x60]-' ';
					c = gpsHUN[c-0x20]-' '+0x60;
					if( d >= 'a'-' ' && d <= 'z'-' ' )
						c += 8;
					src.x = (d%8)*src.w + scx;
					src.y = (d/8)*src.h + scy;
					SDL_BlitScaled( win.pSRFchar, &src, win.pSRFwin, &dst );

				}

				src.x = (c%8)*src.w + scx;
				src.y = (c/8)*src.h + scy;

				SDL_BlitScaled( win.pSRFchar, &src, win.pSRFwin, &dst );
			}




		}
		return false;
	}

	for( U4 i = 0; i < nMINI; i++ )
	{
		if( c = pMINI[i].y )
		{
			dst.x = (i%CRSfrm.z)*dst.w + div.x;
			dst.y = (i/CRSfrm.z)*dst.h + div.y;
			c = pMINI[i].y+0xb0;
			cc = pMINI[i].x;
			scx = (cc%gpeCLR_violet)*cx;
			scy = (cc/gpeCLR_violet)*cy;
			src.x = (c%8)*src.w + scx;
			src.y = (c/8)*src.h + scy;
			SDL_BlitSurface( win.pSRFchar, &src, win.pSRFwin, &dst );

		}


		c = pMINI[i].w;
		cc = pMINI[i].z;
		scx = (cc%gpeCLR_violet)*cx;
		scy = (cc/gpeCLR_violet)*cy;
		if( c )
		{
			dst.x = (i%CRSfrm.z)*dst.w + div.x;
			dst.y = (i/CRSfrm.z)*dst.h + div.y;

			if( c > 0x60 )
			{
				d = gpsHUN[c-0x60]-' ';
				c = gpsHUN[c-0x20]-' '+0x60;
				if( d >= 'a'-' ' && d <= 'z'-' ' )
					c += 8;
				src.x = (d%8)*src.w + scx;
				src.y = (d/8)*src.h + scy;
				SDL_BlitScaled( win.pSRFchar, &src, win.pSRFwin, &dst );

			}

			src.x = (c%8)*src.w + scx;
			src.y = (c/8)*src.h + scy;

			SDL_BlitSurface( win.pSRFchar, &src, win.pSRFwin, &dst );
		}


	}
	return false;
}
void gpcCRS::miniINS( U1* pC, U1* pM, U1* pB )
{
	if( miniOFF() )
		return;

	if( !pCRS )
		pCRS = pMINI;

	for( U1 i = 0; pB+i < pM; i++ )
	{
		pMINI[i].w = pB[i]-' ';
	}
	if( pC > pM )
	for( U1 nx; pM < pC; pM++ )
	{
		if( *pM < ' ' )
			continue;

		if( *pM < 0x80 )
		{
			pCRS->w = *pM - ' ';
			pCRS++;
			continue;
		}
		nx = *pM;
		pM++;
		if( !*pM )
			break;

		pCRS->w = *pM - ' ';
		pCRS->w += (nx&4)>>2;
		pCRS++;
	}

}
/// MINIrdy
void gpcCRS::miniRDY( gpcWIN& win, U1 iDIV, gpcMASS& mass, U1* pE, U1* pB )
{
	if( miniOFF() )
		return;
	U4 xFND;
	U4x4 dim;
	bESC = false;
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
	{
		return;
	}
	I4x4 miniALL = 0;
	if( gpcMAP* pMAP = &mass.mapCR )
	{
		U4	*pM = pMAP->pMAP,
			*pC = pMAP->pCOL,
			*pR = pMAP->pROW, i, ie;
		gpcSRC* pEDIT = NULL, *pSRC;
		gpmZn( pC, pMAP->map44.a4x2[1].sum() );
		for( i = 0, ie = pC-pM; i < ie; i++ )
		{
			if( !pM[i] )
				continue;

			xFND = pM[i];
			pSRC = mass.SRCfnd( xFND );
			dim = pSRC->CRSdim( aCRS );
            if( pC[pSRC->spc.x] < dim.x )
				pC[pSRC->spc.x] = dim.x;
			if( pR[pSRC->spc.y] < dim.y )
				pR[pSRC->spc.y] = dim.y;

		}
		for( U4 c = 0; c < pMAP->map44.x; c++ )
		{
			miniALL.z += pC[c];
		}
		for( U4 r = 0; r < pMAP->map44.y; r++ )
		{
			miniALL.w += pR[r];
		}
		if( CRSfrm.x+miniALL.z < 1 )
		{
			CRSfrm.x = -miniALL.z;
			bESC = true;
		}
		if( CRSfrm.y+miniALL.w < 1 )
		{
			CRSfrm.y = -miniALL.w;
			bESC = true;
		}
		if( bESC )
		{
			gpmZn( pMINI, nMINI );
			return;
		}
		gpmZn( pMINI, nMINI );
		miniALL.y = CRSfrm.y;
		gpeCLR	c16bg = gpeCLR_blue,
				c16fr = gpeCLR_blue2,
				c16ch = gpeCLR_blue2;
		I4x4 sel01( selANCR[0].a4x2[0], selANCR[1].a4x2[0] );

		for( U4 r = 0; r < pMAP->map44.y; miniALL.y += pR[r], r++ )
		{
			if( miniALL.y >= CRSfrm.w )
				break;
            if( !pR[r] )
				continue;
			if( (miniALL.y + (int)pR[r]) < 0 )
				continue;

			miniALL.x = CRSfrm.x;
			for( U4 c = 0; c < pMAP->map44.x; miniALL.x += pC[c], c++ )
			{
				if( miniALL.x >= CRSfrm.z )
					break;
				if( !pC[c] )
					continue;

				if( miniALL.x +  pC[c] < 0 )
					continue;

				i = c + r*pMAP->map44.w;
				if( !pM[i] )
					continue;

				xFND = pM[i];
				pSRC = mass.SRCfnd( xFND );

				if( pSRC == apSRC[0] )
				{
					if( pB < pE )
					if( pSRC == apSRC[1] )
					{
						I4	nSUB = anSTR[1] - anSTR[0],
							nSTR = pE-pB,
							nOL = pSRC->nL,
							nNEW = gpmPAD( nOL+nSTR + 1, 0x10 );


						// több karakter írunk át
						U1	*pOA	= pSRC->nA ? pSRC->pA : NULL,
							*pRIG	= pOA + anSTR[1],
							*pRIGe	= pOA + nOL,
							*pLFT	= (pSRC->pA = new U1[nNEW]) + anSTR[0];

						gpmMEMCPY( pSRC->pA, pOA, anSTR[0] );

						for( ; pB < pE; pB++ )
						{
							switch( *pB )
							{
								case '\b':
									if( pLFT > pSRC->pA )
									{
										pLFT--;
										if( pLFT[0] == '\n' )
										if( pLFT >= pSRC->pA )
										if( pLFT[-1] == '\r' )
										{
											pLFT--;
											continue;
										}
									}
									continue;
								case 0x7f:
									pB++;
									if( pB < pE )	// ha még van a bufferban abbol deletézünk
										continue;

									if( pRIG < pRIGe )
										pRIG++;	// ha nincsen akkor a jobb oldalbol
									continue;
							}

							*pLFT = *pB;
							pLFT++;

						}
						anSTR[1] = anSTR[0] = pLFT-pSRC->pA;
						if( pRIG < pRIGe )
						{
							gpmMEMCPY( pLFT, pRIG, pRIGe-pRIG );
							pLFT += pRIGe-pRIG;
						}
						pSRC->nL = pLFT-pSRC->pA;
						pSRC->nA = nNEW;
						pSRC->updt();
						*pLFT = 0;

						gpmDELary(pOA);
						pSRC->hd(mass);
					}
				}
				c16fr = gpeCLR_blue2;
				if( c+1 >= sel01.x	&& r >= sel01.y )
				if( c+1 <= sel01.z	&& r <= sel01.w )
					c16fr = gpeCLR_cyan;
				pSRC->CRSmini(
									pMINI, aCRS, miniALL,
									min(CRSfrm.z, miniALL.x+(int)pC[c]), min(CRSfrm.w, miniALL.y+(int)pR[r]),
									CRSfrm.z,
									gpaC64,
									*this,
									c16bg, c16fr, c16ch
								);

			}
		}

	}
}
