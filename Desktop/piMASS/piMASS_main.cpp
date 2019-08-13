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

class InitError : public std::exception
{
    std::string msg;
public:
    InitError();
    InitError( const std::string & );
    virtual ~InitError() throw();
    virtual const char * what() const throw();
};

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

class SDL
{
	SDL_Rect	txt, chr;
	U1x4		*pTXT;
	U4			nTXT;
	SDL_Surface		*pSRFload,
					*pSRFchar,
					*pSRFwin;
    SDL_Window		*pSDLwin;
    SDL_Renderer	*pSDLrndr;
public:
	U1x4		*pCRS;
    SDL( U4 flags = 0, char* pPATH = NULL, char*pFILE = NULL );
    virtual ~SDL();
    void draw();
    void TXT_draw();
    void ins( U1* pU1 = NULL );
};

SDL::SDL( U4 flags, char* pPATH, char* pFILE )
{
    if ( SDL_Init( flags ) != 0 )
        throw InitError();

    if ( SDL_CreateWindowAndRenderer( txt.w = 640, txt.h = 480, SDL_WINDOW_SHOWN, &pSDLwin, &pSDLrndr ) != 0 )
        throw InitError();



	if( !(pSRFwin = SDL_GetWindowSurface( pSDLwin )) )
		throw InitError();

	if( !pPATH )
		return;

	pSRFload = SDL_LoadBMP( pPATH );
	pSRFchar = SDL_ConvertSurface( pSRFload, pSRFwin->format, 0 );
	gpmSDL_FreeSRF( pSRFload );
	chr.x = 8;
	chr.y = 32;
	chr.w =	pSRFchar->w/chr.x;
	chr.h = pSRFchar->h/chr.y;

	txt.x = (txt.w/chr.w)*2;
	txt.y = (txt.h/chr.h)*2;
	pCRS = pTXT = new U1x4[nTXT = txt.x*txt.y];
	gpmZn( pTXT, nTXT );

	pTXT[0] = U1x4(255,255,255,'!'-' ');
	gpfMEMSET( pTXT+1, 10, pTXT, sizeof(*pTXT) );
}

SDL::~SDL()
{
	gpmSDL_FreeSRF( pSRFload );
	gpmSDL_FreeSRF( pSRFchar );
    SDL_DestroyWindow( pSDLwin );
    SDL_DestroyRenderer( pSDLrndr );
    SDL_Quit();
}
void SDL::ins( U1* pC )
{
	if( pC )
	for( ; *pC; pC++ )
	{
		pCRS->w = *pC > ' ' ? *pC - ' ' :  0;
		pCRS++;
	}
	TXT_draw();
	SDL_UpdateWindowSurface( pSDLwin );
}
void SDL::TXT_draw()
{
	SDL_Rect src, dst;
	src = chr;
	dst.w = txt.w/txt.x;
	dst.h = txt.h/txt.y;

	U1 c;
	if( dst.w != src.w || dst.h != src.h )
	{
		for( U4 i = 0; i < nTXT; i++ )
		{
			c = pTXT[i].w;
			if( !c )
				continue;
			src.x = (c%chr.x)*chr.w;
			src.y = (c/chr.x)*chr.h;
			dst.x = (i%txt.x)*dst.w;
			dst.y = (i/txt.x)*dst.h;
			SDL_BlitScaled( pSRFchar, &src, pSRFwin, &dst );
		}
		return;
	}
	for( U4 i = 0; i < nTXT; i++ )
	{
		c = pTXT[i].w;
		if( !c )
			continue;
		src.x = (c%chr.x)*chr.w;
		src.y = (c/chr.x)*chr.h;
		dst.x = (i%txt.x)*chr.w;
		dst.y = (i/txt.x)*chr.h;
		SDL_BlitSurface( pSRFchar, &src, pSRFwin, &dst );
	}
}
void SDL::draw()
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



}
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
U1 gpsKEYbuff[0x100], *gppKEYbuff = gpsKEYbuff;
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

		strcpy( gppMASSfile, "mini_char.bmp" );
        SDL sdl( SDL_INIT_EVERYTHING, gpsMASSpath, gppMASSfile ); //SDL_INIT_VIDEO | SDL_INIT_TIMER );
        sdl.draw();
        SDL_Event ev;
        U1 c = 0;
        U1* pKEY; // = (U1*)SDL_GetKeyboardState(NULL);
        U4 aKT[0x200];
        gpmZ(aKT);
        while( gppKEYbuff )
        {
			gppKEYbuff = gpsKEYbuff;
			while( SDL_PollEvent( &ev ) )
			{
				switch( ev.type )
				{
					case SDL_QUIT:
						gppKEYbuff = NULL;
						continue;
					case SDL_KEYDOWN:
						aKT[ev.key.keysym.scancode] = ev.key.timestamp|1;
						break;
					case SDL_KEYUP:
						aKT[ev.key.keysym.scancode] = ev.key.timestamp&(~1);
						if( ev.key.keysym.sym >= 0x80 )
						{
							switch( ev.key.keysym.sym )
							{
								case SDLK_UP:
									c = 4;
									break;
								case SDLK_RIGHT:
									c = 3;
									break;
								case SDLK_LEFT:
									c = 2;
									break;
								case SDLK_DOWN:
									c = 1;
									break;

								case SDLK_LSHIFT:
								case SDLK_RSHIFT:
								case SDLK_LALT:
								case SDLK_RALT:
								case SDLK_LCTRL:
								case SDLK_RCTRL:
									break;
								default:
									gppKEYbuff += sprintf( (char*)gppKEYbuff, "%s", pKEY );
							}
							break;
						}

						pKEY = (U1*)SDL_GetKeyName( ev.key.keysym.sym );
						if( pKEY[1] )
						{
							c = ev.key.keysym.sym;

						} else {
							// ez a bilencs kiosztások változása miatt kell
							c = *pKEY;
							if(
								!(
									(
										 aKT[SDL_SCANCODE_LSHIFT]
										|aKT[SDL_SCANCODE_RSHIFT]
									)
								&1)
							)
							if( c >= 'A' && c <= 'Z' )
							{
								c += 'a'-'A';
							}
						}
						break;
				}
				if( c )
				{
					*gppKEYbuff = c;
					gppKEYbuff++;
					c = 0;
				}
			}
			if( gppKEYbuff == gpsKEYbuff )
				continue;

			*gppKEYbuff = 0;
			sdl.ins( gpsKEYbuff );
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
