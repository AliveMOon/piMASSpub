#include "gpcSRC.h"
#include "gpcSRClnk.h"
#include "gpccrs.h"


gpBLOCK* gpcSRC::srcBLKbrakE( char* pS, I4 mnID, gpBLOCK* pBLK, gpeOPid opID, gpcLZY* pDBG ) {
	///kEND(scp);
	U1* pU1 = NULL;
	gpBLOCK* pBLKup = NULL;
	while( pBLKup ? (pBLKup->opIDgrp != gpeOPid_entry) : !!pBLK )
	{
		/// FENT
		gpBLOCK	*pBLKm = lzyBLOCK.pSTPdwn( pBLK->bIDm );
		gpROW	*pR0 = pBLK->pROW(),
				*pRd = pBLKm->pROW(pBLK->bIDmR);
		I4 nR = pBLK->nROW(), iPC, sOF;
		switch( pBLK->opIDgrp )
		{
			case gpeOPid_entry: {
					pBLKm = srcINSTdwn( pS, pBLKm, pBLK, pBLKup, mnID );
				} break;
			case gpeOPid_mov:
				pBLKm = srcINSTmov( pS, pBLKm, pBLK );
				break;
			case gpeOPid_add:
			case gpeOPid_sub:
				pBLKm = srcINSTadd( pS, pBLKm, pBLK );
				// move.l d0, -(A6)
				break;
			case gpeOPid_mul:{
					///			pRl
					/// a, 		_ +
					/// a, 		b +
					/// a = 	b +
					/// a * 	b +
				pBLKm = srcINSTmul( pS, pBLKm, pBLK );
				} break;
		}


		if( !pBLKm )
			break;
		/// LENT
		pBLKup = pBLK;
		pBLK = pBLKm;

	}
	return pBLK;

	gpROW	*pRl = pBLK->pLSTrow();
	if( !pRl )
		return pBLK;

	pRl->pstOP = opID;
	/// a veszö egyenlőre csinál helyet
	pBLK->pNEWrow();
	return pBLK;
}
void gpcSRC::srcCMPLR( gpcLZYdct& dOP, U1 iSCP, gpcWIN* pW, gpcLZY* pSRCstk ) {
	if( !this )
		return;

	pDBG = pDBG ? pDBG->lzyRST() : new gpcLZY;

	if( pMEM )
	{
		gpmDEL(pMEMo);
        if( pMEM ? pMEM->pGL : NULL )
            gpmDEL( pMEM->pGL->pCAM );

		pMEMo = pMEM;
	}
	pMEM = new gpMEM( this, pW, pSRCstk );
#ifdef stdON
	if(bSTDcout){std::cout << stdCMPLR " CMP" stdRESET << std::endl;}
#endif
	I8x4 *pM0 = gpmSCP.pMN(), M, Mnx;
	U4x4 *pL0 = gpmSCP.pLNK(); //, aLNK[0x10];
	gpeCLR clr;
	char		*pS = (char*)gpmSCP.p_str;
	const char	*psDCT;
	I4 nS, nsDCT;
	gpBLOCK* pBLK = srcBLKnew( pS, gpeOPid_stk, NULL, -1, -1, 0 );
	I8x2 AN; gpeCsz acID[2] = {gpeCsz_OFF};
	gpeOPid opID;
	for( U4 nM = gpmSCP.nMN(), mnID = 0, l; mnID < nM; mnID++ ) {
		if( pM0[mnID].MNclr() == gpeCLR_red2 )
			continue;

		M = pM0[mnID];
		pS = (nS=M.nS) ? (char*)gpmSCP.p_str+M.iS : NULL;
		if( M.MNtyp() == gpeTYP_STR )
		{
			if( M.MNclr() != gpeCLR_violet )
				continue;
			///STR
			pBLK = srcBLKarySTR( pBLK, mnID, pS, nS );
			continue;
		}


		U4x4& lnk = pL0[M.dctID];
		if( !lnk.y )
		{
			psDCT = gpmSCP.lzyDCT.sSTRix(M.dctID, NULL);
			if( !psDCT )
				continue;
			nsDCT = gpmSCP.lzyDCT.nSTRix(M.dctID);
			if( !nsDCT )
				continue;
		}

		switch( M.MNclr() )
		{
			case gpeCLR_blue2: 	///ABC
				acID[1] = AN.gpCszALF(psDCT,nsDCT);
				if( acID[1] == gpeCsz_OFF )
					continue;
				pBLK = srcBLKaryAN( pS, pBLK, M.dctID, lnk.x, acID, AN );
				break;
			case gpeCLR_orange:	///NUM
				acID[1] = AN.gpCszNUM(psDCT,nsDCT);
				if( acID[1] == gpeCsz_OFF )
					continue;
				pBLK = srcBLKaryNUM( pS, pBLK, M.dctID, lnk.x, acID, AN );
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
								pBLK = srcBLKblkS( pS, mnID, pBLK, opID );
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
										pBLK = srcBLKent( pS, mnID, pBLK, opID, pDBG );

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
U1 gpcSRC::srcBLD( gpcWIN* pW, gpcLZY* pSRCstk ) //, gpcWIN& win )
{
	if( !this )
		return 0;
	if( msBLD ? msBLD > pW->mSEC.x : true )
		return 2;

	msBLD = 0;
	U1 iSCP = 0;
	if( !gpmSCP.nLNK() )
		return 0;

	srcCMPLR( *pW->piMASS->pOPER(), iSCP, pW, pSRCstk );
	return 1;
}

