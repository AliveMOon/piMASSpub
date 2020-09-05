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
	char		*pS;
	const char	*psDCT;
	I4 nS, nsDCT; //, dctID;
	gpBLOCK* pBLK = NULL;
	I8x2 AN; gpeCsz acID[2] = {gpeCsz_OFF};
	gpeOPid opID;
	for( U4 nM = gpmSCP.nMN(), mnID = 0, l; mnID < nM; mnID++ )
	{
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
										pBLK = srcBLKmov( pS, mnID, pBLK, opID );
										break;
									case gpeOPid_add: /// +
									case gpeOPid_sub: /// ==
										pBLK = srcBLKadd( pS, mnID, pBLK, opID );
										break;
									case gpeOPid_mul: /// *
										pBLK = srcBLKmul( pS, mnID, pBLK, opID );
										break;

									case gpeOPid_entry: { /// (
										pBLK = srcBLKent( pS, mnID, pBLK, opID );

										} break;
									case gpeOPid_out: /// )
										pBLK = srcBLKout( pS, mnID, pBLK, opID );

										break;
									case gpeOPid_stk: /// , ;
										pBLK = srcBLKstk( pS, mnID, pBLK, opID );
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
