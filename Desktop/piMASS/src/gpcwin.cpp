#include "gpcwin.h"

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
SDL_Rect gpcWIN::wDIV( U1 iDIV )
{
	SDL_Rect div;

	switch( iDIV%4)
	{
		case 0:
			div.w = (bSW&0x2) ? winDIV.x : winDIV.z;
			div.h = (bSW&0x4) ? winDIV.y : winDIV.w;

			div.x =
			div.y = 0;
			break;
		case 1:
			div.w = (bSW&0x2) ? winSIZ.z-winDIV.x : 0;
			div.h = (bSW&0x8) ? winDIV.y : winDIV.w;

			div.x = winSIZ.z - div.w;
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

gpcWIN::gpcWIN( char* pPATH, char* pFILE, I4x4& siz )
{
	//ctor
	gpmCLR;
	winDIV = winSIZ = siz;
	winDIV.a4x2[0] *= 4;
	winDIV.a4x2[0] /= 8;
	if( winID.x = SDL_CreateWindowAndRenderer( winSIZ.z, winSIZ.w, SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE, &pSDLwin, &pSDLrndr ) != 0 )
        throw InitError();

	if( !(pSRFwin = SDL_GetWindowSurface( pSDLwin )) )
		throw InitError();

	if( !pPATH )
		return;

	pSRFload = IMG_Load( pPATH );
	pSRFchar = pSRFload;

	if( pSRFchar != pSRFload )
		gpmSDL_FreeSRF( pSRFload );

	chrPIC.x = 8*4;
	chrPIC.y = 32*4;
	chrPIC.w =pSRFchar->w/chrPIC.x;
	chrPIC.h = pSRFchar->h/chrPIC.y;

}
void gpcWIN::gpeWINresize( void )
{
	if( !(pSRFwin = SDL_GetWindowSurface( pSDLwin )) )
		throw InitError();
	SDL_GetWindowSize( pSDLwin, &winSIZ.x, &winSIZ.y );
	winDIV = winSIZ.a4x2[0];
	winDIV.a4x2[0] *= 4;
	winDIV.a4x2[0] /= 8;

	/*winDIV.z = winDIV.x = (winSIZ.x*winDIV.x / winDIV.z);
	winDIV.w = winDIV.y = (winSIZ.y*winDIV.y / winDIV.w);
	winDIV.a4x2[0] *= 4;
	winDIV.a4x2[0] /= 8;*/


}
gpcWIN::~gpcWIN()
{
	gpmSDL_FreeSRF( pSRFload );
	gpmSDL_FreeSRF( pSRFchar );
    SDL_DestroyWindow( pSDLwin );
    SDL_DestroyRenderer( pSDLrndr );
}


