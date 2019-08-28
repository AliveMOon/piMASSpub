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

SDL_Rect gpcWIN::wDIV( U1 nDIV )
{
	SDL_Rect div;
	switch( nDIV%4)
	{
		case 0:
			div.y = div.x = 0;
			div.w = winDIV.x;
			div.h = winDIV.y;
			break;
		case 1:
			div.x = winDIV.x;
			div.w = winSIZ.x-div.x;
			div.y = 0;
			div.h = winDIV.y;
			break;
		case 2:
			div.x = 0;
			div.w = winDIV.x;
			div.y = winDIV.y;
			div.h = winSIZ.x-div.y;
			break;
		case 3:
			div.x = winDIV.x;
			div.w = winSIZ.x-div.x;
			div.y = winDIV.y;
			div.h = winSIZ.x-div.y;
			break;
	}
	return div;
}

gpcWIN::gpcWIN( char* pPATH, char* pFILE, I4x4& siz )
{
	//ctor
	gpmCLR;
	winDIV = winSIZ = siz;
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

	chr.x = 8;
	chr.y = 32;
	chr.w =	pSRFchar->w/chr.x;
	chr.h = pSRFchar->h/chr.y;

}
void gpcWIN::gpeWINresize( void )
{
	if( !(pSRFwin = SDL_GetWindowSurface( pSDLwin )) )
		throw InitError();
	SDL_GetWindowSize( pSDLwin, &winSIZ.x, &winSIZ.y );
	winDIV.z = winDIV.x = winSIZ.x*winDIV.x / winDIV.z;
	winDIV.w = winDIV.y = winSIZ.y*winDIV.y / winDIV.w;



}
gpcWIN::~gpcWIN()
{
	gpmSDL_FreeSRF( pSRFload );
	gpmSDL_FreeSRF( pSRFchar );
    SDL_DestroyWindow( pSDLwin );
    SDL_DestroyRenderer( pSDLrndr );
}


