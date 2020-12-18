
#include "gpcSRC.h"
#include "gpcSRCinst.h"
#include "gpcSRClnk.h"
#include "gpccrs.h"
extern U1 gpaALFsub[];
extern char gpaALF_H_sub[];
gpBLK* gpBLK::iROWblk( char* pS, I4 i, gpROW** ppR )
{
	if( ppR ) *ppR = NULL;
	I4 nR = nROW();
	if( !nR )
		return NULL;
	if( i >= nR )
		return NULL;

	gpROW* pRi = pROW(i);
	if( ppR )
		*ppR = pRi;

	if( pRi ? !pRi->bIDup : true )
		return NULL;

	return pMEM->pSRC->lzyBLOCK.pSTPup( pRi->bIDup, -1, -1, pRi->mnID );
}
gpPTR* gpBLK::iROWptr( char* pS, I4 i, gpROW** ppR, gpOBJ** ppO, gpcSRC** ppSRC, gpOBJ** ppOin )
{
	gpOBJ* pO = NULL;
	gpROW* pRi = NULL;
	gpPTR* pPTR = NULL;
	gpBLK* pBup = iROWblk( pS, i, &pRi );

    if( ppR )
		*ppR = pRi;

	if( pBup )
	if( pBup->iPTR > 0 ) {
		pPTR = pBup->BLKpPTR( pS );
        while( pPTR->cID == gpeCsz_ptr ) {
			pPTR = pMEM->pPTR( pPTR->iPC );
        }
        pO = pMEM->OBJfnd( pPTR->mNdID );
        if( ppO )
			*ppO = pO;
    }
    if( !pRi )
		return NULL;

    if(!pPTR) {
		pO = pMEM->OBJfnd( pRi->mNdID );
		if( !pO )
			return NULL;
		pPTR = pO->pPTR();
	}

	if( ppO )
		*ppO = pO;

	I4x2	DM(1,1);
	I8x2 	AN(0);

	I4 cID = gpeCsz_L;
	if(pO->bAN()) {
		if(ppSRC) {
			U4 xfnd = pMEM->pMASS->getXFNDan( pO->AN );
			if( (*ppSRC) = pMEM->pMASS->srcFND( xfnd ) ) {
				/// STACK ki lehessen szürni a AN loop-okat
				if( !pMEM->pLZYsrcXFND ) {
					if( pMEM->pLZYsrcXFNDall ) {
						pMEM->pLZYsrcXFNDall->lzyRST();
					} else {
						pMEM->pLZYsrcXFNDall = new gpcLZY;
					}
					pMEM->nXFND = 0;
					pMEM->pLZYsrcXFND = pMEM->pLZYsrcXFNDall;
				}
				U4* pXFND = (U4*)(pMEM->pLZYsrcXFND->Ux( pMEM->nXFND, sizeof(*pXFND) ));
				(*pXFND) = xfnd;
			}
		}
		return pPTR;
	}
	else if( pO->bALF() ) {
		if( gpcSRC* pSRC = (ppSRC ? *ppSRC : NULL) )
		{
			/// pSRC BUILD? ----------------------------
			if( !pSRC->pMEM )
				pSRC->msBLD = pMEM->pWIN->mSEC.x + pSRC->msBLTdly;
			if( !pSRC->srcBLD( pMEM->pWIN, pMEM->pLZYsrcXFND ) )
				return pPTR;
			/// ------------------------------------------
			if( ppOin )
				*ppOin = pSRC->pMEM->pOBJ(pO->AN.alf);

			gpPTR* pPin = (*ppOin)->pPTR();
			pPTR->cpyREF(pPin);
			pPTR->iPC = -1;
			return pPTR;
		}

		if( ppOin )
			*ppOin = NULL;
		if( ppSRC )
			*ppSRC = NULL;

		AN = pO->AN;
		switch( AN.alf ) {
			case gpeALF_FPS:			/// beépített FPS
				cID = gpeCsz_L;
				break;
			default:
				AN.alf = gpeALF_null;
		}
	}
	else if( pPTR ) {
		if( pPTR->mNdID < 0 ){
			cID = pPTR->cID;
			DM = *pPTR->pd2D();
		}
		else {
			/// beépített CÉL változó?
			cID = pMEM->instDOit( *pO, NULL );
			if( cID != gpeCsz_OFF )
				pPTR->cID = cID;		/// igen ha kapunk typust akkor találat
			else
				cID = pPTR->cID;		/// nem
		}
	}

	if( AN.alf == gpeALF_null )
		return pPTR;

	U4	nA = pMEM->nALL(pPTR->iPC),
		nC = gpaCsz[cID]*DM.area();
	if( nA < nC ) {
		/// nem lett még lefoglalva
		if( nA )
			pMEM->iFREE( pPTR->iPC );
		pPTR->cID = cID;
		pPTR->d2D( DM );
		pPTR->iPC = pMEM->iALL( pPTR->sOF() );
	}



	_move._l.EAl( pPTR->iPC ).A0;
	_jsr.EAl( AN.alf );
    return pPTR;
}

gpBLK* gpcSRC::srcBLKmov( char* pS, I4 mnID, gpBLK* pBLK, gpeOPid opID, gpcLZY* pDBG ) {
		///kMOV(scp); //, iOPe ); // Ai--; // Ai--; //
		///kOBJ(scp); //, iOPe );
		///++SP;
		///*pSTRT = now;
		//if( !pBLK )
		//	pBLK = srcBLKnew( pS, gpeOPid_stk, NULL, -1, -1 );
		//if( !pBLK )
		//{
		//	gpOBJ* pO = srcOBJmn( pS, mnID, gpeCsz_L, I4x2(1,1) );
		//	pBLK = srcBLKnull( pS, pBLK, pO->dctID, mnID );
		//}

		gpROW* pRl = pBLK->pLSTrow();
		if( !pRl)
			return pBLK;

		switch( pBLK->opIDgrp() )
		{
			case gpeOPid_nop:
				pBLK->opID = opID;
			case gpeOPid_mov:
				///							pRl
				/// a = b += c *= d %= 		e =
				pRl->pstOP = opID;
				pBLK->pNEWrow();
				return pBLK;
			default:
				break;
		}
		///							 		pRl	//up
		/// a, 10, b; c, d;  e = f + 		g =
		//gpBLK* pBup =
		return srcBLKup( pS, pBLK, opID, mnID );
	}

gpBLK* gpcSRC::srcINSTmov( char* pS, gpBLK *pBLKm, gpBLK* pBLK ) {
	if( !pBLKm )
		pBLKm = lzyBLOCK.pSTPdwn( pBLK->bIDm );
    I4  nR = pBLK->nROW();
    if(!nR)
        return pBLKm;
	gpcSRC	*pSRC = NULL;
	gpROW	*pRi;
	gpOBJ	*pOa, *pOs = NULL, *pOb;
	gpPTR	*pPb = NULL, *pPi;
	gpeOPid opB, opA;
	for( I4 iR = nR-1; iR > -1 ; pOb = pOa, opB=opA, iR-- )
	{
		pPi = pBLK->iROWptr( pS, iR, &pRi, &pOa, &pSRC, &pOs );
		opA = pRi->pstOP;
		if( !pPi )
			continue;
		if( !pPb )
		{
			pPb = pPi; // pBLK->BLKpPTR( pS )->cpy( pMEM, pPi );
			continue;
		}

		switch( pRi->pstOP ) {
			case gpeOPid_mov:
			default:
				_nop;
				pPi->cpyREF(pPb);
				pPb = pPi;
				continue;
		}
	}
	return pBLKm;
}
