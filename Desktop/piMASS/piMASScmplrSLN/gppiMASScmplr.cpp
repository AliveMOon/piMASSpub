#include "gpcSRC.h"
#include "gpcSRClnk.h"
#include "gpccrs.h"



///--------------------------
///			gpPTR
///--------------------------
U1* gpPTR::pU1( gpMEM* pMEM )
{
	if( !this )
		return NULL;
	if( pMEM ? (iPC<0) : true )
	{
		if( !bckID )
			return NULL;
		gpOBJ* pO = pMEM->OBJfnd(bckID);
		return pO->pU1();
	}

	return pMEM->pUn( iPC, gpaCsz[cID] );
}
///--------------------------
///			gpOBJ
///--------------------------
gpPTR* gpOBJ::pPTR(){
	if( this ? !pMEM : true )
		return NULL;

	if( iPTR < 0 )
	{
		iPTR = pMEM->nDAT;
		pMEM->nDAT += gpmPAD( sizeof(gpPTR), 0x10 );
		gpPTR* pPTR = pMEM->pPTR( iPTR );
		if( !pPTR )
			return NULL;
		pPTR->mNdID = dctID;
		pPTR->cID = gpeCsz_OFF;
		pPTR->iPC = -1;
		return pPTR;
	}
	return pMEM->pPTR( iPTR );
}
gpPTR* gpOBJ::pPTRu1(){
	gpPTR* pP = pPTR(), *pPu;
	if( !pP )
		return NULL;
	while( pP->cID == gpeCsz_ptr )
	{
		pPu = pMEM->pPTR( pP->iPC );
		if( !pPu )
			break;
		pP = pPu;
	}
	return pP;
}
gpPTR* gpOBJ::pPTRd2D(){
	gpPTR* pPt = pPTR(), *pPu;
	if( !pPt )
		return NULL;

	while( pPt->cID == gpeCsz_ptr )
	{
		pPu = pMEM->pPTR( pPt->iPC );
		if( !pPu )
			break;
		U4 nA = pPu->pd2D()->area();
		if( nA > 1 )
			return pPu;

		pPt = pPu;
	}
	return pPt;
}
U1* gpOBJ::pU1(){
	gpPTR* pP = pPTRu1();
	if( !pP )
		return NULL;
	return pP->pU1(pMEM);

	return pMEM->pUn( pP->iPC, pP->sOF() );
}
///--------------------------
///			gpBLK
///--------------------------
gpPTR* gpBLK::BLKpPTR( char* pS ) {
	if( this ? !pMEM : true )
		return NULL;



	if( iPTR < 0 )
	{
		gpOBJ* pOblk = pMEM->OBJadd( pS, -mnID );
		if( pOblk )
			iPTR = pOblk ? pOblk->iPTR : -1;
		gpPTR* pPTR = pMEM->pPTR( iPTR );
		if( pPTR )
			return pPTR;

		if( iPTR < 0 )
		{
			iPTR = pMEM->nDAT;
			pMEM->nDAT += gpmPAD( sizeof(gpPTR), 0x10 );
			gpPTR* pPTR = pMEM->pPTR( iPTR );
			pPTR->mNdID = mnID;
			if( pPTR->mNdID > 0 )
				pPTR->mNdID *= -1;

			pPTR->cID = gpeCsz_OFF;
			pPTR->iPC = -1;
		}
		return pPTR;
	}
	return pMEM->pPTR( iPTR );
}

///--------------------------
///			gpMEM::gpOBJ
///--------------------------
gpOBJ* gpMEM::OBJfnd( I4 dctID ) {
	if( !this )
		return NULL;
	gpOBJ* pO0 = gpmLZYvali( gpOBJ, &lzyOBJ );
	if( !pO0 )
		return NULL;
	for( U4 nO = lzyOBJ.nLD(sizeof(gpOBJ)), o = 0; o < nO; o++ ) {
		if( pO0[o].dctID != dctID )
			continue;
		return pO0+o;
	}
	return NULL;
}

gpOBJ* gpMEM::OBJadd( char* pS, I4 dctID ) {
	gpOBJ *pO = OBJfnd(dctID);
	if( pO )
		return pO;

	U4 nO = lzyOBJ.nLD(sizeof(gpOBJ));
	pO = (gpOBJ*)lzyOBJ.Ux( nO, sizeof(*pO) );
	if( !pO )
		return NULL;
	pO->pMEM = this;
	pO->dctID = dctID;
	pO->iPTR = -1;
	gpPTR* pPTR = pO->pPTR();
	return pO;
}
gpOBJ* gpcSRC::srcOBJfnd( I4 dctID ) { return pMEM->OBJfnd(dctID); }
gpOBJ* gpcSRC::srcOBJadd( char* pS,I4 dctID ) { return pMEM->OBJadd(pS,dctID); }
///--------------------------
///			gpSRC::gpBLK
///--------------------------
gpBLK* gpcSRC::srcBLKmNdID( char* pS, gpBLK* pBLK, I4 dctID, I4 mnID ) {
	gpOBJ *pO = srcOBJfnd(dctID);
	if( !pO )
	{
		pO = srcOBJadd( pS, dctID );
		if( !pO )
			return pBLK;
	}

	if( !pBLK )
		pBLK = srcBLKnew( pS, gpeOPid_stk, NULL, -1, -1, mnID );

	gpROW* pRl = pBLK->pLSTrow();
	if( !pRl )
		return pBLK;

	*pRl = pO;
	if( pRl->mnID == mnID )
		return pBLK;

	pRl->mnID = mnID;
	return pBLK;
}

gpBLK* gpcSRC::srcBLKaryNUM( char* pS, gpBLK* pBLK, I4 dctID, I4 mnID, gpeCsz cAN, const I8x2& AN ) {

	pBLK = srcBLKmNdID( pS, pBLK, dctID, mnID );
	gpOBJ* pO = srcOBJadd( pS, dctID );
	if( !pO )
		return pBLK;

	gpPTR* pP = pO->pPTRu1();
	pP->x = pP->y = 1;
	pP->z = 0;
	pP->cID = cAN;
	U4 nS = sizeof(AN);

	I4x4* pALL = pMEM->pALL( pP->iPC );
	if( (pALL?pALL->n:0) < nS )
	{
		pMEM->iFREE( pP->iPC );
		pP->iPC = pMEM->iALL( nS );
	}
	U1* pA = pMEM->pUn( pP->iPC, nS );
	gpmMcpy( pA, &AN, nS );
	return pBLK;
}
gpBLK* gpcSRC::srcBLKaryUTF8( gpBLK* pBLK, I4 mnID, char* pS, U4 nS ) {
	I4 mNdID = mnID;
	if( mNdID > 0 )
		mNdID *= -1;
	else
		mNdID *= -1;

	pBLK = srcBLKmNdID( pS, pBLK, mNdID, mnID );
	gpOBJ* pO = srcOBJadd( pS, mNdID );
	if( !pO )
		return pBLK;

	gpPTR* pP = pO->pPTRu1();
	I4x4* pALL = pMEM->pALL( pP->iPC );
	if( (pALL?pALL->n:0) < nS )
	{
		pMEM->iFREE( pP->iPC );
		pP->iPC = pMEM->iALL( nS );
	}
	U1* pA = pMEM->pUn( pP->iPC, nS );
	gpmMcpy( pA, pS, nS );
	pP->x = nS;
	pP->y = 1;
	pP->z = 0;
	pP->cID = gpeCsz_b;

	return pBLK;
}
gpBLK* gpcSRC::srcBLKaryAN( char* pS, gpBLK* pBLK, I4 dctID, I4 mnID, gpeCsz cAN, const I8x2& AN ) {

	pBLK = srcBLKmNdID( pS, pBLK, dctID, mnID );
	gpOBJ* pO = srcOBJadd( pS, dctID );
	if( !pO )
		return pBLK;

	pO->AN = AN;
	pO->cAN = cAN;
	gpPTR* pP = pO->pPTRu1();
	return pBLK;
}



gpINST* gpMEM::instRDY( gpcLZY* pDBG ) {
	if( this ? !nCD : true )
		return this ? pINST : NULL;

	if( !pcCPY )
	{
		pcCPY = gpmPAD(nDAT,0x10);
	}
	U8 s = -1;
	pINST = (gpINST*)lzyMEM.Ux( pcCPY, lzyCODE.n_load, true, 1 );

	gpmMcpy( pINST, lzyCODE.p_alloc, lzyCODE.n_load );
	nDAT = lzyMEM.n_load;
	for( I4 i = 0; i < nCD; i++ )
		pINST[i].instDBG(pDBG,this,lzyMEM.p_alloc);

	pc = 0;
	aA[7] = iSTK;
	return pINST;
}
void gpcSRC::srcCMPLR( gpcLZYdct& dOP, U1 iSCP, gpcWIN* pW, gpcLZY* pSRCstk ) {
	if( !this )
		return;

	pDBG = pDBG ? pDBG->lzyRST() : new gpcLZY;

	if( pMEM ) {
		gpmDEL(pMEMo);
        if( pMEM ? pMEM->pGL : NULL )
            gpmDEL( pMEM->pGL->pCAM );

		pMEMo = pMEM;
	}
	pMEM = new gpMEM( this, pW, pSRCstk );
#ifdef stdON
	if(bSTDcout){std::cout << stdCMPLR " CMP" stdRESET << std::endl;}
#endif
	I8x4 *pM0 = gpmSCP.pMN(), MiN, Mnx;
	U4x4 *pL0 = gpmSCP.pLNK(); //, aLNK[0x10];
	gpeCLR clr;
	char		*pS = (char*)gpmSCP.p_str;
	const char	*psDCT;
	I4 nS, nsDCT;
	gpBLK* pBLK = srcBLKnew( pS, gpeOPid_stk, NULL, -1, -1, 0 );
	I8x2 AN; gpeCsz cID = gpeCsz_OFF;
	gpeOPid opID;
	for( U4 nM = gpmSCP.nMN(), mnID = 0, l; mnID < nM; mnID++ ) {
		if( pM0[mnID].MNclr() == gpeCLR_red2 )
			continue;

		MiN = pM0[mnID];
		pS = (nS=MiN.nS) ? (char*)gpmSCP.p_str+MiN.iS : NULL;
		if( MiN.MNtyp() == gpeTYP_STR )
		{
			if( MiN.MNclr() != gpeCLR_violet )
				continue;
			///STR
 			pBLK = srcBLKaryUTF8( pBLK, mnID, pS, nS );
			continue;
		}


		U4x4& lnk = pL0[MiN.dctID];
		if( !lnk.y )
		{
			psDCT = gpmSCP.lzyDCT.sSTRix(MiN.dctID, NULL);
			if( !psDCT )
				continue;
			nsDCT = gpmSCP.lzyDCT.nSTRix(MiN.dctID);
			if( !nsDCT )
				continue;
		}

		switch( MiN.MNclr() )
		{
			case gpeCLR_blue2: 	///ABC
				cID = AN.gpCszALF(psDCT,nsDCT);
				if( cID == gpeCsz_OFF )
					continue;
				pBLK = srcBLKaryAN( pS, pBLK, MiN.dctID, lnk.x, cID, AN );
				break;
			case gpeCLR_orange:	///NUM
				cID = AN.gpCszNUM(psDCT,nsDCT);
				if( cID == gpeCsz_OFF )
					continue;
				pBLK = srcBLKaryNUM( pS, pBLK, MiN.dctID, lnk.x, cID, AN );
				break;
			case gpeCLR_green2: { ///OPER
					U1* pSs = (U1*)psDCT, *pSe = pSs+nsDCT;
					U4 nOx;
					while( pSs < pSe )
					{
						opID = (gpeOPid)dOP.dctMILLfnd( pSs, pSe-pSs, nOx = (U4)gpeOPid_jsr );
						pSs += dOP.nSTRix(opID);
						switch( opID )
						{
							case gpeOPid_brakS:
							case gpeOPid_dimS:
							case gpeOPid_begin:
								/// LENT
								pBLK = srcBLKbrakS( pS, mnID, pBLK, opID );
								/// FENT
								break;
							case gpeOPid_end:
							case gpeOPid_dimE:
							case gpeOPid_brakE:
								/// FENT
								pBLK = srcBLKbrakE( pS, mnID, pBLK, opID, pDBG );
								/// LENT
								break;
                            default:
								switch( gpaOPgrp[opID] )
								{
									case gpeOPid_mov: /// =
										pBLK = srcBLKmov( pS, mnID, pBLK, opID, pDBG );
										break;
									case gpeOPid_add: /// +
									case gpeOPid_sub: /// ==
										pBLK = srcBLKadd( pS, mnID, pBLK, opID, pDBG );
										break;
									case gpeOPid_mul: /// *
										pBLK = srcBLKmul( pS, mnID, pBLK, opID, pDBG );
										break;

									case gpeOPid_entry: { /// (
										pBLK = srcBLKanDalf( pS, mnID, pBLK, opID, pDBG );
										} break;
									case gpeOPid_out: /// )
										pBLK = srcBLKout( pS, mnID, pBLK, opID, pDBG );
										break;
									case gpeOPid_stk: /// , ;
										pBLK = srcBLKstk( pS, mnID, pBLK, opID, pDBG );
										break;
								}
						}

						break;
					}

				} break;
			case gpeCLR_red2:	///BREAK
				break;

			case gpeCLR_violet:	///STR
			case gpeCLR_green:	///NOTE
			default:
				continue;
		}

	}

	pMEM->instRDY( pDBG );
}


