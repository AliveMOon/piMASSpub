
#include "gpcSRC.h"
#include "gpcSRCinst.h"
#include "gpcSRClnk.h"
#include "gpccrs.h"
extern U1 gpaALFsub[];
extern char gpaALF_H_sub[];

gpPTR* gpBLK::iROWptr( char* pS, U4 i, gpOBJ** ppO, gpROW** ppR, gpcSRC** ppSRC, gpOBJ** ppOin )
{
	if( i >= nROW() )
		return NULL;
	gpPTR* pPTR = NULL;
	gpROW* pRW = pROW(i);
	if( ppR )
		*ppR = pRW;

	gpOBJ* pO = NULL;
	if( gpBLK* pBup	= 	pRW->bIDup
						? pMEM->pSRC->lzyBLOCK.pSTPup( pRW->bIDup, -1, -1, pRW->mnID )
						: NULL )
    if( pBup->iPTR > 0 ) {
		pPTR = pBup->BLKpPTR( pS );
        while( pPTR->cID == gpeCsz_ptr ) {
			pPTR = pMEM->pPTR( pPTR->iPC );
        }
        pO = pMEM->OBJfnd( pPTR->mNdID );
    }

    if(!pPTR) {
		pO = pMEM->OBJfnd( pRW->mNdID );
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

	if( AN.alf )
	{
		_move._l.EAl( pPTR->iPC ).A0;
		if(AN.alf) _jsr.EAl( AN.alf );
	}

    return pPTR;
}

gpPTR* gpBLK::moveAB( gpROW* pRdst, gpPTR* pPsrc ) {
	I4		cIDc,
			oIDs = (pPsrc ? pPsrc->mNdID : 0),
			iPCs = -1,
			cIDs = pPsrc ? pPsrc->cID: gpeCsz_OFF, ns = 0;
	I4x2	DIMs(0);

	I4		oIDd = (pRdst ? pRdst->mNdID : 0),
			iPCd = -1,
			cIDd = gpeCsz_OFF, nd = 0;


	gpOBJ	*pOsrc = pMEM->OBJfnd( oIDs ),
			*pOdst = pMEM->OBJfnd( oIDd );
	gpPTR	*pPdst = pOdst->pPTR();
	I8x2 	ANsrc(0);

	_nop;

	if( oIDd == oIDs ) /// a = a;
		return pPdst;

	if( pPdst ) {
		if( pPdst->mNdID < 0 )
			cIDd = pPdst->cID;
		else { 		/// elörre meghatározott DEST azonosítása
			cIDc = pOdst ? pMEM->instDOit( *pOdst, NULL ) : gpeCsz_OFF;
			if( cIDc != gpeCsz_OFF )
				pPdst->cID = cIDc;		/// ha kapunk typust akkor találat
			else
				cIDd = pPdst->cID;
		}
	}
	bool	bSIGs = false,
			bSIGd = cIDd&((I4)gpeCsz_B);;
	nd = gpaCsz[cIDd];

	if( pOsrc->bAN() ) {

	}
	else if( pOsrc->bALF() ) {			/// forrás VÁLTOZO
		ANsrc = pOsrc->AN;
		switch( ANsrc.alf )
		{
			case gpeALF_FPS:			/// beépített FPS
				cIDs = gpeCsz_L;
				DIMs.x = DIMs.y = 1;
				break;
			default:
				ANsrc.alf = gpeALF_null;
		}

		bSIGs = cIDs&((I4)gpeCsz_B);
		ns = gpaCsz[cIDs]*DIMs.area();
		if( ANsrc.alf )
		{
			U4 nA = pMEM->nALL(pPsrc->iPC);
			if( nA < ns ) {
				/// nem lett még lefoglalva
				if( nA )
					pMEM->iFREE( pPsrc->iPC );
				pPsrc->cID = cIDs;
				pPsrc->d2D( DIMs );
				pPsrc->iPC = pMEM->iALL( pPsrc->sOF() );
			}
			_move._l.EAl( pPsrc->iPC ).A0;
			_jsr.EAl( ANsrc.alf );

			/*if( bSIGs ? false : (nd>ns) )
				_xor._q.D0.D0;

			_move.c((gpeCsz)cIDs).IA0I.D0;

			if( bSIGs  ? (nd>ns) : false )
			switch( ns )
			{
				case 1: _extB._Q.D0; break;
				case 2: _ext._Q.D0; break;
				case 4: _extL._Q.D0; break;
				default:
					break;
			}*/

		}
	}

	if( cIDd == gpeCsz_OFF )	/// nem lett meghatározva
		return pPdst->cpyREF( pPsrc );

	if( cIDd == cIDs ) {		/// ugyan az a forrás és cél típusa
		pPdst->cpyREF( pPsrc );
		return pPdst;
	}



	return pPdst;
}

gpBLK* gpcSRC::srcINSTmov3( char* pS, gpBLK *pBLKm, gpBLK* pBLK ) {
	if( !pBLKm )
		pBLKm = lzyBLOCK.pSTPdwn( pBLK->bIDm );
    I4  nR = pBLK->nROW(), cID0 = gpeCsz_L;
    if(!nR)
        return pBLKm;

    gpROW	*pR0 = pBLK->pROW(),
            *pRa = pR0+nR-1;
    gpPTR 	*pPa,
			*pPb = SRCpPTR( pS, *pRa );
	/// a = a;
	/// b = a += a;
    for( pRa--; pR0 <= pRa; pPb = pPa, pRa-- ) {
		/// Ao B,
		switch( pRa->pstOP ) {
			case gpeOPid_mov:
				pPa = pBLK->moveAB( pRa, pPb );	/// pPa = moveP2ReP(  pRdst, pPsrc )
				break;
			case gpeOPid_add:
				_add.c((gpeCsz)cID0).D1.D0; break;
			case gpeOPid_sub:
				_sub.c((gpeCsz)cID0).D1.D0; break;
			case gpeOPid_addM: _add.c((gpeCsz)cID0).D1.D0; break;
			case gpeOPid_subM: _sub.c((gpeCsz)cID0).D1.D0; break;
			case gpeOPid_mulM: _mul.c((gpeCsz)cID0).D1.D0; break;
			case gpeOPid_divM: _div.c((gpeCsz)cID0).D1.D0; break;
			case gpeOPid_remM: _rem.c((gpeCsz)cID0).D1.D0; break;
			default: break;
		}
	}


	return pBLKm;
}

gpBLK* gpcSRC::srcINSTmov( char* pS, gpBLK *pBLKm, gpBLK* pBLK ) {
	if( !pBLKm )
		pBLKm = lzyBLOCK.pSTPdwn( pBLK->bIDm );
    I4  nR = pBLK->nROW();
    if(!nR)
        return pBLKm;
	gpcSRC	*pSRC = NULL;
	gpROW	*pRa;
	gpOBJ	*pOa, *pOs = NULL;
	gpPTR	*pPb = NULL, *pPa;
	gpeOPid opB, opA;
	for( I4 iR = nR-1; iR > -1 ; opB=opA, iR-- )
	{
		pPa = pBLK->iROWptr( pS, iR, &pOa, &pRa, &pSRC, &pOs );
		opA = pRa->pstOP;
		if( !pPa )
			continue;
		if( !pPb )
		{
			pPb = pPa; // pBLK->BLKpPTR( pS )->cpy( pMEM, pPa );
			continue;
		}

		switch( pRa->pstOP ) {
			case gpeOPid_mov:
			default:
				_nop;
				pPa->cpyREF(pPb);
				pPb = pPa;
				continue;
		}
	}
	return pBLKm;
}
