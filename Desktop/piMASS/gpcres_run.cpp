#include "gpcres.h"
extern U1 gpaALFadd[];

bool bITT = true; // false; //

gpcRES* gpcRES::run( gpcRES* pOUT, gpcLAZY* pMN, gpcMASS* pMASS, gpcSRC* pSRC, gpcRES* pMOM, U4 deep, gpcSTK* pSM )
{
	if( this ? !pISA : true )
		return pOUT;

	gpcSTK stk( pSM );
	if( pOUT ? !deep : false )
	{
		pOUT = new gpcRES;
	}
	U8 u8; I8 i8; double d8;
	U1 sBUFF[0x1000], *pB = sBUFF+0x20, *pS = pB, nB;
	bool bROW = false;
	for( U4 i = 0; i < nISA.x; i++ )
	{
		gpcISA &IS = pISA[i];
		if( (U1)IS.isa.aISA[0] > ' ' )
		{
			*pB = (U1)IS.isa.aISA[0];
			pB++;
		}

		switch( IS.isa.aISA[0] )
        {
			case gpeISA_trg:{
					pB += sprintf( (char*)pB, "[%d:%d]", IS.an.x, IS.an.y );
					stk.aTRG[stk.nT] = IS.an;
					stk.nT++;
				} break;
 			case gpeISA_u8: {
					pB += sprintf( (char*)pB, "%lld", IS.an.u8 );
					stk.D[stk.nD] = IS.an.u8;
					stk.nD++;
				} break;
			case gpeISA_i8: {
					pB += sprintf( (char*)pB, "%lld", -((I8)IS.an.u8) );
					stk.D[stk.nD] = -((I8)IS.an.u8);
					stk.nD++;
				} break;
			case gpeISA_d8: {
					pB += sprintf( (char*)pB, "%f",  IS.an.d8 );
					stk.D[stk.nD] = IS.an.d8;
					stk.nD++;
				} break;

			case gpeISA_an: {
					pB += sprintf( (char*)pB, "%s", IS.an.strA4N( sBUFF ));
					stk.aTRG[stk.nA] = IS.an;
					stk.nA++;
				} break;
			case gpeISA_anFUN: {
					pB += sprintf( (char*)pB, "%s", IS.an.strA4N( sBUFF ));
					stk.aTRG[stk.nA] = IS.an;
					stk.nA++;
				} break;

			case gpeISA_tag: {
					 pB += sprintf( (char*)pB, "%s", IS.an.strVAR( sBUFF ) );
					stk.aTG[stk.nG] = IS.an.var;
					stk.nG++;
				} break;

			case gpeISA_var: {
					pB += sprintf( (char*)pB, "%s", IS.an.strVAR( sBUFF ) );
					stk.aVR[stk.nV] = IS.an.var;
					stk.nV++;
				} break;
			case gpeISA_FUN: {
					pB += sprintf( (char*)pB, "%s", IS.an.strVAR( sBUFF ) ) ;
					stk.aVR[stk.nV] = IS.an.var;
					stk.nV++;
				} break;
			case gpeISA_str: {
					pB += sprintf( (char*)pB, "%s", (stk.apSTR[stk.nS] = IS.an.aSTR[0])) ;
					stk.nS++;
				} break;
			case gpeISA_row:
				bROW = true;
				break;
			case gpeISA_nop:
			default:
				break;
		}
		if( IS.pRES )
			pOUT = IS.pRES->run( pOUT, pMN, pMASS, pSRC, this, deep+1, &stk );

		if( (U1)IS.isa.aISA[1] > ' ' )
		{
			*pB = (U1)IS.isa.aISA[1];
			pB++;
		}
		switch( IS.isa.aISA[1] )
		{
			case gpeISA_u8:{} break;
			case gpeISA_i8:{} break;
			case gpeISA_d8:{} break;
			case gpeISA_an:{} break;
			case gpeISA_anFUN:{} break;
			case gpeISA_var:{} break;
			case gpeISA_FUN:{} break;
			case gpeISA_dim:{} break;
			case gpeISA_not:{} break;
			case gpeISA_str:{} break;
			case gpeISA_tag:{} break;
			case gpeISA_dollar:{} break;
			case gpeISA_rem:{} break;
			case gpeISA_and:{} break;
			case gpeISA_brkB:{} break;
			case gpeISA_brkE:{} break;
			case gpeISA_mul:{} break;
			case gpeISA_plus:{} break;
			case gpeISA_sub:{} break;
			case gpeISA_dot:{} break;
			case gpeISA_div:{} break;
			case gpeISA_row:
				bROW = true;
				break;
			case gpeISA_litl:{} break;
			case gpeISA_assign:{} break;
			case gpeISA_gret:{} break;
			case gpeISA_exp:{} break;
			case gpeISA_root:{} break;
			case gpeISA_brkDB:{} break;
			case gpeISA_brkDE:{} break;
			case gpeISA_equ:{} break;
			case gpeISA_trg:{} break;
			case gpeISA_blkB:{} break;
			case gpeISA_or:{} break;
			case gpeISA_blkE:{} break;
			case gpeISA_nop:
			default:
				break;
		}

		if( !bITT )
			continue;
		if( !bROW )
			continue;

		bROW = false;

		if( pS < pB )
		{
			*pB = 0;
			cout << (char*)pS << endl; // ; //
			pB = pS;
		}
	}
	*pB = 0;

	if( bITT )
	if( pS < pB )
	{
		cout << (char*)pS << endl; // ; //
		pB = pS;
	}

	if( !pSRC )
		return pOUT;

	pSRC->pMINI = pOUT->res2mini(
									pSRC->pMINI, sBUFF,

									( nASG && (pSRC->bSW&gpeMASSdebugMSK) )
									? this : NULL,

									deep
								);

	if( deep )
		return pOUT;

	if( pSRC->pMINI ? !pSRC->pMINI->n_load : false  )
		gpmDEL(pSRC->pMINI);
	return pOUT;
}

U1* gpcMASS::justDOit( U1* sKEYbuff, I4x4& mouseXY, U4* pKT, I4x4& SRCxycr, I4x4& SRCin )
{
	U1* pKEYbuff = sKEYbuff;
	gpcSRC	tmp, *pSRC;
	U4 xFND;
	if( U4 *pM = mapCR.pMAP )
	if( U4 *pC = mapCR.pCOL )
	if( U4 *pR = mapCR.pROW )
	if( pM < pC )
	for( U4 i = 0, ie = pC-pM; i < ie; i++ )
	{
		if( !pM[i] )
			continue;

		xFND = pM[i];
		pSRC = SRCfnd( xFND );
		if( !pSRC )
			continue;

		gpmDEL( pSRC->apOUT[0] );
		if( pSRC->qBLD() )
		{
			gpmDEL( pSRC->pEXE );
			pSRC->pEXE = pSRC->pEXE->compiEASY( pSRC->pSRCstart( true ), NULL, NULL, NULL );
			pSRC->rdyBLD();
			pSRC->apOUT[0] = pSRC->apOUT[1];
			pSRC->apOUT[1] = pSRC->apOUT[2];
			pSRC->apOUT[2] = pSRC->apOUT[3];
			pSRC->apOUT[3] = NULL;
		}
		else if( pSRC->bSW&gpeMASSloopMSK )
		{
			pSRC->apOUT[0] = pSRC->apOUT[1];
			pSRC->apOUT[1] = pSRC->apOUT[2];
			pSRC->apOUT[2] = pSRC->apOUT[3];
			pSRC->apOUT[3] = NULL;
		}

		if( !pSRC->apOUT[3] )
		{
			if( !pSRC->pEXE )
			{
				pSRC->pMINI->lzy_reset();
				continue;
			}

			pSRC->pMINI->lzy_reset();
			pSRC->apOUT[3] = pSRC->pEXE->run( pSRC->apOUT[3], NULL, this, pSRC, NULL );
		}
	}

	return pKEYbuff;
}
