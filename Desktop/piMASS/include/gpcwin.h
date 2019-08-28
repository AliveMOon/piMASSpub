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

		SDL_Rect		chr;

		SDL_Rect	wDIV( U1 nDIV );
		void		gpeWINresize( void );
		gpcWIN( char* pPATH, char* pFILE, I4x4& siz );
		virtual ~gpcWIN();

	protected:

	private:
};

#endif // GPCWIN_H
