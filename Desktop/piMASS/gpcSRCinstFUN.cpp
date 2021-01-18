#include "gpcSRC.h"
#include "gpcSRCinst.h"
#include "gpcSRClnk.h"
#include "gpccrs.h"
extern U1 gpaALFsub[];
extern char gpaALF_H_sub[];

gpBLK* gpcSRC::srcBLKbrakS( char* pS, I4 mnID, gpBLK* pBLK, gpeOPid opID ) {

		///					pRl
		/// a + 			{		//block level up
		/// a + 			b{		//block rutine
		/// a + 			(		//brake block
		/// a + 			b(		//function
		/// a + 			b[		//index b
		//if( !pBLK )
		//	pBLK = srcBLKnew( pS, gpeOPid_stk, NULL, -1, -1 );

		gpROW* pRl = pBLK->pLSTrow();
		if( !pRl )
			return pBLK;
		if( pBLK->opIDgrp() == gpaOPgrp[opID] )
		{
			/// a + 			{{{{		//block level up
			/// a + 			b{c(d((		//block rutinepRl->pstOP = opID;
			pRl->pstOP = opID;
			pBLK->pNEWrow();
			return pBLK;
		}

		gpOBJ* pO = srcOBJfnd( pRl->mNdID );
		if( pO )
		{
			/// FUN(
		} else {
			/// (
			pO = srcOBJadd( pS, mnID<0 ? mnID :-mnID );
		}
		*pRl = *pO;
		return srcBLKup( pS, pBLK, opID, mnID );
}
gpPTR* gpBLK::instFUN( char* pS, I8x2& AN, gpBLK *pARG )
{

	switch( AN.alf ) {
		case gpeALF_SIN:
		case gpeALF_COS:
		case gpeALF_PRINT:
			break;
		default:
			return pARG->BLKpPTR( pS );
	}

	I4 nR = pARG->nROW();
	_move._q.A7.D7;
	if( nR ) {
		gpOBJ	*pOa;
		gpROW	*pRi;
		gpPTR	*pPi;
		for( I4 iR = 0; iR < pARG->nROW(); iR++ ) {
			pPi = pARG->iROWptr( pS, iR, &pRi, &pOa );
			if( !pPi )
				continue;
			_move._l.EAl( pOa->iPTR ).sIA7I;
		}
	}
		gpPTR* pP = BLKpPTR( pS );

		switch( AN.alf ) {
			case gpeALF_SIN:
			case gpeALF_COS:
				pP->cID(gpeCsz_L);
				break;
			case gpeALF_PRINT:
				pP->cID(gpeCsz_b);
				break;
			default:
				break;
		}

		_move._l.EAl( iPTR ).A0;
		_jsr.EAl( AN.alf );

	if( nR ) _move._q.D7.A7;

	return pP;
}
gpBLK* gpcSRC::srcINSTdwn( char* pS, gpBLK *pBLKm, gpBLK* pBLK, gpBLK* pBLKup, I4 mnID ) {
	if( !pBLKm )
		pBLKm = lzyBLOCK.pSTPdwn( pBLK->bIDm );

	I4  nR = pBLK->nROW();
    if(!nR)
        return pBLKm;

	gpBLK	*pBu = NULL;
	gpROW	*pRi, *pRm;
	gpOBJ	*pOi, *pOb = NULL, *pOm = NULL;
	gpPTR	*pPi, *pPb = NULL, *pPd = NULL;

	for( I4 iR = nR-1; iR > -1 ; pPb=pPi, pOb = pOi, iR-- ) {
		pBu = pBLK->iROWblk( pS, iR, &pRi );
		if( !pBu )
			continue;

		pPi = pBu->BLKpPTR( pS );
		pOi = pMEM->OBJfnd( pPi->mNdID );
		pPd = pBLKm->iROWptr( pS, pBLK->bIDmR, &pRm, &pOm );
		pPb = pBLK->instFUN( pS, pOm->AN, pBu );
		if( pPi == pPb ) {
			/// IGEN ugyan az mint az pBu NEM történt fun
			// akkor a pBLK->iPRT <--= pBu->iPRT
			_nop;
			pPd->cpyREF( pMEM->pUn(), pPb);
		} else {
			/// NEM ugyan az mint az pBu IGEN történt FUN
			// akkor a pBLK->iPRT marad
			_nop;
			pPd->cpyREF(pMEM->pUn(), pPb);
		}

		break;
	}
	/*_nop;
	pPi->cpyREF(pPb);*/
	return pBLKm;


	/*_move._l.EAl(pPu->iPC).A0;
	_move._l.EAl(pPd->iPC).A1;
	_move._L.IA0I.IA1I;
	_nop;
	return pBLKm;*/
}

gpBLK* gpcSRC::srcBLKbrakE( char* pS, I4 mnID, gpBLK* pBLK, gpeOPid opID, gpcLZY* pDBG ) {
	///kEND(scp);
	U1* pU1 = NULL;
	gpBLK* pBLKup = NULL;
	while( pBLKup ? (pBLKup->opIDgrp() != gpeOPid_begin) : !!pBLK )
	{
		/// FENT
		gpBLK	*pBLKm = lzyBLOCK.pSTPdwn( pBLK->bIDm );
		gpROW	*pR0 = pBLK->pROW(),
				*pRd = pBLKm->pROW(pBLK->bIDmR);
		I4 nR = pBLK->nROW(), iPC, sOF;
		switch( pBLK->opIDgrp() ) {
			case gpeOPid_begin: {
					pBLKm = srcINSTdwn( pS, pBLKm, pBLK, pBLKup, mnID );
				} break;
			case gpeOPid_mov:
				pBLKm = srcINSTmov( pS, pBLKm, pBLK );
				break;
			case gpeOPid_add:
			case gpeOPid_sub:
				pBLKm = srcINSTadd( pS, pBLKm, pBLK );
				break;
			case gpeOPid_mul:{
					///			pRl
					/// a, 		_ +
					/// a, 		b +
					/// a = 	b +
					/// a * 	b +
					pBLKm = srcINSTmul( pS, pBLKm, pBLK );
				} break;
			case gpeOPid_stk:
					pBLKm = lzyBLOCK.pSTPdwn( pBLK->bIDm );
				break;
		}


		if( !pBLKm )
			break;
		/// LENT
		pBLKup = pBLK;
		pBLK = pBLKm;

	}
	gpROW	*pRl = pBLK->pLSTrow();
	return pBLK;
	/// a veszö egyenlőre csinál helyet
	//pBLK->pNEWrow();
}

