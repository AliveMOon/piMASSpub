//#include "gpcwin.h"
#include "gpccrs.h"
//#include "gpcSRC.h"
extern U1 gp_s_key_map_sdl[];


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
/*bool gpcWIN::WINvar( gpcREG& out, gpeALF alf )
{
	out.err();
	if( !alf )
		return out.bGD();
	gpcCRS& crs = *apCRS[onDIV.x];
	if( alf < gpeALF_AAAAAA ) {
		if( alf < gpeALF_AAAAA ) {
			if( alf < gpeALF_AAAA ) {
				if( alf < gpeALF_AAA ) {
					if( alf < gpeALF_AA ) {		// A - Z ------------------------------------------

					} else {					// AA - ZZ ----------------------------------------
						switch( alf )
						{
							case gpeALF_IA:
								out = crs.scnZN.x+1;
								break;
							case gpeALF_IN:
								out = crs.scnZN.y;
								break;
							case gpeALF_IX:
								out = crs.scnIN.x;
								break;
							case gpeALF_IY:
								out = crs.scnIN.y;
								break;
							case gpeALF_IW:
								out = crs.scnIN.z;
								break;
							case gpeALF_IH:
								out = crs.scnIN.w;
								break;
							default:
								break;
						}
					}
				} else {						// AAA - ZZZ --------------------------------------
						switch( alf )
						{
							case gpeALF_MLB:
								out = nMB;
								break;
							case gpeALF_MRB:
								out = nMB;
								break;
							case gpeALF_FPS:
								out = mSEC.w;
								break;
							default:
								break;
						}
				}
			} else {							// AAAA - ZZZZ ------------------------------------
						switch( alf )
						{
							case gpeALF_MSEC:
								out = mSEC.x&(~1);
								break;
							default:
								break;
						}
			}
		} else {								// AAAAA - ZZZZZ ----------------------------------

		}
	} else {									// AAAAAA - ZZZZZZ --------------------------------


	}
	return out.bGD();
}*/
SDL_Rect gpcWIN::wDIV( U1 iDIV )
{
	SDL_Rect div;

	switch( iDIV%4)
	{
		case 0:
			div.w = (bSW&0x2) ? winDIV.x : winDIV.z;
			div.h = (bSW&0xc) ? winDIV.y : winDIV.w;

			div.x =
			div.y = 0;
			break;
		case 1:
			div.w = (bSW&0x2) ? winDIV.z-winDIV.x : 0;
			div.h = (bSW&0x8) ? winDIV.y : winDIV.w;

			div.x = winDIV.z - div.w;
			div.y = 0;
			break;
		case 2:
			if( bSW&0x4 )
			{
				div.w = (bSW&0xa) ? winDIV.x : winDIV.z;
				div.h = winDIV.w-winDIV.y;
			} else {
				div.w = div.h = 0;
			}

			div.x = 0;
			div.y = winDIV.w-div.h;
			break;
		case 3:
			if( bSW&0x8 )
			{
				div.w = (bSW&0x4) ? winDIV.x : winDIV.z;
				div.h = winDIV.y;
			} else {
				div.w = div.h = 0;
			}

			div.x = winDIV.z-div.w;
			div.y = winDIV.w-div.h;
			break;
	}
	return div;
}
char gpsSHDRvx[] =
	"#version 120\n"
	"attribute vec2 v_vx;\n"
	"varying vec2 v_uv;\n"
	"void main()\n"
	"{\n"
	"\tgl_Position = vec4( v_vx.xy, 0.125f, 1.0f );\n"
	"\tv_uv = v_vx.xy;\n"
	"}\n\0";
char gpsSHDRfr[] =
	"#version 120\n"
	"varying vec2 v_uv;\n"
	//"uniform sampler2D renderedTexture;\n"
	"void main()\n"
	"{\n"
	"\tgl_FragColor = vec4( v_uv, 1.0, 1.0 );\n"
	//"\tgl_FragColor = texture2D( renderedTexture, UV );\n"
	"}\n\0";
//VBO data
GLfloat aVxD[] =
{
	-0.5f, -0.5f,
	 0.0f, -0.5f,
	 0.0f,  0.5f,
	-0.5f,  0.5f,
};

//IBO data
GLuint aIxD[] = // { 3, 2, 1, 0, };
				{ 0, 1, 2, 3 };

gpcGL::gpcGL( gpcWIN& win )
{
	gpmCLR;
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
	gCntxt = SDL_GL_CreateContext( win.pSDLwin );
	if( !gCntxt )
	{
		cout << endl << "gpcGL init error" << endl;
		return;
	}
	glewExperimental = GL_TRUE;
	glewErr = glewInit();
	if( glewErr != GLEW_OK )
	{
		cout << endl << "gpcGL GLEW_NOK error" << endl;
		return;
	}

	pTXback = SDL_CreateTexture(
									win.pSDLrndr, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
									win.winSIZ.z, win.winSIZ.w
								);

	gVxSucc = GL_FALSE;



}



gpcWIN::gpcWIN( char* pPATH, char* pFILE, char* sNAME, gpcMASS* piM ) //, char* pPATH, char* pFILE )
{
	//ctor
	gpmCLR;
	piMASS = piM;
	SDL_DisplayMode sdlDM;
	SDL_GetCurrentDisplayMode( 0, &sdlDM );
	winSIZ.z = (sdlDM.w*7)/8;
	winSIZ.w = sdlDM.h-64;
	winSIZ.a4x2[0] = winSIZ.a4x2[1];

	winDIV = winSIZ/I4x4(2,2,1,1);
	pSDLwin = SDL_CreateWindow(	"Custom shader with SDL2 renderer!", SDL_WINDOWPOS_CENTERED,
								SDL_WINDOWPOS_CENTERED, winSIZ.z, winSIZ.w, SDL_WINDOW_RESIZABLE );

	SDL_SetHint( SDL_HINT_RENDER_DRIVER, "ope gpmSDL_FreeTX( pGL->pTXback );ngl" );

	pSDLrndr = SDL_CreateRenderer(	pSDLwin, -1,
									SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

	/*if(
		winID.x = SDL_CreateWindowAndRenderer(	winSIZ.z, winSIZ.w,
												SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE|SDL_WINDOW_OPENGL,
												&pSDLwin, &pSDLrndr ) != 0
	)
        throw InitError();*/

	//SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1" );


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

	pTXchar = SDL_CreateTextureFromSurface( pSDLrndr, pSRFchar );
	if( !pTXchar )
		cout << SDL_GetError() << endl;

	chrPIC.x = 8*4;
	chrPIC.y = 32*4;
	chrPIC.w = pSRFchar->w/chrPIC.x;
	chrPIC.h = pSRFchar->h/chrPIC.y;

	gppKEYbuff = gpsKEYbuff;
	gppMOUSEbuff = gpsKEYbuff;

	pHOST = sHOST;
	pUSER = sUSER;

	pGL = true ? new gpcGL( *this ) : NULL;
	if( pGL ? !pGL->pTXback : true )
	{
		gpmDEL(pGL);
		return;
	}

	pGL->VxScmp( gpsSHDRvx );
	if( pGL->gVxSucc != GL_TRUE )
		return;

	pGL->FrScmp( gpsSHDRfr );
	if( pGL->gFrSucc != GL_TRUE )
		return;

	pGL->VxFrLink();
	if( pGL->gPrgSucc != GL_TRUE )
		return;

	pGL->VBOnew( aVxD, gpmN(aVxD)/2, 2 );
	pGL->IBOnew( aIxD, gpmN(aIxD) );

}
void gpcWIN::gpeWINresize( void )
{
	for( U1 i = 0; i < 4; i++ )
	{
		if( apT[i] )
		{
			aT[i].join();
			apT[i] = NULL;
		}

	}

	if( !(pSRFwin = SDL_GetWindowSurface( pSDLwin )) )
		throw InitError();
	SDL_GetWindowSize( pSDLwin, &winSIZ.x, &winSIZ.y );
	winDIV = winSIZ.a4x2[0];
	winDIV.a4x2[0] /= 2;

	if( !pGL )
		return;

    gpmSDL_FreeTX( pGL->pTXback );
    pGL->pTXback = SDL_CreateTexture( pSDLrndr, SDL_PIXELFORMAT_BGRA8888, SDL_TEXTUREACCESS_TARGET, winSIZ.z, winSIZ.w );
	if( pGL->pTXback )
		return;

	gpmDEL(pGL);

}


void gpcWIN::WINrun( const char* pWELLCOME )
{
	U4 scan, bug = 0, nBUG;
	U1 aXY[] = "00", c = 0;

	gppKEYbuff = ( gppMOUSEbuff +  sprintf( (char*)gppMOUSEbuff,pWELLCOME ) );

	while( gppKEYbuff ) {
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
			if( pGL )
			{


				pGL->rndr( pSDLrndr, pSDLwin, mSEC.x );


			}
			else if( pSDLrndr)
				SDL_RenderPresent( pSDLrndr );
			else if( pSDLwin )
				SDL_UpdateWindowSurface( pSDLwin );


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
					crs.miniRDY( *this, srcDIV, *piMASS, gppKEYbuff, pS );
					pS = gppKEYbuff;
				} else {
					iRDY = crs.id;
					while( pE < gppKEYbuff ) {
						switch( *pE )
						{
							case '\v': {
								crs.miniRDY( *this, srcDIV, *piMASS, pE, pS );
								pS = pE+1;
								// tehát ha bent van ki kell lépni a szerkeszttett cellából
								crs.CRSbEDswitch();
							} break;
							case '\t': {
								if( crs.CRSbEDget() )
									break;

								crs.miniRDY( *this, srcDIV, *piMASS, pE, pS );
								if( *pE == '\r' )
								if( pE[1] == '\n' )
									pE++;

								pS = pE+1;
								crs.CRSstpCL( *this, *piMASS, 3, bSHIFT );
							} break;if( pSDLrndr)
							case '\r':
							case '\n': {
								if( crs.CRSbEDget() )
									break;


								crs.miniRDY( *this, srcDIV, *piMASS, pE, pS );
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

								crs.miniRDY( *this, srcDIV, *piMASS, pE, pS );
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

				crs.miniRDY( *this, srcDIV, *piMASS, gppKEYbuff, pS );
				gppKEYbuff = gppMOUSEbuff;
				*gppKEYbuff = 0;
			} else {
				crs.miniINS( gppKEYbuff, gppMOUSEbuff, gpsKEYbuff );
			}

			for( U1 i = 0; i < 4; i++ )
			{
				if( crs.id != i )
					apCRS[i]->miniRDY( *this, srcDIV, *piMASS, gppKEYbuff, gppKEYbuff );

				if( pSDLrndr)
					apCRS[i]->miniDRWtx( *this, srcDIV, onDIV.x, dstDIV, SRCxycr, bSHIFT );
				else
					apCRS[i]->miniDRW( *this, srcDIV, onDIV.x, dstDIV, SRCxycr, bSHIFT );
				//cout <<  (int)i << ":" << (SDL_GetTicks()-mSEC.x) << " " ;
			}
			//if( pSDLrndr)
			//	SDL_RenderPresent( pSDLrndr );

			//SDL_UpdateWindowSurface( pSDLwin );
			//cout << "s" << SDL_GetTicks() << endl;
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
				SRCxycr = apCRS[onDIV.x]->scnZNCR(	*this, *piMASS, mouseXY.a4x2[0] );

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
								I4 mag = -ev.wheel.y;
								SDL_Rect div = wDIV(onDIV.x);
								if( mag < 0 )
								{
									if( apCRS[onDIV.x]->gtFRMwh().x == 4 )
										break;

									if( apCRS[onDIV.x]->gtFRMwh().x < 4 )
									{
										apCRS[onDIV.x]->stFRMwh(
																	*this,
																	4,
																	(4*div.h*2) / (div.w*3)
																);
										nIRQ++;
										break;
									}
								} else {
									if( apCRS[onDIV.x]->gtFRMwh().x == div.w/8 )
										break;

									if( apCRS[onDIV.x]->gtFRMwh().x > div.w/8 )
									{
										apCRS[onDIV.x]->stFRMwh(
																	*this,
																	div.w/8,
																	((div.w>>3)*div.h*2) / (div.w*3)
																);
										nIRQ++;
										break;
									}
								}



								apCRS[onDIV.x]->stFRMwh(
															*this,
															apCRS[onDIV.x]->gtFRMwh().x+mag, 0,
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

						aXY[0] = c = gp_s_key_map_sdl[scan];
						aXY[1] = gp_s_key_map_sdl[scan+0x10];
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
							switch( aXY[1] )
							{
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
							switch( aXY[1] )
							{
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
							switch( aXY[1] )
							{
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
							} else {
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
							}
							bSW |= 1;
							if( div < 4 )
							{
								if( !apCRS[div] )
										apCRS[div] = new gpcCRS( *this, div );

								for( U1 i = 0, sw = bSW; i < 4; i++, sw >>= 1 )
								{
									if( !(sw&1) )
										continue;

									if( !apCRS[i] )
										continue;

									apCRS[i]->stFRMwh( *this, apCRS[i]->gtFRMwh().x, 0 );
								}
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





