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
"       :/2// :'\""		"3456789on9bt uoo"
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
"       :/2// :'\""		"+!%/=()ON9BT UOO" // \n
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



/*InitError::InitError() :
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
}*/


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
};


/*gpcWIN::gpcWIN( U4 flags, char* pPATH, char* pFILE )
{






	if( !(pSRFwin = SDL_GetWindowSurface( pSDLwin )) )
		throw InitError();

	if( !pPATH )
		return;

	pSRFload = IMG_Load( pPATH ); // SDL_LoadBMP( pPATH );
	pSRFchar = pSRFload; //SDL_ConvertSurface( pSRFload, pSRFwin->format, 0 );

	if( pSRFchar != pSRFload )
		gpmSDL_FreeSRF( pSRFload );
	chr.x = 8;
	chr.y = 32;
	chr.w =	pSRFchar->w/chr.x;
	chr.h = pSRFchar->h/chr.y;
	nX = 2, dX = 2;
	txt.x = ((txt.w/chr.w)*nX)/dX;
	txt.y = ((txt.h/chr.h)*nX)/dX;
	pCRS = pTXT = new U1x4[nTXT = txt.x*txt.y];
	gpmZn( pTXT, nTXT );

	pTXT[0] = U1x4(255,255,255,'!'-' ');
	gpfMEMSET( pTXT+1, 10, pTXT, sizeof(*pTXT) );
}

gpcWIN::~gpcWIN()
{
	//if( pSRFchar != pSRFload )
	gpmSDL_FreeSRF( pSRFload );
	gpmSDL_FreeSRF( pSRFchar );
    SDL_DestroyWindow( pSDLwin );
    SDL_DestroyRenderer( pSDLrndr );
    SDL_Quit();
}*/



/*void gpcWIN::draw()
{
    // Clear the window with a black background
    SDL_SetRenderDrawColor( pSDLrndr, 0, 0, 0, 255 );
    SDL_RenderClear( pSDLrndr );

    // Show the window
    SDL_RenderPresent( pSDLrndr );

    int rgb[] = { 203, 203, 203, // Gray
                  254, 254,  31, // Yellow
                    0, 255, 255, // Cyan
                    0, 254,  30, // Green
                  255,  16, 253, // Magenta
                  253,   3,   2, // Red
                   18,  14, 252, // Blue
                    0,   0,   0  // Black
                };

    SDL_Rect colorBar, src, dst;
    colorBar.x = 0; colorBar.y = 0; colorBar.w = 90; colorBar.h = 480;

	src = dst = chr;
	U1 c;

    // Render a new color bar every 0.5 seconds
    for ( int i = 0; i != sizeof rgb / sizeof *rgb; i += 3, colorBar.x += 90 )
    if(true)
    {
		SDL_FillRect( pSRFwin, &colorBar, SDL_MapRGB(pSRFwin->format, rgb[i], rgb[i + 1], rgb[i + 2] ) );

		TXT_draw();

		SDL_UpdateWindowSurface( pSDLwin );
		SDL_Delay( 500 );
    } else {
        SDL_SetRenderDrawColor( pSDLrndr, rgb[i], rgb[i + 1], rgb[i + 2], 255 );
        SDL_RenderFillRect( pSDLrndr, &colorBar );
        SDL_RenderPresent( pSDLrndr );
        SDL_Delay( 500 );
    }



}*/
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
	U4 is, n, xadd = 1, id, mom = 0;
	nSP = 1;

	while( pS < pSe ? *pS : false )
	{
		tmp.reset( pS, pSe, &pS, aSP44[nSP] );
		if(!tmp.nL)
			continue;
		tmp.IX = nLST;
		if( tmp.bSUB( *this ) )
		{
			mom = nSP;
            nSP++;
            aSP44[nSP].null();
		}
		else if( tmp.bENTR( *this, aSP44[nSP] ) )
		{
			cout << "[ENTER]"; // << endl;
		}

		if( aSP44[mom].z < aSP44[nSP].x )
				aSP44[mom].z = aSP44[nSP].x;
		if( aSP44[mom].w < aSP44[nSP].y )
				aSP44[mom].w = aSP44[nSP].y;

        apSP[nSP] = add( &tmp, xadd, aSPix[nSP], n );

		apSP[nSP]->bMAIN( *this, true );

		while( apSP[nSP]->bRET( *this ) )
		{
			//apSP[mom]->space = aSP44[mom];
			apSP[mom]->retIX = aSPix[nSP];
			mom--;
			nSP--;
		}

		xadd++;

	}
}
U1 gpsKEYbuff[0x100], *gppKEYbuff = gpsKEYbuff, *gppMOUSEbuff;
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

		gpcMASS* pSRCc = new gpcMASS( gpMASS.p_alloc, gpMASS.n_load );

		strcpy( gppMASSfile, "mini_char.png" ); //bmp" );

		I4x4 mouseXY(0,0), mouseW(0), winSIZ(640,480,640,480);
		gpcWIN win( gpsMASSpath, gppMASSfile, winSIZ ); //SDL_INIT_VIDEO | SDL_INIT_TIMER );
        gpcCRS main_crs( win ), *apCRS[4];
        gpmZ(apCRS);
        U4 nDIV = 0;
        apCRS[nDIV] = &main_crs;
        //sdl.draw();
        SDL_Event ev;
        U1 c = 0;
        U1* pKEY; // = (U1*)SDL_GetKeyboardState(NULL);
        U4 aKT[0x200], scan;
        gpmZ(aKT);
        U1 aXY[] = "00";

        I4 nMOV, nMAG = 0, nMB = 0, nMBB = 0, nF = 0;
        while( gppKEYbuff )
        {
			gpcCRS& crs = apCRS[nDIV] ? *apCRS[nDIV] : main_crs;
			if( gppKEYbuff != gpsKEYbuff )
			{
				*gppKEYbuff = 0;
				crs.MINI_ins( gppKEYbuff, gppMOUSEbuff, gpsKEYbuff );
				crs.MINI_draw( win, nDIV );
				SDL_UpdateWindowSurface( win.pSDLwin );
			}


			gppMOUSEbuff = gppKEYbuff = gpsKEYbuff;
			nMB = SDL_GetMouseState( &mouseXY.x, &mouseXY.y );
			if( (
					nMOV =	abs( mouseXY.z-mouseXY.x)+abs( mouseXY.w-mouseXY.y)	// pntr pos
							+abs(nMBB-nMB)										// mBUTTON
							+abs( mouseW.z-mouseW.x)+abs( mouseW.w-mouseW.y)	// mWheel
							+nF
							+nMAG
				) > 0 )
			{
				gppKEYbuff += sprintf( (char*)gppKEYbuff, "x:%d y:%d wx:%d wy:%d %d F%d    .", mouseXY.x, mouseXY.y, mouseW.x, mouseW.y, nMB, nF );
				mouseXY.z=mouseXY.x;
				mouseXY.w=mouseXY.y;
				mouseW.z=mouseW.x;
				mouseW.w=mouseW.y;
				nMBB = nMB;
				gppMOUSEbuff = gppKEYbuff;
				*gppKEYbuff = 0;
				if( nF )
				{

					nF = 0;
				}
				nMAG = 0;
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
								I4 zm = min( crs.frm.z, crs.frm.w ), zd = zm, mag = -ev.wheel.y;
								if( zm < 9 )
								if( mag < 0 )
									break;
								if( zm > 512 )
								if( mag > 0 )
									break;
								zm += mag;
								crs.frm.z *= zm;
								crs.frm.w *= zm;
								crs.frm.z /= zd;
								crs.frm.w /= zd;
								nMAG = 1;
							}
							break;
						}
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

						winSIZ.z = winSIZ.x;
						winSIZ.w = winSIZ.y;
				}
				if( c == 'x' )
					continue;
				if( c )
				{
					char *pUTF8 = NULL;
					switch( c )
					{
						case ' ':
							*gppKEYbuff = aXY[1];
							gppKEYbuff++;
							break;
						// enter tab izé bizé
						case '/':

							break;
						// cursor nyilak
						case '_':

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
								nF = aXY[1] >= 'a' ?	((aXY[1]-'a')+10) :
														aXY[1]-'0';
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
