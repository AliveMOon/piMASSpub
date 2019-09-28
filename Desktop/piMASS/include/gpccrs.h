#ifndef GPCCRS_H
#define GPCCRS_H

#include "gpcwin.h"

class gpcSRC;

class gpcCRS
{
	public:
		I4x4	scnCR,	scnIN,
				selANCR[2];
		gpcSRC	*apSRC[2];

		U1x4	*pMINI, *pCRS;

		U4 nMINI, anSTR[2];
		U4x4 aCRS[2];
		bool bESC, bED;

		gpcCRS( gpcWIN& win, U1 _id );
		virtual ~gpcCRS();
		U1*		gtUTF8( U1* pBUFF ), id;

		bool CRSbEDget( void )
		{
			return bED;
		}

		bool CRSbEDset( bool b )
		{
			return bED = b;
		}

		bool CRSbEDswitch( void )
		{
			return bED = !bED;
		}


		I4x2 gtFRMxy( gpcWIN& win, U1 iDIV )
		{

			return CRSfrm.a4x2[0];
		}
		I4x2 gtFRMwh( gpcWIN& win, U1 iDIV )
		{

			return CRSfrm.a4x2[1];
		}
		I4x2 stFRMxy( gpcWIN& win, U1 iDIV, I4 x, I4 y )
		{
			CRSfrm.x = x;
			CRSfrm.y = y;
			return CRSfrm.a4x2[0];
		}
		I4x2 addFRMxy( I4 x, I4 y = 0 )
		{
			CRSfrm.x += x;
			CRSfrm.y += y;
			return CRSfrm.a4x2[0];
		}
		I4x2 stFRMwh( gpcWIN& win, U1 iDIV, I4 w, I4 h, I4 mag = 0 )
		{
			if( w > 3 )
				CRSfrm.z = w;

			SDL_Rect div = win.wDIV(iDIV);
			I4 bug = div.w/CRSfrm.z, nBUG = 0;
			if( mag > 0 )
				CRSfrm.z = div.w/bug;

			while( (bug = div.w - CRSfrm.z*bug ) > 8 )
			{
				CRSfrm.z += mag;
				bug = div.w/CRSfrm.z;
				nBUG++;
			}

			if( h > 0 )
				CRSfrm.w = h;
			else
				CRSfrm.w = (CRSfrm.z*div.h*2) / (div.w*3);

			return CRSfrm.a4x2[1];
		}

		bool	miniOFF( void );
		void 	miniINS( U1* pC, U1* pM, U1* pB );
		bool	miniDRW( gpcWIN& win, U1 iDIV = 0 );
		void	miniRDY( gpcWIN& win, U1 iDIV, gpcMASS& mass, U1* pE, U1* pB );

		void CRSsel( gpcWIN& win, gpcCRS& crs, gpcMASS& mass, bool bSH );
		void CRSstpCL( gpcWIN& win, gpcMASS& mass, U1 stp, bool bSH = false, bool bCT = false );
		void CRSstpED( gpcWIN& win, gpcMASS& mass, U1 stp, bool bSH = false, bool bCT = false );

		I4x4 srcXYCR( gpcWIN& win, U1 iDIV, gpcMASS& mass, const I4x2& _xy );
	protected:

	private:
		I4x4 		CRSfrm;
		SDL_Rect 	wDIVfrm;
		//gpcCRS(const gpcCRS& other);
		//gpcCRS& operator=(const gpcCRS& other);
};

#endif // GPCCRS_H
