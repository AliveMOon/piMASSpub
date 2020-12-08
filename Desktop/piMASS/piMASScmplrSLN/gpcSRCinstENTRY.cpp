#include "gpcSRC.h"
#include "gpcSRCinst.h"
#include "gpcSRClnk.h"
#include "gpccrs.h"
extern U1 gpaALFsub[];
extern char gpaALF_H_sub[];
U1* gpMEM::instVAR( U1* p_dst, gpINST& inst )
{
	switch( inst.a8x2.alf )
	{
		case gpeALF_FPS:
			*(U4*)p_dst = pWIN->mSEC.w;
			break;
        case gpeALF_entry: if(pSRC) {
                I4      *pI4 = (I4*)pUn(pA[7], sizeof(*pI4)*2); // pSRC->srcMEMiPC( pA[7], 4 );
                gpOBJ   *pOan = pSRC->srcOBJfnd( pI4[1] ),
						*pOa = NULL,
						*pOfrm = NULL;
                gpPTR	*pPhr = NULL,	*pPfrm = NULL;

                U4      xfnd = pMASS->getXFNDan( pOan->AN ),
						nA, 	nCPY;
                U1		*pDST,	*pCPY;

                if( gpcSRC  *pSfrm = pMASS->srcFND( xfnd ) )
				if( pOa = pSRC->srcOBJfnd(pI4[0]) )
                if( pOfrm = pSfrm->pMEM->pOBJ(pOa->AN.alf) )
				{
					I4 iPhr = pA[0];
					pPhr = pPTR(iPhr);

					/// tervek szerint a isza térési cim A0-ban lesz
					pPfrm = pOfrm->pPTR();
					nCPY = pOfrm->sOF();
					pCPY = pOfrm->pU1();

					nA = nALL(pPhr->iPC);
					if( nA < nCPY ) {
						if( nA )
							iFREE( pPhr->iPC );
						pPhr->iPC = iALL( nCPY );
					}
					nA = nALL(pPhr->iPC);
					pPhr->cID = pPfrm->cID;
					pPhr->d2D( *pPfrm->pd2D() );

					pDST = pPhr->pU1( this );
					if( pDST )
						gpmMcpy( pDST, pCPY, nCPY )[nCPY] = 0;

					if(bSTDcout_ent)
						{std::cout	<< stdCYAN << "gpeALF_entry:"
									<< stdALU << (pDST?(char*)pDST:"?") << std::endl;}
				}

            } break;
		default:
			break;
	}
	return p_dst;
}

gpBLK* gpcSRC::srcINSTent( char* pS, gpBLK *pBLKm, gpBLK* pBLK ) {
	if( !pBLKm )
		pBLKm = lzyBLOCK.pSTPdwn( pBLK->bIDm );
    I4  nR = pBLK->nROW();
    if(!nR)
        return pBLKm;
	gpcSRC	*pSRC = NULL;
	gpROW	*pRa;
	gpOBJ	*pOblk,*pOa, *pOin = NULL;
	gpPTR	*pPb = NULL, *pPa;
	gpeOPid opB, opA;
	for( I4 iR = 0; iR < nR; opB=opA, iR++ )
	{
		pPa = pBLK->iROWptr( pS, iR, &pOa, &pRa, &pSRC, &pOin );
		opA = pRa->pstOP;
		if( !pPa )
			continue;
		if( !pPb ) {
			pPb = pPa; // pBLK->BLKpPTR( pS )->cpy( pMEM, pPa );
			continue;
		}

		if( !pOin ) {
			pSRC = NULL;
			pPb = NULL;
			continue;
		}

		#ifdef stdON
			if(bSTDcout){std::cout << stdALU "pSRC" << std::endl;}
		#endif

		_move._q.A7.D7;

			_move._l.EAl( pPb->mNdID ).sIA7I;
			_move._l.EAl( pPa->mNdID ).sIA7I;
			pPb = pBLK->BLKpPTR( pS );
			pPb->cpyREF(pPa);
			pPb->iPC = -1;
			_move._l.EAl( pBLK->iPTR ).A0;
			/*pPb->cID = pOin->cID();
			pPb->d2D( *pOin->pd2D() );*/
			_jsr.EAl( gpeALF_entry );

		_move._q.D7.A7;
		pOin = NULL;
		pSRC = NULL;
		pPb = NULL;
	}
	return pBLKm;
}
