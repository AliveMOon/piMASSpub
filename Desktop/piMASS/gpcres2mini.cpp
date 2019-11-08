#include "gpcres.h"
extern U1 gpaALFadd[];
//extern U1 gpsTABrun[];
U1	gpsTABrun[] = "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t", *gppTABrun = gpsTABrun + strlen( (char*)gpsTABrun );


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
	U1* pB = pBUFF+0x100;
	for( U4 a = 0; a < nA; a++ )
	{
		ALU( a );
		pB = pB + gpfALF2STR( pBUFF, alu.alf ) + 1;
		pLZY->lzy_format( s = -1, "%s%s%s.%s // [%dx%d]sof(%d) ", a ? "\r\n":"", gpsTABrun-deep, alu.typ.typ2str(pBUFF+0x10), pBUFF, alu.AN.x, alu.AN.y, alu.nLOAD() );
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
