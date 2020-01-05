#ifndef GPCCRS_H
#define GPCCRS_H

#include "gpcwin.h"

class gpcSRC;

class gpcCRS
{
	public:
		I4x4	scnZN0, scnZN,
				scnIN,
				selANIN[2]; //, edANIN[2];
		gpcSRC	*apSRC[2];

		U1x4	*pMINI, *pLOCK; //, *pCRS;

		U4x2 iSTR;
		U4 	nMINI, //aiSTR[2],
			nCp, nRp;
		I4	*pCp, *pRp;

		U4x4 aCRS[2];
		bool bESC, bED ;


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


		I4x2 gtFRMxy( ) //gpcWIN& win, U1 iDIV )
		{

			return CRSfrm.a4x2[0];
		}
		I4x2& gtFRMwh(  )  //gpcWIN& win ) //, U1 iDIV )
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
						I4 c, I4 r, I4 mag = 0 )
		{
			I4x2 CR = win.wDIVcr(id).a4x2[1];
			if( c > CR.x || r > CR.y )
			{
				CRSfrm.a4x2[1] = CR;
				return CRSfrm.a4x2[1];
			}

			if( c < 3 )
				c = 3;

			r = c*CR.y;
			r /= CR.x;
			CRSfrm.a4x2[1] = CR;
			return CRSfrm.a4x2[1];
/*





			if( w > 3 )
				CRSfrm.z = w;

			SDL_Rect div = win.wDIV(id).xyWH;
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

			return CRSfrm.a4x2[1];*/
		}

		bool	miniLOCK( gpcPIC* pPIC, SDL_Renderer* pRNDR, I4x2 wh );
		void	miniRDYgl( gpcWIN& win, gpcMASS& mass, gpcPIC* pPIC, SDL_Renderer* pRNDR );


		bool	miniOFF( gpcPIC* pPIC = NULL, SDL_Renderer* pRNDR = NULL );

		void 	miniINS( U1* pC, U1* pM, U1* pB );
		bool	miniDRW( gpcWIN& win, U1 sDIV, U1 oDIV, U1 dDIV, I4x4 scnXYCR, bool bSHFT );
		bool	miniDRWtx( gpcWIN& win, U1 sDIV, U1 oDIV, U1 dDIV, I4x4 scnXYCR, bool bSHFT );
		void	miniRDY2( gpcWIN& win, U1 iDIV, gpcMASS& mass, U1* pE, U1* pB, gpcPIC* pPIC = NULL, SDL_Renderer* pRNDR = NULL );
		///------------------------------
		///
		/// 		CRSins
		///
		///------------------------------
		void CRSins( gpcMASS& mass, U1* pE, U1* pB )
		{
			if( pB >= pE )
				return;

			I4x4	selAN0AN1( selANIN[0].a4x2[0], selANIN[1].a4x2[0] ),
					lurdAN = selAN0AN1.lurd();
			if( !lurdAN.x )
				return;

			gpcSRC	tmp, *pSRC, *pS2;

			if( pSRC = mass.SRCnew( tmp, NULL, lurdAN.a4x2[0], -1 ) )
			{
				if( max( iSTR.y, iSTR.x ) > pSRC->nL )
					iSTR.y = iSTR.x = pSRC->nL;

				I4	nSTRT = pSRC->pSRCstart(true)-pSRC->pSRCalloc(true);

				if( iSTR.x < nSTRT )
					iSTR.x = nSTRT;
				if( !bED )
				{
					iSTR.y = pSRC->nL;
				}
				else if( iSTR.y < iSTR.x )
					iSTR.y = iSTR.x;

				I4	nSUB = iSTR.y - iSTR.x,
					nSTR = pE-pB,
					nOL = pSRC->nL,
					nNEW = gpmPAD( nOL+nSTR + 1, 0x10 );

				// több karakter írunk át
				U1	*pOA	= pSRC->nA ? pSRC->pA : NULL,
					*pRIG	= pOA + iSTR.y,
					*pRIGe	= pOA + nOL,
					*pLFT	= (pSRC->pA = new U1[nNEW]) + iSTR.x;

				gpmMEMCPY( pSRC->pA, pOA, iSTR.x );

				for( ; pB < pE; pB++ )
				{
					switch( *pB )
					{
						case '\b':
							if( pLFT > pSRC->pA )
							{
								pLFT--;
								if( pLFT[0] == '\n' )
								if( pLFT >= pSRC->pA )
								if( pLFT[-1] == '\r' )
								{
									pLFT--;
									continue;
								}
							}
							continue;
						case 0x7e:
							continue;
						case 0x7f:
							pB++;
							if( pB < pE )	// ha még van a bufferban abbol deletézünk
								continue;

							if( pRIG < pRIGe )
								pRIG++;	// ha nincsen akkor a jobb oldalbol
							continue;
					}

					*pLFT = *pB;
					pLFT++;

				}
				iSTR.y = iSTR.x = pLFT-pSRC->pA;
				if( pRIG < pRIGe )
				{
					gpmMEMCPY( pLFT, pRIG, pRIGe-pRIG );
					pLFT += pRIGe-pRIG;
				}
				pSRC->nL = pLFT-pSRC->pA;
				pSRC->nA = nNEW;
				pSRC->srcUPDT();
				*pLFT = 0;

				gpmDELary(pOA);
				pSRC->hd(mass);


				/*for( I4x2 s = lurdAN.a4x2[0]; s.y <= lurdAN.w; s.y++ )
				{
					for( s.x = lurdAN.x; s.x <= lurdAN.z; s.x++ )
					{*/

				for( I4x2 s = lurdAN.a4x2[1]; s.y >= lurdAN.y; s.y-- )
				for( s.x = lurdAN.a4x2[1].x; s.x >= lurdAN.x; s.x-- )
				{

					U4 x_fnd = mass.getXFNDan( s );
					pS2 = x_fnd ? mass.SRCfnd( x_fnd ) : NULL;
					if( !pS2 )
					{
						pS2 = mass.SRCnew( tmp, NULL, s, -1 );
						if( !pS2 )
							continue;
					}
					if( pS2 == pSRC )
						continue;


					pS2->SRCcpy( pSRC->pA, pSRC->pA+pSRC->nL );
					pS2->srcUPDT();
				}
			}
		}

		void CRSsel( gpcWIN& win, gpcCRS& crs, gpcMASS& mass, bool bSH, U1 src = 4 );
		void CRSstpCL( gpcWIN& win, gpcMASS& mass, U1 stp, bool bSH = false, bool bCT = false );
		void CRSstpED( gpcWIN& win, gpcMASS& mass, U1 stp, bool bSH = false, bool bCT = false );

		I4x4 scnZNCR(	gpcWIN& win, gpcMASS& mass, const I4x2& _xy );
		I4x4& gtFRM()
		{
			return CRSfrm;
		}
		void frmDRWtx( 	SDL_Rect dst, SDL_Rect src,
						I4x2 wh, SDL_Renderer* pTRG, SDL_Texture* pCHAR,
						U1 frmC, U4 x, const U1* pSTR ) {
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
				scx = (frmC&3)*cx,
				scy = (frmC>>2)*cy,
				c = 1+0xb0;


			src.x = (c&7)*src.w + scx;
			src.y = (c>>3)*src.h + scy;

			if( wh.x > (2+nSTR) )
			{
				dst2 = dst;

				dst2.x += (nSTR+1)*dst.w;	// (i%CRSfrm.z)*dst.w + div.x;
				dst2.w *= wh.x - (2+nSTR);
				gpdBLTstx( pCHAR, &src, pTRG, &dst2 );
			}

			if( nSTR )
			{
				if( bHALF )
				{
					for( U4 i = 0; i < nSTR; i++ )
					{
						c = pSTR[i] - ' ';
						src.x = (c&7)*src.w + scx;
						src.y = (c>>3)*src.h + scy;
						dst2 = dst;

						dst2.w /= 2;
						dst2.h /= 2;

						dst2.x += (i+3)*(dst.w/2);	// (i%CRSfrm.z)*dst.w + div.x;
						if( dst2.y > (dst.h/4) )
							dst2.y -= (dst.h/4);
						gpdBLTstx( pCHAR, &src, pTRG, &dst2 );
					}
				}
                else for( U4 i = 0; i < nSTR; i++ )
                {
					c = pSTR[i] - ' ';
					src.x = (c&7)*src.w + scx;
					src.y = (c>>3)*src.h + scy;
					dst2 = dst;
					dst2.x += (i+1)*dst.w;	// (i%CRSfrm.z)*dst.w + div.x;

					gpdBLTstx( pCHAR, &src, pTRG, &dst2 );
                }
			}


			c = 3+0xb0;
			src.x = (c&7)*src.w + scx;
			src.y = (c>>3)*src.h + scy;

			dst2 = dst;

			dst2.x += (wh.x-1)*dst2.w;
			gpdBLTstx( pCHAR, &src, pTRG, &dst2 );


			if( wh.y > 2 )
			{
				c = 2+0xb0;
				src.x = (c&7)*src.w + scx;
				src.y = (c>>3)*src.h + scy;

				dst2 = dst;

				dst2.x += (wh.x-1)*dst.w;
				dst2.y += dst.h;
				dst2.h *= wh.y-2;
				gpdBLTstx( pCHAR, &src, pTRG, &dst2 );

				c = 0x8+0xb0;
				src.x = (c&7)*src.w + scx;
				src.y = (c>>3)*src.h + scy;

				dst2 = dst;

				dst2.y += dst.h;
				dst2.h *= wh.y-2;
				gpdBLTstx( pCHAR, &src, pTRG, &dst2 );
			}


			c = 6+0xb0;
			src.x = (c&7)*src.w + scx;
			src.y = (c>>3)*src.h + scy;

			dst2 = dst;

			dst2.x += (wh.x-1)*dst.w;
			dst2.y += (wh.y-1)*dst.h;
			gpdBLTstx( pCHAR, &src, pTRG, &dst2 );


			if( wh.x > 2 )
			{
				c = 4+0xb0;
				src.x = (c&7)*src.w + scx;
				src.y = (c>>3)*src.h + scy;

				dst2 = dst;

				dst2.x += dst.w;
				dst2.y += (wh.y-1)*dst.h;
				dst2.w *= wh.x-2;
				gpdBLTstx( pCHAR, &src, pTRG, &dst2 );
			}


			c = 0xc+0xb0;
			src.x = (c&7)*src.w + scx;
			src.y = (c>>3)*src.h + scy;

			dst2 = dst;
			dst2.y += (wh.y-1)*dst.h;

			gpdBLTstx( pCHAR, &src, pTRG, &dst2 );

			c = 0x9+0xb0;
			src.x = (c&7)*src.w + scx;
			src.y = (c>>3)*src.h + scy;

			dst2 = dst;

			gpdBLTstx( pCHAR, &src, pTRG, &dst2 );

		}
		void frmDRW( 	SDL_Rect dst, SDL_Rect src,
						I4x2 wh, SDL_Surface* pTRG, SDL_Surface* pCHAR,
						U1 frmC, U4 x, const U1* pSTR ) {
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
				scx = (frmC&3)*cx,
				scy = (frmC>>2)*cy,
				c = 1+0xb0;


			src.x = (c&7)*src.w + scx;
			src.y = (c>>3)*src.h + scy;

			if( wh.x > (2+nSTR) )
			{
				dst2 = dst;

				dst2.x += (nSTR+1)*dst.w;	// (i%CRSfrm.z)*dst.w + div.x;
				dst2.w *= wh.x - (2+nSTR);
				gpdBLTs( pCHAR, &src, pTRG, &dst2 );
			}

			if( nSTR )
			{
				if( bHALF )
				{
					for( U4 i = 0; i < nSTR; i++ )
					{
						c = pSTR[i] - ' ';
						src.x = (c&7)*src.w + scx;
						src.y = (c>>3)*src.h + scy;
						dst2 = dst;

						dst2.w /= 2;
						dst2.h /= 2;

						dst2.x += (i+3)*(dst.w/2);	// (i%CRSfrm.z)*dst.w + div.x;
						if( dst2.y > (dst.h/4) )
							dst2.y -= (dst.h/4);
						gpdBLTs( pCHAR, &src, pTRG, &dst2 );
					}
				}
                else for( U4 i = 0; i < nSTR; i++ )
                {
					c = pSTR[i] - ' ';
					src.x = (c&7)*src.w + scx;
					src.y = (c>>3)*src.h + scy;
					dst2 = dst;
					dst2.x += (i+1)*dst.w;	// (i%CRSfrm.z)*dst.w + div.x;

					gpdBLTs( pCHAR, &src, pTRG, &dst2 );
                }
			}


			c = 3+0xb0;
			src.x = (c&7)*src.w + scx;
			src.y = (c>>3)*src.h + scy;

			dst2 = dst;

			dst2.x += (wh.x-1)*dst2.w;
			gpdBLTs( pCHAR, &src, pTRG, &dst2 );


			if( wh.y > 2 )
			{
				c = 2+0xb0;
				src.x = (c&7)*src.w + scx;
				src.y = (c>>3)*src.h + scy;

				dst2 = dst;

				dst2.x += (wh.x-1)*dst.w;
				dst2.y += dst.h;
				dst2.h *= wh.y-2;
				gpdBLTs( pCHAR, &src, pTRG, &dst2 );

				c = 0x8+0xb0;
				src.x = (c&7)*src.w + scx;
				src.y = (c>>3)*src.h + scy;

				dst2 = dst;

				dst2.y += dst.h;
				dst2.h *= wh.y-2;
				gpdBLTs( pCHAR, &src, pTRG, &dst2 );
			}


			c = 6+0xb0;
			src.x = (c&7)*src.w + scx;
			src.y = (c>>3)*src.h + scy;

			dst2 = dst;

			dst2.x += (wh.x-1)*dst.w;
			dst2.y += (wh.y-1)*dst.h;
			gpdBLTs( pCHAR, &src, pTRG, &dst2 );


			if( wh.x > 2 )
			{
				c = 4+0xb0;
				src.x = (c&7)*src.w + scx;
				src.y = (c>>3)*src.h + scy;

				dst2 = dst;

				dst2.x += dst.w;
				dst2.y += (wh.y-1)*dst.h;
				dst2.w *= wh.x-2;
				gpdBLTs( pCHAR, &src, pTRG, &dst2 );
			}


			c = 0xc+0xb0;
			src.x = (c&7)*src.w + scx;
			src.y = (c>>3)*src.h + scy;

			dst2 = dst;
			dst2.y += (wh.y-1)*dst.h;

			gpdBLTs( pCHAR, &src, pTRG, &dst2 );

			c = 0x9+0xb0;
			src.x = (c&7)*src.w + scx;
			src.y = (c>>3)*src.h + scy;

			dst2 = dst;

			gpdBLTs( pCHAR, &src, pTRG, &dst2 );

		}
	protected:

	private:
		I4x4 		CRSfrm;
		//SDL_Rect 	wDIVfrm;

};

#endif // GPCCRS_H
