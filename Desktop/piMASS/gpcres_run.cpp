#include "gpcres.h"
extern U1 gpaALFadd[];
U1	gpsTABrun[] = "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t", *gppTABrun = gpsTABrun + strlen( (char*)gpsTABrun );


gpcRES* gpcRES::run( gpcMASS* pMASS, gpcSRC* pSRC, gpcRES* pMOM, U4 deep )
{
	if( this ? !pISA : true )
		return this;
	gpcLAZY mini;
	U8 s = -1;
	for( U4 i = 0; i < nISA.x; i++ )
	{
		mini.lzy_format( s = -1, "\r\n%s%c", gppTABrun-deep, pISA[i].isa.aISA[1] >= ' ' ? pISA[i].isa.aISA[1] : '_' );
        switch( *pISA[i].isa.aISA )
        {
			case gpeISA_nop: {
				} break;
			gpeISA_u8: {
					mini.lzy_format( s = -1, "%lld", pISA[i].an.u8 );
				} break;
			gpeISA_d8: {
					mini.lzy_format( s = -1, "%f", pISA[i].an.d8 );
				} break;
			gpeISA_an: {
				} break;
			gpeISA_anFUN: {
				} break;
			gpeISA_var: {
				} break;
			gpeISA_FUN: {
				} break;
        }
	}
	if( mini.n_load )
	{

		pSRC->pMINI = pSRC->pMINI->lzy_plus( &mini, s );
	}

	return this;
}
