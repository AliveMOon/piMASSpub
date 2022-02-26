#include "gpcSRC.h"
#include "gpcSRClnk.h"
#include "gpccrs.h"
extern U1 gpaALFsub[];
extern char gpaALF_H_sub[];
gpcLZY* gpcSRC::srcMINI( gpcLZY* pLZY, gpcMASS* pMASS, gpcWIN* pWIN, gpCORE* pMOM ) {
	pLZY->lzyRST();
	if( !pCORE )
		return pLZY;

	static const U1 iSCP=0;
	gpCORE& core = *pCORE;
	gpO* pO0 = gpmLZYvali( gpO, &core.oLST );
	if( !pO0 )
		return pLZY;
	char sBUFF[0x100];
	gpmMcpy( sBUFF, "A:", 3 );
	U8 s = aSCP[iSCP].nMN();
	gpcLZY	&mLST = core.mLST;
	gpCNST* pLNK = gpmLZYvali( gpCNST, &scpCNST ); // (gpCNST*)aSCP[iSCP].obj.p_alloc;
	I8	*pO, *pC;
	for( U4 o = 0, n = core.oLST.nLD(sizeof(gpO)); o < n; o++ )
	{
		gpO& obj = pO0[o];
		gpCNST& O = pLNK[obj.oID];
		gpfALF2STR( sBUFF, O.obj.alf );

		pLZY = pLZY->lzyFRMT( s = -1, "\n %s", sBUFF );

        U1* p_src = mLST.Ux( obj.iX, sizeof(U1) );
        s = -1;
		switch( obj.cID )
		{
			case gpeCsz_Q: pLZY->lzyFRMT( s, " %lld,", *(I8*)p_src ); break;
			case gpeCsz_q: pLZY->lzyFRMT( s, " %lld,", *(U8*)p_src ); break;
			case gpeCsz_L: pLZY->lzyFRMT( s, " %d,", *(int*)p_src ); break;
			case gpeCsz_l: pLZY->lzyFRMT( s, " %d,", *(U4*)p_src ); break;
			case gpeCsz_W: pLZY->lzyFRMT( s, " %d,", *(I2*)p_src ); break;
			case gpeCsz_w: pLZY->lzyFRMT( s, " %d,", *(U2*)p_src ); break;
			case gpeCsz_B: pLZY->lzyFRMT( s, " %d,", *(I1*)p_src ); break;
			case gpeCsz_b:
						pLZY->lzyFRMT( s, " %d,", *(U1*)p_src );
						break;
			case gpeCsz_f: pLZY->lzyFRMT( s, " %f,", *(float*)p_src ); break;
			case gpeCsz_d: pLZY->lzyFRMT( s, " %f,", *(double*)p_src ); break;
			case gpeCsz_4: pLZY->lzyFRMT( s, " %d,", *(I1*)p_src ); break;
			default: break;

		}
	}
	return pLZY;
}
