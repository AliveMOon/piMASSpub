#include "gpcres.h"
extern U1 gpaALFadd[];
U1	gpsTABrun[] = "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t", *gppTABrun = gpsTABrun + strlen( (char*)gpsTABrun );

bool bITT = true; // false; //
gpcLAZY* gpcRES::res2mini( gpcLAZY* pLZY, U1* pBUFF, gpcRES* pMOM, U4 deep )
{
	if( !this )
		return pLZY;

    if( deep ? !pLZY : false )
		return NULL;

	if( pLZY )
		pLZY->lzy_reset();
	else
		pLZY = new gpcLAZY;

	if( !pLZY )
		return NULL;

	U8 s = -1;
	U1* pB = pBUFF;
	for( U4 a = 0; a < nA; a++ )
	{
		ALU( a );
		pB = pB + gpfALF2STR( pBUFF, alu.alf ) + 1;
		pLZY->lzy_format( s = -1, "\r\n%s%s.%s[%dx%d]sof(%d) ", gppTABrun-deep, alu.typ.typ2str(pB), pBUFF, alu.AN.x, alu.AN.y, alu.nLOAD() );
	}
	/// ha nem DEBUG legyen pMOM == NULL és akor nem írja ki a pISA-t
	if( !pMOM )
		return pLZY;

	/*bool b_asg = false;
	for( U4 i = 0; i < nISA.x; i++ )
	{

		if( !pLZY )
			continue;
		b_asg = pISA[i].isa.aISA[1] == '=';
		if( b_asg )
			pLZY->lzy_format( s = -1, "\r\n%s", gppTABrun-deep );

        switch( pISA[i].isa.aISA[0] )
        {

			case gpeISA_trg:{
					pLZY->lzy_format( s = -1, "\r\n%s[%d:%d]", gppTABrun-deep, pISA[i].an.x, pISA[i].an.y );
				} break;
 			case gpeISA_u8: {
					pLZY->lzy_format( s = -1, "%lld", pISA[i].an.u8 );
				} break;
			case gpeISA_d8: {
					pLZY->lzy_format( s = -1, "%f", pISA[i].an.d8 );
				} break;
			case gpeISA_an: {
					gpfALF2STR( sBUFF, pISA[i].an.a4 );
					pLZY->lzy_format( s = -1, "%s%d", sBUFF, pISA[i].an.n4 );
				} break;
			case gpeISA_anFUN: {
					gpfALF2STR( sBUFF, pISA[i].an.a4 );
					pLZY->lzy_format( s = -1, "%s%d(", sBUFF, pISA[i].an.n4 );
				} break;

			case gpeISA_tag: {
					gpfALF2STR( sBUFF, pISA[i].an.var );
					pLZY->lzy_format( s = -1, "#%s", sBUFF );
				} break;

			case gpeISA_var: {
					gpfALF2STR( sBUFF, pISA[i].an.var );
					pLZY->lzy_format( s = -1, "%s%s", b_asg ? "." : "", sBUFF );
				} break;
			case gpeISA_FUN: {
					gpfALF2STR( sBUFF, pISA[i].an.var );
					pLZY->lzy_format( s = -1, "%s(", sBUFF );
					//pOUT = pISA[i].pRES->run( pOUT, pLZY, pMASS, pSRC, this, deep+1 );
				} break;
			case gpeISA_str: {
					pLZY->lzy_format( s = -1, "\"%s\"", pISA[i].an.y ? pISA[i].an.aSTR[0] : (U1*)"" );
				} break;
			case gpeISA_nop:
			default: {
				} break;
        }
        pLZY->lzy_format( s = -1, "%c", pISA[i].isa.aISA[1] >= ' ' ? pISA[i].isa.aISA[1] : '_' );

	}*/
    return  pLZY;
}
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
	U1 sBUFF[0x1000], *pB = sBUFF+0x20, *pCOUT = pB, nB;
	for( U4 i = 0; i < nISA.x; i++ )
	{
		if( (U1)pISA[i].isa.aISA[0] > ' ' )
		{
			*pB =  (U1)pISA[i].isa.aISA[0];
			pB++;
		}

		switch( pISA[i].isa.aISA[0] )
        {
			case gpeISA_trg:{
					pB += sprintf( (char*)pB, "%s", pISA[i].an.str( sBUFF ) );
					stk.aTRG[stk.nT] = pISA[i].an;
					stk.nT++;
				} break;
 			case gpeISA_u8: {
					pB += sprintf( (char*)pB, "%lld", pISA[i].an.u8 );
					stk.D[stk.nD] = pISA[i].an.u8;
					stk.nD++;
				} break;
			case gpeISA_i8: {
					pB += sprintf( (char*)pB, "%lld", -((I8)pISA[i].an.u8) );
					stk.D[stk.nD] = -((I8)pISA[i].an.u8);
					stk.nD++;
				} break;
			case gpeISA_d8: {
					pB += sprintf( (char*)pB, "%f",  pISA[i].an.d8 );
					stk.D[stk.nD] = pISA[i].an.d8;
					stk.nD++;
				} break;

			case gpeISA_an: {
					pB += sprintf( (char*)pB, "%s", pISA[i].an.strA4N( sBUFF ));
					stk.aTRG[stk.nA] = pISA[i].an;
					stk.nA++;
				} break;
			case gpeISA_anFUN: {
					pB += sprintf( (char*)pB, "%s", pISA[i].an.strA4N( sBUFF ));
					stk.aTRG[stk.nA] = pISA[i].an;
					stk.nA++;
				} break;

			case gpeISA_tag: {
					 pB += sprintf( (char*)pB, "%s", pISA[i].an.strVAR( sBUFF ) );
					stk.aTG[stk.nG] = pISA[i].an.var;
					stk.nG++;
				} break;

			case gpeISA_var: {
					pB += sprintf( (char*)pB, "%s", pISA[i].an.strVAR( sBUFF ) );
					stk.aVR[stk.nV] = pISA[i].an.var;
					stk.nV++;
				} break;
			case gpeISA_FUN: {
					pB += sprintf( (char*)pB, "%s", pISA[i].an.strVAR( sBUFF ) ) ;
					stk.aVR[stk.nV] = pISA[i].an.var;
					stk.nV++;
				} break;
			case gpeISA_str: {
					pB += sprintf( (char*)pB, "%s", (stk.apSTR[stk.nS] = pISA[i].an.aSTR[0])) ;
					stk.nS++;
				} break;

			case gpeISA_nop:
			default:
				break;
		}
		if( pISA[i].pRES )
			pOUT = pISA[i].pRES->run( pOUT, pMN, pMASS, pSRC, this, deep+1, &stk );

		if( (U1)pISA[i].isa.aISA[1] > ' ' )
		{
			*pB = (U1)pISA[i].isa.aISA[1];
			pB++;
		}
		switch( pISA[i].isa.aISA[1] )
		{
			gpeISA_u8:{} break;
			gpeISA_i8:{} break;
			gpeISA_d8:{} break;
			gpeISA_an:{} break;
			gpeISA_anFUN:{} break;
			gpeISA_var:{} break;
			gpeISA_FUN:{} break;
			gpeISA_dim:{} break;
			gpeISA_not:{} break;
			gpeISA_str:{} break;
			gpeISA_tag:{} break;
			gpeISA_dollar:{} break;
			gpeISA_rem:{} break;
			gpeISA_and:{} break;
			gpeISA_brkB:{} break;
			gpeISA_brkE:{} break;
			gpeISA_mul:{} break;
			gpeISA_plus:{} break;
			gpeISA_sub:{} break;
			gpeISA_dot:{} break;
			gpeISA_div:{} break;
			gpeISA_litl:{} break;
			gpeISA_assign:{} break;
			gpeISA_gret:{} break;
			gpeISA_exp:{} break;
			gpeISA_root:{} break;
			gpeISA_brkDB:{} break;
			gpeISA_brkDE:{} break;
			gpeISA_equ:{} break;
			gpeISA_trg:{} break;
			gpeISA_blkB:{} break;
			gpeISA_or:{} break;
			gpeISA_blkE:{} break;
			gpeISA_nop:
			default:
				pB--;
				break;
		}
	}
	*pB = 0;

	if( bITT )
	if( pCOUT < pB )
		cout << pCOUT; // << endl;

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
