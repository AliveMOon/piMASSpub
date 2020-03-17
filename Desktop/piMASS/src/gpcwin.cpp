//#include "gpcwin.h"
#include "gpccrs.h"
//#include "gpcSRC.h"
extern U1 gpsKEYMAP[];


gpcWIN::~gpcWIN()
{
	for( U1 i = 0, e = gpmN(apTall); i < e; i++ )
	{
		if( apT[i] )
		{
			aT[i].join();
			apT[i] = NULL;
		}
		gpmDEL( apTall[i] );
	}

	gpmDEL( pSYNwin );
    gpmDEL( pSYNgt );
    if( pSRFload != pSRFchar )
		gpmSDL_FreeSRF( pSRFload );
	gpmSDL_FreeSRF( pSRFchar );
	gpmSDL_FreeSRF( pSRFiso );
	gpmSDL_FreeSRF( pSRFsnd );


	gpmDEL(pGL);
    SDL_DestroyWindow( pSDLwin );
    SDL_DestroyRenderer( pSDLrndr );
}


InitError::InitError() :
    exception(),
    msg( SDL_GetError() )
{
}

InitError::InitError( const std::string & m ) :
    exception(),
    msg( m )
{
}

InitError::~InitError() throw()
{
}

const char * InitError::what() const throw()
{
    return msg.c_str();
}

//VBO data
static const GLfloat aVxD[] =
{
	-1.0f, -1.0f,
	 1.0f, -1.0f,
	 1.0f,  1.0f,
	-1.0f,  1.0f,
};
static const GLfloat aVX4[] =
{
	-1.0f, -1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,  1.0f,
};

//IBO data
static const GLuint aIxD[] = //{ 3, 2, 1, 0, };
								{ 0, 1, 2, 3 };




U1 gpsHUNtx[] =
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
// 0x80 -------------------
"0123456789abcdef";
void gpcWIN::gpeWINresize( void )
{
	for( U1 i = 0; i < 4; i++ )
	if( apT[i] )
	{
		aT[i].join();
		apT[i] = NULL;
	}

	if( !(pSRFwin = SDL_GetWindowSurface( pSDLwin )) )
		throw InitError();
	SDL_GetWindowSize( pSDLwin, &winSIZ.x, &winSIZ.y );

	winSIZ.a4x2[1] = winSIZ.a4x2[0] / (gpdSIZ2CR*2);

	winDIVcr.a4x2[0] = winSIZ.a4x2[1];
	winDIVcr.a4x2[1] = winDIVcr.a4x2[0]*2;

	winSIZ.a4x2[1] = winDIVcr.a4x2[1] & gpdSIZ2CR;

	if( winSIZ.a4x2[0] != winSIZ.a4x2[1] )
	{
		winSIZ.a4x2[0] = winSIZ.a4x2[1];
		SDL_SetWindowSize( pSDLwin, winSIZ.z, winSIZ.w );
	}
	winDIVpx = winSIZ / I4x4( 2,2,1,1 );

}
gpcWIN::gpcWIN( char* pPATH, char* pFILE, char* sNAME, gpcMASS* piM )  {
	//ctor
	gpmCLR;
	piMASS = piM;
	SDL_DisplayMode sdlDM;
	SDL_GetCurrentDisplayMode( 0, &sdlDM );
	winSIZ.x = (sdlDM.w*7)/8;
	winSIZ.y = sdlDM.h-64;

	winSIZ.a4x2[1] = winSIZ.a4x2[0] / (gpdSIZ2CR*2);

	winDIVcr.a4x2[0] = winSIZ.a4x2[1];
	winDIVcr.a4x2[1] = winDIVcr.a4x2[0]*2;

	winSIZ.a4x2[1] = winDIVcr.a4x2[1] & gpdSIZ2CR;

	if( winSIZ.a4x2[0] != winSIZ.a4x2[1] )
		winSIZ.a4x2[0] = winSIZ.a4x2[1];

	winDIVpx = winSIZ / I4x4( 2,2,1,1 );

	pSDLwin = SDL_CreateWindow(	"Custom shader with SDL2 renderer!", SDL_WINDOWPOS_CENTERED,
								SDL_WINDOWPOS_CENTERED, winSIZ.z, winSIZ.w, SDL_WINDOW_RESIZABLE|SDL_WINDOW_OPENGL );

	SDL_SetHint( SDL_HINT_RENDER_DRIVER, "1" );

	pSDLrndr = SDL_CreateRenderer(	pSDLwin, -1,
									SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

	/*if(
		winID.x = SDL_CreateWindowAndRenderer(	winSIZ.z, winSIZ.w,
												SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE|SDL_WINDOW_OPENGL,
												&pSDLwin, &pSDLrndr ) != 0
	)
        throw InitError();
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1" );
	*/

	if( !(pSRFwin = SDL_GetWindowSurface( pSDLwin )) )
		throw InitError();

	if( !pPATH )
		return;

	gpmSTRCPY( gpsMASSname, sNAME );
	gpmSTRCPY( gpsMASSpath, pPATH );
	gppMASSfile = gpsMASSpath+(pFILE-pPATH);

	pSRFload = IMG_Load( gpsMASSpath );
	pSRFchar = pSRFload;
	if( pSRFchar != pSRFload )
		gpmSDL_FreeSRF( pSRFload );


	chrPIC.x = 8; //*4;
	chrPIC.y = 32; //*4;
	chrPIC.w = pSRFchar->w/chrPIC.x;
	chrPIC.h = pSRFchar->h/chrPIC.y;
	U1x4* pC = (U1x4*)(pSRFchar->pixels + 4*pSRFchar->pitch);
	for( U4 i = 0; i < 0x80; i++ )
		pC[(i>>4)*pSRFchar->w + (i&0xf)] = 0x01010101*(gpsHUNtx[i]-' ');

	gpmSTRCPY( gppMASSfile, gpsMINI_ISO );
	pSRFiso = IMG_Load( gpsMASSpath );
	chrTX.x = 32; //*4;
	chrTX.y = 32; //*4;
	chrTX.z = pSRFiso->w/chrTX.x;
	chrTX.w = pSRFiso->h/chrTX.y;

	gppKEYbuff = gpsKEYbuff;
	gppMOUSEbuff = gpsKEYbuff;

	pHOST = sHOST;
	pUSER = sUSER;

	pGL =  new gpcGL( *this ); //true ? new gpcGL( *this ) : NULL;
	if( !pGL ) //? !pGL->pTXback : true )
		return;

	pGL->GLSLset( I8x2( 0, 0 ) );
	//pGL->VBOnew( aVxD, gpmN(aVxD)/2, 2 );
	pGL->IBOnew( aIxD, gpmN(aIxD) );

}


extern char gpsGLSLfrgREF[];
extern char gpsGLSLfrgLINE[];
void gpcWIN::WINrun( const char* pWELLCOME )
{
	U4 scan, bug = 0, nBUG;
	U1 aXY[] = "00", c = 0;

	gppKEYbuff = ( gppMOUSEbuff +  sprintf( (char*)gppMOUSEbuff,pWELLCOME ) );
	I8x2 GLSLline(0,2), GLSLpic(0,1), GLSLiso(0,0);
	Fx4 tmpFx4;
	while( gppKEYbuff ) {
		usleep(33);
		mSEC.y = mSEC.x;
		mSEC.x = SDL_GetTicks();
		mSEC.z = mSEC.x-mSEC.y;
		mSEC.w = 1000/max( 1, mSEC.z);


		gpcCRS& crs = *apCRS[srcDIV]; // ? *apCRS[srcDIV] : main_crs;
		reSCAN();

		if(
			  (gppKEYbuff != gpsKEYbuff)
			|| nIRQ
			|| ((mSEC.x-nJDOIT.z) > gpdJDOIT_tOUT)
		) {

			for( U1 i = 0; i < 4; i++ )
			{
				if( apT[i] )
				{
					aT[i].join();
					apT[i] = NULL;
				}

			}

			gpcPIC* pPIC = pGL ? piMASS->PIC.PIC( I8x2(gpeALF_TRG,0x10) ) : NULL;
			if( pPIC )
			{
				SDL_Texture *pBGtx = (pPICbg ? pPICbg->surDRWtx(pSDLrndr) : NULL),
							*p_tx;
				pGL->TRG( pSDLrndr, 0, winSIZ.a4x2[0], mSEC.x );
				gpcPIC* pP; gpcBOB* pB;

				for( U1 i = 0, n; i < 4; i++ )
				{
					if( i ? !(bSW&(1<<i)) : false )
						continue;

					I4x4	w = wDIVpx(i), *pI, *pIe;
					I4x2	FRMwh = apCRS[i]->gtFRMwh(), layCR = wDIVcrLAY(),
							bgWH = pPICbg ? pPICbg->txWH.a4x2[1] : I4x2(1280,960);
					if( pPICbg )
						pGL
						->GLSLset( GLSLpic, gpsGLSLfrgREF )
						->glSETbox( w.a4x2[0], w.a4x2[1] )
						->glSETcnl( 0, Fx4(0.25f,0.25f,0.5f,1.0f) )
						->glSETtx( 0, pBGtx, bgWH )->glDRW( w.a4x2[0], FRMwh );

					glViewport( w.x, (winSIZ.w-w.w)-w.y, w.z, w.w );


					n = apCRS[i]->picBG.n_load/sizeof(apCRS[0]->aXYuvPC);
					if( pI = n ? (I4x4*)apCRS[i]->picBG.p_alloc : NULL )
					for( pIe = pI+n*3; pI < pIe; pI += gpmN(apCRS[0]->aXYuvPC) )
					if( pP = piMASS->PIC.PIC(pI[2].a4x2[1].x) )
					{
						pGL->GLSLset( GLSLpic );
						if( p_tx = pP->surDRWtx(pSDLrndr) )
						{
							pGL
							->glSETbox( pI[0], I4x4( 0, 0, winSIZ.z, winSIZ.w ), FRMwh ) // w, FRMwh )
							->glSETcnl( 0, Fx4(0.9f,0.9f,0.9f,1.0f) )
							->glSETtx( 0, p_tx, pP->txWH.a4x2[1] )
							->glDRW( w.a4x2[0], FRMwh );
							//->glDRW( 0, w.a4x2[0], FRMwh );
						}
					}

					n = apCRS[i]->bobBG.n_load/sizeof(apCRS[0]->aXYuvPC);
					if( pI = n ? (I4x4*)apCRS[i]->bobBG.p_alloc : NULL )
					for( pIe = pI+n*3; pI < pIe; pI += gpmN(apCRS[0]->aXYuvPC) )
					if( pP = piMASS->PIC.PIC(pI[2].a4x2[1].x) )
					{
						pGL->GLSLset( GLSLline, gpsGLSLfrgLINE );
						for( U4 nB = pP->nBOB, b = 0, m = 2; b < nB; b++ )
						if( pB = pP->ppBOB[b] )
						{
							pGL
							->glSETbob( m, b*0, pB, pI[0], I4x4( 0, 0, winSIZ.z, winSIZ.w ), FRMwh )
							->glSETcnl( 0, Fx4(1.0f,0.0f,0.0f,1.0f) )
							->glDRW( m, w.a4x2[0], FRMwh );
						}

					}

					glViewport( 0, 0, winSIZ.z, winSIZ.w );
					pGL
					->GLSLset(GLSLiso)
					->glSETbox( w.a4x2[0], w.a4x2[1] )
					->glSETtx( 0, pGL->pTXiso, I4x2(32,32) )													// tex0 -- CHAR set
					->glSETtx( 1, pBGtx, bgWH )																// tex1 -- BG background
					->glSETtx( 2, pPIC->pTX, pPIC->txWH.a4x2[1] )											// tex2 -- MINIiso
					->glDRW( w.a4x2[0], FRMwh );
				}

				pGL->SWP( pSDLwin );
			}

			*gppKEYbuff = 0;
			U1 iRDY = 0x10;


			nJDOIT.z = mSEC.x;
			if( piMASS )
			{

				U1	*pS = gppMOUSEbuff,
					*pE = pS;

				if( gppKEYbuff == pS ) //nMAG )
				{
					// nincsen begépelve semmi
					// mondjuk ZOOM, stb..?
					iRDY = crs.id;
					crs.CRSins( *piMASS, gppKEYbuff, pS );
					//crs.miniRDY( *this, srcDIV, *piMASS, gppKEYbuff, pS, pPIC, pSDLrndr );
					pS = gppKEYbuff;
				} else {
					iRDY = crs.id;
					while( pE < gppKEYbuff ) {
						switch( *pE )
						{
							case '\v': {
								crs.CRSins( *piMASS, pE, pS );
								//crs.miniRDY( *this, srcDIV, *piMASS, pE, pS, pPIC, pSDLrndr );
								pS = pE+1;
								// tehát ha bent van ki kell lépni a szerkeszttett cellából
								crs.CRSbEDswitch();
							} break;
							case '\t': {
								if( crs.CRSbEDget() )
									break;

								crs.CRSins( *piMASS, pE, pS );
								//crs.miniRDY( *this, srcDIV, *piMASS, pE, pS, pPIC, pSDLrndr );
								if( *pE == '\r' )
								if( pE[1] == '\n' )
									pE++;

								pS = pE+1;
								crs.CRSstpCL( *this, *piMASS, 3, bSHIFT );
							} break;
							case '\r':
							case '\n': {
								if( crs.CRSbEDget() )
									break;


								crs.CRSins( *piMASS, pE, pS );
								//crs.miniRDY( *this, srcDIV, *piMASS, pE, pS, pPIC, pSDLrndr );
								if( *pE == '\r' )
								if( pE[1] == '\n' )
									pE++;

								pS = pE+1;
								crs.CRSstpCL( *this, *piMASS, 5, bSHIFT );
							} break;


							case 2:			// left
							case 3:			// right
							case 4:			// up
							case 5:	{ 		// down

								crs.CRSins( *piMASS, pE, pS );
								//crs.miniRDY( *this, srcDIV, *piMASS, pE, pS, pPIC, pSDLrndr );
								pS = pE+1;
								if( !crs.CRSbEDget() )
								{
									crs.CRSstpCL( *this, *piMASS, *pE, bSHIFT );

									break;
								}

								//------------------------------------
								//
								//			CRS MOVE
								//
								//------------------------------------
								crs.CRSstpED(
												*this, *piMASS,
												*pE, bSHIFT // (1&(aKT[SDL_SCANCODE_LSHIFT]|aKT[SDL_SCANCODE_RSHIFT]))
											);

							} break;
						}
						pE++;
					}

				}

				if( pS < gppKEYbuff )
					crs.CRSins( *piMASS, gppKEYbuff, pS );

				crs.miniRDYgl( *this, *piMASS, pPIC, pSDLrndr, bSHIFT );

				gppKEYbuff = gppMOUSEbuff;
				*gppKEYbuff = 0;

				for( U1 i = 0; i < 4; i++ )
				{
					if( crs.id == i )
						continue;
					if( bSW&(1<<i) )
						apCRS[i]->miniRDYgl( *this, *piMASS, pPIC, pSDLrndr, bSHIFT );
				}
			} else {
				crs.miniINS( gppKEYbuff, gppMOUSEbuff, gpsKEYbuff );
			}


			if( !pPIC->unLOCK() )
			for( U1 i = 0; i < 4; i++ )
			{
				if( crs.id != i )
					apCRS[i]->miniRDY2( *this, srcDIV, *piMASS, gppKEYbuff, gppKEYbuff );

				if( pSDLrndr)
					apCRS[i]->miniDRWtx( *this, srcDIV, onDIV.x, dstDIV, SRCxycr, bSHIFT );
				else
					apCRS[i]->miniDRW( *this, srcDIV, onDIV.x, dstDIV, SRCxycr, bSHIFT );
				//std::cout <<  (int)i << ":" << (SDL_GetTicks()-mSEC.x) << " " ;
			}
			//if( pSDLrndr)
			//	SDL_RenderPresent( pSDLrndr );

			//SDL_UpdateWindowSurface( pSDLwin );
			//std::cout << "s" << SDL_GetTicks() <<std::endl;
		}


		//nMB = SDL_GetMouseState( &mouseXY.x, &mouseXY.y );

		gppMOUSEbuff = gppKEYbuff = piMASS->justDOit( *this );

		if(
			(
				nMOV =	abs( mouseXY.z-mouseXY.x)+abs( mouseXY.w-mouseXY.y)	// pntr pos
						+abs(nMBB-nMB)										// mBUTTON
						+abs( mouseW.z-mouseW.x)+abs( mouseW.w-mouseW.y)	// mWheel
						+nF
						+nIRQ
			) > 0
		) {
			gppKEYbuff += sprintf( (char*)gppKEYbuff, "move" );
			gppMOUSEbuff = gppKEYbuff;

			*gpsMAINpub = 0;
			onDIV.y = onDIV.x;
			onDIV.x = onDIVf( mouseXY.a4x2[0] );
			if( apCRS[onDIV.x] )
			{
				SRCxycr = apCRS[onDIV.x]->scnZNCR(	*this, *piMASS, mouseXY.a4x2[0], bSHIFT ? srcDIV : onDIV.x  );

				char *pE = gpsMAINpub + gpfALF2STR( (U1*)gpsMAINpub, apCRS[onDIV.x]->scnZN.x+1 );
				pE += sprintf( pE, "%d", apCRS[onDIV.x]->scnZN.y );
				SRCin = apCRS[onDIV.x]->scnIN;

				if( (nMBB&1) )
				if( !(nMB&1) )
				{
					// RELEASE leftMOUSE button
					// SELECT
					if( bCTRL )
					{
						//------------------------------
						//
						// 		AN INSERT
						//
						//---------------------
						gppKEYbuff += sprintf( (char*)gppKEYbuff, "%s%s",
													bSHIFT ? "#":"",
													gpsMAINpub );
					}
					else if( bSHIFT && ( srcDIV != onDIV.x ) )
					{
						if( dstDIV != onDIV.x )
                        {
							dstDIV = onDIV.x;
                        }
						apCRS[srcDIV]->CRSsel(
												*this, *apCRS[onDIV.x], *piMASS,
												true
											);
					} else {
						if( onDIV.x != srcDIV )
						{
							U4 tmp = srcDIV;
							srcDIV = onDIV.x;
							dstDIV = tmp;
						}

                        /*if(( srcDIV != onDIV.x ) && ( dstDIV != onDIV.x ) )
                        {
							dstDIV = onDIV.x;
                        }*/
						apCRS[srcDIV]->CRSsel(
												*this, *apCRS[srcDIV], *piMASS,
												bSHIFT //, bSHIFT ? 4 : srcDIV
											);
					}


				}
			}
			*gppKEYbuff = 0;

			mouseXY.z=mouseXY.x;
			mouseXY.w=mouseXY.y;
			mouseW.z=mouseW.x;
			mouseW.w=mouseW.y;
			nMBB = nMB;

			if( nF )
				nF = 0;
			nIRQ = 0;
			//SDL_SetWindowTitle( pSDLwin, gpsTITLEpub );

		} else {
			char *pE = gpsMAINpub + gpfALF2STR( (U1*)gpsMAINpub, apCRS[onDIV.x]->scnZN.x+1 );

			pE += sprintf( pE, "%d", apCRS[onDIV.x]->scnZN.y );
			SRCin = apCRS[onDIV.x]->scnIN;

			sprintf(
						gpsTITLEpub,
						"-= piMASS%d::%s"
						" x:%d y:%d, onDIV: %d"
						" xycr:%s AN:%s"
						" IN %s %0.2f %0.2f"
						" wx:%d wy:%d"
						" %d F%d =-"
						" %d, %d",

						mSEC.w,
						gpsMASSname,
						mouseXY.x, mouseXY.y, onDIV.x,
						SRCxycr.str(gpsMAINpub+0x40), gpsMAINpub,
						SRCin.str(gpsMAINpub+0x80),
						(float)SRCin.x/SRCin.z, (float)SRCin.y/SRCin.w,
						mouseW.x, mouseW.y,
						nMB, nF,
						bug, nBUG
					);
		}


		//while( SDL_PollEvent( &ev ) )
		//if( SDL_PeepEvents( &ev, 1, SDL_PEEKEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT ) )
		/*if( mSEC.x-msEVNT < gpdEV_tOUT )
			continue;*/

		SDL_SetWindowTitle( pSDLwin, gpsTITLEpub );
		nMB = SDL_GetMouseState( &mouseXY.x, &mouseXY.y );

		gpnTITLE++;
		msEVNT = mSEC.x;

		U4 tout = gpdSDL_tOUT;
		if( !SDL_WaitEventTimeout( &ev, tout ) )
			continue;
		gpnTITLE = gpnEVENT;

		do { /// EVENTS --------------------------------------------------
			tout = 0;
			gpnEVENT++;
			switch( ev.type )
			{
				case SDL_MOUSEWHEEL: {
						if( bCTRL )
						{
							if( ev.wheel.y )
							{
								//------------------------------
								//
								// 		WHEEL ZOOM
								//
								//---------------------
								I4 mag = apCRS[onDIV.x]->gtFRMwh().x - ev.wheel.y*2;

								apCRS[onDIV.x]->stFRMwh(
															*this,
															mag, 0,
															mag
														);
								nIRQ++;
								break;
							}
							break;
						}
						//------------------------------
						//
						// 		WHEEL SCROLL
						//
						//---------------------
						if( bSHIFT )
							apCRS[onDIV.x]->addFRMxy( ev.wheel.y );
						else
							apCRS[onDIV.x]->addFRMxy( 0, ev.wheel.y );

						nIRQ++;

						mouseW.x += ev.wheel.x;
						mouseW.y += ev.wheel.y;
					} break;
				case SDL_QUIT:
					gppKEYbuff = NULL;
					continue;
				case SDL_KEYDOWN:
					aKT[ev.key.keysym.scancode] = ev.key.timestamp|1;
					reSCAN();
					break;
				case SDL_KEYUP: {
						aKT[ev.key.keysym.scancode] = ev.key.timestamp;
						aKT[ev.key.keysym.scancode] &= ~1;

						scan = ev.key.keysym.scancode&0xff;
						// az SDL scan codjábol csinál egy 0x20*y öszeget
						// ami a táblázatban a kívánt karakterre fog mutatni
						scan = (scan%0x10) + (scan/0x10)*0x20;

						// SHIFT & ALT modosíthatja
						// tehát 0x800-as táblázatban tud válogatni
						scan |= reSCAN();
						/*if( bSHIFT )
							scan |= 0x200;
						if( bALT )
							scan |= 0x400;*/

						// a táblázat első 0x10 / 16 a vezérlő kód
						// ' ' sima ASCII
						// - :"' - ékezetes betűk
						// fF azok a felső funkció gombok f1f2f3f4 etc...
						// _ kurzor nyilak
						// / azok a szeparátor azaz enter tab cell etc...

						aXY[0] = c = gpsKEYMAP[scan];
						aXY[1] = gpsKEYMAP[scan+0x10];
					} break;
				case SDL_WINDOWEVENT:
					if( ev.window.event != SDL_WINDOWEVENT_RESIZED )
						break;
					SDL_GetWindowSize( pSDLwin, &winSIZ.x, &winSIZ.y );
					if( !(abs( winSIZ.z-winSIZ.x)+abs( winSIZ.w-winSIZ.y))	)
						break;

					gpeWINresize();

					winSIZ.a4x2[1] = winSIZ.a4x2[0];

			}
			if( c == 'x' )
				continue;

			if( c )
			{
				char *pUTF8 = NULL;
				switch( c )
				{
					case ' ': {
							if( bCTRL )
							{
								switch( aXY[1] )
								{
									case 'v':
									case 'V':
										// paste
										gppKEYbuff += sprintf( (char*)gppKEYbuff, "%s", SDL_GetClipboardText());
										aXY[1] = 0;
										break;
									case 'c':
									case 'C':
										if( crs.gtUTF8( gppKEYbuff ) > gppKEYbuff )
											SDL_SetClipboardText( (char*)gppKEYbuff );

										*gppKEYbuff = aXY[1] = 0;
										break;
									case 'x':
									case 'X':
										if( crs.gtUTF8( gppKEYbuff ) > gppKEYbuff )
											SDL_SetClipboardText( (char*)gppKEYbuff );

										*gppKEYbuff = 0;
										aXY[1] = 0x7e;
										break;
									case 's':
										strcpy( gppMASSfile, gpsMASSname );
										piMASS->SRCsave( (U1*)gpsMASSpath, (U1*)gppMASSfile );
										*gppKEYbuff = aXY[1] = 0;
										break;
									case 'S':
										// na mencsük ki ami van
										{
											char* pFILE = gppMASSfile+sprintf( gppMASSfile, "HTML/" );
											abALT[0] = 1&aKT[SDL_SCANCODE_LALT];
											abALT[1] = 1&aKT[SDL_SCANCODE_RALT];
											bALT = abALT[0]|abALT[1];

											//bool bALT = ( 1 & (aKT[SDL_SCANCODE_LALT]|aKT[SDL_SCANCODE_RALT]) );
											//strcpy( gppMASSfile, gpsMASSname );
											piMASS->HTMLsave( (U1*)gpsMASSpath, (U1*)pFILE, (U1*)gpsMASSname, bALT );
										}
										*gppKEYbuff = aXY[1] = 0;
										break;
								}

							}
							if( !aXY[1] )
								break;
							*gppKEYbuff = aXY[1];
							gppKEYbuff++;
						} break;
					// enter tab izé bizé
					case '/': {
							switch( aXY[1] )
							{
								case 'e':
								case 'E':
									*gppKEYbuff = '\v';
									gppKEYbuff++;
									break;
								case 't':
								case 'T':
									*gppKEYbuff = '\t';
									gppKEYbuff++;
									break;
								case 'b':
								case 'B':
									*gppKEYbuff = '\b';
									gppKEYbuff++;
									break;
								case 'd':
								case 'D':
									*gppKEYbuff = 0x7f;
									gppKEYbuff++;
									break;
								case 'n':
								case 'N':
									gppKEYbuff += sprintf( (char*)gppKEYbuff, "\r\n " );
									break;
							}
						} break;
					// cursor nyilak
					case '_': {
							switch( aXY[1] )
							{
								case 'l':
								case 'L':
									*gppKEYbuff = 2;
									gppKEYbuff++;
									break;
								case 'r':
								case 'R':
									*gppKEYbuff = 3;
									gppKEYbuff++;
									break;

								case 'u':
								case 'U':
									*gppKEYbuff = 4;
									gppKEYbuff++;
									break;
								case 'd':
								case 'D':
									*gppKEYbuff = 5;
									gppKEYbuff++;
									break;

							}
						} break;
					// ékezetek ----------------
					case '\'': {
							switch( aXY[1] ) {
								case 'A':
									pUTF8 = "\xc3\x81";
									break;
								case 'a':
									pUTF8 = "\xc3\xa1";
									break;
								case 'E':
									pUTF8 = "\xc3\x89";
									break;
								case 'e':
									pUTF8 = "\xc3\xa9";
									break;
								case 'I':
									pUTF8 = "\xc3\x8d";
									break;
								case 'i':
									pUTF8 = "\xc3\xad";
									break;
								case 'O':
									pUTF8 = "\xc3\x93";
									break;
								case 'o':
									pUTF8 = "\xc3\xb3";
									break;
								case 'U':
									pUTF8 = "\xc3\x9a";
									break;
								case 'u':
									pUTF8 = "\xc3\xba";
									break;
								default:
									pUTF8 = "?";
							}
							gppKEYbuff += sprintf( (char*)gppKEYbuff, "%s", pUTF8 );
						} break;
					case '\"': {
							switch( aXY[1] ) {
								case 'O':
									pUTF8 = "\xc5\x90";
									break;
								case 'o':
									pUTF8 = "\xc5\x91";
									break;
								case 'U':
									pUTF8 = "\xc5\xb0";
									break;
								case 'u':
									pUTF8 = "\xc5\xb1";
									break;
								default:
									pUTF8 = "?";
							}
							gppKEYbuff += sprintf( (char*)gppKEYbuff, "%s", pUTF8 );
						}break;
					case ':': {
							switch( aXY[1] ) {
								case 'O':
									pUTF8 = "\xc3\x96";
									break;
								case 'o':
									pUTF8 = "\xc3\xb6";
									break;
								case 'U':
									pUTF8 = "\xc3\x90";
									break;
								case 'u':
									pUTF8 = "\xc3\xbc";
									break;
								default:
									pUTF8 = "?";
							}
							gppKEYbuff += sprintf( (char*)gppKEYbuff, "%s", pUTF8 );
						} break;
					case '=': {
							// € EURO izé
							switch( aXY[1] )
							{
								case 'C':
								case 'c':
										pUTF8 = "\u20AC";
										break;
								default:
									pUTF8 = "?";
							}
							gppKEYbuff += sprintf( (char*)gppKEYbuff, "%s", pUTF8 );
						} break;
					case 'f':
					case 'F': {
							//------------------------------
							//
							// 		PANEL SWITCH
							//
							//---------------------
							nF = aXY[1] >= 'a' ?	((aXY[1]-'a')+10) :
													aXY[1]-'0';
							U1 div = nF-1, msk = (0x1<<(div));
							if( div < 4 )
							{
								if( !div )
								{
									//onDIV.x = 0;

									if( bSHIFT )
									{
										dstDIV = srcDIV = 0;
										bSW = 1;
										div = 4;
									} else {
										if( !srcDIV )
										{
											srcDIV = dstDIV;
											dstDIV = 0;
										} else {
											dstDIV = srcDIV;
											srcDIV = 0;
										}
									}
								}
								else if( div < 4 )
								{
									if( bSHIFT )
									{
										if( srcDIV == div )
										{
											srcDIV = dstDIV;
											dstDIV = 0;
										}
										else if( dstDIV == div )
										{
											dstDIV = 0;
										}
									}
									else if( srcDIV == div )
									{
										srcDIV = dstDIV;
										dstDIV = div;
									} else {
										if( bSW&msk)
										{
											dstDIV = srcDIV;
											srcDIV = div;
										} else
											dstDIV = div;

									}

									//U1 msk = (0x1<<(div));

									if( bSW&msk )
									{
										if( bSHIFT )
										{
											div = 4;
											bSW = (bSW&(~msk));
										}
									} else {
										bSW |= msk;
									}
								}

								bSW |= 1;
								if( div < 4 )
								{
									if( !apCRS[div] )
											apCRS[div] = new gpcCRS( *this, div );
								}
								for( U1 i = 0, sw = bSW; i < 4; i++, sw >>= 1 )
								{
									if( !(sw&1) )
										continue;

									if( !apCRS[i] )
										continue;

									apCRS[i]->stFRMwh( *this, apCRS[i]->gtFRMwh().x, 0 );
								}
								break;
							}



							switch( nF )
							{
								case 5:{	/// F5 ----- COPY
								} break;
								case 6:{	/// F6 ----- MOVE
								} break;
								case 7:{	/// F7 ----- NEW
								} break;
								case 8:{	/// F8 ----- DELETE
								} break;

								case 9:{	/// F9 ----- ?
								} break;
								case 10:{	/// F10 ----- ?
								} break;
								case 11:{	/// F11 ----- ?
								} break;
								case 12:{	/// F12 ----- ?
								} break;

							}



						} break;
					default:
						gppKEYbuff += sprintf( (char*)gppKEYbuff, "%s", aXY );
				}

				c = 0;
			}
		}
		while( SDL_WaitEventTimeout( &ev, tout ) );
		//while( SDL_PollEvent( &ev ) );


	}

}





