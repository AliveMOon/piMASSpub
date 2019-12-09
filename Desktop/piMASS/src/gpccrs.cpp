
#include "gpccrs.h"
#include "gpcSRC.h"

I4x4 gpcCRS::scnZNCR(	gpcWIN& win, //U1 iDIV,
						gpcMASS& mass, const I4x2& _xy )
{
	// XY - pixel
	// CR - Coll/Row
	SDL_Rect div = win.wDIV( id ); //iDIV );
	if( div.w < 1 )
	{
		div = win.wDIV( id );  //iDIV );
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
		if( id )
		{
			U4 nD = pMAP->mapZN44.a4x2[1].sum()*id;
			pC += nD;
			pR += nD;
		}
		scnZN0.null();
		scnZN.null();
		scnIN.null();

		for( scnZN.x = 0; o.z >= scnZN.z; scnZN.x++ )
		{
			scnZN.z += (scnZN.x < pMAP->mapZN44.z) ? pC[scnZN.x] : gpdSRC_COLw;
			if( o.z >= scnZN.z )
			{
				scnZN0.z = scnZN.z;
				continue;
			}

			scnIN.z = ((scnZN.x < pMAP->mapZN44.z) ? pC[scnZN.x] : gpdSRC_COLw)*cr.x;
			scnIN.x = xy.x - (scnZN.z*cr.x - scnIN.z);
			break;
		}

		for( scnZN.y = 0; o.w >= scnZN.w; scnZN.y++ )
		{
			scnZN.w += (scnZN.y < pMAP->mapZN44.w) ? pR[scnZN.y] : gpdSRC_ROWw;
			if( o.w >= scnZN.w )
			{
				scnZN0.w = scnZN.w;
				continue;
			}

			scnIN.w = ((scnZN.y < pMAP->mapZN44.w) ? pR[scnZN.y] : gpdSRC_ROWw)*cr.y;
			scnIN.y = xy.y - (scnZN.w*cr.y - scnIN.w);
			break;
		}

		/*for( scnZN.x = 0; scnZN.x < pMAP->mapZN44.z; scnZN.x++ )
		{
			scnZN.z += pC[scnZN.x];
			if( o.z >= scnZN.z )
			{
				scnZN0.z = scnZN.z;
				continue;
			}

			scnIN.z = pC[scnZN.x]*cr.x;
			scnIN.x = xy.x - (scnZN.z*cr.x - scnIN.z);
			break;
		}

		for( scnZN.y = 0; scnZN.y < pMAP->mapZN44.w; scnZN.y++ )
		{
			scnZN.w += pR[scnZN.y];
			if( o.w >= scnZN.w )
			{
				scnZN0.w = scnZN.w;
				continue;
			}

			scnIN.w = pR[scnZN.y]*cr.y;
			scnIN.y = xy.y - (scnZN.w*cr.y - scnIN.w);
			break;
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
			if( selANIN[1].x < 2 )
			{
				selANIN[1].x = 1;
				break;
			}
			selANIN[1].x--;
			break;
		case 3:	// right
			selANIN[1].x++;
			break;
		case 4:	// up
			if( selANIN[1].y < 2 )
			{
				selANIN[1].y = 0;
				break;
			}
			selANIN[1].y--;
			break;
		case 5:	// down)
			selANIN[1].y++;
	}
	if( !bSH )
		selANIN[0]= selANIN[1];

/*
	if( !edANIN[0].a4x2[0].x )
		return;
	if( (edANIN[0].a4x2[0] == selANIN[0].a4x2[0]) && (edANIN[1].a4x2[0] == selANIN[1].a4x2[0]) )
		return;*/



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
void gpcCRS::CRSsel( gpcWIN& win, gpcCRS& sCRS, gpcMASS& mass, bool bSH, U1 src )
{
	if( this ? !(&sCRS): true )
		return;
	SDL_Rect sDIV = win.wDIV( sCRS.id );
	I4x2 cr( sDIV.w/sCRS.CRSfrm.z, sDIV.h/sCRS.CRSfrm.w );
	U4x4 mpZN;

	//gpcMAP* pMAP =
	U4 *pM = mass.mapCR.MAPalloc( sCRS.scnZN.au4x2[0], mpZN ); //&mass.mapCR;
	if( !pM )
		return;

	U4	i = sCRS.scnZN.a4x2[0]*I4x2( 1, mpZN.z );

	U4 xFND = pM[i];
	gpcSRC* pSRC = mass.SRCfnd( xFND );
	if( pSRC )
	if( pSRC->bSW&gpeMASSunselMSK )
		return; //pSRC = NULL;

	selANIN[1].a4x2[0] = sCRS.scnZN.a4x2[0]+U4x2(1,0);		//AN
	selANIN[1].a4x2[1] = sCRS.scnIN.a4x2[0]/cr;				//IN
	anSTR[1] = (apSRC[1] = pSRC) ? pSRC->CRSminiCR( selANIN[1].a4x2[1], true ) : 0;

	/// ha ezt igazra álítom akkor belemásolja a másik cursorba
	bool bCPY = false; // OFF?

	if( bSH )
	{
		U4	t1 = selANIN[1].a4x2[0]*I4x2(1,mpZN.z),
			t0 = selANIN[0].a4x2[0]*I4x2(1,mpZN.z);
		if( t0 > t1  )
		{
			I4x4 tmp = selANIN[0];
			selANIN[0] = selANIN[1];
			selANIN[1] = tmp;

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
		if( selANIN[0].a4x2[0] == selANIN[1].a4x2[0] )
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

		if( bCPY )
		if( sCRS.id != id )
		{
			// SHIFT klick szinkronizálja a két cursort
			sCRS.bED = bED;
			sCRS.selANIN[0] = selANIN[0];
			sCRS.selANIN[1] = selANIN[1];
			sCRS.anSTR[0] = anSTR[0];
			sCRS.anSTR[1] = anSTR[1];
			apSRC[0] = apSRC[0];
			apSRC[1] = apSRC[1];

		}

		return; // ha le van nyomva a shift akkor meg akarjuk örizni a sel[0]-t.
	}
	else if( src < 4 )
	if( src != id )
	if( win.apCRS[src] )
	{
		gpcCRS& cSRC = *win.apCRS[src];
		selANIN[0] = selANIN[1];
		apSRC[0] = apSRC[1];
		anSTR[0] = anSTR[1];

		selANIN[1].a4x2[0] += cSRC.selANIN[1].a4x2[0] - cSRC.selANIN[0].a4x2[0];

		return;
	}

	selANIN[0] = selANIN[1];
	apSRC[0] = apSRC[1];
	anSTR[0] = anSTR[1];

}

gpcCRS::gpcCRS( gpcWIN& win, U1 _id )
{
	//ctor
	gpmCLR;
	id = _id;
	win.apCRS[id] = this;
	CRSfrm.a4x2[1] = win.wFRM( 0 );
	wDIVfrm = win.wDIV(0);

}

gpcCRS::~gpcCRS()
{
	//dtor
	gpmDELary(pCp);
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
	if( !this )
		return true;

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
	0, 			0xffffffff,	// fekete, fehér
	0xff880000,	0xffaaffee, // piros, cyán
	0xffcc44cc, 0xff00cc55,	// viola, zöld
	0xff0000aa, 0xffeeee77, // kék, sárga
	0xffdd8855, 0xff664400,	// narancs, barna
	0xffff7777, 0x33333333,	// vil.pir, söt.szürke
	0xff777777, 0xffaaff66,	// szürk, vil.zöld
	0xff0088ff, 0xffbbbbbb, // vil.kék, vil szürk
};
class gpcTHRD_DRW
{
public:
	gpcCRS		*pCRS;
	gpcWIN		*pWIN;
	SDL_Rect	src, dstPX;
	U4			frmC, sDIV, oDIV, eDIV;

	gpcTHRD_DRW( gpcCRS* pC, gpcWIN& w )
	{
		gpmCLR;
		pCRS = pC;
		pWIN = &w;
	}
};
void callDRW( gpcTHRD_DRW* pTD )
{
	gpcWIN &win	= *pTD->pWIN;
	gpcCRS &crs	= *pTD->pCRS;
	U4			nMINI 	= crs.nMINI;
	U1x4*		pMINI	= crs.pMINI;
	SDL_Rect	src 	= pTD->src,
				fsrc	= src,
				fdsrc	= src,
				dstPX	= pTD->dstPX,
				divPX 	= win.wDIV( crs.id );
	I4x4		scnZN0	= crs.scnZN0,
				CRSfrm	= crs.gtFRM();
	U4	cs = 0, cc = 255,
		fcs = 0, fcc = 255,
		fds = 0, fdc = 255,
		cx = src.w*8,
		cy = src.h*32, frmC = pTD->frmC,
		scx, scy,
		fscx, fscy;
	I4x2 wh;
	U1 	c,d,
		sDIV = pTD->sDIV,
		oDIV = pTD->oDIV;
	U1 sSTR[0x20];
	dstPX.y = divPX.y-dstPX.h;

	if( dstPX.w != src.w || dstPX.h != src.h ) { /// SCALE Blit -------------------------
		for( U4 i = 0; i < nMINI; i++, dstPX.x += dstPX.w ) {
			if( !(i%CRSfrm.z) )
			{
				dstPX.x = divPX.x;
				dstPX.y += dstPX.h;
			}

			if( c = pMINI[i].y )
			{
				c += 0xb0;
				if( cc != pMINI[i].x )
				{
					cc = pMINI[i].x;
					scx = (cc&3)*cx;
					scy = (cc>>2)*cy;
				}
				if( cs != c )
				{
					cs = c;
					src.x = (cs&7)*src.w + scx;
					src.y = (cs>>3)*src.h + scy;
				}

				//dstPX.x = (i%CRSfrm.z)*dstPX.w + divPX.x;
				//dstPX.y = (i/CRSfrm.z)*dstPX.h + divPX.y;

				SDL_BlitScaled( win.pSRFchar, &src, win.pSRFwin, &dstPX );
			}

			if( !pMINI[i].w )
				continue;

			if( fcc != pMINI[i].z )
			{
				fcc = pMINI[i].z;
				fscx = (fcc&3)*cx;
				fscy = (fcc>>2)*cy;
			}

			c = pMINI[i].w;
			if( c > 0x60 )
			{
				d = gpsHUN[c-0x60]-' ';
				c = gpsHUN[c-0x20]-' '+0x60;
				if( d >= 'a'-' ' && d <= 'z'-' ' )
					c += 8;
				if( fds != d )
				{
					fds = d;
					fdsrc.x =  (fds&7)*src.w + fscx;
					fdsrc.y = (fds>>3)*src.h + fscy;
				}
				//src.x = (d&7)*src.w + scx;
				//src.y = (d>>3)*src.h + scy;
				SDL_BlitScaled( win.pSRFchar, &fdsrc, win.pSRFwin, &dstPX );

			}

			if( fcs != c )
			{
				fcs = c;
				fsrc.x =  (fcs&7)*fsrc.w + fscx;
				fsrc.y = (fcs>>3)*fsrc.h + fscy;
			}
			//src.x = (c&7)*src.w + scx;
			//src.y = (c>>3)*src.h + scy;

			SDL_BlitScaled( win.pSRFchar, &fsrc, win.pSRFwin, &dstPX );

		}
	}
	else for( U4 i = 0; i < nMINI; i++, dstPX.x += dstPX.w ) { /// Blit -------------------------
		if( !(i%CRSfrm.z) )
		{
            dstPX.x = divPX.x;
            dstPX.y += dstPX.h;
		}

		if( c = pMINI[i].y )
		{
			c += 0xb0;
			if( cc != pMINI[i].x )
			{
				cc = pMINI[i].x;
				scx = (cc&3)*cx;
				scy = (cc>>2)*cy;
			}
			if( cs != c )
			{
				cs = c;
				src.x = (cs&7)*src.w + scx;
				src.y = (cs>>3)*src.h + scy;
			}
			//src.x = (c&7)*src.w + scx;
			//src.y = (c>>3)*src.h + scy;

			//dstPX.x = (i%CRSfrm.z)*dstPX.w + divPX.x;
			//dstPX.y = (i/CRSfrm.z)*dstPX.h + divPX.y;

			SDL_BlitSurface( win.pSRFchar, &src, win.pSRFwin, &dstPX );

		}

		if( !pMINI[i].w )
			continue;

		if( fcc != pMINI[i].z )
		{
			fcc = pMINI[i].z;
			fscx = (fcc&3)*cx;
			fscy = (fcc>>2)*cy;
		}

		c = pMINI[i].w;
		if( c > 0x60 )
		{
			d = gpsHUN[c-0x60]-' ';
			c = gpsHUN[c-0x20]-' '+0x60;
			if( d >= 'a'-' ' && d <= 'z'-' ' )
				c += 8;
			if( fds != d )
			{
				fds = d;
				fdsrc.x =  (fds&7)*src.w + fscx;
				fdsrc.y = (fds>>3)*src.h + fscy;
			}
			//src.x = (d&7)*src.w + scx;
			//src.y = (d>>3)*src.h + scy;
			SDL_BlitScaled( win.pSRFchar, &fdsrc, win.pSRFwin, &dstPX );
		}

		if( fcs != c )
		{
			fcs = c;
			fsrc.x = (fcs&7)*src.w + fscx;
			fsrc.y = (fcs>>3)*src.h + fscy;
		}
		//src.x = (c&7)*src.w + scx;
		//src.y = (c>>3)*src.h + scy;

		SDL_BlitSurface( win.pSRFchar, &fsrc, win.pSRFwin, &dstPX );
	}

	/// ON -----------------------------------------------------
	if( crs.id == win.onDIV.x )
	{
		dstPX.x = max( 0, scnZN0.z+CRSfrm.x );
		dstPX.y = max( 0, scnZN0.w+CRSfrm.y );

		wh = crs.scnZN.a4x2[1]; //+CRSfrm.a4x2[0];
		wh += CRSfrm.a4x2[0];
		wh.mn( CRSfrm.a4x2[1] );


		if( wh.mn() > 0 )
		{
			wh.x -= dstPX.x;
			wh.y -= dstPX.y;

			dstPX.x *= dstPX.w;
			dstPX.x += divPX.x;

			dstPX.y *= dstPX.h;
			dstPX.y += divPX.y;
			crs.frmDRW( dstPX, src, wh, win.pSRFwin, win.pSRFchar, gpeCLR_white, 0,  (crs.scnZN.au4x2[0]+U4x2(1,0)).strA4N(sSTR) );
		}
	}




	//if( frmC )
	//	return false;

	if( win.bSW < 2 )
		return;

	dstPX.x = CRSfrm.x > 0 ? (CRSfrm.x*divPX.w)/CRSfrm.z : 0;
	dstPX.y = CRSfrm.y > 0 ? (CRSfrm.y*divPX.h)/CRSfrm.w : 0;
	dstPX.x += divPX.x;
	dstPX.y += divPX.y;

	crs.frmDRW( dstPX, src, CRSfrm.a4x2[1], win.pSRFwin, win.pSRFchar, frmC, 0, (U1*)(crs.id == sDIV ? "EDIT" : "TARGET") );

}

// FRM 1 up // 2 right // 4 down // 8 left
bool gpcCRS::miniDRW( gpcWIN& win, U1 sDIV, U1 oDIV, U1 dDIV, I4x4 scnXYCR, bool bSHFT )
{
	if( !this )
		return false;

	if( win.apT[id] )
	{
		win.aT[id].join();
		win.apT[id] = NULL;
	}

	SDL_Rect src = win.chrPIC, divPX = win.wDIV( id );
	if( bESC ) {
		SDL_FillRect( win.pSRFwin, &divPX, gpaC64[14] ); // 0x000000AA );
		return false;
	}
	SDL_Rect dstPX = divPX, dst2, cWH;

	if( CRSfrm.x > 0 ) {
		dstPX.w = (CRSfrm.x*divPX.w)/CRSfrm.z;
		dstPX.h = divPX.h;
		SDL_FillRect( win.pSRFwin, &dstPX, gpaC64[14] );
	}

	if( CRSfrm.y > 0 ) {
		dstPX.h = (CRSfrm.y*divPX.h)/CRSfrm.w;
		dstPX.w = divPX.w;
		SDL_FillRect( win.pSRFwin, &dstPX, gpaC64[14] );
	}

	if( nMINI != CRSfrm.a4x2[1].area() ) {
		gpmDELary( pMINI );
		nMINI = CRSfrm.a4x2[1].area();
		if( !nMINI )
			return false;	// nincsen mérete ki lett kapcsolva?

		pCRS = pMINI = new U1x4[nMINI];
		gpmZn( pMINI, nMINI );
		return true; // resized mini, print new
	}




	dstPX = divPX;
	dstPX.x = CRSfrm.x > 0 ? (CRSfrm.x*divPX.w)/CRSfrm.z : 0;
	dstPX.y = CRSfrm.y > 0 ? (CRSfrm.y*divPX.h)/CRSfrm.w : 0;
	dstPX.w -= dstPX.x;
	dstPX.h -= dstPX.y;
	dstPX.x += divPX.x;
	dstPX.y += divPX.y;
	if( win.pPICbg ? win.pPICbg->surDRW() : NULL )
		SDL_BlitScaled( win.pPICbg->surDRW(), &win.pPICbg->xyOUT.xyWH, win.pSRFwin, &dstPX );
	else
		SDL_FillRect( win.pSRFwin, &dstPX, gpaC64[6] );

	dstPX.w = divPX.w/CRSfrm.z;	// cél karakter szélessége pix-be
	dstPX.h = divPX.h/CRSfrm.w;	// cél karakter magassága pix-be

	U4	cx = src.w*8,
		cy = src.h*32,
		scx, scy,
		frmC = 0;

	U1 sSTR[0x20];
	const U1	*pSTR = NULL;

	U1 eDIV = id;
	if( bSHFT )
	if( id == oDIV )
	if( id != sDIV ) {
		eDIV = sDIV;
		sDIV = oDIV;
	}

	if( id == sDIV ) 				// sárga
		frmC = gpeCLR_orange;
	else if( id == dDIV )			// zöld
		frmC = gpeCLR_green;



	U1 c,d, cc;
	I4x2 wh;

	if( frmC ) {



		/// SEL -----------------------------------------------------
		if( gpcMAP* pMAP = win.piMASS ? &win.piMASS->mapCR : NULL )
		{
			I4x4	selAN0AN1( win.apCRS[eDIV]->selANIN[0].a4x2[0], win.apCRS[eDIV]->selANIN[1].a4x2[0] ),
					lurdAN = selAN0AN1.lurd();

			U4x2 	spcZN = lurdAN.a4x2[1]; // - U4x2(1,0);
			U4x4	mZN;

			/// SEL? -----------------------------------------------------
			if( win.apCRS[eDIV]->selANIN[0].a4x2[0].x )
				spcZN.x -= 1;

			U4	*pM = pMAP->MAPalloc( spcZN, mZN ),
				nCR = mZN.a4x2[1].sum(), c, r;
			I4	*pC = (I4*)pMAP->pCOL,
				*pR = (I4*)pMAP->pROW,
				i, ie = pC-(I4*)pM;

			if( id )
			{
				i = nCR*(U4)id;
				pC += i;
				pR += i;
			}

			if( (nCp <= mZN.a4x2[1].x) || (nRp <= mZN.a4x2[1].y) )
			{
				nCp = mZN.a4x2[1].x+1;
				nRp = mZN.a4x2[1].y+1;
				gpmDELary(pCp);
			}
			if( !pCp )
			{
				pCp = new I4[nCp+nRp];
				pRp = pCp + nCp;
			}


			*pCp = 0;
			for( i = 0; i < mZN.a4x2[1].x; i++ )
			{
				pCp[i+1] = pCp[i] + pC[i];
			}

			*pRp = 0;
			for( i = 0; i < mZN.a4x2[1].y; i++ )
			{
				pRp[i+1] = pRp[i] + pR[i];
			}

			//U4

			if( !win.apCRS[eDIV]->selANIN[0].a4x2[0].x  ) //? win.apCRS[eDIV]->selANIN[0].a4x2[0] == win.apCRS[eDIV]->selANIN[1].a4x2[0] : true )
			{
				for( i = 0; i < ie; i++ )
				{
					//if( iSKIP.x == i || iSKIP.y == i )
					//	continue;
					c = i%mZN.z;
					r = i/mZN.z;

					if( pM[i] )
					if( U4 xFND = pM[i] )
					if(	gpcSRC* pSRC = win.piMASS->SRCfnd( xFND ) )
					if( pSRC->picID )
					if( gpcPIC* pPIC = win.piMASS->PIC.PIC(pSRC->picID-1) )
					if( SDL_Surface* pSRF = pPIC->pSRF )
					{
						I4x4 s, d, w;
						s.x = s.y = 0;
						s.z = pSRF->w;
						s.w = pSRF->h;

						w.x = pCp[c];
						w.y = pRp[r];
						w.z	= pC[c]+w.x;
						w.w	= pR[r]+w.y;

						w += CRSfrm.a4x2[0];
						if( w.a4x2[1].mn() > 0 )
						{
							d = w;
							d.a4x2[1].mn( CRSfrm.a4x2[1] );
							w.a4x2[1] -= w.a4x2[0];
							d.a4x2[1] -= d.a4x2[0];
							d.a4x2[0].mx(0);
							//w = d-w;
                            if( w.a4x2[0].x < 0 )
							{
								s.x -= (w.a4x2[0].x*s.z) / w.a4x2[1].x;
							}
							if( w.a4x2[0].y < 0 )
							{
								s.y -= (w.a4x2[0].y*s.w) / w.a4x2[1].y;
							}

							s.a4x2[1] &= d.a4x2[1];
							s.a4x2[1] /= w.a4x2[1];

							//d.a4x2[1] -= d.a4x2[0];


							d.x *= dstPX.w;
							d.x += divPX.x;

							d.y *= dstPX.h;
							d.y += divPX.y;

							d.z *= dstPX.w;
							d.w *= dstPX.h;

							SDL_BlitScaled( pSRF, &s.xyWH, win.pSRFwin, &d.xyWH );
						}
					}
				}
			} else {
				c = lurdAN.a4x2[0].x-1,
				r = lurdAN.a4x2[0].y;

				U4x2 iSKIP = c+r*mZN.z;

				dstPX.x = max( 0, pCp[c]+CRSfrm.x );
				dstPX.y = max( 0, pRp[r]+CRSfrm.y );
				wh.x =	( c < mZN.a4x2[1].x )
						? pC[c]+pCp[c]
						: (pC[mZN.a4x2[1].x]+pCp[mZN.a4x2[1].x] + (c-mZN.a4x2[1].x));
				wh.y =	( r < mZN.a4x2[1].y )
						? (pR[r]+pRp[r])
						: (pR[mZN.a4x2[1].y]+pRp[mZN.a4x2[1].y] + (r-mZN.a4x2[1].y)*gpdSRC_COLw);
				wh += CRSfrm.a4x2[0];

				if( wh.mn() ) {
					wh.mn( CRSfrm.a4x2[1] );
					wh.x -= dstPX.x;
					wh.y -= dstPX.y;

					dstPX.x *= dstPX.w;
					dstPX.x += divPX.x;

					dstPX.y *= dstPX.h;
					dstPX.y += divPX.y;

					frmDRW( dstPX, src, wh, win.pSRFwin, win.pSRFchar, frmC, 0, lurdAN.a4x2[0].strA4N(sSTR) );
				}


				c = lurdAN.a4x2[1].x-1, r = lurdAN.a4x2[1].y;
				wh.x =	( c < mZN.a4x2[1].x )
						? pC[c]+pCp[c]
						: (pC[mZN.a4x2[1].x]+pCp[mZN.a4x2[1].x] + (c-mZN.a4x2[1].x));
				wh.y =	( r < mZN.a4x2[1].y )
						? (pR[r]+pRp[r])
						: (pR[mZN.a4x2[1].y]+pRp[mZN.a4x2[1].y] + (r-mZN.a4x2[1].y)*gpdSRC_COLw);
				wh += CRSfrm.a4x2[0];

				iSKIP.y = c+r*mZN.z;

				dstPX.x = max( 0, pCp[c]+CRSfrm.x );
				dstPX.y = max( 0, pRp[r]+CRSfrm.y );


				if( wh.mn() ) {
					wh.mn( CRSfrm.a4x2[1] );
					wh.x -= dstPX.x;
					wh.y -= dstPX.y;

					dstPX.x *= dstPX.w;
					dstPX.x += divPX.x;

					dstPX.y *= dstPX.h;
					dstPX.y += divPX.y;

					frmDRW( dstPX, src, wh, win.pSRFwin, win.pSRFchar, frmC, 0, lurdAN.a4x2[1].strA4N(sSTR) );
				}

				for( i = 0; i < ie; i++ )
				{
					if( iSKIP.x == i || iSKIP.y == i )
						continue;
					c = i%mZN.z;
					r = i/mZN.z;

					if( pM[i] )
					if( U4 xFND = pM[i] )
					if(	gpcSRC* pSRC = win.piMASS->SRCfnd( xFND ) )
					if( pSRC->picID )
					if( gpcPIC* pPIC = win.piMASS->PIC.PIC(pSRC->picID-1) )
					if( SDL_Surface* pSRF = pPIC->surDRW() )
					{
						I4x4 s, d, w;
						s.x = s.y = 0;
						s.z = pSRF->w;
						s.w = pSRF->h;

						w.x = pCp[c];
						w.y = pRp[r];
						w.z	= pC[c]+w.x;
						w.w	= pR[r]+w.y;

						w += CRSfrm.a4x2[0];
						if( w.a4x2[1].mn() > 0 )
						{
							d = w;
							d.a4x2[1].mn( CRSfrm.a4x2[1] );
							w.a4x2[1] -= w.a4x2[0];
							d.a4x2[1] -= d.a4x2[0];
							d.a4x2[0].mx(0);
							//w = d-w;
                            if( w.a4x2[0].x < 0 )
							{
								s.x -= (w.a4x2[0].x*s.z) / w.a4x2[1].x;
							}
							if( w.a4x2[0].y < 0 )
							{
								s.y -= (w.a4x2[0].y*s.w) / w.a4x2[1].y;
							}

							s.a4x2[1] &= d.a4x2[1];
							s.a4x2[1] /= w.a4x2[1];

							//d.a4x2[1] -= d.a4x2[0];


							d.x *= dstPX.w;
							d.x += divPX.x;

							d.y *= dstPX.h;
							d.y += divPX.y;

							d.z *= dstPX.w;
							d.w *= dstPX.h;

							SDL_BlitScaled( pSRF, &s.xyWH, win.pSRFwin, &d.xyWH );
						}
					}

					if( c+1 < lurdAN.x )
						continue;
					if( c+1 > lurdAN.z )
						continue;

					if( r < lurdAN.y )
						continue;
					if( r > lurdAN.w )
						continue;
					/*if( I4x2(c,r) == scnZN.a4x2[0] )
						continue;*/

					if(
						   ( (c+1 >= lurdAN.x)	&& (r >= lurdAN.y) )
						&& ( (c+1 <= lurdAN.z)	&& (r <= lurdAN.w) )
					) {
						dstPX.x = max( 0, pCp[c]+CRSfrm.x );
						dstPX.y = max( 0, pRp[r]+CRSfrm.y );

						wh.x =	( c < mZN.a4x2[1].x )
								? pC[c]+pCp[c]
								: (pC[mZN.a4x2[1].x]+pCp[mZN.a4x2[1].x] + (c-mZN.a4x2[1].x));
						wh.y =	( r < mZN.a4x2[1].y )
								? (pR[r]+pRp[r])
								: (pR[mZN.a4x2[1].y]+pRp[mZN.a4x2[1].y] + (r-mZN.a4x2[1].y)*gpdSRC_COLw);
						wh += CRSfrm.a4x2[0];
						if( wh.mn() < 0 )
							continue;

						wh.mn( CRSfrm.a4x2[1] );
						wh.x -= dstPX.x;
						wh.y -= dstPX.y;

						dstPX.x *= dstPX.w;
						dstPX.x += divPX.x;

						dstPX.y *= dstPX.h;
						dstPX.y += divPX.y;

						frmDRW( dstPX, src, wh, win.pSRFwin, win.pSRFchar, frmC, 1, NULL );
					}
				}
			}
		}

	}


	if( !win.apTall[id] )
	{
		win.apTall[id] = new gpcTHRD_DRW( this, win );
	}

	if( win.apT[id] = win.apTall[id] )
	{
		win.apT[id]->frmC = frmC;
		win.apT[id]->sDIV = sDIV;
		win.apT[id]->oDIV = oDIV;
		win.apT[id]->eDIV = eDIV;
		win.apT[id]->src = src;
		win.apT[id]->dstPX = dstPX;

		win.aT[id] = std::thread( callDRW, win.apT[id] );
		return false;
	}


	if( dstPX.w != src.w || dstPX.h != src.h ) { /// SCALE Blit -------------------------
		for( U4 i = 0; i < nMINI; i++ ) {
			if( pMINI[i].y )
			{
				c = pMINI[i].y+0xb0;
				cc = pMINI[i].x; // FRM COLOR
				scx = (cc&3)*cx;
				scy = (cc>>2)*cy;
				src.x = (c&7)*src.w + scx;
				src.y = (c>>3)*src.h + scy;

				dstPX.x = (i%CRSfrm.z)*dstPX.w + divPX.x;
				dstPX.y = (i/CRSfrm.z)*dstPX.h + divPX.y;

				SDL_BlitScaled( win.pSRFchar, &src, win.pSRFwin, &dstPX );
			}

			if( !pMINI[i].w )
				continue;

			c = pMINI[i].w;
			cc = pMINI[i].z;
			scx = (cc&3)*cx;
			scy = (cc>>2)*cy;

			dstPX.x = (i%CRSfrm.z)*dstPX.w + divPX.x;
			dstPX.y = (i/CRSfrm.z)*dstPX.h + divPX.y;

			if( c > 0x60 )
			{
				d = gpsHUN[c-0x60]-' ';
				c = gpsHUN[c-0x20]-' '+0x60;
				if( d >= 'a'-' ' && d <= 'z'-' ' )
					c += 8;
				src.x = (d&7)*src.w + scx;
				src.y = (d>>3)*src.h + scy;
				SDL_BlitScaled( win.pSRFchar, &src, win.pSRFwin, &dstPX );

			}

			src.x = (c&7)*src.w + scx;
			src.y = (c>>3)*src.h + scy;

			SDL_BlitScaled( win.pSRFchar, &src, win.pSRFwin, &dstPX );

		}
	}
	else for( U4 i = 0; i < nMINI; i++ ) { /// Blit -------------------------
		if( c = pMINI[i].y )
		{
			c = pMINI[i].y+0xb0;
			cc = pMINI[i].x;
			scx = (cc&3)*cx;
			scy = (cc>>2)*cy;
			src.x = (c&7)*src.w + scx;
			src.y = (c>>3)*src.h + scy;

			dstPX.x = (i%CRSfrm.z)*dstPX.w + divPX.x;
			dstPX.y = (i/CRSfrm.z)*dstPX.h + divPX.y;

			SDL_BlitSurface( win.pSRFchar, &src, win.pSRFwin, &dstPX );

		}

		if( !pMINI[i].w )
			continue;

		c = pMINI[i].w;
		cc = pMINI[i].z;
		scx = (cc&3)*cx;
		scy = (cc>>2)*cy;

		dstPX.x = (i%CRSfrm.z)*dstPX.w + divPX.x;
		dstPX.y = (i/CRSfrm.z)*dstPX.h + divPX.y;

		if( c > 0x60 )
		{
			d = gpsHUN[c-0x60]-' ';
			c = gpsHUN[c-0x20]-' '+0x60;
			if( d >= 'a'-' ' && d <= 'z'-' ' )
				c += 8;
			src.x = (d&7)*src.w + scx;
			src.y = (d>>3)*src.h + scy;
			SDL_BlitScaled( win.pSRFchar, &src, win.pSRFwin, &dstPX );
		}

		src.x = (c&7)*src.w + scx;
		src.y = (c>>3)*src.h + scy;

		SDL_BlitSurface( win.pSRFchar, &src, win.pSRFwin, &dstPX );
	}

	/// ON -----------------------------------------------------
	if( id == oDIV  )
	{
		dstPX.x = max( 0, scnZN0.z+CRSfrm.x );
		dstPX.y = max( 0, scnZN0.w+CRSfrm.y );

		wh = scnZN.a4x2[1]; //+CRSfrm.a4x2[0];
		wh += CRSfrm.a4x2[0];
		wh.mn( CRSfrm.a4x2[1] );


		if( wh.mn() > 0 )
		{
			wh.x -= dstPX.x;
			wh.y -= dstPX.y;

			dstPX.x *= dstPX.w;
			dstPX.x += divPX.x;

			dstPX.y *= dstPX.h;
			dstPX.y += divPX.y;
			frmDRW( dstPX, src, wh, win.pSRFwin, win.pSRFchar, gpeCLR_white, 0,  (scnZN.au4x2[0]+U4x2(1,0)).strA4N(sSTR) );
		}
	}




	//if( frmC )
	//	return false;

	if( win.bSW < 2 )
		return false;

	dstPX.x = CRSfrm.x > 0 ? (CRSfrm.x*divPX.w)/CRSfrm.z : 0;
	dstPX.y = CRSfrm.y > 0 ? (CRSfrm.y*divPX.h)/CRSfrm.w : 0;
	dstPX.x += divPX.x;
	dstPX.y += divPX.y;

	frmDRW( dstPX, src, CRSfrm.a4x2[1], win.pSRFwin, win.pSRFchar, frmC, 0, (U1*)(id == sDIV ? "EDIT" : "TARGET") );
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

		I4x4	selAN0AN1( selANIN[0].a4x2[0], selANIN[1].a4x2[0] ),
				lurdAN = selAN0AN1.lurd();
		U4x2	spcZN = lurdAN.a4x2[1] - U4x2(1,0);
		U4x4	mCR;

		U4	*pM = pMAP->MAPalloc( spcZN, mCR, id ),//pMAP->pMAP,
			*pC = pMAP->pCOL,
			*pR = pMAP->pROW, i, ie = pC-pM, c, r;
		if( id )
		{
			i = pMAP->mapZN44.a4x2[1].sum()*(U4)id;
			pC += i;
			pR += i;
		}

		gpcSRC	tmp,
				*pEDIT = NULL,
				*pSRC = NULL,
				*pS2 = NULL;
		bool bNoMini = false;

		for( i = 0; i < ie; i++ )
		{
			if( !pM[i] )
				continue;
			c = i%pMAP->mapZN44.z;
			r = i/pMAP->mapZN44.z;
			if(
				   ( c+1 >= lurdAN.x	&& r >= lurdAN.y )
				&& ( c+1 <= lurdAN.z	&& r <= lurdAN.w )
			)
				bNoMini = true;
			else
				bNoMini = false;

			xFND = pM[i];
			pSRC = mass.SRCfnd( xFND );
			dim = pSRC->CRSdim( aCRS, bNoMini );
            if( pC[c] < dim.x )
				pC[c] = dim.x;
			if( pR[r] < dim.y )
				pR[r] = dim.y;

		}
		for( U4 c = 0; c < pMAP->mapZN44.x; c++ )
		{
			miniALL.z += pC[c];
		}
		for( U4 r = 0; r < pMAP->mapZN44.y; r++ )
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
		if( pSRC = mass.SRCnew( tmp, NULL, lurdAN.a4x2[0], -1 ) )
		{
			//gpmMEMCPY( edANIN, selANIN, 2 );
			if( pSRC )
			{
				if( max( anSTR[1], anSTR[0] ) > pSRC->nL )
					anSTR[1] = anSTR[0] = pSRC->nL;

				I4	nSTRT = pSRC->pSRCstart(true)-pSRC->pSRCalloc(true);

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
				pSRC->srcUPDT();
				*pLFT = 0;

				gpmDELary(pOA);
				pSRC->hd(mass);

				for( I4x2 s = lurdAN.a4x2[0]; s.y <= lurdAN.w; s.y++ )
				{
					for( s.x = lurdAN.x; s.x <= lurdAN.z; s.x++ )
					{
						U4	i = (s * I4x2( 1, mass.mapCR.mapZN44.z ))-1,
							x_fnd = mass.mapCR.pMAP[i];
						pS2 = x_fnd ? mass.SRCfnd( x_fnd ) : NULL;
						if( pS2 == pSRC )
							continue;
						if( !pS2 )
						{
							pS2 = mass.SRCnew( tmp, NULL, s, -1 );
							if( !pS2 )
								continue;
						}

						U1* pSS;
						U4x4 mCR44 = s;


						//pS2->reset( pSRC->pA, pSRC->pA+pSRC->nL, &pSS, mCR44, 0 );
						pS2->SRCcpy( pSRC->pA, pSRC->pA+pSRC->nL );
						pS2->srcUPDT();
					}
				}

			}
		}

		for( U4 r = 0; r < pMAP->mapZN44.y; miniALL.y += pR[r], r++ )
		{
			if( miniALL.y >= CRSfrm.w )
				break;
            if( !pR[r] )
				continue;
			if( (miniALL.y + (int)pR[r]) < 0 )
				continue;

			miniALL.x = CRSfrm.x;
			for( U4 c = 0; c < pMAP->mapZN44.x; miniALL.x += pC[c], c++ )
			{
				if( miniALL.x >= CRSfrm.z )
					break;
				if( !pC[c] )
					continue;

				if( miniALL.x +  pC[c] < 0 )
					continue;

				i = c + r*pMAP->mapZN44.z;
				if( !pM[i] )
					continue;

				xFND = pM[i];
				pSRC = mass.SRCfnd( xFND );

				bNoMini = false;
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
					bNoMini = true;
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
									c16bg, c16fr, c16ch,
									bNoMini
								);

			}
		}

	}
}
