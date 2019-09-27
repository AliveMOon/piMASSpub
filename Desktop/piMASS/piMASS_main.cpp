//~ MIT License
//
//~ Copyright (c) 2019 AliveMOon
//
//~ Permission is hereby granted, free of charge, to any person obtaining a copy
//~ of this software and associated documentation files (the "Software"), to deal
//~ in the Software without restriction, including without limitation the rights
//~ to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//~ copies of the Software, and to permit persons to whom the Software is
//~ furnished to do so, subject to the following conditions:
//
//~ The above copyright notice and this permission notice shall be included in all
//~ copies or substantial portions of the Software.
//
//~ THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//~ IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//~ FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//~ AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//~ LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//~ OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//~ SOFTWARE.

#include "piMASS.h"
#include "gpcSRC.h"
#include "gpccrs.h"

U1 gp_s_key_map_sdl[] =
/*
 0123456789abcdef
*/
"0000            "		"0123abcdefghijkl"
"                "		"mnopqrstuvwxzy12"
"       ://// :'\""		"3456789onebt uoo"
"'\"3''    3ffffff"		"uu2ea0,.-9123456"
"ffffff444444/44_"		"789abc6789abdder"
"___5555555555555"		"ldu3456789abcdef"
"6666'66666666666"		"0123i56789abcdef"
"7777777777777777"		"0123456789abcdef"
"8888888888888888"		"0123456789abcdef"
"9999999999999999"		"0123456789abcdef"
"aaaaaaaaaaaaaaaa"		"0123456789abcdef"
"bbbbbbbbbbbbbbbb"		"0123456789abcdef"
"cccccccccccccccc"		"0123456789abcdef"
"dddddddddddddddd"		"0123456789abcdef"
"xxxxxxxxeeeeeeee"		"0123456789abcdef"
"ffffffffffffffff"		"0123456789abcdef"
// shift
"0000            "		"0123ABCDEFGHIJKL"
"                "		"MNOPQRSTUVWXZY'\""
"       ://// :'\""		"+!%/=()ONEBT UOO" // \n
"'\"3''    3FFFFFF"		"UU2EA0?:_9123456"
"FFFFFF444444/44_"		"789abc6789ABDDER"
"___5555555555555"		"LDU3456789ABCDEF"
"6666'66666666666"		"0123I56789ABCDEF"
"7777777777777777"		"0123456789ABCDEF"
"8888888888888888"		"0123456789ABCDEF"
"9999999999999999"		"0123456789ABCDEF"
"AAAAAAAAAAAAAAAA"		"0123456789ABCDEF"
"BBBBBBBBBBBBBBBB"		"0123456789ABCDEF"
"CCCCCCCCCCCCCCCC"		"0123456789ABCDEF"
"DDDDDDDDDDDDDDDD"		"0123456789ABCDEF"
"xxxxxxxxEEEEEEEE"		"0123456789ABCDEF"
"FFFFFFFFFFFFFFFF"		"0123456789ABCDEF"
// alt
"0000            "		"0123A{&DE[]HIJKL"
"        =       "		"M}OP\\RSTC@|# >~?"
" 2222222/222 222"		"^1234567N9AB DEF"
"333 33 3 3333333"		"012$45;7*9123456"
"FFFFFF444444/44_"		"789abc6789ABDDER"
"___5555555555555"		"LDU3456789ABCDEF"
"6666 66666666666"		"0123<56789ABCDEF"
"7777777777777777"		"0123456789ABCDEF"
"8888888888888888"		"0123456789ABCDEF"
"9999999999999999"		"0123456789ABCDEF"
"AAAAAAAAAAAAAAAA"		"0123456789ABCDEF"
"BBBBBBBBBBBBBBBB"		"0123456789ABCDEF"
"CCCCCCCCCCCCCCCC"		"0123456789ABCDEF"
"DDDDDDDDDDDDDDDD"		"0123456789ABCDEF"
"xxxxxxxxEEEEEEEE"		"0123456789ABCDEF"
"FFFFFFFFFFFFFFFF"		"0123456789ABCDEF"
// shift+alt
"0000            "		"0123ABCDEFGHIJKL"
"              11"		"MNOPQRSTUVWXZYEF"
"22222222 222 222"		"01234567\n9AB DEF"
"3333333333333333"		"0123456789123456"
"FFFFFF444444/44_"		"789abc6789ABDDER"
"___5555555555555"		"LDU3456789ABCDEF"
"6666666666666666"		"0123456789ABCDEF"
"7777777777777777"		"0123456789ABCDEF"
"8888888888888888"		"0123456789ABCDEF"
"9999999999999999"		"0123456789ABCDEF"
"AAAAAAAAAAAAAAAA"		"0123456789ABCDEF"
"BBBBBBBBBBBBBBBB"		"0123456789ABCDEF"
"CCCCCCCCCCCCCCCC"		"0123456789ABCDEF"
"DDDDDDDDDDDDDDDD"		"0123456789ABCDEF"
"xxxxxxxxEEEEEEEE"		"0123456789ABCDEF"
"FFFFFFFFFFFFFFFF"		"0123456789ABCDEF"
;
/*
U1 gp_s_key_map[] =
// simple
"00123456789-=000"
"qwertzuiopőú11as"
"dfghjkléá022yxcv"
"bnm,.-3*3 3fffff"
"fffff44789-456+1"
"230.555ff5555555"
"6666fff666666666"
"7777777777777777"
"8888888888888888"
"9999999999999999"
"aaaaaaaaaaaaaaaa"
"bbbbbbbbbbbbbbbb"
"cccccccccccccccc"
"dddddddddddddddd"
"eeeeeeeeeeeeeeee"
"ffffffffffffffff"
// shift
"00\'\"+!%/=()0-=00"
"QWERTZUIOP1111AS"
"DFGHJKL22222YXCV"
"BNM?:_3*3 3fffff"
"fffff44789-456+1"
"230.555ff5555555"
"6666fff666666666"
"7777777777777777"
"8888888888888888"
"9999999999999999"
"aaaaaaaaaaaaaaaa"
"bbbbbbbbbbbbbbbb"
"cccccccccccccccc"
"dddddddddddddddd"
"eeeeeeeeeeeeeeee"
"ffffffffffffffff"
// alt
"001234567890-=00"
"\\|ERTYUIOP[]11AS"
"D[]HJKL$2'22>#&@"
"{}M;.*3*3 3fffff"
"fffff44789-456+1"
"230.55<ff5555555"
"6666fff666666666"
"7777777777777777"
"8888888888888888"
"9999999999999999"
"aaaaaaaaaaaaaaaa"
"bbbbbbbbbbbbbbbb"
"cccccccccccccccc"
"dddddddddddddddd"
"eeeeeeeeeeeeeeee"
"ffffffffffffffff";






class gpcWINo
{
	SDL_Rect	txt, chr;
	U1x4		*pTXT;
	U4			nTXT, nX,dX;
	I4			winID;
	SDL_Surface		*pSRFload,
					*pSRFchar,
					*pSRFwin;
    SDL_Renderer	*pSDLrndr;
public:
    SDL_Window		*pSDLwin;
	U1x4			*pCRS;
    gpcWINo( U4 flags = 0, char* pPATH = NULL, char*pFILE = NULL );
    virtual ~gpcWINo();
    void draw();
    void TXT_draw();
    void ins( U1* pC, U1* pM, U1* pB  );
};*/



char gpsEXEpath[gpeMXPATH], *gppEXEfile = gpsEXEpath,
	 gpsEXEname[0x100],
	 gpsMASSpath[gpeMXPATH], *gppMASSfile = gpsMASSpath,
	 gpsMASSname[0x100];

gpcLAZY gpMASS;

gpcMASS::gpcMASS( const U1* pU, U8 nU )
{
	gpmCLR;
	if(!nU)
		return;
	if( pU ? !*pU : true )
		return;

	U1	*pS = (U1*)pU,
		*pSe = pS+nU;
	gpcSRC tmp;
	U4 is, n, xadd = 1, id, momLV = 0, mCR, *pMAP;
	nSP = 1;
	U4x4 mCR44;
	while( pS < pSe ? *pS : false )
	{
		tmp.reset( pS, pSe, &pS, aSP44[nSP] );
		if(!tmp.nL)
			continue;
		tmp.IX = nLST;
		if( tmp.bSUB( *this ) )
		{
			momLV = nSP;
            nSP++;
            aSP44[nSP].null();
		}
		else if( tmp.bENTR( *this, aSP44[nSP] ) )
		{
			cout << "[ENTER]"; // << endl;
		}

		if( aSP44[momLV].z < aSP44[nSP].x )
				aSP44[momLV].z = aSP44[nSP].x;
		if( aSP44[momLV].w < aSP44[nSP].y )
				aSP44[momLV].w = aSP44[nSP].y;


        apSP[nSP] = SRCadd( &tmp, xadd, aSPix[nSP], n );

        gpcSRC &spREF = *apSP[nSP];
        if( apSP[momLV] )
        {
			if( !apSP[momLV]->pMAP )
				apSP[momLV]->pMAP = new gpcMAP;
			pMAP = apSP[momLV]->pMAP->MAPalloc( spREF.spc, mCR44 );
        }
        else
			pMAP = mapCR.MAPalloc( spREF.spc, mCR44 );

		spREF.bMAIN( *this, true );

		if( pMAP )
		{
			mCR = spREF.spc.x + spREF.spc.y*mCR44.z;
			pMAP[mCR] = xadd; //aSPix[nSP];
		}

		while( apSP[nSP]->bRET( *this ) )
		{
			//apSP[mom]->space = aSP44[mom];
			apSP[momLV]->retIX = aSPix[nSP];
			momLV--;
			nSP--;
		}


		xadd++;

	}
}
U1	gpsKEYbuff[0x100], *gppKEYbuff = gpsKEYbuff, *gppMOUSEbuff = gpsKEYbuff;
char gpsMAINpub[0x100], gpsTITLEpub[0x100];
#ifdef _WIN64
//int WINAPI WinMain( int nA, char *apA[] )
//int Main(int nA, char **apA )
int main(int nA, char** apA )
#else
int main( int nA, char *apA[] )
#endif
{
	gpf_aALF_init();

	if( nA > 0 )
	{
		gppEXEfile = gpfP2F( gpsEXEpath, gpsEXEname, apA[0] );

		cout << "Start in:" << gpsEXEpath << endl;
		cout << "Exe is:" << gpsEXEname << endl;
		*gppMASSfile = 0;
	}


    try
    {
		if( SDL_Init( SDL_INIT_EVERYTHING ) != 0 )
			throw InitError();

		for( int i = 1; i < nA; i++ )
		{
			cout << apA[i] << endl;
			if( strstr( apA[i], ".mass" ) )
			{
				gppMASSfile = gpfP2F( gpsMASSpath, gpsMASSname, apA[i] );

				cout << "MASS is:" << gpsMASSname << endl;
				continue;
			}
		}

		if( !*gpsMASSname )
		{
			strcpy( gpsMASSname, "pi.mass" );
		}
		strcpy( gppMASSfile, gpsMASSname );
		U8 s;
		gpMASS.lazy_read( gpsMASSpath, s = -1, -1 );

		gpcMASS* piMASS = new gpcMASS( gpMASS.p_alloc, gpMASS.n_load );

		strcpy( gppMASSfile, "mini_char.png" ); //bmp" );0

		I4x4 mouseXY(0,0), mouseW(0), winSIZ(800,600,800,600), SRCxycr(0), SRCin(0);
		gpcWIN win( gpsMASSpath, gppMASSfile, winSIZ ); //SDL_INIT_VIDEO | SDL_INIT_TIMER );
        gpcCRS main_crs( win, 0 ), *apCRS[4];
        U4 iDIV = 0, nDIV = 1, mDIV = iDIV, selDIV = iDIV;

        gpmZ(apCRS);
        apCRS[iDIV] = &main_crs;
        //sdl.draw();
        SDL_Event ev;
        U1 c = 0;
        U1* pKEY; // = (U1*)SDL_GetKeyboardState(NULL);
        U4 aKT[0x200], scan, bug = 0, nBUG;
        gpmZ(aKT);
        U1 aXY[] = "00";

        I4 nMOV, nMAG = 0, nMB = 0, nMBB = 0, nF = 0;
        gppKEYbuff = (
						gppMOUSEbuff +  sprintf( (char*)gppMOUSEbuff,
																	"                                        "
																	"     **** GRANDPACE piMASS V0.1 ****    "
																	"                                        "
																	" 1GB RAM SYSTEM  38911 piMASS BYTE FREE "
																	"                                        "
																	"READY                                   "
																	//"_"
												)
					);
		//----------------------------------------------------
        //
        //						MAIN WHILE
        //
        //		ha gppKEYbuff == NULL akkor zártuk be a programot
        //
        //----------------------------------------------------
        while( gppKEYbuff )
        {
			gpcCRS& crs = apCRS[iDIV] ? *apCRS[iDIV] : main_crs;

			if( (gppKEYbuff != gpsKEYbuff) || nMAG )
			{
				*gppKEYbuff = 0;
				if( piMASS )
				{

					U1	*pS = gppMOUSEbuff,
						*pE = pS;

					if( gppKEYbuff == pS ) //nMAG )
					{
						// nincsen begépelve semmi
						// mondjuk ZOOM, stb..?
						crs.miniRDY(  win, iDIV, *piMASS, gppKEYbuff, pS );
						pS = gppKEYbuff;
					} else {

						while( pE < gppKEYbuff )
						{
							switch( *pE )
							{
								case '\v':
									crs.miniRDY( win, iDIV, *piMASS, pE, pS );
									//pS = pE+1;
									// tehát ha bent van ki kell lépni a szerkeszttett cellából
									crs.CRSbEDswitch();
									break;
								case '\t':
									if( crs.CRSbEDget() )
										break;

									crs.miniRDY( win, iDIV, *piMASS, pE, pS );
									if( *pE == '\r' )
									if( pE[1] == '\n' )
										pE++;

									pS = pE+1;
									crs.CRSstpCL(
													win, *piMASS,
													3, (1&(aKT[SDL_SCANCODE_LSHIFT]|aKT[SDL_SCANCODE_RSHIFT]))
												);



									break;
								case '\r':
								case '\n':
                                    if( crs.CRSbEDget() )
										break;


									crs.miniRDY( win, iDIV, *piMASS, pE, pS );
									if( *pE == '\r' )
									if( pE[1] == '\n' )
										pE++;

									pS = pE+1;
									crs.CRSstpCL(
													win, *piMASS,
													5, (1&(aKT[SDL_SCANCODE_LSHIFT]|aKT[SDL_SCANCODE_RSHIFT]))
												);
									break;


								case 2:	// left
								case 3:	// right
								case 4:	// up
								case 5:	// down
									crs.miniRDY( win, iDIV, *piMASS, pE, pS );
									pS = pE+1;
									if( !crs.CRSbEDget() )
									{
										crs.CRSstpCL(
														win, *piMASS,
														*pE, (1&(aKT[SDL_SCANCODE_LSHIFT]|aKT[SDL_SCANCODE_RSHIFT]))
													);

										break;
									}
									//crs.miniRDY( win, iDIV, *piMASS, pE, pS );
									//pS = pE+1;

									//------------------------------------
									//
									//			CRS MOVE
									//
									//------------------------------------
									crs.CRSstpED(
													win, *piMASS,
													*pE, (1&(aKT[SDL_SCANCODE_LSHIFT]|aKT[SDL_SCANCODE_RSHIFT]))
												);
									break;
							}
							pE++;
						}

					}
					//if( pS < gppKEYbuff )
					crs.miniRDY(win, iDIV, *piMASS, gppKEYbuff, pS );
					gppKEYbuff = gppMOUSEbuff;
					*gppKEYbuff = 0;
				} else {
					crs.miniINS( gppKEYbuff, gppMOUSEbuff, gpsKEYbuff );
				}
				for( U1 i = 0; i < 4; i++ )
				{
					if( iDIV != i )
					{
						if( !apCRS[i] )
							continue;

						apCRS[i]->miniRDY( win, iDIV, *piMASS, gppKEYbuff, gppKEYbuff );
					}
					apCRS[i]->miniDRW( win, i ); //DIV );
				}
				SDL_UpdateWindowSurface( win.pSDLwin );
			}


			gppMOUSEbuff = gppKEYbuff = gpsKEYbuff;
			nMB = SDL_GetMouseState( &mouseXY.x, &mouseXY.y );
			if(
				(
					nMOV =	abs( mouseXY.z-mouseXY.x)+abs( mouseXY.w-mouseXY.y)	// pntr pos
							+abs(nMBB-nMB)										// mBUTTON
							+abs( mouseW.z-mouseW.x)+abs( mouseW.w-mouseW.y)	// mWheel
							+nF
							+nMAG
				) > 0
			)
			{
				gppKEYbuff += sprintf( (char*)gppKEYbuff, "move" );
				gppMOUSEbuff = gppKEYbuff;

				*gpsMAINpub = 0;
				mDIV = win.mDIV( mouseXY.a4x2[0] );
				if( apCRS[mDIV] )
				{

					SRCxycr = apCRS[mDIV]->srcXYCR( win, mDIV, *piMASS, mouseXY.a4x2[0] );

					char *pE = gpsMAINpub + gpfALF2STR( gpsMAINpub, apCRS[mDIV]->scnAN.x );
					pE += sprintf( pE, "%d", apCRS[mDIV]->scnAN.y );
					SRCin = apCRS[mDIV]->scnIN;

					if( (nMBB&1) )
					if( !(nMB&1) )
					{
						// SELECT
						if( 1&(aKT[SDL_SCANCODE_LCTRL]|aKT[SDL_SCANCODE_RCTRL]) )
						{
							//------------------------------
							//
							// 		AN INSERT
							//
							//---------------------
							gppKEYbuff += sprintf( (char*)gppKEYbuff, "%s%s",
														(1&(aKT[SDL_SCANCODE_LSHIFT]|aKT[SDL_SCANCODE_RSHIFT])) ? "#":"",
														gpsMAINpub );
						} else {
							if( iDIV != mDIV )
							if(!(1&(aKT[SDL_SCANCODE_LSHIFT]|aKT[SDL_SCANCODE_RSHIFT])))
								iDIV = mDIV;
							apCRS[iDIV]->CRSsel(
													win, *apCRS[mDIV], *piMASS,
													(1&(aKT[SDL_SCANCODE_LSHIFT]|aKT[SDL_SCANCODE_RSHIFT]))
												);
						}
					}
				}
				*gppKEYbuff = 0;

				//gppKEYbuff +=
								sprintf(
											gpsTITLEpub,
											"-= piMASS::%s"
											" x:%d y:%d, mDIV: %d"
											" xycr:%s AN:%s"
											" IN %s %0.2f %0.2f"
											" wx:%d wy:%d"
											" %d F%d =-"
											" %d, %d",
											gpsMASSname,
											mouseXY.x, mouseXY.y, mDIV,
											SRCxycr.str(gpsMAINpub+0x40),
											gpsMAINpub,
											SRCin.str(gpsMAINpub+0x80),
											(float)SRCin.x/SRCin.z, (float)SRCin.y/SRCin.w,
											mouseW.x, mouseW.y,
											nMB, nF,
											bug, nBUG
										);
				mouseXY.z=mouseXY.x;
				mouseXY.w=mouseXY.y;
				mouseW.z=mouseW.x;
				mouseW.w=mouseW.y;
				nMBB = nMB;

				if( nF )
				{
					nF = 0;
				}
				nMAG = 0;

				SDL_SetWindowTitle( win.pSDLwin, gpsTITLEpub );
			}



			while( SDL_PollEvent( &ev ) )
			{
				switch( ev.type )
				{
					case SDL_MOUSEWHEEL:
						if( 1 & (aKT[SDL_SCANCODE_LCTRL]|aKT[SDL_SCANCODE_RCTRL]) )
						{
							if( ev.wheel.y )
							{
								//------------------------------
								//
								// 		WHEEL ZOOM
								//
								//---------------------
								I4 mag = -ev.wheel.y;
								SDL_Rect div = win.wDIV(mDIV);
								if( mag < 0 )
								{
									if( apCRS[mDIV]->gtFRMwh(win,mDIV).x == 4 )
										break;

									if( apCRS[mDIV]->gtFRMwh(win,mDIV).x < 4 )
									{
										apCRS[mDIV]->stFRMwh(
																win,mDIV,
																4,
																(4*div.h*2) / (div.w*3)
															);
										nMAG = 1;
										break;
									}
								} else {
									if( apCRS[mDIV]->gtFRMwh(win,mDIV).x == div.w/8 )
										break;

									if( apCRS[mDIV]->gtFRMwh(win,mDIV).x > div.w/8 )
									{
										apCRS[mDIV]->stFRMwh( 	win,mDIV,
																div.w/8,
																((div.w/8)*div.h*2) / (div.w*3)
															);

										/*crs.CRSfrm.a4x2[1].x = div.w/8;
										crs.CRSfrm.a4x2[1].y = max( 1, (crs.CRSfrm.a4x2[1].x*div.h*2) / (div.w*3) );*/
										nMAG = 1;
										break;
									}
								}



								apCRS[mDIV]->stFRMwh(
														win,mDIV,
														apCRS[mDIV]->gtFRMwh(win,mDIV).x+mag, 0,
														mag
													);


								nMAG = 1;
								break;
							}
							break;
						}
						//------------------------------
						//
						// 		WHEEL SCROLL
						//
						//---------------------
						if( 1 & (aKT[SDL_SCANCODE_LSHIFT]|aKT[SDL_SCANCODE_RSHIFT]) )
							apCRS[mDIV]->addFRMxy( ev.wheel.y );
						else
							apCRS[mDIV]->addFRMxy( 0, ev.wheel.y );
						nMAG = 1;

						mouseW.x += ev.wheel.x;
						mouseW.y += ev.wheel.y;
						break;
					case SDL_QUIT:
						gppKEYbuff = NULL;
						continue;
					case SDL_KEYDOWN:
						aKT[ev.key.keysym.scancode] = ev.key.timestamp|1;
						break;
					case SDL_KEYUP:

						aKT[ev.key.keysym.scancode] = ev.key.timestamp;
						aKT[ev.key.keysym.scancode] &= ~1;

						scan = ev.key.keysym.scancode&0xff;
						// az SDL scan codjábol csinál egy 0x20*y öszeget
						// ami a táblázatban a kívánt karakterre fog mutatni
						scan = (scan%0x10) + (scan/0x10)*0x20;

						// SHIFT & ALT modosíthatja
						// tehát 0x800-as táblázatban tud válogatni
						if( 1 & (aKT[SDL_SCANCODE_LSHIFT]|aKT[SDL_SCANCODE_RSHIFT]) )
							scan |= 0x200;
						if( 1 & (aKT[SDL_SCANCODE_LALT]|aKT[SDL_SCANCODE_RALT]) )
							scan |= 0x400;

						// a táblázat első 0x10 / 16 a vezérlő kód
						// ' ' sima ASCII
						// - :"' - ékezetes betűk
						// fF azok a felső funkció gombok f1f2f3f4 etc...
						// _ kurzor nyilak
						// / azok a szeparátor azaz enter tab cell etc...

						aXY[0] = c = gp_s_key_map_sdl[scan];
						aXY[1] = gp_s_key_map_sdl[scan+0x10];

						break;
					case SDL_WINDOWEVENT:
						if( ev.window.event != SDL_WINDOWEVENT_RESIZED )
							break;
						SDL_GetWindowSize( win.pSDLwin, &winSIZ.x, &winSIZ.y );
						if( !(abs( winSIZ.z-winSIZ.x)+abs( winSIZ.w-winSIZ.y))	)
							break;

						win.gpeWINresize();

						winSIZ.a4x2[1] = winSIZ.a4x2[0];

				}
				if( c == 'x' )
					continue;
				if( c )
				{
					char *pUTF8 = NULL;
					switch( c )
					{
						case ' ':
							if( 1 & (aKT[SDL_SCANCODE_LCTRL]|aKT[SDL_SCANCODE_RCTRL]) )
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

										*gppKEYbuff =
										aXY[1] = 0;
										break;
									case 'x':
									case 'X':
										if( crs.gtUTF8( gppKEYbuff ) > gppKEYbuff )
											SDL_SetClipboardText( (char*)gppKEYbuff );

										*gppKEYbuff = 0;
										aXY[1] = 0x7e;
										break;
								}

							}
							if( !aXY[1] )
								break;
							*gppKEYbuff = aXY[1];
							gppKEYbuff++;
							break;
						// enter tab izé bizé
						case '/':
							{
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
										gppKEYbuff += sprintf( (char*)gppKEYbuff, "\r\n" );
										break;
								}
							}
							break;
						// cursor nyilak
						case '_':
							{
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
							}
							break;
						// ékezetek ----------------
						case '\'':
							{
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
							}
							break;
						case '\"':
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
							break;
						case ':':
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
							break;
						case '=':
							// € izé
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
							break;
						case 'f':
						case 'F':
							{
								//------------------------------
								//
								// 		PANEL SWITCH
								//
								//---------------------
								nF = aXY[1] >= 'a' ?	((aXY[1]-'a')+10) :
														aXY[1]-'0';
								if( nF < 2 )
								{
									win.bSW = 1;
									mDIV = 0;
 								}
								else if( nF < 5 )
								{
									mDIV = nF-1;

									U1 msk = (0x1<<mDIV);
									if( win.bSW&msk )
										win.bSW = (win.bSW&(~msk));
									else {
										win.bSW |= msk;
									}
								}
								win.bSW |= 1;
								//if( iDIV != mDIV )
								{
									//iDIV = mDIV;
									if( !apCRS[mDIV] )
											apCRS[mDIV] = new gpcCRS( win, mDIV );
									for( U1 i = 0, sw = win.bSW; i < 4; i++, sw >>= 1 )
									{
										if( !(sw&1) )
											continue;

										if( !apCRS[i] )
											continue;

										apCRS[i]->stFRMwh( win, i, apCRS[i]->gtFRMwh(win, i).x, 0 );


									}
								}
							}

							break;
						default:
							gppKEYbuff += sprintf( (char*)gppKEYbuff, "%s", aXY );
					}

					c = 0;
				}
			}

        }

        return 0;
    }
    catch ( const InitError & err )
    {
        std::cerr << "Error while initializing SDL:  "
                  << err.what()
                  << std::endl;
    }

    return 1;
}
