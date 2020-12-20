#include "gpcSRC.h"
#include "gpcSRCinst.h"
#include "gpcSRClnk.h"
#include "gpccrs.h"
extern U1 gpaALFsub[];
extern char gpaALF_H_sub[];



gpPTR* gpPTR::pNULL( I4 i ){ gpmCLR; iPC = i; return this; }
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
    if(!cSZ)
        cSZ = gpaCsz[cID()];

    return w = area()*cSZ;
}
gpPTR& gpPTR::operator = ( gpBLK* pBLK ) {
    /// ennek csak az a célja pBLK targetból kimásolja ami van
    gpmCLR;
    gpPTR* pBp = pBLK->BLKpPTR( NULL );
    if( !pBp ) {
        iPC = -1;
        cID( gpeCsz_OFF );
        return *this;
    }
	mNdID = pBp->mNdID;
    iPC = pBp->iPC;
    cID(pBp->cID());
    cSZ = gpaCsz[cID()];
    d2D( *pBp->pd2D() );
    return *this;
}
gpPTR& gpPTR::operator = ( gpOBJ* pO ) {
    /// ennek csak az a célja pBLK targetból kimásolja ami van
    gpmCLR;
	gpPTR* pPo = pO->pPTR();
    if( !pO ) {
        iPC = -1;
		cID( gpeCsz_OFF );
        return *this;
    }
    mNdID = pPo->mNdID;
    iPC = pPo->iPC;
    cID(pPo->cID());
    cSZ = gpaCsz[cID()];
    d2D( *pPo->pd2D() );
    return *this;
}

gpPTR* gpcSRC::SRCpPTR( char* pS, gpROW& R ) {
	gpPTR* pPTR = NULL;
    if( gpBLK* pBup	= 	R.bIDup
						? lzyBLOCK.pSTPup( R.bIDup, -1, -1, R.mnID )
						: NULL )
    if( pBup->iPTR > 0 ) {
		pPTR = pBup->BLKpPTR( pS );
        while( pPTR->cID() == gpeCsz_ptr ) {
			pPTR = pMEM->pPTR( pPTR->iPC );
        }
        return pPTR;
    }
	gpOBJ* pO = srcOBJfnd( R.mNdID );
	if( !pO )
		return pPTR;
    pPTR = pO->pPTR();
    return pPTR;
}
gpPTR* gpPTR::pPTR( gpMEM* pMEM ){
	return this ? (gpPTR*)pMEM->pUn( iPC ) : NULL;
}
gpPTR* gpPTR::pPTRu1( gpMEM* pMEM ) {
	gpPTR* pPi = this, *pPu;
	while(pPi->bPTR()){
		pPu = pPi->pPTR( pMEM );
		if( pPu ) {
			pPi = pPu;
			continue;
		}
		break;
	}
	return pPi;
}
gpPTR* gpPTR::cpyREF( U1* pALL, gpPTR* pRF )
{
	if( !pRF )
		return pNULL();

	pRF->sOF();
	if(pRF->bPTR()) {
		gpmMcpy( this, pRF, gpmOFF(gpPTR,mNdID) );
		return this;
	}
	iPC = (U1*)pRF - pALL;
	x = y = 1;
	cID( gpeCsz_ptr );
	return this;
}
gpPTR* gpPTR::cpy( gpMEM* pMEM, gpPTR* pPb )
{
	if( !this )
		return NULL;

	I4 iPPC = iPC;
	pPb = pPb->pPTRu1(pMEM);
	*this = *pPb;
	iPC = iPPC;

	U4	nA = pMEM->nALL(iPC),
		nB = sOF();	/// ez azért jó mert átmásolta a REF az adatokat

	if( nA < nB ) {
		/// nem lett még lefoglalva
		if( nA )
			pMEM->iFREE( iPC );
		iPC = pMEM->iALL( nB );
	}

	_move._l.EAl( pPb->iPC ).A0;
	_move._l.EAl( iPC ).A1;
	_move.c((gpeCsz)cID()).IA0I.IA1I;
	return this;
}
gpBLK* gpBLK::pRST( gpMEM* pM )
{
	if( this ? !!pMEM : true )
		return this;

	pMEM = pM;
	return this;
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
        pPTR = pO->pPTR();
        return pO;
    }
    // nem volt a SZóTáRBAN
    iDb = nDCTbin; // a végére kerül ha hozzá adjuk
    lzyDCTbin.dctADD( pU1, nBYTE );
    nDCTbin++;

    iOb = lzyOBJ.nLD(sizeof(gpOBJ));
    *(U4*)lzyBINlnk.Ux( iDb, sizeof(iOb) ) = iOb;

    pO = pSRC->srcOBJadd( NULL, nmID);
    //pO->REcID(gpeCsz_ptr);
    pPTR = pO->pPTR(); //(gpPTR*)pSRC->srcMEMiPC( pO->iPC = nDAT, pO->sOF() );
    /*U4 n = gpmPAD( pO->sOF(), 0x10 );
    nDAT += n;
    pPTR->pNULL()->oID = iOb;*/
    return pO;
}


