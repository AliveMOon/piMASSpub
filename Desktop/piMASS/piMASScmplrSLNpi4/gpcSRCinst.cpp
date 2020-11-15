#include "gpcSRC.h"
#include "gpcSRCinst.h"
#include "gpcSRClnk.h"
#include "gpccrs.h"
extern U1 gpaALFsub[];
extern char gpaALF_H_sub[];

gpBLOCK* gpcSRC::srcINSTdwn( char* pS, gpBLOCK *pBLKm, gpBLOCK* pBLK, gpBLOCK* pBLKup, I4 mnID ) {
	if( !pBLKm )
		pBLKm = lzyBLOCK.pSTPdwn( pBLK->bIDm );
	if( pBLKup )
		pBLK = pBLKup;
	I4 aiPC[2], sOF = 0, cID = -1;
	U1* pU1 = NULL;

	//gpROW *pR0 = pBLK->pROW();
	//aiPC[0] = iPCrow( pR0, sOF, true );
	_move._l.EAl(pBLK->iPC).A0;

	gpROW *pRm = pBLKm->pROW(pBLK->bIDmR);
	aiPC[1] = pRm ? iPCrow( *pRm, sOF, false ) : gpeCsz_L;
	//pU1 = srcMEMiPC( iPC, sOF );
	_move._l.EAl(aiPC[1]).A1;
	_move._L.IA0I.IA1I;
	_nop;
	return pBLKm;
}

gpMEM::gpMEM( gpcSRC* pS, gpcWIN* pW, gpcLZY* pSRCstk, I4 i ) {
	gpmCLR;
	pSRC = pS;
	pWIN = pW;
	pMASS = pWIN ? pWIN->piMASS : NULL;
	pA = (I8*)aA;
	pD = (I8*)(aA+8);
	pA[7] = iSTK = nDAT = i;
	nDAT += 0x100;
	nDCTscp = pSRC->aSCOOP[0].nDCT;

	pLZYsrcXFND = pSRCstk;
	if( !pLZYsrcXFND )
		return;

	nXFND = pLZYsrcXFND->nLD(sizeof(U4));
}
gpPTR* gpPTR::pNULL(){ gpmCLR; return this; }
U4x2* gpPTR::pd2D(){ return (U4x2*)&x; };
gpPTR* gpPTR::d2D( I4x2& d2 ){
    x = d2.x;
    y = d2.y;
    z = 0;
    return this;
}
U4 gpPTR::area(){
    if( z )
        return z;
    return z = x*y;
}
U4 gpPTR::sOF(){
    if( w )
        return w;
    if(!sof)
        sof = gpaCsz[cID];

    return w = area()*sof;
}
U1* gpMEM::instVAR( U1* p_dst, gpINST& inst )
{
	switch( inst.a8x2.alf )
	{
		case gpeALF_FPS:
			*(U4*)p_dst = pWIN->mSEC.w;
			break;
        case gpeALF_entry: if(pSRC) {
                U4      *pU4    = (U4*)pSRC->srcMEMiPC( pA[7], 4 );
                gpOBJ   *pOa    = pSRC->srcOBJfnd( pU4[1] ),
                        *pOb = NULL, *pOc = NULL;
                gpPTR* pPTR = NULL;

                U4      xfnd    = pMASS->getXFNDan( pOa->AN );
                if( gpcSRC  *pSRCb  = pMASS->srcFND( xfnd ))
                {
                    if( pOb    = pSRC->srcOBJfnd( pU4[0] ))
                    if( gpOBJ   *pOin   = pSRCb->pMEM->pOBJ(pOb->AN.alf) )
                    {
                        /// tervek szerint a isza térési cim A0-ban lesz
                        U4 nCPY = pOin->sOF();
                        U1  *pUin = pSRCb->srcMEMiPC( pOin->iPC, nCPY ),
                            *pDST = NULL;
                        if(bSTDcout_ent)
                            {std::cout << stdALU << pUin << std::endl;}
                        pOc = getOBJptr( (U1*)pU4, (pD[7]-pA[7]), 0 );
                        if( pPTR = (gpPTR*)pSRC->srcMEMiPC( pOc->iPC, pOc->sOF() ) )
                        {
                            U4  asOF[2];
                            asOF[0] = pPTR->iPC ? pPTR->sOF() : 0;
                            asOF[1] = 0;
                            if( asOF[0] < nCPY )
                            {
                                if( pPTR->cID != pOin->cID() )
                                {
                                    pPTR->cID = pOin->cID();
                                }
                                pPTR->d2D( pOin->d2D );
                                asOF[1] = gpmPAD( pPTR->sOF(), 0x10 );
                                if( !pPTR->iPC )
                                {
                                    pPTR->iPC = nDAT;
                                    nDAT += asOF[1];
                                }
                                pDST = pSRC->srcMEMiPC( pPTR->iPC, asOF[1] );
                            } else
                                pDST = pSRC->srcMEMiPC( pPTR->iPC, pPTR->sOF() );

                            gpmMcpy( pDST, pUin, asOF[1] );
                        }

                        if(bSTDcout_ent)
                            {std::cout << stdALU "pSRCb" << std::endl;}
                    }
                }
                pA[0] = pPTR ? pPTR->iPC : 0;
            } break;
		default:
			break;
	}
	return p_dst;
}


gpOBJ* gpMEM::getOBJptr( U1* pU1, U4 nBYTE, I4 nmID )
{
    if( this ? (nBYTE?!pU1:true) : true )
        return NULL;

    if( nmID > 0 )
        nmID *= -1;

    U4 iDb = lzyDCTbin.dctFND( pU1, nBYTE, nDCTbin ), iDbN, iOb;
    if( iDb < nDCTbin )
    if( (iDbN=lzyDCTbin.nBINix(iDb)) != nBYTE )
        iDb = nDCTbin;

    gpOBJ* pO = NULL;
    gpPTR* pPTR = NULL;

    if( iDb < nDCTbin )
    {
        iOb = *(U4*)lzyBINlnk.Ux( iDb, sizeof(iOb) );
        gpOBJ* pO0 = gpmLZYvali( gpOBJ, &lzyOBJ );
        pO = pO0+iOb;
        pPTR = (gpPTR*)pSRC->srcMEMiPC( pO->iPC, pO->sOF() );
        return pO;
    }
    // nem volt a SZóTáRBAN
    iDb = nDCTbin; // a végére kerül ha hozzá adjuk
    lzyDCTbin.dctADD( pU1, nBYTE );
    nDCTbin++;

    iOb = lzyOBJ.nLD(sizeof(gpOBJ));
    *(U4*)lzyBINlnk.Ux( iDb, sizeof(iOb) ) = iOb;

    pO = pSRC->srcOBJadd( NULL, nmID);
    pO->REcID(gpeCsz_ptr);
    pPTR = (gpPTR*)pSRC->srcMEMiPC( pO->iPC = nDAT, pO->sOF() );
    U4 n = gpmPAD( pO->sOF(), 0x10 );
    nDAT += n;
    pPTR->pNULL()->oID = iOb;
    return pO;
}


