#ifndef GPCCRS_H
#define GPCCRS_H

#include "gpcwin.h"

class gpcSRC;

class gpcCRS
{
	public:
		I4x4	scnZN0, scnZN,
				scnIN,
				selANIN[2];
		gpcSRC	*apSRC[2];

		U1x4	*pMINI, *pCRS;

		U4 	nMINI, anSTR[2], nCp, nRp;
		I4	*pCp, *pRp;

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
		I4x2 gtFRMwh( void ) //gpcWIN& win ) //, U1 iDIV )
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
		I4x2 stFRMwh(	gpcWIN& win, //U1 iDIV,
						I4 w, I4 h, I4 mag = 0 )
		{
			if( w > 3 )
				CRSfrm.z = w;

			SDL_Rect div = win.wDIV(id);
			I4 bug = div.w/CRSfrm.z, nBUG = 0;
			if( mag > 0 )
				CRSfrm.z = div.w/bug;

			if(mag)
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
		bool	miniDRW( gpcWIN& win, U1 sDIV, U1 oDIV, U1 dDIV, I4x4 scnXYCR, bool bSHFT );
		void	miniRDY( gpcWIN& win, U1 iDIV, gpcMASS& mass, U1* pE, U1* pB );

		void CRSsel( gpcWIN& win, gpcCRS& crs, gpcMASS& mass, bool bSH, U1 src = 4 );
		void CRSstpCL( gpcWIN& win, gpcMASS& mass, U1 stp, bool bSH = false, bool bCT = false );
		void CRSstpED( gpcWIN& win, gpcMASS& mass, U1 stp, bool bSH = false, bool bCT = false );

		I4x4 scnZNCR(	gpcWIN& win, gpcMASS& mass, const I4x2& _xy );
	protected:

	private:
		I4x4 		CRSfrm;
		SDL_Rect 	wDIVfrm;

		void frmDRW( SDL_Rect dst, SDL_Rect src, I4x2 wh, SDL_Surface* pTRG, SDL_Surface* pCHAR, U1 frmC, U4 x, const U1* pSTR )
		{
			if( this ? !frmC : true )
				return;
			bool bHALF = false;
			if( x )
			{
				dst.w *= x;
				dst.h *= x;
			} else {
				x = 1;
				bHALF = true;
			}
			wh /= x;
			U4 nSTR = pSTR ? strlen( (char*)pSTR ) : 0;
			SDL_Rect dst2;
			U4	cx = src.w*8,
				cy = src.h*32,
				scx = (frmC%gpeCLR_violet)*cx,
				scy = (frmC/gpeCLR_violet)*cy,
				c = 1+0xb0;


			src.x = (c%8)*src.w + scx;
			src.y = (c/8)*src.h + scy;

			if( wh.x > (2+nSTR) )
			{
				dst2 = dst;

				dst2.x += (nSTR+1)*dst.w;	// (i%CRSfrm.z)*dst.w + div.x;
				dst2.w *= wh.x - (2+nSTR);
				SDL_BlitScaled( pCHAR, &src, pTRG, &dst2 );
			}

			if( nSTR )
			{
				if( bHALF )
				{
					for( U4 i = 0; i < nSTR; i++ )
					{
						c = pSTR[i] - ' ';
						src.x = (c%8)*src.w + scx;
						src.y = (c/8)*src.h + scy;
						dst2 = dst;

						dst2.w /= 2;
						dst2.h /= 2;

						dst2.x += (i+3)*(dst.w/2);	// (i%CRSfrm.z)*dst.w + div.x;
						if( dst2.y > (dst.h/4) )
							dst2.y -= (dst.h/4);
						SDL_BlitScaled( pCHAR, &src, pTRG, &dst2 );
					}
				}
                else for( U4 i = 0; i < nSTR; i++ )
                {
					c = pSTR[i] - ' ';
					src.x = (c%8)*src.w + scx;
					src.y = (c/8)*src.h + scy;
					dst2 = dst;
					dst2.x += (i+1)*dst.w;	// (i%CRSfrm.z)*dst.w + div.x;

					SDL_BlitScaled( pCHAR, &src, pTRG, &dst2 );
                }
			}


			c = 3+0xb0;
			src.x = (c%8)*src.w + scx;
			src.y = (c/8)*src.h + scy;

			dst2 = dst;

			dst2.x += (wh.x-1)*dst2.w;
			SDL_BlitScaled( pCHAR, &src, pTRG, &dst2 );


			if( wh.y > 2 )
			{
				c = 2+0xb0;
				src.x = (c%8)*src.w + scx;
				src.y = (c/8)*src.h + scy;

				dst2 = dst;

				dst2.x += (wh.x-1)*dst.w;
				dst2.y += dst.h;
				dst2.h *= wh.y-2;
				SDL_BlitScaled( pCHAR, &src, pTRG, &dst2 );

				c = 0x8+0xb0;
				src.x = (c%8)*src.w + scx;
				src.y = (c/8)*src.h + scy;

				dst2 = dst;

				dst2.y += dst.h;
				dst2.h *= wh.y-2;
				SDL_BlitScaled( pCHAR, &src, pTRG, &dst2 );
			}


			c = 6+0xb0;
			src.x = (c%8)*src.w + scx;
			src.y = (c/8)*src.h + scy;

			dst2 = dst;

			dst2.x += (wh.x-1)*dst.w;
			dst2.y += (wh.y-1)*dst.h;
			SDL_BlitScaled( pCHAR, &src, pTRG, &dst2 );


			if( wh.x > 2 )
			{
				c = 4+0xb0;
				src.x = (c%8)*src.w + scx;
				src.y = (c/8)*src.h + scy;

				dst2 = dst;

				dst2.x += dst.w;
				dst2.y += (wh.y-1)*dst.h;
				dst2.w *= wh.x-2;
				SDL_BlitScaled( pCHAR, &src, pTRG, &dst2 );
			}


			c = 0xc+0xb0;
			src.x = (c%8)*src.w + scx;
			src.y = (c/8)*src.h + scy;

			dst2 = dst;
			dst2.y += (wh.y-1)*dst.h;

			SDL_BlitScaled( pCHAR, &src, pTRG, &dst2 );

			c = 0x9+0xb0;
			src.x = (c%8)*src.w + scx;
			src.y = (c/8)*src.h + scy;

			dst2 = dst;

			SDL_BlitScaled( pCHAR, &src, pTRG, &dst2 );

		}
};

#endif // GPCCRS_H
