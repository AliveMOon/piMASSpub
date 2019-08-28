#ifndef GPCCRS_H
#define GPCCRS_H

#include "gpcwin.h"

class gpcCRS
{
	public:
		I4x4 	frm;
		U1x4	*pMINI, *pCRS;

		U4 nMINI;

		gpcCRS( gpcWIN& win, I4 mag0 = 1 );
		virtual ~gpcCRS();

		I4x4 	gtFRM() { return frm; }
		void 	stFRM(I4x4 val) { frm = val; }

		bool		MINI_off( void );
		void 		MINI_ins( U1* pC, U1* pM, U1* pB );
		bool		MINI_draw( gpcWIN& win, U1 nDIV = 0 );

	protected:

	private:

		//gpcCRS(const gpcCRS& other);
		//gpcCRS& operator=(const gpcCRS& other);
};

#endif // GPCCRS_H
