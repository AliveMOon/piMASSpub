#include "gpccrs.h"



gpcCRS::gpcCRS( gpcWIN& win, I4 mag0 )
{
	//ctor
	gpmCLR;
	SDL_Rect div = win.wDIV(0);
	CRSfrm.z = div.w/win.chrPIC.w;
	CRSfrm.w = div.h/win.chrPIC.h;
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
	if( nMINI == CRSfrm.area_zw() )
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
	U4 cx = src.x;

	if( nMINI != CRSfrm.area_zw() )
	{
		gpmDELary( pMINI );
		nMINI = CRSfrm.area_zw();
		if( !nMINI )
			return false;	// nincsen mérete ki lett kapcsolva?

		pCRS = pMINI = new U1x4[nMINI];
		gpmZn( pMINI, nMINI );
		return true; // resized mini, print new
	}


	SDL_FillRect( win.pSRFwin, &div, gpaC64[6] ); // 0x000000AA );

	dst.w = div.w/CRSfrm.z;
	dst.h = div.h/CRSfrm.w;


	U1 c,d;
	if( dst.w != src.w || dst.h != src.h )
	{
		for( U4 i = 0; i < nMINI; i++ )
		{
			c = pMINI[i].w;
			if( !c )
				continue;
			dst.x = (i%CRSfrm.z)*dst.w + div.x;
			dst.y = (i/CRSfrm.z)*dst.h + div.y;

			if( c > 0x60 )
			{
				d = gpsHUN[c-0x60]-' ';
				c = gpsHUN[c-0x20]-' '+0x60;
				if( d >= 'a'-' ' && d <= 'z'-' ' )
					c += 8;
				src.x = (d%cx)*src.w;
				src.y = (d/cx)*src.h;
				SDL_BlitScaled( win.pSRFchar, &src, win.pSRFwin, &dst );

			}

			src.x = (c%cx)*src.w;
			src.y = (c/cx)*src.h;

			SDL_BlitScaled( win.pSRFchar, &src, win.pSRFwin, &dst );
		}
		return false;
	}

	for( U4 i = 0; i < nMINI; i++ )
	{
		c = pMINI[i].w;
		if( !c )
			continue;
		dst.x = (i%CRSfrm.z)*dst.w + div.x;
		dst.y = (i/CRSfrm.z)*dst.h + div.y;

		if( c > 0x60 )
		{
			d = gpsHUN[c-0x60]-' ';
			c = gpsHUN[c-0x20]-' '+0x60;
			if( d >= 'a'-' ' && d <= 'z'-' ' )
				c += 8;
			src.x = (d%cx)*src.w;
			src.y = (d/cx)*src.h;
			SDL_BlitScaled( win.pSRFchar, &src, win.pSRFwin, &dst );

		}

		src.x = (c%cx)*src.w;
		src.y = (c/cx)*src.h;

		SDL_BlitSurface( win.pSRFchar, &src, win.pSRFwin, &dst );
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
	bool bESC = false;
	if( CRSfrm.x > CRSfrm.z )
	{
		CRSfrm.x = CRSfrm.z;
		bESC = true;
	}
	if( CRSfrm.y > CRSfrm.w )
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

		miniALL.y = CRSfrm.y;
		for( U4 r = 0; r < pMAP->map44.y; miniALL.y += pR[r], r++ )
		{
			if( miniALL.y >= CRSfrm.w )
				break;

			if( (miniALL.y + (int)pR[r]) < 0 )
			{
				continue;
			}

			miniALL.x = CRSfrm.x;
			for( U4 c = 0; c < pMAP->map44.x; miniALL.x += pC[c], c++ )
			{
				if( miniALL.x >= CRSfrm.z )
					break;

				if( miniALL.x +  pC[c] < 0 )
					continue;

				i = c + r*pMAP->map44.w;
				if( !pM[i] )
					continue;

				xFND = pM[i];
				pSRC = mass.SRCfnd( xFND );
				pSRC->CRSmini( 	pMINI, aCRS, miniALL,
								min(CRSfrm.z, miniALL.x+pC[c]), min(CRSfrm.w, miniALL.y+pR[r]),
								CRSfrm.z,
								gpaC64 );

			}
		}

	}
}
