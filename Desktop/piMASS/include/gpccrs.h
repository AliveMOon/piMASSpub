#ifndef GPCCRS_H
#define GPCCRS_H

#include "gpcwin.h"

class gpcCRS
{
	public:
		I4x4 	CRSfrm,
				scnAN,	scnIN;
				selANCR[2];
		gpcSRC	*aSRC[2];

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

		void CRSsel( gpcWIN& win, U1 iDIV, gpcMASS& mass, bool bSH )
		{
			SDL_Rect div = win.wDIV( iDIV );
			I4x2 cr( div.w/CRSfrm.z, div.h/CRSfrm.w );

			U4	*pM = pMAP->pMAP,
				an = scnAN.a4x2[0]*I4x2( 1, pMAP->map44.x );
			if( !pM[an] )
				return;
			xFND = pM[i];
			gpcSRC* pSRC = mass.SRCfnd( xFND );
			if( !pSRC )
				return;

			selANCR[1].a4x2[0] = scnAN.a4x2[0];		//AN

			selANCR[1].a4x2[1] = scnIN.a4x2[0]/cr;	//IN
			pSRC->CRSminiCR( selANCR[1].a4x2[1] );


			aSRC[1] = pSRC;

			if( bSH )
				return; // ha le van nyomva a shift akkor meg akarjuk örizni a sel[0]-t.

			selANCR[0] = selANCR[1];
			aSRC[0] = aSRC[1];

		}
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

				scnAN.null();
				for( scnAN.x = 0; scnAN.x < pMAP->map44.x; scnAN.x++ )
				{
					scnAN.z += pC[scnAN.x];
					if( o.z >= scnAN.z )
						continue;

					scnIN.z = pC[scnAN.x]*cr.x;
					scnIN.x = xy.x - (scnAN.z*cr.x - scnIN.z);
					break;
				}
                if( scnAN.x >= pMAP->map44.x )
                {
					scnIN.z = cr.x*9;
					scnIN.x = xy.x - (scnAN.z*cr.x);
					scnAN.x = pMAP->map44.x + 1 + scnIN.x/scnIN.z;
					scnIN.x %= scnIN.z;
				} else
					scnAN.x++; // ALF 'A' == 1

				for( scnAN.y = 0; scnAN.y < pMAP->map44.y; scnAN.y++ )
				{
					scnAN.w += pR[scnAN.y];
					if( o.w >= scnAN.w )
						continue;

					scnIN.w = pR[scnAN.y]*cr.y;
					scnIN.y = xy.y - (scnAN.w*cr.y - scnIN.w);
					break;
				}
				if( scnAN.y >= pMAP->map44.y )
                {
					scnIN.w = cr.y;
					scnIN.y = xy.y - (scnAN.w*cr.y);
					scnAN.y = pMAP->map44.y + scnIN.y/scnIN.w;
					scnIN.y %= scnIN.w;
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
