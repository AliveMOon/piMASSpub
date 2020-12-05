#include "gpcSRC.h"
#include "gpcSRCinst.h"
#include "gpcSRClnk.h"
#include "gpccrs.h"
extern U1 gpaALFsub[];
extern char gpaALF_H_sub[];

gpBLK* gpcSRC::srcINSTadd( char* pS, gpBLK *pBLKm, gpBLK* pBLK ) {
	if( !pBLKm )
		pBLKm = lzyBLOCK.pSTPdwn( pBLK->bIDm );
    I4  nR = pBLK->nROW();
    if(!nR)
        return pBLKm;
	gpROW	*pRa;
	gpOBJ	*pOa;
	gpPTR	*pPb = NULL, *pPa;
	gpeOPid opB, opA;
	for( U4 iR = 0; iR < nR; opB=opA, iR++ )
	{
		pPa = pBLK->iROWptr( pS, iR, &pOa, &pRa );
		opA = pRa->pstOP;
		if( !pPb )
		{
			pPb = pBLK->BLKpPTR( pS )->cpy( pMEM, pPa );
			continue;
		}
		if( !pPa )
			continue;

		bool 	bSIGa = pPa->cID&((I4)gpeCsz_B),
				bSIGb = pPb->cID&((I4)gpeCsz_B),
				bSIGc = bSIGb|bSIGa;

		U4	nA = gpaCsz[pPa->cID], //pPa->sOF(),
			nB = gpaCsz[pPb->cID], //pPb->sOF(),
			nMN = gpmMIN(nA,nB),
			nMX = gpmMAX(nA,nB);

		if(!bSIGa)
		if(nA<nMX)
			_xor._q.D0.D0;
		_move._l.EAl( pPa->iPC ).A0;
		_move.c((gpeCsz)pPa->cID).IA0I.D0;
		if(bSIGa)
		if(nA<nMX)
		switch(nA) {
			case 1: _extB._Q.D0; break;
			case 2: _ext._Q.D0; break;
			case 4: _extL._Q.D0; break;
			default:
				break;
		}

		if(!bSIGb)
		if(nB<nMX)
			_xor._q.D1.D1;
		_move._l.EAl( pPb->iPC ).A0;
		_move.c((gpeCsz)pPb->cID).IA0I.D1;
		if(bSIGb)
		if(nB<nMX)
		switch(nB) {
			case 1: _extB._Q.D1; break;
			case 2: _ext._Q.D1; break;
			case 4: _extL._Q.D1; break;
			default:
				break;
		}

		I4 cIDc = pPb->cID;
		if( !bSIGc ) {
			switch( nMX ) {
				case 0:
				case 1: cIDc = gpeCsz_b; break;
				case 2: cIDc = gpeCsz_w; break;
				case 3:
				case 4: cIDc = gpeCsz_l; break;
				case 5:
				case 6:
				case 7:
				case 8:
				default:
					cIDc = gpeCsz_q; break;
			}
		} else {
			switch( nMX )
			{
				case 0:
				case 1: cIDc = gpeCsz_B; break;
				case 2: cIDc = gpeCsz_W; break;
				case 3:
				case 4: cIDc = gpeCsz_L; break;
				case 5:
				case 6:
				case 7:
				case 8:
				default:
					cIDc = gpeCsz_Q; break;
			}
		}

		switch( opB )
		{
			case gpeOPid_add:
			case gpeOPid_sub:
			case gpeOPid_or:
				pMEM->inst( opB ).c((gpeCsz)cIDc).D0.D1;
			break;
			default: break;
		}

		pPb->cID = cIDc;
		I4x2 DMb = pPb->pd2D()->MX( *pPa->pd2D() );

		U4 nC = pMEM->nALL(pPb->iPC);
		if( nC < nMX ) {
			/// nem lett még lefoglalva
			if( nC )
				pMEM->iFREE( pPb->iPC );
			pPb->d2D( DMb );
			pPb->iPC = pMEM->iALL( pPb->sOF() );
		}
		_move._l.EAl( pPb->iPC ).A0;
		_move.c((gpeCsz)pPb->cID).D1.IA0I;


	}


	return pBLKm;
}
