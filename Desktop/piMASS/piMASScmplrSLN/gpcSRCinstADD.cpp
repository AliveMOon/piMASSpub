#include "gpcSRC.h"
#include "gpcSRCinst.h"
#include "gpcSRClnk.h"
#include "gpccrs.h"
extern U1 gpaALFsub[];
extern char gpaALF_H_sub[];
gpBLK* gpcSRC::srcBLKadd( char* pS, I4 mnID, gpBLK* pBLK, gpeOPid opID, gpcLZY* pDBG ) {
		///kOBJ(scp); //, iOPe );
		// a =b*c +d		// iMUL 1
		// a =b*c/d +e		// iMUL 2
		// a +=b*c/d +e		// iMUL 3
		// a +b*c +e		// iMUL 1
		// a*b +b*c			// iMUL 1
		///kMUL(scp,false);
		///LEVaddEXP()[lADD++] = now;
		///++SP;
		//if( !pBLK )
		//	pBLK = srcBLKnew( pS, gpeOPid_stk, NULL, -1, -1 );

		gpROW* pRl = pBLK->pLSTrow();
		if( !pRl )
			return pBLK;

		U1* pU1 = NULL;
		switch( pBLK->opIDgrp() )
		{
			case gpeOPid_brakS:
			case gpeOPid_dimS:
			case gpeOPid_begin:
				return srcBLKup( pS, pBLK, opID, mnID );
			case gpeOPid_nop:
				pBLK->opID = opID;
			case gpeOPid_add:
				///							pRl
				/// a + b - c == d - e + 	f -
				pRl->pstOP = opID;
				pBLK->pNEWrow();
				return pBLK;
			case gpeOPid_entry: {
					gpBLK	*pBLKm = srcINSTanDalf( pS, NULL, pBLK );
					if( pBLKm->opIDgrp() == gpeOPid_add )
					{
						pRl = pBLKm->pLSTrow();
						pRl->pstOP = opID;
						pBLKm->pNEWrow();
						return pBLKm;
					}
					return srcBLKinsrt( pS, pBLK, opID, mnID );
				} break;
			case gpeOPid_mul: {
					gpBLK	*pBLKm = srcINSTmul( pS, NULL, pBLK );
					if( pBLKm->opIDgrp() == gpeOPid_add )
					{
						/// IGEN volt alatta ADD
						///				pRl
						/// a -	b *		c +
						pRl = pBLKm->pLSTrow();
						pRl->pstOP = opID;
						pBLKm->pNEWrow();
						return pBLKm;
					}
					/// NEM volt alatta ADD
					///				pRl
					/// a *	b *		c +
					return srcBLKinsrt( pS, pBLK, opID, mnID );
				} break;
			default:
				break;
		}

		return srcBLKup( pS, pBLK, opID, mnID );
	}
gpBLK* gpcSRC::srcINSTadd( char* pS, gpBLK *pBLKm, gpBLK* pBLK ) {
	if( !pBLKm )
		pBLKm = lzyBLOCK.pSTPdwn( pBLK->bIDm );
    I4  nR = pBLK->nROW();
    if(!nR)
        return pBLKm;
	gpcSRC	*pSRC = NULL;
	gpROW	*pRi;
	gpOBJ	*pOi, *pOin = NULL;
	gpPTR	*pPb = NULL, *pPi;
	gpeOPid opB, opA;
	for( I4 iR = 0; iR < nR; opB=opA, iR++ )
	{
		pPi = pBLK->iROWptr( pS, iR, &pRi, &pOi, &pSRC, &pOin );
		opA = pRi->pstOP;
		if( !pPi )
			continue;
		if( !pPb ) {
			pPb = pBLK->BLKpPTR( pS )->cpy( pMEM, pPi );
			continue;
		}

		bool 	bSIGa = pPi->cID()&((I4)gpeCsz_B),
				bSIGb = pPb->cID()&((I4)gpeCsz_B),
				bSIGc = bSIGb|bSIGa;

		U4	nA = gpaCsz[pPi->cID()], //pPa->sOF(),
			nB = gpaCsz[pPb->cID()], //pPb->sOF(),
			nMN = gpmMIN(nA,nB),
			nMX = gpmMAX(nA,nB);

		if(!bSIGa)
		if(nA<nMX)
			_xor._q.D0.D0;
		_move._l.EAl( pPi->iPC ).A0;
		_move.c((gpeCsz)pPi->cID()).IA0I.D0;
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
		_move.c((gpeCsz)pPb->cID()).IA0I.D1;
		if(bSIGb)
		if(nB<nMX)
		switch(nB) {
			case 1: _extB._Q.D1; break;
			case 2: _ext._Q.D1; break;
			case 4: _extL._Q.D1; break;
			default:
				break;
		}

		I4 cIDc = pPb->cID();
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

		pPb->cID(cIDc);
		I4x2 DMb = pPb->pd2D()->MX( *pPi->pd2D() );

		U4 nC = pMEM->nALL(pPb->iPC);
		if( nC < nMX ) {
			/// nem lett még lefoglalva
			if( nC )
				pMEM->iFREE( pPb->iPC );
			pPb->d2D( DMb );
			pPb->iPC = pMEM->iALL( pPb->sOF() );
		}
		_move._l.EAl( pPb->iPC ).A0;
		_move.c((gpeCsz)pPb->cID()).D1.IA0I;


	}


	return pBLKm;
}
