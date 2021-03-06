
#include "gpccrs.h"
#include "gpcSRC.h"


gpcCRS::gpcCRS( gpcWIN& win, U1 _id )
{
	//ctor
	gpmCLR;
	id = _id;
	win.apCRS[id] = this;
	CRSfrm.a4x2[1] = win.wFRM()*6;
	CRSfrm.a4x2[1].mn( win.wDIVcr(0).a4x2[1] );
	//wDIVfrm = win.wDIV(0).xyWH;
}

gpcCRS::~gpcCRS() {
	//dtor
	gpmDELary(pCp);
}
I4x4 gpcCRS::scnZNCR(	gpcWIN& win, //U1 iDIV,
						gpcMASS& mass, const I4x2& _xy, U1 srcDIV )
{

	// XY - pixel
	// CR - Coll/Row
	I4x4	divSZ = win.wDIVsz( id );
	if( !(divSZ.z*divSZ.w) )
		return 0;
	if( !this )
		return 0;

	I4x2 xyIN = _xy-divSZ.a4x2[0],
		 crIN = (xyIN&CRSfrm.a4x2[1])/divSZ.a4x2[1],
		 xy2CR = crIN - CRSfrm.a4x2[0],
		 endCR = CRSfrm.a4x2[1] - CRSfrm.a4x2[0];

	if( srcDIV > 3 ? true : !win.apCRS[srcDIV] )
		srcDIV = id;
	if( gpcMAP* pMAP = &mass.mapCR ) {
		U4	*pC = pMAP->pCOL,
			*pR = pMAP->pROW;
		if( srcDIV ) {
			U4 nD = pMAP->mapZN44.a4x2[1].sum()*id;
			pC += nD;
			pR += nD;
		}
		scnZN0.null();
		scnZN.null();
		scnIN.null();

		for( scnZN.x = 0; endCR.x >= scnZN.z; scnZN.x++ ) {
			scnZN.z += (scnZN.x < pMAP->mapZN44.z) ? pC[scnZN.x] : gpdSRC_COLw;
			if( xy2CR.x >= scnZN.z ) {
				scnZN0.z = scnZN.z;
				continue;
			}
			scnIN.z = (((scnZN.x < pMAP->mapZN44.z) ? pC[scnZN.x] : gpdSRC_COLw)*divSZ.z) / CRSfrm.z;
			scnIN.x = xyIN.x - ((scnZN0.z+CRSfrm.x)*divSZ.z)/CRSfrm.z;
			break;
		}

		for( scnZN.y = 0; endCR.y >= scnZN.w; scnZN.y++ ) {
			scnZN.w += (scnZN.y < pMAP->mapZN44.w) ? pR[scnZN.y] : gpdSRC_ROWw;
			if( xy2CR.y >= scnZN.w ) {
				scnZN0.w = scnZN.w;
				continue;
			}
			scnIN.w = (((scnZN.y < pMAP->mapZN44.w) ? pR[scnZN.y] : gpdSRC_ROWw)*divSZ.w) / CRSfrm.w;
			scnIN.y = xyIN.y - ((scnZN0.w+CRSfrm.y)*divSZ.w)/CRSfrm.w;
			break;
		}
	}
	return I4x4( xyIN+(CRSfrm.a4x2[0]&divSZ.a4x2[1])/CRSfrm.a4x2[1], xy2CR );
}


void gpcCRS::CRSstpCL( //gpcWIN& win,
						gpcMASS* pMASS, U1 stp, bool bSH, bool bCT )
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
		selANIN[0] = selANIN[1];

	gpmZ(apSRC);
	U4 xFND = pMASS->getXFNDan( selANIN[1].au4x2[0] );
    if( !xFND )
		return;
	apSRC[1] = pMASS->srcFND(xFND);
	if( selANIN[0].au4x2[0] == selANIN[1].au4x2[0] )
		apSRC[0] = apSRC[1];
	if( !apSRC[1] )
		return;
	iSTR.x = apSRC[1]->iPUB();
	iSTR.y = apSRC[1]->n_ld_add();
}
void gpcCRS::CRSstpED( //gpcWIN* pWIN, gpcMASS* pMASS,
						U1 stp, bool bSH, bool bCT )
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
			*pLFT	= pOA + iSTR.x,
			*pRIG	= pOA + iSTR.y;
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
				if( pRIG-pOA < pSRC->n_ld_add() )
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

					pRIG = gpfUTF8stpX( pRIG, pOA+pSRC->n_ld_add(), x );
				}
				break;
		}
		if( pRIG < pSRC->pA )
			pRIG = pSRC->pA;
		if( pRIG >= pSRC->pA+pSRC->n_ld_add() )
			pRIG = pSRC->pA+pSRC->n_ld_add();

		iSTR.y = pRIG-pSRC->pA;
		if( !bSH )
			iSTR.x = iSTR.y;
		else if( iSTR.x > iSTR.y )
		{
			U4 tmp = iSTR.x;
			iSTR.x = iSTR.y;
			iSTR.y = tmp;
		}
	}
}
void gpcCRS::CRSsel( gpcWIN& win, gpcCRS& sCRS, gpcMASS& mass, bool bSH, U1 src )
{
	if( this ? !(&sCRS): true )
		return;

	I4x2 cr = win.wDIVpx( sCRS.id ).a4x2[1]/sCRS.CRSfrm.a4x2[1];
	U4x4 mpZN;

	U4 *pM = mass.mapCR.MAPalloc( sCRS.scnZN.au4x2[0], mpZN );
	if( !pM )
		return;

	U4	i = sCRS.scnZN.a4x2[0]*I4x2( 1, mpZN.z );

	U4 xFND = pM[i];
	gpcSRC* pSRC = mass.srcFND( xFND );
	if( pSRC )
	if( pSRC->bSW&gpeMASSunselMSK )
		return; //pSRC = NULL;

	selANIN[1].a4x2[0] = sCRS.scnZN.a4x2[0]+U4x2(1,0);		//AN
	selANIN[1].a4x2[1] = sCRS.scnIN.a4x2[0]/cr;				//IN
	iSTR.y = (apSRC[1] = pSRC) ? pSRC->SRCmnCR( selANIN[1].a4x2[1], true, 4 ) : 0; // bNoMini==true azaz p_src

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

			t0 = iSTR.y;
			iSTR.y = iSTR.x;
			iSTR.x = t0;
			return;
		}

		if( iSTR.y < iSTR.x )
		{
			t0 = iSTR.y;
			iSTR.y = iSTR.x;
			iSTR.x = t0;
		}
		if( selANIN[0].a4x2[0] == selANIN[1].a4x2[0] )
		{
			CRSbEDset(true);
		}
		else if( CRSbEDget() )
			CRSbEDset(false);

		if( !bED )
		if( iSTR.y != iSTR.x )
		{
			iSTR.y = iSTR.x = 0;
		}

		if( bCPY )
		if( sCRS.id != id ) {
			// SHIFT klick szinkronizálja a két cursort
			sCRS.bED = bED;
			sCRS.selANIN[0] = selANIN[0];
			sCRS.selANIN[1] = selANIN[1];
			sCRS.iSTR = iSTR;
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
		iSTR.x = iSTR.y;

		selANIN[1].a4x2[0] += cSRC.selANIN[1].a4x2[0] - cSRC.selANIN[0].a4x2[0];

		return;
	}

	selANIN[0] = selANIN[1];
	apSRC[0] = apSRC[1];
	iSTR.x = iSTR.y;

}


U1 gpsHUN[] =
" A       E   I  "
"UOoO  O   U U   "
// 0x20 -------------------
" a       e   i  "
"uUuo  o   u u   "
" '       '   '  "
":\"\"'  :   ' :   "
// 0x60 -------------------
" '       '   '  "
":\"\"'  :   ' :   "
"0123456789abcdef";

bool gpcCRS::miniOFF( gpcPIC* pPIC, SDL_Renderer* pRNDR ) {
	if( !this )
		return true;

	if( !pPIC || !pRNDR )
	{
		if( nMINI == CRSfrm.a4x2[1].area() )
			return !nMINI;

		if( !pLOCK )
			gpmDELary( pMINI );

		nMINI = CRSfrm.a4x2[1].area(); //.z*CRSfrm.w;
		if( !nMINI )
			return true;	// nincsen mérete ki lett kapcsolva?

		//pCRS =
		pMINI = new U1x4[nMINI];
		gpmZnOF( pMINI, nMINI );
		return false; // resized mini, print new
	}

	if( pLOCK != pMINI )
		gpmDELary( pMINI );

	pLOCK = pPIC->u1x4LOCK( pRNDR, CRSfrm.au4x2[1], CRSfrm.a4x2[1].x );

	pMINI = pLOCK;
	if( !pLOCK )
	{
		nMINI = 0;
		return true;
	}


	nMINI = CRSfrm.a4x2[1].area();
	return false;
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
U1 gpaCLR2C64[] = {
	0, 2, 5, 6, 4, 10, 13, 14, 9, 8, 7, 3, 11, 12, 15, 1,
};
class gpcTHRD_DRW {
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
	static void callDRW( gpcTHRD_DRW* pTD ) {
		gpcWIN &win	= *pTD->pWIN;
		gpcCRS &crs	= *pTD->pCRS;
		U4			nMINI 	= crs.nMINI;
		U1x4*		pMINI	= crs.pMINI;
		SDL_Rect	src 	= pTD->src,
					fsrc	= src,
					fdsrc	= src,
					dstPX	= pTD->dstPX,
					divPX 	= win.wDIVpx( crs.id ).xyWH;
		I4x4		scnZN0	= crs.scnZN0,
					CRSfrm	= crs.gtFRM();
		U4	z = CRSfrm.z,
			cs = 0, 	cc = 255,
			fcs = 0, 	fcc = 255,
			fds = 0, 	fdc = 255,
			cx = src.w*8,
			cy = src.h*32, frmC = pTD->frmC,
			scx, scy,
			fscx, fscy;
		I4x2 wh;
		U1 	c,d,
			//sDIV = pTD->sDIV,
			//oDIV = pTD->oDIV,
			sSTR[0x20];

		dstPX.y = divPX.y-dstPX.h;

		if( dstPX.w != src.w || dstPX.h != src.h ) { /// SCALE Blit -------------------------
			for( U4 i = 0; i < nMINI; i++, dstPX.x += dstPX.w ) {
				if( !(i%z) )
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

					gpdBLTs( win.pSRFchar, &src, win.pSRFwin, &dstPX );
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

					gpdBLTs( win.pSRFchar, &fdsrc, win.pSRFwin, &dstPX );

				}

				if( fcs != c )
				{
					fcs = c;
					fsrc.x =  (fcs&7)*fsrc.w + fscx;
					fsrc.y = (fcs>>3)*fsrc.h + fscy;
				}

				gpdBLTs( win.pSRFchar, &fsrc, win.pSRFwin, &dstPX );

			}
		}
		else for( U4 i = 0; i < nMINI; i++, dstPX.x += dstPX.w ) { /// Blit -------------------------
			if( !(i%z) )
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

				gpdBLT( win.pSRFchar, &src, win.pSRFwin, &dstPX );

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
				gpdBLT( win.pSRFchar, &fdsrc, win.pSRFwin, &dstPX );
			}

			if( fcs != c )
			{
				fcs = c;
				fsrc.x = (fcs&7)*src.w + fscx;
				fsrc.y = (fcs>>3)*src.h + fscy;
			}
			//src.x = (c&7)*src.w + scx;
			//src.y = (c>>3)*src.h + scy;

 			gpdBLT( win.pSRFchar, &fsrc, win.pSRFwin, &dstPX );
		}

		/// ON -----------------------------------------------------
		if( crs.id ==win.onDIV.x )
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
				crs.frmDRW( dstPX, src, wh, win.pSRFwin, win.pSRFchar, gpeCLR_white, 0,  (crs.scnZN.au4x2[0]+U4x2(1,0)).pSTRalf4n(sSTR) );
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

		crs.frmDRW( dstPX, src, CRSfrm.a4x2[1], win.pSRFwin, win.pSRFchar, frmC, 0, (U1*)(crs.id == pTD->sDIV ? "EDIT" : "TARGET") );

	}

	static void callDRWtx( gpcTHRD_DRW* pTD )
	{
		gpcWIN &win	= *pTD->pWIN;
		gpcCRS &crs	= *pTD->pCRS;
		U4			nMINI 	= crs.nMINI;
		U1x4*		pMINI	= crs.pMINI;
		SDL_Rect	src = pTD->src;
					//src.w--;
					//src.h--;
					//src.h = pTD->src.h-1; // 	= pTD->src,
		SDL_Rect	fsrc	= src,
					fdsrc	= src,
					dstPX	= pTD->dstPX,
					divPX 	= win.wDIVpx( crs.id ).xyWH;
		I4x4		scnZN0	= crs.scnZN0,
					CRSfrm	= crs.gtFRM();
		U4	z = CRSfrm.z,
			cs = 0, 	cc = 255,
			fcs = 0, 	fcc = 255,
			fds = 0, 	fdc = 255,
			cx = src.w*8,
			cy = src.h*32, frmC = pTD->frmC,
			scx, scy,
			fscx, fscy;
		I4x2 wh;
		U1 	c,d,
			sSTR[0x20];

		dstPX.y = divPX.y-dstPX.h;

		//if( dstPX.w != src.w || dstPX.h != src.h )
		{ /// SCALE Blit -------------------------
			for( U4 i = 0; i < nMINI; i++, dstPX.x += dstPX.w ) {
				if( !(i%z) )
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

					gpdBLTstx( win.pTXize, &src, win.pSDLrndr, &dstPX );
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

					gpdBLTstx( win.pTXize, &fdsrc, win.pSDLrndr, &dstPX );

				}

				if( fcs != c )
				{
					fcs = c;
					fsrc.x =  (fcs&7)*fsrc.w + fscx;
					fsrc.y = (fcs>>3)*fsrc.h + fscy;
				}

				gpdBLTstx( win.pTXize, &fsrc, win.pSDLrndr, &dstPX );

			}
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
				crs.frmDRWtx( dstPX, src, wh, win.pSDLrndr, win.pTXize, gpeCLR_white, 0,  (crs.scnZN.au4x2[0]+U4x2(1,0)).pSTRalf4n(sSTR) );
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

		crs.frmDRWtx( dstPX, src, CRSfrm.a4x2[1], win.pSDLrndr, win.pTXize, frmC, 0, (U1*)(crs.id == pTD->sDIV ? "EDIT" : "TARGET") );

	}
};


// FRM 1 up // 2 right // 4 down // 8 left

void gpcCRS::miniINS( U1* pC, U1* pM, U1* pB )
{
	if( miniOFF() )
		return;

	U1x4 *pCRS = pMINI;

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
	if( nCPY = iSTR.y - iSTR.x )
	if( pS = nCPY > 0 ? (char*)apSRC[0]->pA : NULL )
	{
		pS += iSTR.x;
	}
	if( pS ? !nCPY : true )
		return pBUFF;

	gpmMcpyOF( pBUFF, pS, nCPY );
	pBUFF[nCPY] = 0;

	return pBUFF+nCPY;
}



///------------------------------
///
/// 		miniRDYsdl
///
///------------------------------
bool gpcCRS::miniDRW( gpcWIN& win, U1 sDIV, U1 oDIV, U1 dDIV, I4x4 scnXYCR, bool bSHFT ) {
	if( !this )
		return false;

	if( win.apT[id] )
	{
		win.aT[id].join();
		win.apT[id] = NULL;
	}

	SDL_Rect src = win.chrPIC, divPX = win.wDIVpx( id ).xyWH;
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

		//pCRS =
		pMINI = new U1x4[nMINI];
		gpmZnOF( pMINI, nMINI );
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
		gpdBLTs( win.pPICbg->surDRW(), &win.pPICbg->xyOUT.xyWH, win.pSRFwin, &dstPX );
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
				pCp = new U4[nCp+nRp];
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
					if(	gpcSRC* pSRC = win.piMASS->srcFND( xFND ) )
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
							d.a4x2[0].mx(0);
							d.a4x2[1] -= d.a4x2[0];
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

							gpdBLTs( pSRF, &s.xyWH, win.pSRFwin, &d.xyWH );
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

					frmDRW( dstPX, src, wh, win.pSRFwin, win.pSRFchar, frmC, 0, lurdAN.a4x2[0].pSTRalf4n(sSTR) );
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

					frmDRW( dstPX, src, wh, win.pSRFwin, win.pSRFchar, frmC, 0, lurdAN.a4x2[1].pSTRalf4n(sSTR) );
				}

				for( i = 0; i < ie; i++ )
				{
					if( iSKIP.x == i || iSKIP.y == i )
						continue;
					c = i%mZN.z;
					r = i/mZN.z;

					if( pM[i] )
					if( U4 xFND = pM[i] )
					if(	gpcSRC* pSRC = win.piMASS->srcFND( xFND ) )
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
							d.a4x2[0].mx(0);
							d.a4x2[1] -= d.a4x2[0];
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

							gpdBLTs( pSRF, &s.xyWH, win.pSRFwin, &d.xyWH );
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

#ifdef gpdSYSpi
	/// THREADed
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

		win.aT[id] = std::thread( win.apT[id]->callDRW, win.apT[id] );
		return false;
	}
#endif

	/// NO THREAD
	if( !win.apTall[id] )
	{
		win.apTall[id] = new gpcTHRD_DRW( this, win );
	}
	if( win.apTall[id] )
	{
		win.apTall[id]->frmC = frmC;
		win.apTall[id]->sDIV = sDIV;
		win.apTall[id]->oDIV = oDIV;
		win.apTall[id]->eDIV = eDIV;
		win.apTall[id]->src = src;
		win.apTall[id]->dstPX = dstPX;

		win.apTall[id]->callDRW( win.apTall[id] ); //, win.apT[id] );
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

				gpdBLTs( win.pSRFchar, &src, win.pSRFwin, &dstPX );
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
				gpdBLTs( win.pSRFchar, &src, win.pSRFwin, &dstPX );

			}

			src.x = (c&7)*src.w + scx;
			src.y = (c>>3)*src.h + scy;

			gpdBLTs( win.pSRFchar, &src, win.pSRFwin, &dstPX );

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

			gpdBLT( win.pSRFchar, &src, win.pSRFwin, &dstPX );

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
			gpdBLT( win.pSRFchar, &src, win.pSRFwin, &dstPX );
		}

		src.x = (c&7)*src.w + scx;
		src.y = (c>>3)*src.h + scy;

		gpdBLT( win.pSRFchar, &src, win.pSRFwin, &dstPX );
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
			frmDRW( dstPX, src, wh, win.pSRFwin, win.pSRFchar, gpeCLR_white, 0,  (scnZN.au4x2[0]+U4x2(1,0)).pSTRalf4n(sSTR) );
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
bool gpcCRS::miniDRWtx( gpcWIN& win, U1 sDIV, U1 oDIV, U1 dDIV, I4x4 scnXYCR, bool bSHFT ) {
	if( !this )
		return false;

	if( win.apT[id] )
	{
		win.aT[id].join();
		win.apT[id] = NULL;
	}

	SDL_Rect src = win.chrPIC, divPX = win.wDIVpx( id ).xyWH;
	U1x4 *pC64 = (U1x4*)&gpaC64;

	U1 color = gpaCLR2C64[gpeCLR_blue2];
	SDL_SetRenderDrawColor( win.pSDLrndr, pC64[color].z, pC64[color].y, pC64[color].x, pC64[color].w );
	if( bESC ) {
		SDL_RenderFillRect( win.pSDLrndr, &divPX); // 0x000000AA );
		return false;
	}
	SDL_Rect dstPX = divPX, dst2, cWH;

	if( CRSfrm.x > 0 ) {
		dstPX.w = (CRSfrm.x*divPX.w)/CRSfrm.z;
		dstPX.h = divPX.h;
		SDL_RenderFillRect( win.pSDLrndr, &dstPX );
		//SDL_FillRect( win.pSRFwin, &dstPX, gpaC64[14] );
	}

	if( CRSfrm.y > 0 ) {
		dstPX.h = (CRSfrm.y*divPX.h)/CRSfrm.w;
		dstPX.w = divPX.w;
		SDL_RenderFillRect( win.pSDLrndr, &dstPX );
		//
	}

	if( nMINI != CRSfrm.a4x2[1].area() ) {
		gpmDELary( pMINI );
		nMINI = CRSfrm.a4x2[1].area();
		if( !nMINI )
			return false;	// nincsen mérete ki lett kapcsolva?

		//pCRS =
		pMINI = new U1x4[nMINI];
		gpmZnOF( pMINI, nMINI );
		return true; // resized mini, print new
	}




	dstPX = divPX;
	dstPX.x = CRSfrm.x > 0 ? (CRSfrm.x*divPX.w)/CRSfrm.z : 0;
	dstPX.y = CRSfrm.y > 0 ? (CRSfrm.y*divPX.h)/CRSfrm.w : 0;
	dstPX.w -= dstPX.x;
	dstPX.h -= dstPX.y;
	dstPX.x += divPX.x;
	dstPX.y += divPX.y;

	color = gpaCLR2C64[gpeCLR_blue];
	SDL_SetRenderDrawColor( win.pSDLrndr, pC64[color].z, pC64[color].y, pC64[color].x, pC64[color].w );
	if( win.pPICbg ? win.pPICbg->surDRWtx(win.pSDLrndr) : NULL )
		gpdBLTstx( win.pPICbg->surDRWtx(win.pSDLrndr), &win.pPICbg->xyOUT.xyWH, win.pSDLrndr, &dstPX );
	else
		SDL_RenderFillRect( win.pSDLrndr, &dstPX );
		//

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
				pCp = new U4[nCp+nRp];
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
					if(	gpcSRC* pSRC = win.piMASS->srcFND( xFND ) )
					if( pSRC->picID )
					if( gpcPIC* pPIC = win.piMASS->PIC.PIC(pSRC->picID-1) )
					if( SDL_Texture* pTX = pPIC->surDRWtx(win.pSDLrndr) )
					{
						I4x4 s, d, w;
						s.x = s.y = 0;
						s.z = pPIC->pREF->w;
						s.w = pPIC->pREF->h;

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
							d.a4x2[0].mx(0);
							d.a4x2[1] -= d.a4x2[0];
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

							gpdBLTstx( pTX, &s.xyWH, win.pSDLrndr, &d.xyWH );
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

					frmDRWtx( dstPX, src, wh, win.pSDLrndr, win.pTXize, frmC, 0, lurdAN.a4x2[0].pSTRalf4n(sSTR) );
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

					frmDRWtx( dstPX, src, wh, win.pSDLrndr, win.pTXize, frmC, 0, lurdAN.a4x2[1].pSTRalf4n(sSTR) );
				}

				for( i = 0; i < ie; i++ )
				{
					if( iSKIP.x == i || iSKIP.y == i )
						continue;
					c = i%mZN.z;
					r = i/mZN.z;

					if( pM[i] )
					if( U4 xFND = pM[i] )
					if(	gpcSRC* pSRC = win.piMASS->srcFND( xFND ) )
					if( pSRC->picID )
					if( gpcPIC* pPIC = win.piMASS->PIC.PIC(pSRC->picID-1) )
					if( SDL_Texture* pTX = pPIC->surDRWtx(win.pSDLrndr) )
					{
						I4x4 s, d, w;
						s.x = s.y = 0;
						s.z = pPIC->pREF->w;
						s.w = pPIC->pREF->h;

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
							d.a4x2[0].mx(0);
							d.a4x2[1] -= d.a4x2[0];
							//w = d-w;
                            if( w.a4x2[0].x < 0 )
								s.x -= (w.a4x2[0].x*s.z) / w.a4x2[1].x;
							if( w.a4x2[0].y < 0 )
								s.y -= (w.a4x2[0].y*s.w) / w.a4x2[1].y;

							s.a4x2[1] &= d.a4x2[1];
							s.a4x2[1] /= w.a4x2[1];

							d.x *= dstPX.w;
							d.x += divPX.x;

							d.y *= dstPX.h;
							d.y += divPX.y;

							d.z *= dstPX.w;
							d.w *= dstPX.h;

							gpdBLTstx( pTX, &s.xyWH, win.pSDLrndr, &d.xyWH );
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

						frmDRWtx( dstPX, src, wh, win.pSDLrndr, win.pTXize, frmC, 1, NULL );
					}
				}
			}
		}

	}

	/// NO THREAD
	if( !win.apTall[id] )
	{
		win.apTall[id] = new gpcTHRD_DRW( this, win );
	}
	if( win.apTall[id] )
	{
		win.apTall[id]->frmC = frmC;
		win.apTall[id]->sDIV = sDIV;
		win.apTall[id]->oDIV = oDIV;
		win.apTall[id]->eDIV = eDIV;
		win.apTall[id]->src = src;
		win.apTall[id]->dstPX = dstPX;

		win.apTall[id]->callDRWtx( win.apTall[id] ); //, win.apT[id] );
		return false;
	}

	//if( dstPX.w != src.w || dstPX.h != src.h )
	{ /// SCALE Blit -------------------------
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

				gpdBLTstx( win.pTXize, &src, win.pSDLrndr, &dstPX );
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
				gpdBLTstx( win.pTXize, &src, win.pSDLrndr, &dstPX );

			}

			src.x = (c&7)*src.w + scx;
			src.y = (c>>3)*src.h + scy;

			gpdBLTstx( win.pTXize, &src, win.pSDLrndr, &dstPX );

		}
	}


	/// ON ----------------------------------------------pTX-------
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
			frmDRWtx( dstPX, src, wh, win.pSDLrndr, win.pTXize, gpeCLR_white, 0,  (scnZN.au4x2[0]+U4x2(1,0)).pSTRalf4n(sSTR) );
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

	frmDRWtx( dstPX, src, CRSfrm.a4x2[1], win.pSDLrndr, win.pTXize, frmC, 0, (U1*)(id == sDIV ? "EDIT" : "TARGET") );
	return false;
}
void gpcCRS::miniRDYsdl( gpcWIN& win, U1 iDIV, gpcMASS& mass, U1* pE, U1* pB, gpcPIC* pPIC, SDL_Renderer* pRNDR ) {
	if( miniOFF( pPIC, pRNDR ) )
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
	gpmZnOF( pMINI, nMINI );
	if( bESC )
	{
		return;
	}
	I4x4 miniALL = 0;
	if( gpcMAP* pMAP = &mass.mapCR ) {
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
			pSRC = mass.srcFND( xFND );
			dim = pSRC->CRSdim( bNoMini, id );
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
			gpmZnOF( pMINI, nMINI );
			return;
		}

		gpmZnOF( pMINI, nMINI );
		miniALL.y = CRSfrm.y;
		gpeCLR	c16bg = gpeCLR_blue,
				c16fr = gpeCLR_blue2,
				c16ch = gpeCLR_blue2;

		if( pB < pE )
			CRSins( &mass, pE, pB );

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
				pSRC = mass.srcFND( xFND );

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


				pSRC->SRCmini(
									pMINI, //NULL, //aCRS,
									miniALL.a4x2[0],
									min(CRSfrm.z, miniALL.x+(int)pC[c]), min(CRSfrm.w, miniALL.y+(int)pR[r]),
									CRSfrm.z, CRSfrm.z,
									//gpaC64,
									*this,
									c16bg, //c16fr,
									c16ch,
									bNoMini, id
								);

			}
		}

	}
}


