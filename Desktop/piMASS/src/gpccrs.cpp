
#include "gpccrs.h"
#include "gpcSRC.h"

I4x4 gpcCRS::srcXYCR( gpcWIN& win, U1 iDIV, gpcMASS& mass, const I4x2& _xy )
{
	// XY - pixel
	// CR - Coll/Row
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

		scnZN.null();
		scnIN.null();
		for( scnZN.x = 0; scnZN.x < pMAP->mapCR44.z; scnZN.x++ )
		{
			scnZN.z += pC[scnZN.x];
			if( o.z >= scnZN.z )
				continue;

			scnIN.z = pC[scnZN.x]*cr.x;
			scnIN.x = xy.x - (scnZN.z*cr.x - scnIN.z);
			break;
		}
		/*if( scnZN.x >= pMAP->mapCR44.x )
		{
			scnIN.z = cr.x*gpdSRC_COLw;
			scnIN.x = xy.x - (scnZN.z*cr.x);

			scnZN.x = 	  pMAP->mapCR44.x //+ 1
						+ scnIN.x/scnIN.z;
			scnIN.x %= scnIN.z;
		} 	*/
			//else
			//scnZN.x++; // ALF 'A' == 1

		for( scnZN.y = 0; scnZN.y < pMAP->mapCR44.w; scnZN.y++ )
		{
			scnZN.w += pR[scnZN.y];
			if( o.w >= scnZN.w )
				continue;

			scnIN.w = pR[scnZN.y]*cr.y;
			scnIN.y = xy.y - (scnZN.w*cr.y - scnIN.w);
			break;
		}
		/*if( scnZN.y >= pMAP->mapCR44.y )
		{
			scnIN.w = cr.y;
			scnIN.y = xy.y - (scnZN.w*cr.y);
			scnZN.y = pMAP->mapCR44.y + scnIN.y/scnIN.w;
			scnIN.y %= scnIN.w;
		}*/
	}

	return o;

}
void gpcCRS::CRSstpCL( gpcWIN& win, gpcMASS& mass, U1 stp, bool bSH, bool bCT )
{
	// ha van shift akkor a 2. cursort mozgatja
	if( !this )
		return;

	switch(stp&0x7)
	{
		case 2:	// left
			if( selANCR[1].x < 2 )
			{
				selANCR[1].x = 1;
				break;
			}
			selANCR[1].x--;
			break;
		case 3:	// right
			selANCR[1].x++;
			break;
		case 4:	// up
			if( selANCR[1].y < 2 )
			{
				selANCR[1].y = 0;
				break;
			}
			selANCR[1].y--;
			break;
		case 5:	// down)
			selANCR[1].y++;
	}
	if( bSH )
		return;

	selANCR[0]= selANCR[1];

}
void gpcCRS::CRSstpED( gpcWIN& win, gpcMASS& mass, U1 stp, bool bSH, bool bCT )
{
	// ha van shift akkor a 2. cursort mozgatja
	if( !this )
		return;
	if( !apSRC[0] )
	{
		// ha nincsen csin
		return;
	}

	gpcSRC *pSRC = apSRC[1];
	if( pSRC == apSRC[1] )
	{
		// ugyan abban a rublikában van
		U1	*pOA	= pSRC->pA,
			*pLFT	= pOA + anSTR[0],
			*pRIG	= pOA + anSTR[1];
		switch( stp )
		{
			case 2:
				// hátra megy
				if( pRIG <= pOA )
					break;
				if( pRIG[-1] == '\a' )
					break;

				pRIG--;
				if( pRIG <= pOA )
					break;


				if(  (pRIG[-1]&0xc0) == 0xc0  )
				{
					pRIG--;
					break;
				}
				if( pRIG[0] != '\n' )
					break;

				if( pRIG <= pOA )
					break;

				if( pRIG[-1] == '\r' )
						pRIG--;
				break;
			case 3:
				if( pRIG-pOA < pSRC->nL )
				{
					if( pRIG[0] != '\r' )
					{
						if(  (pRIG[0]&0xc0) == 0xc0 )
							pRIG += 2;
						else
							pRIG++;
					}
					else if( pRIG[1] == '\n' )
						pRIG += 2;
				}
				// elölre megy
				break;
			case 4:
				// felfele egy sorral
				{

					U1 *pRe = pRIG, *pDWN = gpfUTF8left( pOA, pRe, '\n', '\a' );
					U4 x = gpfUTF8rig( pDWN, pRe );

					pRIG = pDWN;
					if( pRIG > pOA )
					if( pRIG[-1] == '\n' )
					{
						pRIG--;
						if( pRIG > pOA )
						if( pRIG[-1] == '\r' )
							pRIG--;
					}

					pRe = pRIG;
					while( pRIG > pOA )
					{
						if( pRIG[-1] == '\n' )
							break;
						if( pRIG[-1] == '\a' )
							break;
						pRIG--;
					}

					pRIG = gpfUTF8left( pOA, pRIG, '\n', '\a' );
					if( !x )
						break;
					pRIG = gpfUTF8stpX( pRIG, pRe, x );

				}
				break;
			case 5:
				// lefele egy sorral
				{
					U1* pLFT = gpfUTF8left( pOA, pRIG, '\n', '\a' );
					U4 x = gpfUTF8rig( pLFT, pRIG );
					U8 nLEN = 0;
					pRIG += gpfVAN( pRIG, (U1*)"\n", nLEN );

					if( *pRIG == '\n')
						pRIG++;

					pRIG = gpfUTF8stpX( pRIG, pOA+pSRC->nL, x );
				}
				break;
		}
		if( pRIG < pSRC->pA )
			pRIG = pSRC->pA;
		if( pRIG >= pSRC->pA+pSRC->nL )
			pRIG = pSRC->pA+pSRC->nL;

		anSTR[1] = pRIG-pSRC->pA;
		if( !bSH )
			anSTR[0] = anSTR[1];
		else if( anSTR[0] > anSTR[1] )
		{
			U4 tmp = anSTR[0];
			anSTR[0] = anSTR[1];
			anSTR[1] = tmp;
		}
	}
}
void gpcCRS::CRSsel( gpcWIN& win, gpcCRS& sCRS, gpcMASS& mass, bool bSH )
{
	if( this ? !(&sCRS): true )
		return;
	SDL_Rect sDIV = win.wDIV( sCRS.id );
	I4x2 cr( sDIV.w/sCRS.CRSfrm.z, sDIV.h/sCRS.CRSfrm.w );

	gpcMAP* pMAP = &mass.mapCR;
	if( !pMAP )
		return;

	U4	*pM = pMAP->pMAP,
		i = sCRS.scnZN.a4x2[0]*I4x2( 1, pMAP->mapCR44.z );
	//if( !pM[i] )
	//	return;

	U4 xFND = pM[i];
	gpcSRC* pSRC = mass.SRCfnd( xFND );
	//if( !pSRC )
	//	return;

	selANCR[1].a4x2[0] = sCRS.scnZN.a4x2[0]+U4x2(1,0);		//AN
	selANCR[1].a4x2[1] = sCRS.scnIN.a4x2[0]/cr;	//IN
	anSTR[1] = (apSRC[1] = pSRC) ? pSRC->CRSminiCR( selANCR[1].a4x2[1] ) : 0;


	if( bSH )
	{
		U4	t1 = selANCR[1].a4x2[0]*I4x2(1,pMAP->mapCR44.z),
			t0 = selANCR[0].a4x2[0]*I4x2(1,pMAP->mapCR44.z);
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
		if( selANCR[0].a4x2[0] == selANCR[1].a4x2[0] )
		{
			CRSbEDset(true);
		}
		else if( CRSbEDget() )
			CRSbEDset(false);
		if( !bED )
		if( anSTR[1] != anSTR[0] )
		{
			anSTR[1] = anSTR[0] = 0;
		}

		if( sCRS.id != id )
		{
			// SHIFT klick szinkronizálja a két cursort
			sCRS.bED = bED;
			sCRS.selANCR[0] = selANCR[0];
			sCRS.selANCR[1] = selANCR[1];
			sCRS.anSTR[0] = anSTR[0];
			sCRS.anSTR[1] = anSTR[1];
			apSRC[0] = apSRC[0];
			apSRC[1] = apSRC[1];

		}

		return; // ha le van nyomva a shift akkor meg akarjuk örizni a sel[0]-t.
	}

	selANCR[0] = selANCR[1];
	apSRC[0] = apSRC[1];
	anSTR[0] = anSTR[1];

}

gpcCRS::gpcCRS( gpcWIN& win, U1 _id )
{
	//ctor
	gpmCLR;
	id = _id;
	CRSfrm.a4x2[1] = win.wFRM( 0 );
	wDIVfrm = win.wDIV(0);
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
U1* gpcCRS::gtUTF8( U1* pBUFF )
{
	char* pS = NULL;
	I4 nCPY = 0;
	if( apSRC[1] )
	if( apSRC[1] == apSRC[0] )
	if( nCPY = anSTR[1] - anSTR[0] )
	if( pS = nCPY > 0 ? (char*)apSRC[0]->pA : NULL )
	{
		pS += anSTR[0];
	}
	if( pS ? !nCPY : true )
		return pBUFF;

	gpmMEMCPY( pBUFF, pS, nCPY );
	pBUFF[nCPY] = 0;

	return pBUFF+nCPY;
}

///------------------------------
///
/// 		miniRDY
///
///------------------------------
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
		// nem a 0 cella a vezér hanem az 1-es
		//if( lurd.a4x2[1].x > ie )

		I4x4	selAN0AN1( selANCR[0].a4x2[0], selANCR[1].a4x2[0] ),
				lurdAN = selAN0AN1.lurd();
		U4x4	spcZN = lurdAN.a4x2[1] - U4x2(1,0),
				mCR;
		mass.mapCR.MAPalloc( spcZN, mCR );

		U4	*pM = pMAP->pMAP,
			*pC = pMAP->pCOL,
			*pR = pMAP->pROW, i, ie = pC-pM;
		gpcSRC	*pEDIT = NULL,
				*pSRC, tmp, *pS2;




		gpmZn( pC, pMAP->mapCR44.a4x2[1].sum() );

		for( i = 0, ie = pC-pM; i < ie; i++ )
		{
			if( !pM[i] )
				continue;

			xFND = pM[i];
			pSRC = mass.SRCfnd( xFND );
			dim = pSRC->CRSdim( aCRS );
            if( pC[i%pMAP->mapCR44.z] < dim.x )
				pC[i%pMAP->mapCR44.z] = dim.x;
			if( pR[i/pMAP->mapCR44.z] < dim.y )
				pR[i/pMAP->mapCR44.z] = dim.y;

		}
		for( U4 c = 0; c < pMAP->mapCR44.x; c++ )
		{
			miniALL.z += pC[c];
		}
		for( U4 r = 0; r < pMAP->mapCR44.y; r++ )
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

		if( lurdAN.x )
		if( pB < pE )
		if( pSRC = mass.SRCadd( tmp, NULL, lurdAN.a4x2[0] ) )
		{
			if( pSRC )
			{

				/*if( anSTR[0] < nSTRT )
					anSTR[0] = nSTRT;
				else if( anSTR[0] > pSRC->nL )
					anSTR[0] = pSRC->nL;

				if( anSTR[1] < anSTR[0] )
					anSTR[1] = anSTR[0];*/


				if( max( anSTR[1], anSTR[0] ) > pSRC->nL )
					anSTR[1] = anSTR[0] = pSRC->nL;

				I4	nSTRT = pSRC->pSRCstart()-pSRC->pSRCalloc();

				if( anSTR[0] < nSTRT )
					anSTR[0] = nSTRT;
				if( !bED )
				{
					anSTR[1] = pSRC->nL;
				}
				else if( anSTR[1] < anSTR[0] )
					anSTR[1] = anSTR[0];

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
						case 0x7e:
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

				for( I4x2 s = lurdAN.a4x2[0]; s.y <= lurdAN.w; s.y++ )
				{
					for( s.x = lurdAN.x; s.x <= lurdAN.z; s.x++ )
					{
						U4	i = (s * I4x2( 1, mass.mapCR.mapCR44.z ))-1,
							x_fnd = mass.mapCR.pMAP[i];
						pS2 = x_fnd ? mass.SRCfnd( x_fnd ) : NULL;
						if( pS2 == pSRC )
							continue;
						if( !pS2 )
						{
							pS2 = mass.SRCadd( tmp, NULL, s );
							if( !pS2 )
								continue;
						}

						U1* pSS;
						U4x4 mCR44 = s;


						//pS2->reset( pSRC->pA, pSRC->pA+pSRC->nL, &pSS, mCR44, 0 );
						pS2->SRCcpy( pSRC->pA, pSRC->pA+pSRC->nL );
						pS2->updt();
					}
				}

			}
		}

		for( U4 r = 0; r < pMAP->mapCR44.y; miniALL.y += pR[r], r++ )
		{
			if( miniALL.y >= CRSfrm.w )
				break;
            if( !pR[r] )
				continue;
			if( (miniALL.y + (int)pR[r]) < 0 )
				continue;

			miniALL.x = CRSfrm.x;
			for( U4 c = 0; c < pMAP->mapCR44.x; miniALL.x += pC[c], c++ )
			{
				if( miniALL.x >= CRSfrm.z )
					break;
				if( !pC[c] )
					continue;

				if( miniALL.x +  pC[c] < 0 )
					continue;

				i = c + r*pMAP->mapCR44.z;
				if( !pM[i] )
					continue;

				xFND = pM[i];
				pSRC = mass.SRCfnd( xFND );


				if( !lurdAN.x )
				{
					c16fr = gpeCLR_blue2;
					c16ch = gpeCLR_blue2;
				}
				else if(
						   ( c+1 >= lurdAN.x	&& r >= lurdAN.y )
						&& ( c+1 <= lurdAN.z	&& r <= lurdAN.w )
				)
				{
					c16fr = bED ? gpeCLR_green2 : gpeCLR_cyan;
					c16ch = bED ? gpeCLR_cyan : gpeCLR_blue2;
				} else {
					c16fr = gpeCLR_blue2;
					c16ch = gpeCLR_blue2;
				}
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
