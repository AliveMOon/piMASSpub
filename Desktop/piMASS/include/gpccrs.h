#ifndef GPCCRS_H
#define GPCCRS_H

#include "gpcwin.h"

class gpcCRS
{
	public:
		I4x4 	CRSfrm, AN;
		U1x4	*pMINI, *pCRS;

		U4 nMINI;
		U4x4 aCRS[2];
		bool bESC;

		gpcCRS( gpcWIN& win, I4 mag0 = 1 );
		virtual ~gpcCRS();

		I4x4 	gtFRM() { return CRSfrm; }
		void 	stFRM(I4x4 val) { CRSfrm = val; }

		bool	miniOFF( void );
		void 	miniINS( U1* pC, U1* pM, U1* pB );
		bool	miniDRW( gpcWIN& win, U1 iDIV = 0 );
		void	miniRDY( gpcWIN& win, U1 iDIV, gpcMASS& mass, U1* pE, U1* pB );
		I4x4 srcXYCR( gpcWIN& win, U1 iDIV, gpcMASS& mass, const I4x2& _xy )
		{
			SDL_Rect div = win.wDIV( iDIV );
			I4x2 cr( div.w/CRSfrm.z, div.h/CRSfrm.w );
			I4x4 o = CRSfrm & cr;
			if( !this )
				return o;

			I4x2 xy = _xy - o.a4x2[0];
			o = I4x4( xy, xy/cr );
			if( gpcMAP* pMAP = &mass.mapCR )
			{
				U4	*pC = pMAP->pCOL,
					*pR = pMAP->pROW;

				AN.null();
				for( AN.x = 0; AN.x < pMAP->map44.x; AN.x++ )
				{
					AN.z += pC[AN.x];
					if( o.a4x2[1].x > AN.z )
						continue;
					AN.x++;
					break;
				}
				for( AN.y = 0; AN.y < pMAP->map44.y; AN.y++ )
				{
					AN.w += pR[AN.y];
					if( o.a4x2[1].y < AN.w )
						break;
				}
			}

			return o;

		}
	protected:

	private:

		//gpcCRS(const gpcCRS& other);
		//gpcCRS& operator=(const gpcCRS& other);
};

#endif // GPCCRS_H
