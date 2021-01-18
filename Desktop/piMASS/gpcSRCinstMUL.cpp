#include "gpcSRC.h"
#include "gpcSRCinst.h"
#include "gpcSRClnk.h"
#include "gpccrs.h"
extern U1 gpaALFsub[];
extern char gpaALF_H_sub[];
gpBLK* gpcSRC::srcBLKmul( char* pS, I4 mnID, gpBLK* pBLK, gpeOPid opID, gpcLZY* pDBG ) {
		// a =b +c*d 	// iADD 1 // de nem adtam hozzá még a c-t
		//   -1-^
		// a =b+c +d*e  // iADD 2 // de nem adtam hozzá még a d-t
		//   -2-1-^
		// d*e +f*g		// iADD 1
		//-2-1-^
		// d*e +f -g*h	// iADD 2
		//-3-2 -1-^
		///kADD(scp); //, iOPe );
		///kOBJ(scp); //, iOPe );
		///LEVmulEXP()[lMUL++] = now;
		///++SP;
		///++stkCD;
		//if( !pBLK )
		//	pBLK = srcBLKnew( pS, gpeOPid_stk, NULL, -1, -1 );

		gpROW* pRl = pBLK->pLSTrow();
		if( !pRl )
			return pBLK;

		switch( pBLK->opIDgrp() )
		{
			case gpeOPid_brakS:
			case gpeOPid_dimS:
			case gpeOPid_begin:
				return srcBLKup( pS, pBLK, opID, mnID );
			case gpeOPid_nop:
				pBLK->opID = opID;
			case gpeOPid_mul:
				///					pRl
				/// a = b + c *		d /
				pRl->pstOP = opID;
				pBLK->pNEWrow();
				return pBLK;
			case gpeOPid_entry:{
					gpBLK	*pBLKm = srcINSTanDalf( pS, NULL, pBLK );
					if( pBLKm->opIDgrp() == gpeOPid_mul )
					{
						pRl = pBLKm->pLSTrow();
						pRl->pstOP = opID;
						pBLKm->pNEWrow();
						return pBLKm;
					}
					return srcBLKinsrt( pS, pBLK, opID, mnID );
				} break;
			default:
				break;
		}

		///				pRl	// up
		/// a = b +		c *
		return srcBLKup( pS, pBLK, opID, mnID );
	}
gpBLK* gpcSRC::srcINSTmul( char* pS, gpBLK *pBLKm, gpBLK* pBLK ) {
	if( !pBLKm )
		pBLKm = lzyBLOCK.pSTPdwn( pBLK->bIDm );
    I4  nR = pBLK->nROW();
    if(!nR)
        return pBLKm;
	gpcSRC	*pSRC = NULL;
	gpROW	*pRi;
	gpOBJ	*pOa, *pOin = NULL;
	gpPTR	*pPb = NULL, *pPi;
	gpeOPid opB, opA;
	for( I4 iR = 0; iR < nR; opB=opA, iR++ )
	{
		pPi = pBLK->iROWptr( pS, iR, &pRi, &pOa, &pSRC, &pOin );
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

		U4	nA = gpaCsz[pPi->cID()], //pPi->sOF(),
			nB = gpaCsz[pPb->cID()], //pPb->sOF(),
			nMN = gpmMIN(nA,nB),
			nMX = gpmMIN(gpmMAX(nA,nB)*2, 8 );


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

		switch( opB ) {
			case gpeOPid_mul:
			case gpeOPid_div:
			case gpeOPid_and:
			case gpeOPid_rem:
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
