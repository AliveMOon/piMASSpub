#include "gpcSRC.h"
#include "gpcSRCinst.h"
#include "gpcSRClnk.h"
#include "gpccrs.h"
extern U1 gpaALFsub[];
extern char gpaALF_H_sub[];

gpBLK* gpcSRC::srcBLKstk( char* pS, I4 mnID, gpBLK* pBLK, gpeOPid opID, gpcLZY* pDBG ) {
		///kEND(scp);
		U1* pU1 = NULL;
		while( pBLK ? (pBLK->opIDgrp() != gpeOPid_stk) : false )
		{
			/// FENT
			gpBLK	*pBLKm = lzyBLOCK.pSTPdwn( pBLK->bIDm );
			gpROW	*pR0 = pBLK->pROW(),
					*pRd = pBLKm->pROW(pBLK->bIDmR);
			I4 nR = pBLK->nROW(), iPC, sOF;
			switch( pBLK->opIDgrp() )
			{
				case gpeOPid_brakS:
				case gpeOPid_dimS:
				case gpeOPid_begin:
					return srcBLKup( pS, pBLK, gpeOPid_stk, mnID );
					break;
				case gpeOPid_mov:
					pBLKm = srcINSTmov( pS, pBLKm, pBLK );
					break;
				case gpeOPid_add:
				case gpeOPid_sub:
					pBLKm = srcINSTadd( pS, pBLKm, pBLK );
					break;
				case gpeOPid_mul:
					pBLKm = srcINSTmul( pS, pBLKm, pBLK );
					break;
				case gpeOPid_entry:
					pBLKm = srcINSTanDalf( pS, pBLKm, pBLK );
					break;
				default: break;
			}


			if( !pBLKm )
				break;
			/// LENT
			pBLK = pBLKm;

		}

		gpROW	*pRl = pBLK->pLSTrow();
		if( !pRl )
			return pBLK;

		pRl->pstOP = opID;
		/// a veszö egyenlőre csinál helyet
		pBLK->pNEWrow();
		return pBLK;
	}
