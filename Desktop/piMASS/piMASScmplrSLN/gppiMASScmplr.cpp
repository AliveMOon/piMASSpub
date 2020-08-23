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
			case gpeCLR_green2: ///OPER

				break;
			case gpeCLR_red2:	///BREAK
				break;

			case gpeCLR_violet:	///STR
			case gpeCLR_green:	///NOTE
			default:
				continue;
		}
	}

}
