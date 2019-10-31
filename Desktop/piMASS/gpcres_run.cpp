#include "gpcres.h"
extern U1 gpaALFadd[];
U1	gpsTABrun[] = "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t", *gppTABrun = gpsTABrun + strlen( (char*)gpsTABrun );


gpcRES* gpcRES::run( gpcLAZY* pLZY, gpcMASS* pMASS, gpcSRC* pSRC, gpcRES* pMOM, U4 deep )
{
	if( this ? !pISA : true )
		return this;

	if( nASG && pSRC )
	if( pLZY ? false : !deep )
	{
		if( pSRC->pMINI )
			pSRC->pMINI->lzy_reset();
		else
			pSRC->pMINI = new gpcLAZY;

		pLZY = pSRC->pMINI;
	}

	//gpcLAZY mini;
	U8 s = -1;
	U1 sBUFF[0x1000], *pB = sBUFF, nB;
	if( pLZY )
	{
		for( U4 a = 0; a < nA; a++ )
		{
			ALU( a );
			pB = sBUFF + gpfALF2STR( sBUFF, alu.alf ) + 1;
			pLZY->lzy_format( s = -1, "\r\n%s%s.%s[%dx%d]sof(%d) ", gppTABrun-deep, alu.typ.typ2str(pB), sBUFF, alu.AN.x, alu.AN.y, alu.nLOAD() );
		}
	}
	bool b_asg = false;
	for( U4 i = 0; i < nISA.x; i++ )
	{
		if( !pLZY )
			continue;
		b_asg = pISA[i].isa.aISA[1] == '=';
		if( b_asg )
			pLZY->lzy_format( s = -1, "\r\n%s", gppTABrun-deep );


        switch( pISA[i].isa.aISA[0] )
        {
			case gpeISA_nop: {
				} break;
			case gpeISA_trg:{
					pLZY->lzy_format( s = -1, "[%d:%d]", pISA[i].an.x, pISA[i].an.y );
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
					pLZY->lzy_format( s = -1, "%s%d", sBUFF, pISA[i].an.n4 );
				} break;
			case gpeISA_var: {
					gpfALF2STR( sBUFF, pISA[i].an.var );
					pLZY->lzy_format( s = -1, "%s%s", b_asg ? "." : "", sBUFF );
				} break;
			case gpeISA_tag: {
					gpfALF2STR( sBUFF, pISA[i].an.var );
					pLZY->lzy_format( s = -1, "#%s", sBUFF );
				} break;
			case gpeISA_FUN: {
					gpfALF2STR( sBUFF, pISA[i].an.var );
					pLZY->lzy_format( s = -1, "%s", sBUFF );
					pISA[i].pRES->run( pLZY, pMASS, pSRC, this, deep+1 );
				} break;
			case gpeISA_str: {
					pLZY->lzy_format( s = -1, "\"%s\"", pISA[i].an.y ? pISA[i].an.aSTR[0] : (U1*)"" );
				} break;
        }
        pLZY->lzy_format( s = -1, "%c", pISA[i].isa.aISA[1] >= ' ' ? pISA[i].isa.aISA[1] : '_' );

	}

	if( !deep )
	if( pSRC->pMINI ? !pSRC->pMINI->n_load : false  )
		gpmDEL(pSRC->pMINI);

	return this;
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

		if( pSRC->qBLD() )
		{
			if( !pSRC->apRES[3] )
			{
				pSRC->apRES[3] = pSRC->apRES[3]->compiEASY( pSRC->pSRCstart( true ), NULL, NULL, NULL );
				//gpmDEL( pSRC->apRES[2] ); // OFF? 2
				if( pSRC->apRES[3] )
				{
					//gpmDEL( pSRC->apRES[2] ); // OFF? 1
					if( pSRC->apRES[2] )
					{
						gpmDEL( pSRC->apRES[1] ); // OFF 0
						if( pSRC->apRES[1] )
						{
							gpmDEL( pSRC->apRES[0] );
							pSRC->apRES[0] = pSRC->apRES[1];
						}


						pSRC->apRES[1] = pSRC->apRES[2];
					}
					pSRC->apRES[2] = pSRC->apRES[3];
				}
				pSRC->apRES[3] = NULL;
			}
			pSRC->rdyBLD();
		}

		if( !pSRC->apRES[2] )
			continue;	// ha nincsen 2 nem sikerült az építés

		// egyébként meg kell probálni futatni
		pSRC->pMINI->lzy_reset();
		pSRC->apRES[2]->run( NULL, this, pSRC, NULL );

	}

	return pKEYbuff;
}
