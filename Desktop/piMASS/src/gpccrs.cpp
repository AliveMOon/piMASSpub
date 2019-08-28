#include "gpccrs.h"



gpcCRS::gpcCRS( gpcWIN& win )
{
	//ctor
	gpmCLR;
	SDL_Rect div = win.wDIV(0);
	frm.z = div.w/win.chr.w;
	frm.w = div.h/win.chr.h;

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

bool gpcCRS::MINI_off( void )
{
	if( nMINI == frm.z*frm.w )
		return !nMINI;

	gpmDELary( pMINI );
	nMINI = frm.z*frm.w;
	if( !nMINI )
		return true;	// nincsen mérete ki lett kapcsolva?

	pCRS = pMINI = new U1x4[nMINI];
	gpmZn( pMINI, nMINI );
	return false; // resized mini, print new
}

bool gpcCRS::MINI_draw( gpcWIN& win, U1 nDIV )
{
	SDL_Rect src = win.chr, div = win.wDIV( nDIV ), dst;
	U4 cx = src.x;

	if( nMINI != frm.z*frm.w )
	{
		gpmDELary( pMINI );
		nMINI = frm.z*frm.w;
		if( !nMINI )
			return false;	// nincsen mérete ki lett kapcsolva?

		pCRS = pMINI = new U1x4[nMINI];
		gpmZn( pMINI, nMINI );
		return true; // resized mini, print new
	}


	SDL_FillRect( win.pSRFwin, &div, 0x00000000 );

	dst.w = div.w/frm.z;
	dst.h = div.h/frm.w;


	U1 c,d;
	if( dst.w != src.w || dst.h != src.h )
	{
		for( U4 i = 0; i < nMINI; i++ )
		{
			c = pMINI[i].w;
			if( !c )
				continue;
			dst.x = (i%frm.z)*dst.w + div.x;
			dst.y = (i/frm.z)*dst.h + div.y;

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
		dst.x = (i%frm.z)*dst.w + div.x;
		dst.y = (i/frm.z)*dst.h + div.y;

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
void gpcCRS::MINI_ins( U1* pC, U1* pM, U1* pB )
{
	if( MINI_off() )
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
