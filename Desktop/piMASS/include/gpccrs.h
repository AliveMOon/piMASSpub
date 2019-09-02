#ifndef GPCCRS_H
#define GPCCRS_H

#include "gpcwin.h"

class gpcSRC;

class gpcCRS
{
	public:
		I4x4 	CRSfrm,
				scnAN,	scnIN,
				selANCR[2];
		gpcSRC	*apSRC[2];

		U1x4	*pMINI, *pCRS;

		U4 nMINI, anSTR[2];
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

		void CRSsel( gpcWIN& win, U1 iDIV, gpcMASS& mass, bool bSH );

		I4x4 srcXYCR( gpcWIN& win, U1 iDIV, gpcMASS& mass, const I4x2& _xy );
	protected:

	private:

		//gpcCRS(const gpcCRS& other);
		//gpcCRS& operator=(const gpcCRS& other);
};

#endif // GPCCRS_H
