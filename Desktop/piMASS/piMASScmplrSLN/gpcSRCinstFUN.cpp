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
		if( pBLK->opIDgrp() == opID )
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
			*pRl = *pO;
		}
		return srcBLKup( pS, pBLK, opID, mnID );
}
gpBLK* gpcSRC::srcBLKbrakE( char* pS, I4 mnID, gpBLK* pBLK, gpeOPid opID, gpcLZY* pDBG ) {
	///kEND(scp);
	U1* pU1 = NULL;
	gpBLK* pBLKup = NULL;
	while( pBLKup ? (pBLKup->opIDgrp() != gpeOPid_entry) : !!pBLK )
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
		}


		if( !pBLKm )
			break;
		/// LENT
		pBLKup = pBLK;
		pBLK = pBLKm;

	}
	return pBLK;

	gpROW	*pRl = pBLK->pLSTrow();
	if( !pRl )
		return pBLK;

	pRl->pstOP = opID;
	/// a veszö egyenlőre csinál helyet
	pBLK->pNEWrow();
	return pBLK;
}
gpBLK* gpcSRC::srcINSTdwn( char* pS, gpBLK *pBLKm, gpBLK* pBLK, gpBLK* pBLKup, I4 mnID ) {
	if( !pBLKm )
		pBLKm = lzyBLOCK.pSTPdwn( pBLK->bIDm );
	if( pBLKup )
		pBLK = pBLKup;
	I4 aiPC[2], sOF = 0, cID = -1;
	U1* pU1 = NULL;

	gpROW *pRm; // = pBLKm->pROW(pBLK->bIDmR);
	gpOBJ* pO; // = srcOBJfnd( pRm->mNdID );
	gpPTR	*pPu = pBLK->BLKpPTR( pS ),
			*pPd = pBLKm->iROWptr(	pS, pBLK->bIDmR,
									&pO, &pRm ); //, &pSRC, &pOs );
	//pDWptr = pO->pPTRu1();

	//aiPC[1] = pRm ? iPCrow( *pRm, sOF, false ) : gpeCsz_L;
	//pU1 = srcMEMiPC( iPC, sOF );
	if( pPd->iPC < 0 ) {
		pPd->cpyREF(pPu);
		_nop;
		return pBLKm;
	}
	_move._l.EAl(pPu->iPC).A0;
	_move._l.EAl(pPd->iPC).A1;
	_move._L.IA0I.IA1I;
	_nop;
	return pBLKm;
}
