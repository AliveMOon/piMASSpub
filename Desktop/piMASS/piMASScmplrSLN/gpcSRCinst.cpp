#include "gpcSRC.h"
#include "gpcSRCinst.h"
#include "gpcSRClnk.h"
#include "gpccrs.h"
extern U1 gpaALFsub[];
extern char gpaALF_H_sub[];

gpBLOCK* gpcSRC::srcINSTdwn( char* pS, gpBLOCK *pBLKm, gpBLOCK* pBLK, gpBLOCK* pBLKup ) {
	if( !pBLKm )
		pBLKm = lzyBLOCK.pSTPdwn( pBLK->bIDm );
	if( pBLKup )
		pBLK = pBLKup;
	I4 aiPC[2], sOF = 0, cID = -1;
	U1* pU1 = NULL;

	//gpROW *pR0 = pBLK->pROW();
	//aiPC[0] = iPCrow( pR0, sOF, true );
	_move._l.EAl(pBLK->iPC).A0;

	gpROW *pRm = pBLKm->pROW(pBLK->bIDmR);
	aiPC[1] = pRm ? iPCrow( *pRm, sOF, false ) : gpeCsz_L;
	//pU1 = srcMEMiPC( iPC, sOF );
	_move._l.EAl(aiPC[1]).A1;
	_move._L.IA0I.IA1I;
	_nop;
	return pBLKm;
}


gpMEM::gpMEM( gpcSRC* pS, gpcWIN* pW, I4 i ) {
	gpmCLR;
	pSRC = pS;
	pWIN = pW;
	pMASS = pWIN ? pWIN->piMASS : NULL;
	iSTK = nDAT = i;
	nDAT += 0x100;
	pA = (I8*)aA;
	pD = (I8*)(aA+8);
}

U1* gpMEM::instVAR( U1* p_dst, gpINST& inst )
{
	switch( inst.a8x2.alf )
	{
		case gpeALF_FPS:
			*(U4*)p_dst = pWIN->mSEC.w;
			break;
		default:
			break;
	}
	return p_dst;
}


gpBLOCK* gpcSRC::srcINSTadd2( char* pS, gpBLOCK *pBLKm, gpBLOCK* pBLK ) {
	if( !pBLKm )
		pBLKm = lzyBLOCK.pSTPdwn( pBLK->bIDm );

	gpROW	*pR0 = pBLK->pROW();
	I4 iPC, sOF = 0, nR = pBLK->nROW();
	U1* pU1 = NULL;
	for( I4 i = 0; i < nR; i++ )
	{
		gpROW& Ri = pR0[i];
		iPC = iPCrow( Ri, sOF, true );
		pU1 = srcMEMiPC( iPC, sOF );
		_move._l.EAl(iPC).A0;
		if( !i ){
			_move.c((gpeCsz)Ri.cID).IA0I.D0;
			continue;
		}
		pMEM->inst( (gpeOPid)pR0[i-1].pstOP ).c((gpeCsz)Ri.cID).IA0I.D0;
	}

	pU1 = srcMEMiPC( pBLK->iPC, sOF );
	_move._l.EAl( pBLK->iPC ).A0;
	_move.c((gpeCsz)pBLK->cID).D0.IA0I;
	_nop;
	return pBLKm;
}
gpBLOCK* gpcSRC::srcINSTmul2( char* pS, gpBLOCK *pBLKm, gpBLOCK* pBLK  ) {
	if( !pBLKm )
		pBLKm = lzyBLOCK.pSTPdwn( pBLK->bIDm );

	gpROW	*pR0 = pBLK->pROW();
	I4 iPC = 0 , sOF = 0, nR = pBLK->nROW();
	U1* pU1 = NULL;
	for( I4 i = 0; i < nR; i++ )
	{
		gpROW& Ri = pR0[i];
		iPC = iPCrow( Ri, sOF, true );
		pU1 = srcMEMiPC( iPC, sOF );
		//_move.l #iPC.Aa;
		_move._l.EAl(iPC).A0;
		if( !i ){
			// move.l (A0),d0
			_move.c((gpeCsz)Ri.cID).IA0I.D0;
			continue;
		}
		// add.l (A0),d0
		pMEM->inst( (gpeOPid)pR0[i-1].pstOP ).c((gpeCsz)Ri.cID).IA0I.D0;
	}

	pU1 = srcMEMiPC( pBLK->iPC, sOF );
	_move._l.EAl( pBLK->iPC ).A0;
	_move.c((gpeCsz)pBLK->cID).D0.IA0I;
	_nop;
	return pBLKm;
}
