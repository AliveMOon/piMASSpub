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
    SDL_Window * pSDLwin;
    SDL_Renderer * pSDLrndr;
public:
    SDL( U4 flags = 0 );
    virtual ~SDL();
    void draw();
};

SDL::SDL( U4 flags )
{
    if ( SDL_Init( flags ) != 0 )
        throw InitError();

    if ( SDL_CreateWindowAndRenderer( 640, 480, SDL_WINDOW_SHOWN,
                                      &pSDLwin, &pSDLrndr ) != 0 )
        throw InitError();
}

SDL::~SDL()
{
    SDL_DestroyWindow( pSDLwin );
    SDL_DestroyRenderer( pSDLrndr );
    SDL_Quit();
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

    SDL_Rect colorBar;
    colorBar.x = 0; colorBar.y = 0; colorBar.w = 90; colorBar.h = 480;

    // Render a new color bar every 0.5 seconds
    for ( int i = 0; i != sizeof rgb / sizeof *rgb; i += 3, colorBar.x += 90 )
    {
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

int main( int nA, char *apA[] )
{
	if( nA > 0 )
	{
		gppEXEfile = strrchr( strcpy( gpsEXEpath, apA[0] ), '/' );
		if( !gppEXEfile)
			gppEXEfile = gpsEXEpath;
		else if( *gppEXEfile == '/' )
			gppEXEfile++;
		strcpy( gpsEXEname, gppEXEfile );
		*gppEXEfile = 0;

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
				strcpy( gpsMASSpath, apA[i] );
				gppMASSfile = strrchr( gpsMASSpath, '/' );
				if( !gppMASSfile )
					gppMASSfile = gpsMASSpath;
				else if( *gppMASSfile == '/' )
					gppMASSfile++;
				strcpy( gpsMASSname, gppMASSfile );
				cout << "MASS is:" << gpsMASSname << endl;
				continue;
			}
		}

		if( !*gpsMASSname )
		{
			strcpy( gpsMASSname, "pi.mass" );
		}
		U8 s;
		gpMASS.lazy_read( gpsMASSpath, s = -1, -1 );


        SDL sdl( SDL_INIT_VIDEO | SDL_INIT_TIMER );
        sdl.draw();

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
