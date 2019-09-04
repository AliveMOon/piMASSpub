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
						winSIZ;

		SDL_Window		*pSDLwin;
		SDL_Renderer	*pSDLrndr;
		SDL_Surface		*pSRFload,
						*pSRFchar,
						*pSRFwin;

		SDL_Rect		chrPIC;
		U1				bSW;

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
		I4x2& winWH()
		{
			return winDIV.a4x2[1];
		}
	protected:

	private:
		I4x4 winDIV;
};

#endif // GPCWIN_H
