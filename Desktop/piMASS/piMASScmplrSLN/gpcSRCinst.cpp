#include "gpcSRC.h"
#include "gpcSRClnk.h"
#include "gpccrs.h"
extern U1 gpaALFsub[];
extern char gpaALF_H_sub[];


#define _b c(gpeCsz_b)
#define _w c(gpeCsz_w)
#define _l c(gpeCsz_l)
#define _q c(gpeCsz_q)
#define _4 c(gpeCsz_4)

#define _ptr c(gpeCsz_ptr)
#define _a c(gpeCsz_a)
#define _c c(gpeCsz_c)

#define _B c(gpeCsz_B)
#define _W c(gpeCsz_W)
#define _L c(gpeCsz_L)
#define _Q c(gpeCsz_Q)

#define _f c(gpeCsz_f)
#define _d c(gpeCsz_d)

#define _K c(gpeCsz_K)
#define _OFF c(gpeCsz_OFF)

#define _move	pMEM->inst( gpeOPid_mov )
#define _add	pMEM->inst( gpeOPid_add )
#define _sub	pMEM->inst( gpeOPid_sub )

#define _mul	pMEM->inst( gpeOPid_mul )
#define _div	pMEM->inst( gpeOPid_div )
#define _rem	pMEM->inst( gpeOPid_rem )

gpBLOCK* gpcSRC::srcINSTmov( char* pS, gpBLOCK *pBLKm, gpBLOCK* pBLK, gpcLZY* pDBG )
{
	if( !pBLKm )
		pBLKm = lzyBLOCK.pSTPdwn( pBLK->bIDm );

	gpROW	*pR = pBLK->pROW();
	I4 iPC, sOF = 0;
	U1* pU1 = NULL;
	for( I4 i = pBLK->nROW()-1, nR = pBLK->nROW(); i >= 0; i-- )
	{
		iPC = iPCrow( pR+i, sOF );
		pU1 = srcMEMiPC( iPC, sOF );
		//_move.l #iPC.Aa;
		_move._L.EAl( iPC ).An(0)							.dbg(pDBG);
		if( !i ){
			// move.l (A0),d0
			_move._L.An(0).Dn(0)							.dbg(pDBG);
			continue;
		}
		// move.l d0,(A0)
		pMEM->inst( (gpeOPid)pR[i].pstOP )._L.Dn(0).An(0)	.dbg(pDBG);
	}

	pR = pBLKm->pROW(pBLK->bIDmR);
	iPC = iPCrow( pR, sOF );
	pU1 = srcMEMiPC( iPC, sOF );
	_move._L.EAl( iPC ).An(0)							.dbg(pDBG);
	_move._L.Dn(0).An(0)								.dbg(pDBG);
	return pBLKm;
}

gpBLOCK* gpcSRC::srcINSTadd( char* pS, gpBLOCK *pBLKm, gpBLOCK* pBLK, gpcLZY* pDBG )
{
	if( !pBLKm )
		pBLKm = lzyBLOCK.pSTPdwn( pBLK->bIDm );

	gpROW	*pR = pBLK->pROW();
	I4 iPC, sOF = 0;
	U1* pU1 = NULL;
	for( I4 i = 0, nR = pBLK->nROW(); i < nR; i++ )
	{
		iPC = iPCrow( pR+i, sOF );
		pU1 = srcMEMiPC( iPC, sOF );
		//_move.l #iPC.Aa;
		_move._L.EAl( iPC ).An(0)							.dbg(pDBG);
		if( !i ){
			// move.l (A0),d0
			_move._L.An(0).Dn(0)							.dbg(pDBG);
			continue;
		}
		// add.l (A0),d0
		pMEM->inst( (gpeOPid)pR[i].pstOP )._L.An(0).Dn(0)	.dbg(pDBG);
	}

	pR = pBLKm->pROW(pBLK->bIDmR);
	iPC = iPCrow( pR, sOF );
	pU1 = srcMEMiPC( iPC, sOF );
	_move._L.EAl( iPC ).An(0)							.dbg(pDBG);
	_move._L.Dn(0).An(0)								.dbg(pDBG);
	// move.l d0, -(A6)
	return pBLKm;
}
gpBLOCK* gpcSRC::srcINSTmul( char* pS, gpBLOCK *pBLKm, gpBLOCK* pBLK, gpcLZY* pDBG  )
{
	if( !pBLKm )
		pBLKm = lzyBLOCK.pSTPdwn( pBLK->bIDm );

	gpROW	*pR = pBLK->pROW();
	I4 iPC = 0 , sOF = 0;
	U1* pU1 = NULL;
	for( I4 i = 0, nR = pBLK->nROW(); i < nR; i++ )
	{
		iPC = iPCrow( pR+i, sOF );
		pU1 = srcMEMiPC( iPC, sOF );
		//_move.l #iPC.Aa;
		_move._L.EAl( iPC ).An(0)						.dbg(pDBG);
		if( !i ){
			// move.l (A0),d0
			_move._L.An(0).Dn(0)						.dbg(pDBG);
			continue;
		}
		pMEM->inst( pR[i].pstOP )._L.An(0).Dn(0)		.dbg(pDBG);
	}

	pR = pBLKm->pROW(pBLK->bIDmR);
	iPC = iPCrow( pR, sOF );
	pU1 = srcMEMiPC( iPC, sOF );
	_move._L.EAl( iPC ).An(0)							.dbg(pDBG);
	_move._L.Dn(0).An(0)								.dbg(pDBG);
	return pBLKm;
}
