#ifndef GPCWIN_H
#define GPCWIN_H

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

class gpcWIN
{
	public:

		I4x4			winID,
						winSIZ,
						winDIV;

		SDL_Window		*pSDLwin;
		SDL_Renderer	*pSDLrndr;
		SDL_Surface		*pSRFload,
						*pSRFchar,
						*pSRFwin;

		SDL_Rect		chrPIC;

		SDL_Rect	wDIV( U1 iDIV );
		void		gpeWINresize( void );
		gpcWIN( char* pPATH, char* pFILE, I4x4& siz );
		virtual ~gpcWIN();

		U4x2 chrWH( void )
		{
			return U4x2( chrPIC.w, chrPIC.h );
		}

		I4x2 winFRM( U1 iDIV )
		{
			SDL_Rect div = wDIV( iDIV );
			return I4x2( div.w/chrPIC.w, div.h/chrPIC.h );
		}
		U1 mDIV( const I4x2& mXY )
		{
			return (mXY.x/winDIV.x) | ((mXY.y/winDIV.y)<<1);
		}

	protected:

	private:
};

#endif // GPCWIN_H
