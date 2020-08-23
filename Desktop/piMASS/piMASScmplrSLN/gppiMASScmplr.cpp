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
	U1 *pSTR;
	I4 nSTR;
	gpBLOCK* pBLOCK = NULL;
	for( U4 nM = gpmSCP.nMN(), m = 0, l; m < nM; m++ )
	{
		if( pM0[m].MNclt() == gpeCLR_red2 )
			continue;

		M = pM0[m];
		if( M.MNtyp() == gpeTYP_STR )
		{
			if( M.MNclt() != gpeCLR_violet )
				continue;
			///STR
			pSTR = (nSTR=M.n) ? gpmSCP.p_str+M.i : NULL;
			srcBLKarySTR( m, pSTR, nSTR );
			continue;
		}


		U4x4& lnk = pL0[l=M.dct];
		switch( M.MNclr() )
		{
			case gpeCLR_blue2: 	///ABC

				break;
			case gpeCLR_orange:	///NUM
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
