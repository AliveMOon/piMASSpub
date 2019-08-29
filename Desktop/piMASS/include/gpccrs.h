#ifndef GPCCRS_H
#define GPCCRS_H

#include "gpcwin.h"

class gpcCRS
{
	public:
		I4x4 	frm;
		U1x4	*pMINI, *pCRS;

		U4 nMINI;
		U4x4 aCRS[2];

		gpcCRS( gpcWIN& win, I4 mag0 = 1 );
		virtual ~gpcCRS();

		I4x4 	gtFRM() { return frm; }
		void 	stFRM(I4x4 val) { frm = val; }

		bool	miniOFF( void );
		void 	miniINS( U1* pC, U1* pM, U1* pB );
		bool	miniDRW( gpcWIN& win, U1 iDIV = 0 );
		void	miniRDY( gpcWIN& win, U1 iDIV, gpcMASS& mass, U1* pE, U1* pB );
	protected:

	private:

		//gpcCRS(const gpcCRS& other);
		//gpcCRS& operator=(const gpcCRS& other);
};

#endif // GPCCRS_H
