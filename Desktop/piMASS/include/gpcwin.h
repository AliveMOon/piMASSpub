#ifndef GPCWIN_H
#define GPCWIN_H

//#include "piMASS.h"
//#include "gpcSRC.h"
#include "gpcres.h"

class gpcCRS;
class gpdADR;
//class gpcRES;

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
		gpcMASS*		piMASS;
		gpcCRS			*apCRS[4];

		I4x4			winID,
						winSIZ,
						mouseXY, mouseW,
						SRCxycr, SRCin;

		SDL_Window		*pSDLwin;
		SDL_Renderer	*pSDLrndr;

		SDL_Surface		*pSRFload,
						*pSRFchar,
						*pSRFwin;
		gpcPIC			*pPICbg;

		SDL_Event 		ev;
		SDL_Rect		chrPIC;

		char 	gpsMAINpub[0x100],
				gpsTITLEpub[0x100],
				gpsMASSpath[gpeMXPATH], *gppMASSfile,
				gpsMASSname[0x100];
		U1		gpsKEYbuff[0x100],
				*gppKEYbuff,
				*gppMOUSEbuff;

		I8	gpnEVENT = 0,
			gpnTITLE = 0;

		U4	aKT[0x200],
			srcDIV, dstDIV;
		U4x2 onDIV = srcDIV;
		U4x4	mSEC = 0, // x: new // y: prev // z: elapse // w: fps
				nJDOIT;
		I4 nMOV, nIRQ,  nMB, nMBB, nF;

		bool	bSHIFT, bCTRL,
				bALT, abALT[2];
		U1		bSW;



		SDL_Rect	wDIV( U1 iDIV );
		void		gpeWINresize( void );
		gpcWIN( char* pPATH, char* pFILE, char* sNAME, gpcMASS* piMASS ); //, char* pPATH, char* pFILE ); //, I4x4& siz );
		virtual ~gpcWIN();

		U4 reSCAN( void )
		{
			bSHIFT = 1&(aKT[SDL_SCANCODE_LSHIFT]|aKT[SDL_SCANCODE_RSHIFT]);
			bCTRL = 1&(aKT[SDL_SCANCODE_LCTRL]|aKT[SDL_SCANCODE_RCTRL]);
			abALT[0] = 1&aKT[SDL_SCANCODE_LALT];
			abALT[1] = 1&aKT[SDL_SCANCODE_RALT];
			bALT = abALT[0]|abALT[1];

			return (((U4)bSHIFT)<<9)|((U4)bALT<<10);
		}

		U4x2 chrWH( void )
		{
			return U4x2( chrPIC.w, chrPIC.h );
		}

		I4x2 wFRM( U1 iDIV )
		{
			SDL_Rect div = wDIV( iDIV );
			return I4x2( div.w/chrPIC.w, div.h/chrPIC.h );
		}
		U1 onDIVf( const I4x2& mXY )
		{
			SDL_Rect dim;
			for( U4 i = 0; i < 4; i++ )
			{
				dim = wDIV(i);
				if( dim.x > mXY.x || dim.y > mXY.y )
					continue;
				if( mXY.x-dim.x >= dim.w || mXY.y-dim.y >= dim.h  )
					continue;
				return i;
			}
			return (mXY.x/winDIV.x) | ((mXY.y/winDIV.y)<<1);
		}
		I4x2& winWH()
		{
			return winDIV.a4x2[1];
		}
		void	WINrun( const char* pWELLCOME );
		bool	WINvar( gpcREG& out, gpeALF alf );
	protected:

	private:
		I4x4 winDIV;
};

#endif // GPCWIN_H
