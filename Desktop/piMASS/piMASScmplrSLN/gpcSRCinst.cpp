#include "gpcSRC.h"
#include "gpcSRClnk.h"
#include "gpccrs.h"
extern U1 gpaALFsub[];
extern char gpaALF_H_sub[];
#define _move pMEM->MOVE()

#define _b b()
#define _w w()
#define _l l()
#define _q q()
#define _4 b4()

#define _ptr ptr()
#define _a a()
#define _c c()

#define _B B()
#define _W W()
#define _L L()
#define _Q Q()

#define _f f()
#define _d d()

#define _K K()
#define _OFF OFF()


gpBLOCK* gpcSRC::srcINSTadd( char* pS, gpBLOCK *pBLKm, gpBLOCK* pBLK  )
{
	if( !pBLKm )
		pBLKm = lzyBLOCK.pSTPdwn( pBLK->bIDm );

	gpROW	*pR = pBLK->pROW();
	I4 iPC, sOF;
	U1* pU1 = NULL;
	for( I4 i = 0, nR = pBLK->nROW(); i < nR; i++ )
	{
		iPC = iPCrow( pR+i );
		pU1 = srcMEMiPC( iPC, sOF );
		sOF = pR[i].sOF;
		if( !i ){

			_move._L.IAnI(0).Dn(0);
			// move.l (iPC),d0
			continue;
		}
		// add.l (iPC),d0
	}

	pR = pBLKm->pROW(pBLK->bIDmR);
	iPC = iPCrow( pR );
	pU1 = srcMEMiPC( iPC, pR->sOF );
	// move.l d0, -(A6)
	return pBLKm;
}
gpBLOCK* gpcSRC::srcINSTmul( char* pS, gpBLOCK *pBLKm, gpBLOCK* pBLK  )
{
	if( !pBLKm )
		pBLKm = lzyBLOCK.pSTPdwn( pBLK->bIDm );

	gpROW	*pR = pBLK->pROW();
	I4 iPC, sOF;
	U1* pU1 = NULL;
	for( I4 i = 0, nR = pBLK->nROW(); i < nR; i++ )
	{
		iPC = iPCrow( pR+i );
		pU1 = srcMEMiPC( iPC, sOF );
		sOF = pR[i].sOF;
		if( !i ){
			// move.l (iPC),d0
			continue;
		}
		// mul.l (iPC),d0
	}

	pR = pBLKm->pROW(pBLK->bIDmR);
	iPC = iPCrow( pR );
	pU1 = srcMEMiPC( iPC, pR->sOF );
	// move.l d0, -(A6)
	return pBLKm;
}
