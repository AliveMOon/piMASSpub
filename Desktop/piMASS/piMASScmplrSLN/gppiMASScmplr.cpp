#include "gpcSRC.h"
#include "gpcSRClnk.h"
#include "gpccrs.h"

void gpcSRC::srcCMPLR( gpcLZYdct& dOP, U1 iSCP ) {
	if( !this )
		return;
	pDBG->lzyRST();

	I8x4 *pM0 = gpmSCP.pMN(), M, Mnx;
	U4x4 *pL0 = gpmSCP.pLNK(); //, aLNK[0x10];
	gpeCLR clr;
	U1		*pSTR;
	const char *pS;
	I4 nSTR, nS, iDCT;
	gpBLOCK* pBLOCK = NULL;
	I8x2 AN; gpeCsz acID[2] = {gpeCsz_OFF};
	gpeOPid opID;
	for( U4 nM = gpmSCP.nMN(), mID = 0, l; mID < nM; mID++ )
	{
		if( pM0[mID].MNclr() == gpeCLR_red2 )
			continue;

		M = pM0[mID];
		if( M.MNtyp() == gpeTYP_STR )
		{
			if( M.MNclr() != gpeCLR_violet )
				continue;
			///STR
			pSTR = (nSTR=M.n) ? gpmSCP.p_str+M.i : NULL;
			pBLOCK = srcBLKarySTR( pBLOCK, mID, pSTR, nSTR );
			continue;
		}


		U4x4& lnk = pL0[iDCT=M.dct];
		if( !lnk.y )
		{
			pS = gpmSCP.lzyDCT.sSTRix(iDCT, NULL);
			if( !pS )
				continue;
			nS = gpmSCP.lzyDCT.nSTRix(iDCT);
			if( !nS )
				continue;
		}

		switch( M.MNclr() )
		{
			case gpeCLR_blue2: 	///ABC
				acID[1] = AN.gpCszALF(pS,nS);
				if( acID[1] == gpeCsz_OFF )
					continue;
				pBLOCK = srcBLKaryAN( pBLOCK, iDCT, acID, AN );

				break;
			case gpeCLR_orange:	///NUM
				acID[1] = AN.gpCszNUM(pS,nS);
				if( acID[1] == gpeCsz_OFF )
					continue;
				pBLOCK = srcBLKaryNUM( pBLOCK, iDCT, acID, AN );
				break;
			case gpeCLR_green2: { ///OPER
					U1* pSs = (U1*)pS, *pSe = pSs+nS;
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

								/// FENT
								break;
							case gpeOPid_end:
							case gpeOPid_dimE:
							case gpeOPid_brakE:
								/// FENT

								/// LENT
								break;

                            default:
								switch( gpaOPgrp[opID] )
								{
									case gpeOPid_mov: /// =
										pBLOCK = srcBLKmov( pBLOCK, opID );
										break;
									case gpeOPid_add: /// +
									case gpeOPid_sub: /// ==
										pBLOCK = srcBLKadd( pBLOCK, opID );
										break;
									case gpeOPid_mul: /// *
										pBLOCK = srcBLKmul( pBLOCK, opID );


										break;

									case gpeOPid_entry: { /// (
										pBLOCK = srcBLKentry( pBLOCK, opID );

										} break;
									case gpeOPid_out: /// )
										pBLOCK = srcBLKout( pBLOCK, opID );

										break;
									case gpeOPid_stk: /// ,
										pBLOCK = srcBLKstk( pBLOCK, opID );
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

}
