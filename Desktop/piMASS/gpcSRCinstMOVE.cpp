
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
	gpOBJ* pOi = NULL;
	gpROW* pRi = NULL;
	gpPTR* pPi = NULL, *pPu;
	gpBLK* pBup = iROWblk( pS, i, &pRi );

    if( ppR )
		*ppR = pRi;

	if( pBup )
	if( pBup->iPTR > 0 )
	if(	pPi = pBup->BLKpPTR( pS ) ) {
        while( pPi->cID() == gpeCsz_ptr ) {
			pPu = pMEM->pPTR( pPi->iPC );
			if( pPu ) {
				pPi = pPu;
				continue;
			}
			pPu = pMEM->pPTR( pPi->iPC );;
			break;
        }
        pOi = pMEM->OBJfnd( pPi->mNdID );
        if( ppO )
			*ppO = pOi;
    } else {
    	pPi = pBup->BLKpPTR( pS );
    }
    if( !pRi )
		return NULL;

    if(!pPi) {
		pOi = pMEM->OBJfnd( pRi->mNdID );
		if( !pOi )
			return NULL;
		pPi = pOi->pPTRu1();
	}

	if( ppO )
		*ppO = pOi;

	I4x2	DM(1,1);
	I8x2 	AN(0);

	I4 cIDblk = gpeCsz_L;
	if(pOi->bAN()) {
		if(ppSRC) {
			U4 xfnd = pMEM->pMASS->getXFNDan( pOi->AN );
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
		return pPi;
	}
	else if( pOi->bALF() ) {
		if( gpcSRC* pSRC = (ppSRC ? *ppSRC : NULL) )
		{
			/// pSRC BUILD? ----------------------------
			if( !pSRC->pMEM )
				pSRC->msBLD = pMEM->pWIN->mSEC.x + pSRC->msBLTdly;
			if( !pSRC->srcBLD( pMEM->pWIN, pMEM->pLZYsrcXFND ) )
				return pPi;
			/// ------------------------------------------
			if( ppOin )
				*ppOin = pSRC->pMEM->pOBJ(pOi->AN.alf);

			I4 imNdID = pPi->mNdID; // (*ppOin) ? (*ppOin)->oID : 0;
			gpPTR* pPin = (*ppOin)->pPTRu1();
			//pPi->cpyREF( pMEM->pUn(), pPin );
			*pPi = *pPin;
			pPi->iPC = -1;
			pPi->mNdID = imNdID;
			return pPi;
		}

		if( ppOin )
			*ppOin = NULL;
		if( ppSRC )
			*ppSRC = NULL;

		AN = pOi->AN;
		switch( AN.alf ) {
			case gpeALF_FPS:			/// beépített FPS
				cIDblk = gpeCsz_L;
				break;
			default:
				AN.alf = gpeALF_null;
		}
	}
	else if( pPi ) {
		if( pPi->mNdID < 0 ){
			cIDblk = pPi->cID();
			DM = *pPi->pd2D();
		}
		else {
			/// beépített CÉL változó?
			cIDblk = pMEM->instDOit( *pOi, NULL );
			if( cIDblk != gpeCsz_OFF )
				pPi->cID( cIDblk );		/// igen ha kapunk typust akkor találat
			else
				cIDblk = pPi->cID();		/// nem
		}
	}

	if( AN.alf == gpeALF_null )
		return pPi;

	U4	nA = pMEM->nALL(pPi->iPC),
		nC = gpaCsz[cIDblk]*DM.area();
	if( nA < nC ) {
		/// nem lett még lefoglalva
		if( nA )
			pMEM->iFREE( pPi->iPC );
		pPi->cID( cIDblk );
		pPi->d2D( DM );
		pPi->iPC = pMEM->iALL( pPi->sOF() );
	}



	_move._l.EAl( pPi->iPC ).A0;
	_jsr.EAl( AN.alf );
    return pPi;
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
	gpOBJ	*pOi, *pOs = NULL, *pOb;
	gpPTR	*pPb = NULL, *pPi;
	gpeOPid opB, opA;
	for( I4 iR = nR-1; iR > -1 ; pOb = pOi, opB=opA, iR-- )
	{
		pPi = pBLK->iROWptr( pS, iR, &pRi, &pOi, &pSRC, &pOs );
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
				pPi->cpyREF( pMEM->pUn(), pPb );
				pPb = pPi;
				continue;
		}
	}
	return pBLKm;
}
