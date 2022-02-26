#include "gpcSRC.h"
#include "gpcSRCinst.h"
#include "gpcSRClnk.h"
#include "gpccrs.h"
extern U1 gpaALFsub[];
extern char gpaALF_H_sub[];
U1* gpMEM::instJSR( U1* p_dst, gpINST& inst ) {
	switch( inst.a8x2.alf ) {
		case gpeALF_MLB: *(I4*)p_dst = pWIN->instVARmlb(); break;
		case gpeALF_MX: *(I4*)p_dst = pWIN->instVARmx(); break;
		case gpeALF_MY: *(I4*)p_dst = pWIN->instVARmy(); break;

		case gpeALF_MSEC:
		case gpeALF_MS: *(I4*)p_dst = pWIN->instVARms(); break;

		case gpeALF_iA: *(I4*)p_dst = pWIN->instVARia(); break;
		case gpeALF_iN: *(I4*)p_dst = pWIN->instVARin(); break;

		case gpeALF_IX: *(I4*)p_dst = pWIN->instVARix(); break;
		case gpeALF_IY: *(I4*)p_dst = pWIN->instVARiy(); break;
		case gpeALF_IW: *(I4*)p_dst = pWIN->instVARiw(); break;
		case gpeALF_IH: *(I4*)p_dst = pWIN->instVARih(); break;


		case gpeALF_FPS: *(U4*)p_dst = pWIN->mSEC.w; break;
        case gpeALF_entry: if(pSRC) {
                I4      *pI4 = (I4*)pUn(pA[7], sizeof(*pI4)*2); // pSRC->srcMEMiPC( pA[7], 4 );
                gpOBJ   *pOan = pSRC->srcOBJfnd( pI4[1] ),
						*pOhr = NULL,
						*pOfrm = NULL;
                gpPTR	*pPhr = NULL,
						*pPfrm = NULL;

                U4      xfnd = pMASS->getXFNDan( pOan->AN ),
						nA, 	nCPY;
                U1		*pDST,	*pCPY;

                if( gpcSRC  *pSfrm = pMASS->srcFND( xfnd ) )
				if( pOhr = pSRC->srcOBJfnd(pI4[0]) )
                if( pOfrm = pSfrm->pMEM->pOBJ(pOhr->AN.alf) ) {
					//					0x00002770 move.L A7,D7
					//					0x00002788 move.l 0x10,-(A7)
					//					0x000027a0 move.l 0x12,-(A7)
					//					0x000027b8 jsr entry
					//					0x000027d0 move.L D7,A7
					//					0x000027e8 move.l 0x21d0,A1		; //0,
					//					0x00002800 move.l A0,(A1)

					// FROM->HERE
					pPhr = pPTRu1(pA[0]);

					/// tervek szerint a visza térési cim A0-ban lesz
					pPfrm = pOfrm->pPTRu1();
					nCPY = pOfrm->sOF();
					pCPY = pOfrm->pU1();

					pOhr->bENTRY = true;

					nA = nALL(pPhr->iPC);
					if( nA < nCPY ) {
						if( nA )
							iFREE( pPhr->iPC );
						pPhr->iPC = iALL( nCPY );
					}
					nA = nALL(pPhr->iPC);
					pPhr->cID(pPfrm->cID());
					pPhr->d2D( *pPfrm->pd2D() );

					pDST = pPhr->pU1( this );
					if( pDST )
						gpmMcpy( pDST, pCPY, nCPY )[nCPY] = 0;

					if(bSTDcout_jsr)
					{gpdCOUT	<< stdCYAN << "gpeALF_entry:"
								<< stdALU << (pDST?(char*)pDST:"?") << gpdENDL;}
				}

            } break;
		case gpeALF_PRINT:	funPRINT(); break;
		case gpeALF_FND:	funFND(); break;
		case gpeALF_NEW:	funNEW(); break;

		default:
			*(I4*)p_dst = 0;
			break;
	}
	return p_dst;
}
U1* gpMEM::instENTRY( U1* p_dst ) {
	U4 n = pD[7]-pA[7];
	U1* pALL = lzyMEM.p_alloc, *p_src;
	int d0;
	p_src = pALL+pA[7];
	for( U4 i = 0; i < n; i+=4 ){
		d0 = *(int*)(p_src+i);
	}
	return p_dst;
}
gpBLK* gpcSRC::srcBLKanDalf( char* pS, I4 mnID, gpBLK* pBLK, gpeOPid opID, gpcLZY* pDBG ) {
	/// + a0.b
	/// * a0.b

	gpROW* pRl = pBLK->pLASTrow();
	if( !pRl )
		return pBLK;

	gpOBJ* pO = srcOBJfnd(pRl->mNdID);
	switch( pO ? pO->cAN : gpeCsz_OFF )
	{
		case gpeCsz_a:
		case gpeCsz_c:
			/// a0.b
			/// b.a0
			return srcBLKup( pS, pBLK, opID, mnID );
		case gpeCsz_b:
			if( pO->bUTF8() )
			{
				// itt lehetne egy név alapján keresni

			}
		default: break;
	}
	return pBLK;
}
gpBLK* gpcSRC::srcINSTanDalf( char* pS, gpBLK *pBLKm, gpBLK* pBLK ) {
	if( !pBLKm )
		pBLKm = lzyBLOCK.pSTPdwn( pBLK->bIDm );
    I4  nR = pBLK->nROW();
    if(!nR)
        return pBLKm;
	gpcSRC	*pSRC = NULL;
	gpROW	*pRi;
	gpOBJ	*pOblk,*pOa, *pOin = NULL;
	gpPTR	*pPb = NULL, *pPi;
	gpeOPid opB, opA;
	for( I4 iR = 0; iR < nR; opB=opA, iR++ )
	{
		pPi = pBLK->iROWptr( pS, iR, &pRi, &pOa, &pSRC, &pOin );
		opA = pRi->pstOP;
		if( !pPi )
			continue;
		if( !pPb ) {
			pPb = pPi; // pBLK->BLKpPTR( pS )->cpy( pMEM, pPi );
			continue;
		}

		if( !pOin ) {
			pSRC = NULL;
			pPb = NULL;
			continue;
		}

		#ifdef stdON
			if(bSTDcout){gpdCOUT << stdALU "pSRC" << gpdENDL;}
		#endif

		_move._q.A7.D7;

			_move._l.EAl( pPb->mNdID ).sIA7I;
			_move._l.EAl( pPi->mNdID ).sIA7I;

			pPb = pBLK->BLKpPTR( pS );
			//pPb->cpyREF( pMEM->pUn(), pPi);
			//pPb->iPC = -1;

			_move._l.EAl( pBLK->iPTR ).A0;
			_jsr.EAl( gpeALF_entry );

		_move._q.D7.A7;
		pOin = NULL;
		pSRC = NULL;
		pPb = NULL;
	}
	return pBLKm;
}
