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

	if( !bPTR() )
		return pMEM->pUn( iPC, gpaCsz[cID()] );

	gpPTR* pPi = pPTRu1(pMEM);
	if( !pPi->bPTR() )
		return NULL;

	return pMEM->pUn( pPi->iPC, gpaCsz[pPi->cID()] );
}
///--------------------------
///			gpOBJ
///--------------------------

gpcVAR gpVAR[] = {
		gpcVAR( gpeALF_MSEC, gpeCsz_L ),
		gpcVAR( gpeALF_MS, gpeCsz_L ),

		gpcVAR( gpeALF_IA, gpeCsz_L ),
		gpcVAR( gpeALF_IN, gpeCsz_L ),

		gpcVAR( gpeALF_IW, gpeCsz_L ),
		gpcVAR( gpeALF_IH, gpeCsz_L ),

		gpcVAR( gpeALF_MX, gpeCsz_L ),
		gpcVAR( gpeALF_MY, gpeCsz_L ),
		gpcVAR( gpeALF_MLB, gpeCsz_L ),

		gpcVAR( gpeALF_IX, gpeCsz_L ),
		gpcVAR( gpeALF_IY, gpeCsz_L ),

		gpcVAR( gpeALF_FPS, gpeCsz_L ),
};
gpcLZY::gpcLZY( gpcVAR* pVAR, U4 n ) {
	gpmCLR;
	I8 nI8 = 0, iI8;
	for( U4 i = 0; i < n; i++ ) {
		iI8 = tree_fnd( pVAR[i].alf, nI8 );
		if( iI8 < nI8 )
			continue;

		tree_add( pVAR[i].alf, nI8 );
	}
}

gpcLZY gpLZYvar( gpVAR, gpmN(gpVAR) );

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
		pPTR->cID(gpeCsz_OFF);
		pPTR->iPC = -1;
		return pPTR;
	}
	return pMEM->pPTR( iPTR );
}
gpPTR* gpOBJ::pPTRu1(){
	if( !this )
		return NULL;
	return pPTR()->pPTRu1( pMEM );
}
gpPTR* gpOBJ::pPTRd2D(){
	gpPTR* pPt = pPTR(), *pPu;
	if( !pPt )
		return NULL;

	while( pPt->bPTR() )
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
U4 gpOBJ::sOF() {
	return pPTRu1()->sOF();
}
I4 gpOBJ::cID() {
	return pPTRu1()->cID();
}
I8 gpOBJ::iVAR() {
	if( this ? !AN.alf : true )
		return -1;
	I8	n = gpLZYvar.nLD(sizeof(gpcVAR)),
		i = gpLZYvar.tree_fnd( (I8)AN.alf,n);
	return i < n ? i : -1;
}
bool gpOBJ::bVAR() {
	return iVAR() >= 0;
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

			pPTR->cID( gpeCsz_OFF );
			pPTR->iPC = -1;
		}
		return pPTR;
	}
	return pMEM->pPTR( iPTR );
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
	if(bSTDcoutCMP){std::cout << stdCMPLR " CMP" stdRESET << (char*)pDBG->p_alloc  << std::endl;}

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
				pBLK = srcBLKaryAN( pS, pBLK, MiN.dctID, mnID, cID, AN );
				break;
			case gpeCLR_orange:	///NUM
				cID = AN.gpCszNUM(psDCT,nsDCT);
				if( cID == gpeCsz_OFF )
					continue;
				pBLK = srcBLKaryNUM( pS, pBLK, MiN.dctID, mnID, cID, AN );
				break;
			case gpeCLR_green2: { ///OPER
					U1* pSs = (U1*)psDCT, *pSe = pSs+nsDCT;
					U4 nOx, nSTP;
					while( pSs < pSe )
					{
						opID = (gpeOPid)dOP.dctMILLfnd( pSs, pSe-pSs, nOx = (U4)gpeOPid_jsr );
						nSTP = dOP.nSTRix(opID);
						pSs += nSTP;
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
						//break;
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


